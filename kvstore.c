/*
 * Key value store library for storing value block for some key
 *
 * supported operations size,put,get and delete
 *
 * Author:  Abhijit Shanbhag<abhijit.shanbhag@rutgers.edu>
 * 		    Priyanka Dhingra<pd374@scarletmail.rutgers.edu>
 */
/*
 * Further improvements:
 * 	Use variable MAX block counts
 * 	Use char[] or bitmap for free blocks
 * 	Open files when required
 * 	Use logging for rollback
 */
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include "kvstore.h"
#include <glib.h>
#include <stdlib.h>

/*********************************************************************
 * Constants
 ********************************************************************/
/*
 * Internal files used
 */
#define DATAFILE_1B ".datafile1b"
#define DATAFILE_1KB ".datafile1kb"
#define DATAFILE_512KB ".datafile512kb"
#define DATAFILE_1MB ".datafile1mb"
#define KVSTORE_METADATA_FILE ".kvstoremeta"
#define KVSTORE_MAPPING_FILE ".kvstoremapping"
/*
 * Constants
 */
#define MAX_BLOCKS 1024
#define OUT_OF_FILE 32767
/*
 * Globals
 */
int DATA_BLOCK_TYPE_SIZE[4] = { 1, 1024, 1024 * 512, 1024 * 1024 };
int BV_DATA_BLOCK_OFFSET[4] = { 0, MAX_BLOCKS, MAX_BLOCKS * 2, MAX_BLOCKS * 3 };
/*
 * File descriptors for files
 */
FILE *fd_kvstore_meta, *fd_kvstore_mapping, *fd_data_1b, *fd_data_1kb,
		*fd_data_512kb, *fd_data_1mb;
int first_time_flag = 0;
/*
 * Tuple stored in KVSTORE_METADATA_FILE
 * valueset_t tuple -> file, blocknumber ,size
 */
typedef struct valuetuple {
	int key;
	int data_fd;
	int n_block;
	int size;
} valuetuple_t;
valuetuple_t *temp_tuple;
/*
 * Variable to check if inmemory hash table is loaded for mapping file
 */
int hash_table_flag = 0;
GHashTable *ht;
/*********************************************************************
 * Helper functions
 ********************************************************************/
int fileExists(const char *filename) {
	FILE *fp = fopen(filename, "r");
	if (fp != NULL)
		fclose(fp);
	return (fp != NULL);
}
void load_hash_table() {
	//Initialize the hash table
	ht = g_hash_table_new(g_int_hash, g_int_equal);
	int n_mappings = 0;
	//Key Value for the HT
	gint *key = g_new(gint, 1);
	gint *value = g_new(gint, 1);
	fseek(fd_kvstore_meta, MAX_BLOCKS * 4, SEEK_SET);
	fread(&n_mappings, sizeof(int), 1, fd_kvstore_meta);
	int i;
	fseek(fd_kvstore_mapping, 0, SEEK_SET);
	for (i = 0; i < n_mappings; i++) {
		fread(temp_tuple, sizeof(valuetuple_t), 1, fd_kvstore_mapping);
		*key = temp_tuple->key;
		*value = i;
		if (*key != -1) {
			g_hash_table_replace(ht, key, value);
		}
		memset(temp_tuple, '\0', sizeof(valuetuple_t));
	}
}
void open_kvstore_files() {
	//check if running for the first time
	if (fileExists(DATAFILE_1B) && fileExists(DATAFILE_1KB)
			&& fileExists(DATAFILE_512KB) && fileExists(KVSTORE_METADATA_FILE)
			&& fileExists(KVSTORE_MAPPING_FILE)) {
		//If it reaches here it exists. Open it for reading and writing
		fd_data_1b = fopen(DATAFILE_1B, "r+");
		fd_data_1kb = fopen(DATAFILE_1KB, "r+");
		fd_data_512kb = fopen(DATAFILE_512KB, "r+");
		fd_data_1mb = fopen(DATAFILE_1MB, "r+");
		fd_kvstore_meta = fopen(KVSTORE_METADATA_FILE, "r+");
		fd_kvstore_mapping = fopen(KVSTORE_MAPPING_FILE, "r+");
		first_time_flag = 0;
	} else {
		// all files doesnt exist
		//Erase and open new files for consistency.
		fd_data_1b = fopen(DATAFILE_1B, "w+");
		fd_data_1kb = fopen(DATAFILE_1KB, "w+");
		fd_data_512kb = fopen(DATAFILE_512KB, "w+");
		fd_data_1mb = fopen(DATAFILE_1MB, "w+");
		fd_kvstore_meta = fopen(KVSTORE_METADATA_FILE, "w+");
		fd_kvstore_mapping = fopen(KVSTORE_MAPPING_FILE, "w+");
		first_time_flag = 1;
		//Write init KVSTORE_METADATA_FILE + count of mappings
		int temp = 0;
		fseek(fd_kvstore_meta, 0, SEEK_SET);
		fwrite(&temp, sizeof(int), MAX_BLOCKS * 4, fd_kvstore_meta);
		fwrite(&temp, sizeof(int), 1, fd_kvstore_meta);
	}
	//Allocate temp tuple
	temp_tuple = (valuetuple_t *) malloc(sizeof(valuetuple_t));
	memset(temp_tuple, '\0', sizeof(valuetuple_t));
	//Load the persistant mappings to memory if not already done so
	if (!hash_table_flag) {
		load_hash_table();
		hash_table_flag = 1;
	}
}
void close_kvstore_files() {
	free(temp_tuple);
	fclose(fd_data_1b);
	fclose(fd_data_1kb);
	fclose(fd_data_512kb);
	fclose(fd_data_1mb);
	fclose(fd_kvstore_meta);
	fclose(fd_kvstore_mapping);
}
int determine_data_block_type(int size) {
	if (size <= 0 || size > 1024 * 1024) {
		return -1;
	} else if (size == 1) {
		return 0;
	} else if (size <= 1024) {
		return 1;
	} else if (size <= 1024 * 512) {
		return 2;
	} else if (size <= 1024 * 1024) {
		return 3;
	}
}
FILE* get_data_fd_type(int data_block_type) {
	switch (data_block_type) {
	case 1:
		return fd_data_1b;
	case 2:
		return fd_data_1kb;
	case 3:
		return fd_data_512kb;
	case 4:
		return fd_data_1mb;
	default:
		return NULL;
	}
}
int find_first_free_data_block( data_block_type) {
	int i;
	fseek(fd_kvstore_meta, BV_DATA_BLOCK_OFFSET[data_block_type], SEEK_SET);
	for (i = 0; i < MAX_BLOCKS; i++) {
		if (fgetc(fd_kvstore_meta) == 0) {
			return i;
		}
	}
	return -1;
}
/**********************************************************************
 *Library functions
 *********************************************************************/
int size(void* keyPtr) {
	int sizeval;
	//Key Value for the HT
	gint *key = g_new(gint, 1);
	gint *value = g_new(gint, 1);
	open_kvstore_files();
	if (first_time_flag) {
		close_kvstore_files();
		return 0;
	} else {
		*key = *((int*) keyPtr);
		if (g_hash_table_lookup(ht, key) == NULL) {
			close_kvstore_files();
			return 0;
		} else {
			value = g_hash_table_lookup(ht, key);
		}
		fseek(fd_kvstore_mapping, (*value) * (sizeof(valuetuple_t)), SEEK_SET);
		fread(temp_tuple, sizeof(valuetuple_t), 1, fd_kvstore_mapping);
		sizeval = temp_tuple->size;
	}
	close_kvstore_files();
	//Edge case when unwritten file is read
	if (sizeval == OUT_OF_FILE)
		return 0;
	return sizeval;
}
int put(void* keyPtr, void* valPtr, int size) {
	int data_block_type = determine_data_block_type(size);
	//Key Value for the HT
	gint *key = g_new(gint, 1);
	gint *value = g_new(gint, 1);
	*key = *((int*) keyPtr);
	//Invalid size
	if (data_block_type == -1) {
		return 0;
	}
	open_kvstore_files();
	FILE *fd = get_data_fd_type(data_block_type);
	int block_num = find_first_free_data_block(data_block_type);
	//Free block not found
	if (block_num == -1) {
		close_kvstore_files();
		return 0;
	}
	//Update DATAFILE*
	fseek(fd, DATA_BLOCK_TYPE_SIZE[data_block_type] * block_num, SEEK_SET);
	fwrite(valPtr, sizeof(char), size, fd);
	//Update KVSTORE_METADATA_FILE
	fseek(fd_kvstore_meta, BV_DATA_BLOCK_OFFSET[data_block_type] * block_num,
			SEEK_SET);
	int temp = 1;
	int offset;
	fwrite(&temp, sizeof(int), 1, fd_kvstore_meta);
	//void* result = g_hash_table_lookup(ht, &key);
	if (g_hash_table_lookup(ht, key) == NULL) {
		//Insert new entry at HT
		//Get size of KVSTORE_MAPPING_FILE
		fseek(fd_kvstore_meta, MAX_BLOCKS * 4, SEEK_SET);
		fread(&offset, sizeof(int), 1, fd_kvstore_meta);
		*value = offset;
		g_hash_table_replace(ht, key, value);
		int new_offset = offset + 1;
		fseek(fd_kvstore_meta, MAX_BLOCKS * 4, SEEK_SET);
		fwrite(&new_offset, sizeof(int), 1, fd_kvstore_meta);
	} else {
		value = g_hash_table_lookup(ht, key);
	}
	//Update KVSTORE_MAPPING_FILE
	temp_tuple->data_fd = data_block_type;
	temp_tuple->n_block = block_num;
	temp_tuple->size = size;
	temp_tuple->key = *key;
	fseek(fd_kvstore_mapping, ((*value) * sizeof(valuetuple_t)), SEEK_SET);
	fwrite(temp_tuple, sizeof(valuetuple_t), 1, fd_kvstore_mapping);
	close_kvstore_files();
	return 1;
}
int get(void* keyPtr, void* valPtr, int buffer_size) {
	int sizeval = size(keyPtr);
	//Key Value for the HT
	gint *key = g_new(gint, 1);
	gint *value = g_new(gint, 1);
	*key = *((int*) keyPtr);
//Exit if key not found or the value size is greater than buffer size
	if (sizeval == 0 || sizeval > buffer_size) {
		return 0;
	}
	open_kvstore_files();
//get tuple value using key from KVSTORE_MAPPING_FILE
	value = g_hash_table_lookup(ht, key);
	fseek(fd_kvstore_mapping, ((*value) * sizeof(valuetuple_t)), SEEK_SET);
	fread(temp_tuple, sizeof(valuetuple_t), 1, fd_kvstore_mapping);
//Update the buffer from the data file
	FILE *fd = get_data_fd_type(temp_tuple->data_fd);
	fseek(fd,
			DATA_BLOCK_TYPE_SIZE[(temp_tuple->data_fd)] * (temp_tuple->n_block),
			SEEK_SET);
	fread(valPtr, sizeof(char), (temp_tuple->size), fd);
	close_kvstore_files();
	return 1;
}
int delete(void* keyPtr) {
	int sizeval = size(keyPtr);
	//Key Value for the HT
	gint *key = g_new(gint, 1);
	gint *value = g_new(gint, 1);
	*key = *((int*) keyPtr);
	//Exit if key not found
	if (sizeval == 0) {
		return 0;
	}
	open_kvstore_files();
	//First get the value tuple from KVSTORE_MAPPING_FILE
	value = g_hash_table_lookup(ht, key);
	fseek(fd_kvstore_mapping, ((*value) * sizeof(valuetuple_t)), SEEK_SET);
	fread(temp_tuple, sizeof(valuetuple_t), 1, fd_kvstore_mapping);

	int data_block_type = temp_tuple->data_fd;
	int block_num = temp_tuple->n_block;

	//Delete key value tuple in KVSTORE_MAPPING_FILE by overwriting key as -1
	temp_tuple->key = -1;
	fseek(fd_kvstore_mapping, ((*value) * sizeof(valuetuple_t)), SEEK_SET);
	fread(temp_tuple, sizeof(valuetuple_t), 1, fd_kvstore_mapping);
	//and updating the hash table
	g_hash_table_remove(ht, key);

	//Update KVSTORE_METADATA_FILE with free block
	FILE *fd = get_data_fd_type(data_block_type);
	fseek(fd_kvstore_meta, BV_DATA_BLOCK_OFFSET[data_block_type] * block_num,
			SEEK_SET);
	int temp = 0;
	fwrite(&temp, sizeof(int), 1, fd_kvstore_meta);
	close_kvstore_files();
	return 1;
}

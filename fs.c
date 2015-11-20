/*
 * File System library for handling the file system
 *
 * Author:  Abhijit Shanbhag<abhijit.shanbhag@rutgers.edu>
 * 		    Priyanka Dhingra<pd374@scarletmail.rutgers.edu>
 */
#include <stdlib.h>
#include<malloc.h>
#include<string.h>
#include <stdlib.h>
#include "fs.h"

/*********************************************************************
 * Constants & hash defines
 ********************************************************************/
/*********************************************************************
 * Globals
 ********************************************************************/
char bm_inode[MAX_INODES] = { 0 };
char bm_data[MAX_DATA_BLKS] = { 0 };
int inmem_flag = 0;
/*********************************************************************
 * Helper functions
 ********************************************************************/
inode_dir_t* prepare_default_directory_data(int size) {
	inode_dir_t* dir_data = (inode_dir_t *) malloc(size * sizeof(inode_dir_t));
	//dir_data[0].fname = ".";
	strcpy(dir_data[0].fname, ".");
	//dir_data[1].fname = "..";
	strcpy(dir_data[1].fname, "..");
	return dir_data;
}

int init_fs() {
	int blk_num;
	//Update data block for root
	int root_inode_d_b = START_DATA_BLK;
	//Prepare Inode Data block
	inode_dir_t* root_inode_d = prepare_default_directory_data(2);
	//For root . and .. means the same inode
	root_inode_d[0].block_num = START_INODE_BLK;
	root_inode_d[1].block_num = START_INODE_BLK;
	put(&root_inode_d_b, root_inode_d, 2 * sizeof(inode_dir_t));

	//prepare inode block
	int root_inode_b = START_INODE_BLK;
	inode_t* root_inode = (inode_t *) malloc(sizeof(inode_t));
	root_inode->mode = 1;
	root_inode->size = 2 * sizeof(inode_dir_t);
	root_inode->data[0] = root_inode_d_b;
	put(&root_inode_b, root_inode, sizeof(inode_t));

	//Init BM for inode
	blk_num = INODE_BM_BLK;
	bm_inode[0] = 1;
	put(&blk_num, bm_inode, sizeof(bm_inode));
	//Init BM for data
	blk_num = DATA_BM_BLK;
	bm_data[0] = 1;
	put(&blk_num, bm_data, sizeof(bm_data));
	//Update SB
	int flag = 1;
	blk_num = SB_BLK;
	put(&blk_num, &flag, sizeof(int));

	return 1;
}
int load_bm() {
	int blk_num;
	blk_num = INODE_BM_BLK;
	get(&blk_num, bm_inode, sizeof(bm_inode));
	blk_num = DATA_BM_BLK;
	get(&blk_num, bm_data, sizeof(bm_data));
}

int check_fs_setup() {
	int blk_num = SB_BLK;
	//Check size of SB
	if (!size(&blk_num)) {
		if (!init_fs()) {
			return 1;
		}
	}
	if (!inmem_flag) {
		load_bm();
		inmem_flag = 1;
	}
	return 1;
}

/**********************************************************************
 *Library functions
 *********************************************************************/
inode_t fetch_inode(const char *path) {

}

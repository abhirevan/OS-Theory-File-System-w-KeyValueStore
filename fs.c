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
char bm_inode[MAX_INODES];
char bm_data[MAX_DATA_BLKS];
int inmem_flag = 0;
/*********************************************************************
 * Helper functions
 ********************************************************************/

void init_fs() {
	//Init Root Inode blk

	//Init BM for data
	//Init BM for inode
	//Update SB
}
void load_inode_bm() {

}

void load_data_bm() {

}
int check_fs_setup() {
	int blk_num = SB_BLK;
	//Check size of SB
	if (!size(&blk_num)) {
		init_fs();
	}
	if (!inmem_flag) {
		load_inode_bm();
		load_data_bm();
	}
}

/**********************************************************************
 *Library functions
 *********************************************************************/
inode_t fetch_inode(const char *path) {

}

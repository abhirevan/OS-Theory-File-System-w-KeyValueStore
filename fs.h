/*
 * File System library for handling the file system
 *
 * Author:  Abhijit Shanbhag<abhijit.shanbhag@rutgers.edu>
 * 		    Priyanka Dhingra<pd374@scarletmail.rutgers.edu>
 */
/*********************************************************************
 * Hash Defines
 ********************************************************************/
#define SB_BLK 0
#define INODE_BM_BLK 1
#define DATA_BM_BLK 2
#define START_INODE_BLK 3
#define MAX_INODES 10
#define START_DATA_BLK (START_INODE_BLK+MAX_INODES)
#define MAX_DATA_BLK_PER_INODE 10
#define MAX_DATA_BLKS MAX_INODES*MAX_DATA_BLK_PER_INODE
/**********************************************************************
 * Data structures
 *********************************************************************/
typedef struct inode {
	int mode; //0 -> Regular 1-> Directory
	int size;
	int data[MAX_DATA_BLK_PER_INODE];
} inode_t;
typedef struct dir_record {
	char fname[32];
	int block_num;
} inode_dir_t;
/**********************************************************************
 *Library functions
 *********************************************************************/
inode_t fetch_inode(const char *path);

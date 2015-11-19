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
#define MAX_INODES 10
#define MAX_DATA_BLK_PER_INODE 10
#define MAX_DATA_BLKS MAX_INODES*MAX_DATA_BLK_PER_INODE
/**********************************************************************
 * Data structures
 *********************************************************************/
typedef struct inode {
	char type; //0 -> Regular 1-> Directory
	char[MAX_DATA_BLK_PER_INODE] data;
} inode_t;
/**********************************************************************
 *Library functions
 *********************************************************************/

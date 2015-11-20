#define FUSE_USE_VERSION 26

#include <fuse.h>
#include "fs.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";

static int fuse_getattr(const char *path, struct stat *stbuf) {
	/*
	printf("fuse_getattr invoked\n");
	return 0;
	*/
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, hello_path) == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(hello_str);
	} else
		res = -ENOENT;

	return res;
}

static int fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		off_t offset, struct fuse_file_info *fi) {
	/*
	printf("fuse_readdir invoked\n");
	return 0;
	*/
	(void) offset;
		(void) fi;

		if (strcmp(path, "/") != 0)
			return -ENOENT;

		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, hello_path + 1, NULL, 0);

		return 0;
}

static int fuse_open(const char *path, struct fuse_file_info *fi) {
	/*
	printf("fuse_open invoked\n");
	return 0;
	*/
	if (strcmp(path, hello_path) != 0)
			return -ENOENT;

		if ((fi->flags & 3) != O_RDONLY)
			return -EACCES;

		return 0;
}

static int fuse_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi) {
	/*
	printf("fuse_read invoked\n");
	return 0;
	*/
	size_t len;
		(void) fi;
		if(strcmp(path, hello_path) != 0)
			return -ENOENT;

		len = strlen(hello_str);
		if (offset < len) {
			if (offset + size > len)
				size = len - offset;
			memcpy(buf, hello_str + offset, size);
		} else
			size = 0;

		return size;
}

static struct fuse_operations fuse_oper = { .getattr = fuse_getattr, .readdir =
		fuse_readdir, .open = fuse_open, .read = fuse_read, };

int main(int argc, char *argv[]) {
	return fuse_main(argc, argv, &fuse_oper, NULL);
}

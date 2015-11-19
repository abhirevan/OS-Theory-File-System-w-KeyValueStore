#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

static int fuse_getattr(const char *path, struct stat *stbuf) {
	printf("fuse_getattr invoked\n");
	return 0;
}

static int fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		off_t offset, struct fuse_file_info *fi) {
	printf("fuse_readdir invoked\n");
	return 0;
}

static int fuse_open(const char *path, struct fuse_file_info *fi) {
	printf("fuse_open invoked\n");
	return 0;
}

static int fuse_read(const char *path, char *buf, size_t size, off_t offset,
		struct fuse_file_info *fi) {
	printf("fuse_read invoked\n");
	return 0;
}

static struct fuse_operations fuse_oper = { .getattr = fuse_getattr, .readdir =
		fuse_readdir, .open = fuse_open, .read = fuse_read, };

int main(int argc, char *argv[]) {
	return fuse_main(argc, argv, &fuse_oper, NULL);
}

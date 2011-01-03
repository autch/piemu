/** @file fusepfi.c
 * mounting PFI files using FUSE: Filesystem in Userspace
 * 
 * @author: autch
 */

/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall `pkg-config fuse --cflags --libs` fusexmp.c -o fusexmp
*/

#define FUSE_USE_VERSION 26

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef linux
/* For pread()/pwrite() */
#define _XOPEN_SOURCE 500
#endif

#include <errno.h>
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>

#include "pffs.h"
#include "pfi.h"

static int fusepfi_access(const char *path, int mask)
{
	int res;
/*
	if(strcmp(path, "/") == 0
	   || strcmp(path, ".") == 0
	   || strcmp(path, "..") == 0)
	{
		return access(path, mask);
	}
*/
	return 0;
}

static int fusepfi_getattr(const char *path, struct stat *stbuf)
{
	struct fuse_context* ctx = fuse_get_context();
	PFI* pPfi = ctx->private_data;
	DIRECTORY* pDir;
	char* pp = path;
	int ret;

	if(strcmp(path, "/") == 0
	   || strcmp(path, ".") == 0
	   || strcmp(path, "..") == 0)
	{
		ret = lstat(path, stbuf);
		if(ret == -1)
			return -errno;
		else
			return 0;
	}
	
	if(*pp == '/') pp++;
	pDir = PFFSFindFile(pPfi, pp);
	if(pDir)
	{
		stbuf->st_nlink = 1;
		stbuf->st_mode = S_IFREG | 0660;
		stbuf->st_size = pDir->size;
		stbuf->st_uid = ctx->uid;
		stbuf->st_gid = ctx->gid;
	}
	else
	{
		ret = lstat(path, stbuf);
		if(ret == -1)
			return -errno;
	}

	return 0;
}

static int fusepfi_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
						   off_t offset, struct fuse_file_info *fi)
{
	struct fuse_context* ctx = fuse_get_context();
	PFI* pPfi = ctx->private_data;
	DIRECTORY* pDir;
	int i, dirs;
	struct stat st;

	dirs = PFFSDirCount(pPfi);
	for(i = 0; i < dirs; i++)
	{
		pDir = PFFSDir(pPfi, i);

		st.st_nlink = 1;
		st.st_size = pDir->size;
		st.st_mode = S_IFREG | 0660;
		st.st_uid = ctx->uid;
		st.st_gid = ctx->gid;
		filler(buf, pDir->name, NULL, 0);
	}

	return 0;
}

static int fusepfi_open(const char *path, struct fuse_file_info *fi)
{
	int res;

	res = open(path, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int fusepfi_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int fusepfi_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;

	(void) fi;
	fd = open(path, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int fusepfi_release(const char *path, struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) fi;
	return 0;
}

static int fusepfi_fsync(const char *path, int isdatasync,
		     struct fuse_file_info *fi)
{
	/* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

	(void) path;
	(void) isdatasync;
	(void) fi;
	return 0;
}

void* fusepfi_init(struct fuse_conn_info* conn)
{
	PFI* pPfi = malloc(sizeof(PFI));
	char* filename;

	filename = fuse_get_context()->private_data;

	PFIInit(pPfi);
	PFIOpen(pPfi, filename);

	return pPfi;
}

void fusepfi_destroy(void* data)
{
	PFI* pPfi = data;

	PFIClose(pPfi);
	PFIExit(pPfi);
	free(pPfi);
}

struct fusepfi_param
{
	char* filename;
	int is_help;
};

static struct fuse_operations fusepfi_oper = {
//	.access		= fusepfi_access,
	.getattr	= fusepfi_getattr,
	.readdir	= fusepfi_readdir,
	.open		= fusepfi_open,
	.read		= fusepfi_read,
//	.release	= fusepfi_release,
//	.fsync		= fusepfi_fsync,

	.init		= fusepfi_init,
	.destroy	= fusepfi_destroy,
};

struct fuse_opt fusepfi_opts[] =
{
	FUSE_OPT_KEY("-h", 0),
	FUSE_OPT_KEY("--help", 0),
	FUSE_OPT_END
};

static int fusepfi_process_arg(void *data, const char *arg, int key,
							   struct fuse_args *outargs)
{
	struct fusepfi_param *param = data;
	
	switch(key)
	{
	case FUSE_OPT_KEY_NONOPT:
		if(!param->filename)
		{
			param->filename = arg;
			return 0;
		}
		return 1;
	case 0:
		param->is_help = 1;
		return fuse_opt_add_arg(outargs, "-ho");
	default:
		return 1;
	}
}

int main(int argc, char *argv[])
{
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	struct fusepfi_param param = { NULL, 0 };

	if(fuse_opt_parse(&args, &param, fusepfi_opts, fusepfi_process_arg))
	{
		fprintf(stderr, "Failed to parse options\n");
		return 1;
	}

	return fuse_main(args.argc, args.argv, &fusepfi_oper, param.filename);
}

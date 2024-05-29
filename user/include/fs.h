#ifndef _FS_H_
#define _FS_H_ 1

#include <stdint.h>

// File nodes (both in-memory and on-disk)

// Bytes per file system block - same as page size
#define BLOCK_SIZE PAGE_SIZE
#define BLOCK_SIZE_BIT (BLOCK_SIZE * 8)

// Maximum size of a filename (a single path component), including null
#define MAXNAMELEN 128

// Maximum size of a complete pathname, including null
#define MAXPATHLEN 1024
#define FMODE_R 0x4
#define FMODE_W 0x2
#define FMODE_X 0x1
#define FMODE_RW 0x6
#define FMODE_ALL 0x7

#define STMODE2FMODE(st_mode) (((st_mode) >> 6) & FMODE_ALL)
// Number of (direct) block pointers in a File descriptor
#define NDIRECT 10
#define NINDIRECT (BLOCK_SIZE / 4)

#define MAXFILESIZE (NINDIRECT * BLOCK_SIZE)

#define FILE_STRUCT_SIZE 256

struct File {
	char f_name[MAXNAMELEN]; // filename
	uint32_t f_size;	 // file size in bytes
	uint32_t f_type;	 // file type
	uint32_t f_direct[NDIRECT];
	uint32_t f_indirect;
	uint32_t f_mode;
	struct File *f_dir; // the pointer to the dir where this file is in, valid only in memory.
	char f_pad[FILE_STRUCT_SIZE - MAXNAMELEN - (4 + NDIRECT) * 4 - sizeof(void *)];
} __attribute__((aligned(4), packed));

#define FILE2BLK (BLOCK_SIZE / sizeof(struct File))

// File types
#define FTYPE_REG 0 // Regular file
#define FTYPE_DIR 1 // Directory

// File system super-block (both in-memory and on-disk)

#define FS_MAGIC 0x68286097 // Everyone's favorite OS class

struct Super {
	uint32_t s_magic;   // Magic number: FS_MAGIC
	uint32_t s_nblocks; // Total number of blocks on disk
	struct File s_root; // Root directory node
};

#endif // _FS_H_

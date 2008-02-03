#ifndef PFFS_H
#define PFFS_H

// from /usr/piece/include/piece.h

// piece flash file system

#define MAXDIR 96
#define MAXFAT 496

typedef struct tagpffsMARK
{
  unsigned long ptr;
  unsigned long resv;
  char signature[24];
}pffsMARK;

typedef struct tagDIRECTORY
{
  char name[24];
  unsigned char attr;
  unsigned char resv;
  unsigned short chain;
  unsigned long size;
}DIRECTORY;

typedef struct tagFAT
{
  unsigned short chain;
} FAT;

typedef struct tagpffsMASTERBLOCK
{
  pffsMARK mark;
  DIRECTORY dir[MAXDIR];
  FAT fat[MAXFAT];
} pffsMASTERBLOCK;

#define FAT_FREE    0xffff
#define FAT_END     0xeeee
#define FAT_INVALID 0xdddd
#define FAT_SYSTEM  0xcccc

#endif // !PFFS_H

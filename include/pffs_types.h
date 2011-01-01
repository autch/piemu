#ifndef PFFS_TYPES_H
#define PFFS_TYPES_H

#include "types.h"

// from /usr/piece/include/piece.h
// piece flash file system

#define MAXDIR 96
#define MAXFAT 496

typedef struct pffsMARK
{
  c33word ptr;
  c33word resv;
  c33char signature[24];
}pffsMARK;

typedef struct DIRECTORY
{
  c33char name[24];
  c33byte attr;
  c33byte resv;
  c33hword chain;
  c33word size;
}DIRECTORY;

typedef struct FAT
{
  c33hword chain;
} FAT;

typedef struct pffsMASTERBLOCK
{
  pffsMARK mark;
  DIRECTORY dir[MAXDIR];
  FAT fat[MAXFAT];
} pffsMASTERBLOCK;

#define FAT_FREE    0xffff
#define FAT_END     0xeeee
#define FAT_INVALID 0xdddd
#define FAT_SYSTEM  0xcccc

#endif

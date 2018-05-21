/** @file pfi_types.h
 * P/EMU/SDL Flash Image format and its filesystem
 */

#ifndef PFI_TYPES_H
#define PFI_TYPES_H

#include "types.h"
#include "piece_types.h"
#include "pffs_types.h"

#ifndef FLASH_TOP
#define FLASH_TOP (0x0c00000)
#endif

typedef struct pfiheader
{
  c33word    signature; // 'PFI1', '1' indicates its version
  c33word    offset;    // offset, in bytes, to the flash image
  SYSTEMINFO sysinfo;   // machine information
  // ... and flash image follows.
}PFIHEADER;

typedef struct pfi
{
  PFIHEADER header; // header of pfi
  c33byte*  buffer; // buffer to the content
  c33word   size;   // size, in bytes, of the content buffer.
  pffsMASTERBLOCK* msb;
}PFI;

#endif

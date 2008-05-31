/** @file pfi_types.h
 * P/EMU/SDL Flash Image format
 */

#ifndef PFI_TYPES_H
#define PFI_TYPES_H

#include "types.h"
#include "piece_types.h"

typedef struct pfiheader
{
  c33word    dwSignature;     // 'PFI1', '1' indicates its version
  c33word    dwOffsetToFlash; // offset, in bytes, to the flash image
  SYSTEMINFO siSysInfo;       // machine information
  // ... and flash image follows.
}PFIHEADER;

#endif

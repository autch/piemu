/** @file mkpfi.c
 * generate PFI format image from raw BIOS image
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "pffs.h"
#include "pfi.h"

#define KERNEL_SIGNATURE "Piece Kernel Program"

// 初期値。pffs_end を正しい値にするのを忘れないこと
SYSTEMINFO siDefault =
{
  0x0020, 0x0100, 0x011d, 0x0aa7, 0x016e3600, 0x0ce4, 0x0000,
  0x0100000, 0x0140000, 0x0c28000, 0x0c00000
};

int usage()
{
  fprintf(stderr, "usage: mkpfi [-512kb|-2mb] all.bin [piece.pfi]\n");
  return -1;
}

int run(char* szSource, char* szDest, PFIHEADER* pfiH);

int main(int ac, char** av)
{
  char* szSource = NULL, *szDest = NULL;
  PFIHEADER pfi;

  pfi.signature = 'PFI1';
  pfi.offset = sizeof(PFIHEADER);
  pfi.sysinfo = siDefault;
  pfi.sysinfo.pffs_end = siDefault.pffs_end + (512 << 10); // defaults 512kb

  while(*++av)
  {
    if(strcmp(*av, "-512kb") == 0)
    {
      pfi.sysinfo.pffs_end = siDefault.pffs_end + (512 << 10); // 512kb
    }
    if(strcmp(*av, "-2mb") == 0)
    {
      pfi.sysinfo.pffs_end = siDefault.pffs_end + (2048 << 10); // 2MB
    }
    if(!szSource && **av != '-')
      szSource = *av;
    else if(!szDest && szSource && **av != '-')
      szDest = *av;
  }

  if(!szSource)
    return usage();
  if(!szDest)
    szDest = "piece.pfi";

  return run(szSource, szDest, &pfi);
}

int run(char* szSource, char* szDest, PFIHEADER* pfiH)
{
  char szBuffer[4096];
  const c33byte BootAddress[4] = { 0x04, 0x20, 0xc0, 0x00 }; // 0x00c02004
  FILE* fpi;
  FILE* fpo;
  int nBytesRead, nSectors;

  fpi = fopen(szSource, "rb");
  if(!fpi)
  {
    fprintf(stderr, "mkpfi: cannot open all.bin as %s\n", szSource);
    return usage();
  }
  fread(szBuffer, 1, 8 + strlen(KERNEL_SIGNATURE), fpi);
  if(strncmp(szBuffer + 8, KERNEL_SIGNATURE, strlen(KERNEL_SIGNATURE)))
  {
    fprintf(stderr, "mkpfi: %s is not valid all.bin image\n", szSource);
    fclose(fpi);
    return usage();
  }
  fseek(fpi, 0, SEEK_SET);

  fpo = fopen(szDest, "wb");
  if(!fpo)
  {
    fprintf(stderr, "mkpfi: cannot open flash.pfi as %s\n", szDest);
    fclose(fpi);
    return usage();
  }
  printf("creating PFI %s from all.bin image %s\n", szDest, szSource);

  fwrite(pfiH, 1, sizeof(PFIHEADER), fpo);

  memset(szBuffer, 0xff, 4096);
  memcpy(szBuffer, BootAddress, 4);
  fwrite(szBuffer, 1, 4096, fpo);
  memset(szBuffer, 0xff, 4096);
  fwrite(szBuffer, 1, 4096, fpo);

  // # of sectors
  nSectors = (pfiH->sysinfo.pffs_end - FLASH_TOP) >> 12;
  // 2 sectors i've written above
  nSectors -= 2;

  // xfer sectors from the image
  while((nBytesRead = fread(szBuffer, 1, 4096, fpi)))
  {
    fwrite(szBuffer, 1, nBytesRead, fpo);
    nSectors--;
  }

  // write rest of image
  memset(szBuffer, 0xff, 4096);
  while(nSectors > 0)
  {
    fwrite(szBuffer, 1, 4096, fpo);
    nSectors--;
  }

  fclose(fpo);
  fclose(fpi);

  return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "psi.h"
#include "../pfi.h"

#define KERNEL_SIGNATURE "Piece Kernel Program"

// èâä˙ílÅBpffs_end Çê≥ÇµÇ¢ílÇ…Ç∑ÇÈÇÃÇñYÇÍÇ»Ç¢Ç±Ç∆
SYSTEMINFO siDefault =
{
  0x0020, 0x0100, 0x011d, 0x0aa7, 0x016e3600, 0x0ce4, 0x0000,
  (unsigned char*)0x0100000, (unsigned char*)0x0140000,
  (unsigned char*)0x0c28000, (unsigned char*)0x0c00000
};

void usage()
{
  fprintf(stderr, "usage: mkpfi [-512kb|-2mb] all.bin [piece.pfi]\n");
  exit(-1);
}

int main(int ac, char** av)
{
  char* szSource = NULL, *szDest = NULL;
  PFIHEADER pfi;

  pfi.dwSignature = 'PFI1';
  pfi.dwOffsetToFlash = sizeof(PFIHEADER);
  pfi.siSysInfo = siDefault;
  pfi.siSysInfo.pffs_end = siDefault.pffs_end + (512 << 10); // defaults 512kb

  while(*++av)
  {
    if(strcmp(*av, "-512kb") == 0)
    {
      pfi.siSysInfo.pffs_end = siDefault.pffs_end + (512 << 10); // 512kb
    }
    if(strcmp(*av, "-2mb") == 0)
    {
      pfi.siSysInfo.pffs_end = siDefault.pffs_end + (2048 << 10); // 2MB
    }
    if(!szSource && **av != '-')
      szSource = *av;
    else if(!szDest && szSource && **av != '-')
      szDest = *av;
  }
  if(!szSource)
  {
    usage();
  }
  if(!szDest)
  {
    szDest = "piece.pfi";
  }
  char szBuffer[4096];
  FILE* fpi = fopen(szSource, "rb");
  if(!fpi)
  {
    fprintf(stderr, "mkpfi: cannot open all.bin as %s\n", szSource);
    usage();
  }
  fread(szBuffer, 1, 8 + strlen(KERNEL_SIGNATURE), fpi);
  if(strncmp(szBuffer + 8, KERNEL_SIGNATURE, strlen(KERNEL_SIGNATURE)))
  {
    fprintf(stderr, "mkpfi: %s seems not to be an form of all.bin\n", szSource);
    fclose(fpi);
    usage();
  }
  fseek(fpi, 0, SEEK_SET);

  FILE* fpo = fopen(szDest, "wb");
  if(!fpo)
  {
    fprintf(stderr, "mkpfi: cannot open flash.pfi as %s\n", szDest);
    fclose(fpi);
    usage();
  }
  printf("creating pfi image file %s from all.bin-form image file %s\n", szDest, szSource);

  fwrite(&pfi, 1, sizeof(PFIHEADER), fpo);
  memset(szBuffer, 0xff, 4096);
  *(unsigned long*)szBuffer = 0x00c02004;
  fwrite(szBuffer, 1, 4096, fpo);
  memset(szBuffer, 0xff, 4096);
  fwrite(szBuffer, 1, 4096, fpo);
  int nBytesRead;
  while((nBytesRead = fread(szBuffer, 1, 4096, fpi)))
  {
    fwrite(szBuffer, 1, nBytesRead, fpo);
  }
  fclose(fpo);
  fclose(fpi);
}

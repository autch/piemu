
#include <windows.h>
#include <stdio.h>
#include "pieceif.h"
#include "psi.h"
#include "../pfi.h"

#pragma comment(lib, "pieceif.lib")

#define FLASH_TOP 0x0c00000
#define READ_UNIT 0x1000        // 4096

int die(char* msg)
{
  fprintf(stderr, "FlashRipper: %s\n", msg);
  exit(-1);
  return 0;
}

int main()
{
  int r;
  SYSTEMINFO sInfo;

  ismInit() && die("cannot connect to piece");

  // emu.c
  r = ismGetVersion(&sInfo, 1);
  if(r == PIECE_INVALID_VERSION) die("piece kernel is too old");

  printf("BIOS version = %d.%02d\n", r >> 8, r & 0xff);
  printf("BIOS date    = %d.%02d.%02d\n", 2000 + (sInfo.bios_date >> 9), (sInfo.bios_date >> 5) & 0x0f, sInfo.bios_date & 31);
  printf("SRAM top adr = 0x%x\n", sInfo.sram_top);
  printf("SRAM end adr = 0x%x\n", sInfo.sram_end - 1);
  printf("SRAM size    = %d KB\n", (sInfo.sram_end - sInfo.sram_top) >> 10);
  printf("HW version   = %d.%02d\n", sInfo.hard_ver >> 8, sInfo.hard_ver & 255);
  printf("SYSTEM clock = %5.3f MHz\n", sInfo.sys_clock / 1e6);
  printf("VDDE voltage = %5.3f V\n", sInfo.vdde_voltage / 1e3);
  printf("PFFS top adr = 0x%x\n", sInfo.pffs_top);
  printf("PFFS end adr = 0x%x\n", sInfo.pffs_end - 1);

  // ‚¤‚Í‚—‚—‚—‚—’±ºËßÍß‚—‚—‚—‚—‚—‚—‚—Ã×¾º½‚—‚—‚v‚—‚—‚—‚¤‚¥‚¦‚¤‚¥
  int size = (unsigned int)sInfo.pffs_end - FLASH_TOP;
  int bits;
  for(bits = 0; 1 << bits < size; bits++);
  unsigned int dwBytesToRead = 1 << bits;

  PFIHEADER pfi;
  pfi.dwSignature = 'PFI1';
  pfi.dwOffsetToFlash = sizeof(PFIHEADER);
  pfi.siSysInfo = sInfo;

  FILE* fp = fopen("piece.pfi", "wb");

  fwrite(&pfi, sizeof(PFIHEADER), 1, fp);

  unsigned char buffer[READ_UNIT];

  printf("Reading flash memory");
  for(unsigned int p = FLASH_TOP; p < FLASH_TOP + dwBytesToRead; p += READ_UNIT)
  {
    ismReadMem(buffer, p, READ_UNIT);
    fwrite(buffer, 1, READ_UNIT, fp);
    if((p / READ_UNIT) % 10 == 9) printf(".");
  }
  printf("done\n");

  fclose(fp);
  ismExit();

}

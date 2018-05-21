#ifndef WINDOWS
#error This tool requires Microsoft Windows.
#endif

#include <stdio.h>
#include "pieceif.h"
#include "pfi.h"

#pragma comment(lib, "pieceif.lib")

#define FLASH_TOP 0x0c00000
#define READ_UNIT 0x1000        // 4096

int die(char* msg);
void DumpSystemInfo(SYSTEMINFO* psi);
int ReadFlashImage(SYSTEMINFO* psi);

int main()
{
  int r;
  SYSTEMINFO sInfo;

  if(ismInit())
    return die("cannot connect to piece");

  r = ismGetVersion(&sInfo, 1);
  if(r == PIECE_INVALID_VERSION)
    return die("piece kernel is too old");

  DumpSystemInfo(&sInfo);

  ismExit();

  return 0;
}

int die(char* msg)
{
  fprintf(stderr, "ripper: %s\n", msg);
  return -1;
}

void DumpSystemInfo(SYSTEMINFO* psi)
{
  printf("P/ECE H/W v%d.%02d, BIOS v%d.%02d (%d.%02d.%02d), "
	 "running at %5.3f MHz, %5.3f V\n",
	 psi->hard_ver >> 8, psi->hard_ver & 255,
	 psi->bios_ver >> 8, psi->bios_ver & 255,
	 2000 + (psi->bios_date >> 9), (psi->bios_date >> 5) & 15,
	 psi->bios_date & 31,
	 psi->sys_clock / 1e6, psi->vdde_voltage / 1e3);
  printf("SRAM addr 0x%08x - 0x%08x, %4d KiB\n",
	 psi->sram_top, psi->sram_end-1,
	 (psi->sram_end - psi->sram_top) >> 10);
  printf("PFFS addr 0x%08x - 0x%08x, %4d KiB\n",
	 psi->pffs_top, psi->pffs_end-1,
	 (psi->pffs_end - psi->pffs_top) >> 10);
}

// derived from nsawa's original p/emu
int ReadFlashImage(SYSTEMINFO* psi)
{
  int size, bits;
  c33word bytesToRead, p;
  PFIHEADER pfi;
  FILE* fp;
  c33byte buffer[READ_UNIT];
  
  pfi.signature = 'PFI1';
  pfi.offset = sizeof(PFIHEADER);
  pfi.sysinfo = *psi;

  fp = fopen("piece.pfi", "wb");
  if(!fp)
    return die("cannot open piece.pfi for writing");

  fwrite(&pfi, sizeof(PFIHEADER), 1, fp);

  size = psi->pffs_end - FLASH_TOP;
  for(bits = 1; bits < size; bits <<= 1);
  bytesToRead = bits;

  printf("Reading flash memory");
  for(p = FLASH_TOP; p < FLASH_TOP + bytesToRead; p += READ_UNIT)
  {
    ismReadMem(buffer, p, READ_UNIT);
    fwrite(buffer, 1, READ_UNIT, fp);
    if((p / READ_UNIT) % 10 == 9) printf(".");
  }
  printf("done\n");

  fclose(fp);

  return 0;
}

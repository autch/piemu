/** @pfifile.c
 * PFI editor functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "pfi.h"
#include "pffs.h"

void PFIInit(PFI* pfi)
{
  memset(pfi, 0, sizeof(PFI));
}

void PFIExit(PFI* pfi)
{
  PFIClose(pfi);
}

int PFIOpen(PFI* pfi, char* pFilename)
{
  int r = 0;
  c33byte* p;
  FILE* fp;
  c33word flashsize;

  fp = fopen(pFilename, "rb");
  if(fp && fread(&pfi->header, sizeof(PFIHEADER), 1, fp))
  {
    if((pfi->header.signature & 0xffffff00) == ('PFI0' & 0xffffff00))
    {
      fseek(fp, pfi->header.offset, SEEK_SET);

      flashsize = pfi->header.sysinfo.pffs_end - FLASH_TOP;
      for(pfi->size = 1; pfi->size < flashsize; pfi->size <<= 1)
	;
      pfi->buffer = malloc(pfi->size);
      if(pfi->buffer)
      {
	for(p = pfi->buffer; p < pfi->buffer + pfi->size; p += 1 << 12)
	  fread(p, 1, 1 << 12, fp);
	r = 1;
      }
    }
  }
  if(fp) fclose(fp);
  PFFSLoadMasterBlock(pfi);

  return r;
}

void PFIClose(PFI* pfi)
{
  if(pfi->buffer)
  {
    free(pfi->buffer);
    pfi->buffer = NULL;
  }
}

int PFISave(PFI* pfi, char* pFilename)
{
  FILE* fp;
  fp = fopen(pFilename, "wb");
  if(!fp) return 0;
  pfi->header.offset = sizeof(PFIHEADER);
  fwrite(&pfi->header, sizeof(PFIHEADER), 1, fp);
  fwrite(pfi->buffer, 1, pfi->size, fp);
  fclose(fp);
  return 1;
}

c33byte* PFIGetPFFSTop(PFI* pfi)
{
  return pfi->buffer + (pfi->header.sysinfo.pffs_top - FLASH_TOP);
}

c33word PFIGetPFFSMaxFAT(PFI* pfi)
{
  int maxfat;
  SYSTEMINFO* pSysInfo;

  pSysInfo = &pfi->header.sysinfo;
  maxfat = (pSysInfo->pffs_end - pSysInfo->pffs_top) >> 12;
  if(maxfat > MAXFAT) maxfat = MAXFAT;

  return maxfat;
}

void PFIDumpSystemInfo(PFI* pfi)
{
  SYSTEMINFO* psi;

  psi = &pfi->header.sysinfo;
  printf("P/ECE H/W v%d.%02d, BIOS v%d.%02d (%d.%02d.%02d), "
	 "running at %5.3f MHz, %5.3f V\n",
	 psi->hard_ver >> 8, psi->hard_ver & 255,
	 psi->bios_ver >> 8, psi->bios_ver & 255,
	 2000 + (psi->bios_date >> 9), (psi->bios_date >> 5) & 15,
	 psi->bios_date & 31,
	 psi->sys_clock / 1e6, psi->vdde_voltage / 1e3
    );
  printf("SRAM addr 0x%08x - 0x%08x, %4d KiB\n",
	 psi->sram_top, psi->sram_end-1,
	 (psi->sram_end - psi->sram_top) >> 10);
  printf("PFFS addr 0x%08x - 0x%08x, %4d KiB\n",
	 psi->pffs_top, psi->pffs_end-1,
	 (psi->pffs_end - psi->pffs_top) >> 10);
}

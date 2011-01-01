#ifndef PFI_H
#define PFI_H

#include "types.h"
#include "pfi_types.h"

void PFIInit(PFI* pfi);
void PFIExit(PFI* pfi);
int PFIOpen(PFI* pfi, char* pFilename);
void PFIClose(PFI* pfi);
int PFISave(PFI* pfi, char* pFilename);
c33byte* PFIGetPFFSTop(PFI* pfi);
c33word PFIGetPFFSMaxFAT(PFI* pfi);
void PFIDumpSystemInfo(PFI* pfi);

#endif

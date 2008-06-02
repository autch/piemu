
#ifndef PFFS_H
#define PFFS_H

#include "pfi.h"
#include "pffs_types.h"

void PFFSLoadMasterBlock(PFI* pfi);
c33word PFFSDirCount(PFI* pfi);
DIRECTORY* PFFSDir(PFI* pfi, int nIndex);
c33word PFFSFree(PFI* pfi);
void PFFSDumpDirEntries(PFI* pfi);
int PFFSExtractFile(PFI* pfi, char* pPFFSFileName, char* pDiskFileName);
int PFFSDeleteFile(PFI* pfi, char* pPFFSFileName);
int PFFSAddFile(PFI* pfi, char* pFileName);


#endif

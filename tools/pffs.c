
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "pffs.h"
#include "pfi.h"

int CheckFileName(const char* szFileName);
DIRECTORY* FindNextDirEntry(PFI* pfi, DIRECTORY** pDir);
DIRECTORY* FindFile(PFI* pfi, char* szFileName);
c33byte* GetFilesNthSector(PFI* pfi, DIRECTORY* pDir, int nSector);
c33byte* SectorToPointer(PFI* pfi, int nSector);
c33word GetFreeFAT(PFI* pfi, FAT* pFat, int i);
DIRECTORY* FindFreeDir(PFI* pfi);
int WriteFileToSectors(PFI* pfi, DIRECTORY* pDir, char* szFileName,
		       int nSectors);
int AllocateSectors(PFI* pfi, DIRECTORY* pDir, int nSectors, int nOldSectors);

void PFFSLoadMasterBlock(PFI* pfi)
{
  pfi->msb = (pffsMASTERBLOCK*)PFIGetPFFSTop(pfi);
}

int CheckFileName(const char* szFileName)
{
  int part;
  for(part = 0; part < 2; part++)
  {
    int length = 0;
    char c;
    while(1)
    {
      c = *szFileName++;
      if(!((c >= '0' && c <= '9')
	   || (c >= 'a' && c <= 'z')
	   || (c == '_')
	   )
	)
	break;
      length++;
    }

    if(!part)
    {
      if(length == 0 || length > 8) return 1;
      if(!c) return 0;
      if(c != '.') return 2;
    }
    else
    {
      if(length == 0 || length > 3) return 3;
      if(!c) return 0;
    }
  }
  return 4;
}

// ファイルが割り当てられているディレクトリエントリを順番に返す
DIRECTORY* FindNextDirEntry(PFI* pfi, DIRECTORY** pDir)
{
  DIRECTORY* p;
  c33byte c;
  while(1)
  {
    p = (*pDir)++;
    if(p - pfi->msb->dir >= MAXDIR) return NULL;
    c = p->name[0];
    if(c && c != 0xff) return p;
  }
  return NULL;
}

// ファイル名にマッチするディレクトリエントリを返す
DIRECTORY* FindFile(PFI* pfi, char* szFileName)
{
  DIRECTORY* pDir = NULL;
  DIRECTORY* pWork;
  pWork = pfi->msb->dir;
  if(CheckFileName(szFileName)) return NULL;
  while(1)
  {
    pDir = FindNextDirEntry(pfi, &pWork);
    if(!pDir) break;
    if(!strncmp((char*)pDir->name, (char*)szFileName, 24)) break;
  }
  return pDir;
}

// ファイル pDir の nSector 番目のセクタへのポインタを得る
c33byte* GetFilesNthSector(PFI* pfi, DIRECTORY* pDir, int nSector)
{
  int c;
  c = pDir->chain;
  while(nSector--) c = pfi->msb->fat[c].chain;
  return PFIGetPFFSTop(pfi) + (c << 12);
}

// ファイルシステム先頭から数えたセクタ番号をポインタへ変換
c33byte* SectorToPointer(PFI* pfi, int nSector)
{
  return PFIGetPFFSTop(pfi) + (nSector << 12);
}

// i 番目以降から空き FAT を探す
c33word GetFreeFAT(PFI* pfi, FAT* pFat, int i)
{
  // maximum available sectors
  for(; i < PFIGetPFFSMaxFAT(pfi); i++ )
  {
    if(pFat[i].chain == FAT_FREE) return i;
  }
  return -1;
}

c33word PFFSDirCount(PFI* pfi)
{
  DIRECTORY* pDir = NULL;
  DIRECTORY* pWork;
  int n = 0;

  pWork = pfi->msb->dir;
  while((pDir = FindNextDirEntry(pfi, &pWork)))
    n++;
  return n;
}

// 0 <= nIndex <= PFFSDirCount()
DIRECTORY* PFFSDir(PFI* pfi, int nIndex)
{
  DIRECTORY* pDir = NULL;
  DIRECTORY* pWork;

  pWork = pfi->msb->dir;
  do
  {
    if(!(pDir = FindNextDirEntry(pfi, &pWork)))
      return NULL;
  }while(nIndex-- > 0);
  return pDir;
}

// 空き FAT の個数を返す。<<12 するとバイト単位になる。
c33word PFFSFree(PFI* pfi)
{
  int n = 0, i;

  for(i = 0; i < PFIGetPFFSMaxFAT(pfi); i++)
    if(pfi->msb->fat[i].chain == FAT_FREE) n++;
  return n;
}

// for debugging
void PFFSDumpDirEntries(PFI* pfi)
{
  int i;
  printf("idx: %-24s attr chain %10s\n", "filename", "size");
  for(i = 0; i < PFFSDirCount(pfi); i++)
  {
    DIRECTORY* pDir = PFFSDir(pfi, i);
    if(!pDir) break;
    printf("%3d: %-24s  %02x  %04x  %10d\n",
	   pDir - pfi->msb->dir, pDir->name,
	   pDir->attr, pDir->chain, pDir->size);
  }
  printf("\n%d sectors (%d bytes) free\n",
	 PFFSFree(pfi), PFFSFree(pfi) << 12);
}

int PFFSExtractFile(PFI* pfi, char* pPFFSFileName, char* pDiskFileName)
{
  DIRECTORY* pDir;
  c33word dwFileSize;
  int nSector;
  FILE* fp;
  int nBytesToRead, nBytesRead;

  pDir = FindFile(pfi, pPFFSFileName);
  if(!pDir) return 0;

  if(!pDiskFileName) pDiskFileName = pPFFSFileName;

  fp = fopen(pDiskFileName, "wb");
  if(!fp) return 0;

  dwFileSize = pDir->size;
  nSector = pDir->chain;
  while(dwFileSize && nSector != FAT_END)
  {
    nBytesToRead = (dwFileSize > 4096) ? 4096 : dwFileSize;
    nBytesRead = fwrite(SectorToPointer(pfi, nSector), 1, nBytesToRead, fp);
    dwFileSize -= nBytesRead;
    nSector = pfi->msb->fat[nSector].chain;
  }
  fclose(fp);
  return 1;
}

int PFFSDeleteFile(PFI* pfi, char* pPFFSFileName)
{
  pffsMASTERBLOCK new_msb;
  pffsMASTERBLOCK* old_msb;
  DIRECTORY* pDir;
  int nSector, n;

  old_msb = pfi->msb;
  new_msb = *pfi->msb;
  pfi->msb = &new_msb;
  // この pDir は msb の中を指すので↑のようにポインタごと差し替える必要がある
  pDir = FindFile(pfi, pPFFSFileName);
  if(!pDir) return 0;
  nSector = pDir->chain;
  while(nSector != FAT_END)
  {
    n = new_msb.fat[nSector].chain;
    new_msb.fat[nSector].chain = FAT_FREE;
    nSector = n;
  }
  memset(pDir, 0xff, sizeof(DIRECTORY));
  *old_msb = new_msb;
  pfi->msb = old_msb;
  return 1;
}

DIRECTORY* FindFreeDir(PFI* pfi)
{
  DIRECTORY* pDir;
  int i;

  pDir = pfi->msb->dir;
  for(i = 0; i < MAXDIR; i++)
  {
    if((c33byte)pDir->name[0] == 0xff)
      return pDir;
    pDir++;
  }
  return NULL;
}

int PFFSAddFile(PFI* pfi, char* pFileName)
{
  FILE* fp;
  c33word dwSize;
  pffsMASTERBLOCK new_msb;
  pffsMASTERBLOCK *old_msb;
  int nSectors, nOldSectors, r = 0;
  DIRECTORY* pDir;

  if(CheckFileName(pFileName))
    return 0;

  // measuring size of disk file
  fp = fopen(pFileName, "rb");
  if(!fp) return 0;
  fseek(fp, 0, SEEK_END);
  dwSize = ftell(fp);
  fclose(fp);
  
  nSectors = (dwSize + 4095) >> 12;
  nOldSectors = 0;

  // MSB のバックアップを作る
  old_msb = pfi->msb;
  new_msb = *pfi->msb;
  pfi->msb = &new_msb;

  // find first free DirEntry
  pDir = FindFile(pfi, pFileName);
  if(!pDir)
  {
    pDir = FindFreeDir(pfi);
    if(pDir)
    {
      memset(pDir, 0, sizeof(DIRECTORY));
      strncpy((char*)pDir->name, pFileName, (sizeof pDir->name) - 1);
      pDir->chain = FAT_FREE;
    }
    else
      goto CreateEnd;
  }
  else
  {
    nOldSectors = (pDir->size + 4095) >> 12;
  }
  pDir->size = dwSize;

  if(!AllocateSectors(pfi, pDir, nSectors, nOldSectors))
    goto CreateEnd;
  if(!WriteFileToSectors(pfi, pDir, pFileName, nSectors))
    goto CreateEnd;

CreateSuccess:
  r = 1;
  *old_msb = new_msb;
CreateEnd:
  pfi->msb = old_msb;

  return r;
}

int WriteFileToSectors(PFI* pfi, DIRECTORY* pDir, char* szFileName,
		       int nSectors)
{
  FILE* fp;
  int nSector, i;

  fp = fopen(szFileName, "rb");
  if(!fp) return 0;
  nSector = pDir->chain;

  // ようやくファイル書き込み
  for(i = 0; i < nSectors; i++)
  {
    fread(SectorToPointer(pfi, nSector), 1, 4096, fp);
    nSector = pfi->msb->fat[nSector].chain;
  }

  fclose(fp);
  return 1;
}

int AllocateSectors(PFI* pfi, DIRECTORY* pDir, int nSectors, int nOldSectors)
{
  c33hword* pChain;
  int n, nn = 0, nSector = 0;
  FAT* pFat;

  // 新旧ファイルのクラスタチェイン先頭
  pChain = &pDir->chain;
  // FAT 先頭
  pFat = pfi->msb->fat;

  // 準備したいセクタ数だけ回る
  for(nSector = 0; nSector < nSectors; nSector++)
  {
    n = *pChain;
    if(n == FAT_FREE || n == FAT_END)
    {
      // 旧ファイルよりもたくさんセクタがほしい
      n = GetFreeFAT(pfi, pFat, nn);// nn は最後に未使用セクタを見つけたところ
      if(n < 0) return 0;           // 空きなし
      *pChain = n;                  // チェインの次の要素にする
      nn = n + 1;
    }
    pChain = &pFat[n].chain;        // その次の要素があればそれを使う
    // なければまた割り当てる
  }

  nn = *pChain;                     // 最後の新規セクタ
  *pChain = FAT_END;

  // 余った未使用セクタに FREE マークをつける
  for(; nSector < nOldSectors; nSector++)
  {
    pChain = &pFat[nn].chain;
    nn = *pChain;
    *pChain = FAT_FREE;
    if(nn == FAT_FREE || nn == FAT_END) break;
  }

  return 1;
}

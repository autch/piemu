#ifndef CAUPFFS_H
#define CAUPFFS_H

#include "CAuPFIFile.h"
#include "pffs.h"

#define FLASH_TOP 0x0c00000

class CAuPFFS
{
private:
  CAuPFIFile m_pfiFile;
  pffsMASTERBLOCK* m_pMSB;

  int CheckFileName(const char* szFileName)
  {
    for(int part = 0; part < 2; part++)
    {
      int length = 0;
      char c;
      while(1)
      {
        c = *szFileName++;
        if(!(   (c >= '0' && c <= '9')
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

  VOID LoadMasterBlock()
  {
    m_pMSB = (pffsMASTERBLOCK*)m_pfiFile.GetPFFSTop();
  }
  // ファイルが割り当てられているディレクトリエントリを順番に返す
  DIRECTORY* FindNextDirEntry(DIRECTORY** pDir)
  {
    while(1)
    {
      DIRECTORY* p = (*pDir)++;
      if(p - m_pMSB->dir >= MAXDIR) return NULL;
      unsigned char c = p->name[0];
      if(c && c != 0xff) return p;
    }
    return NULL;
  }
  // ファイル名にマッチするディレクトリエントリを返す
  DIRECTORY* FindFile(char* szFileName)
  {
    DIRECTORY* pDir = NULL;
    DIRECTORY* pWork = m_pMSB->dir;
    if(CheckFileName(szFileName)) return FALSE;
    while(1)
    {
      pDir = FindNextDirEntry(&pWork);
      if(!pDir) break;
      if(!strcmp(pDir->name, szFileName)) break;
    }
    return pDir;
  }
  // ファイル pDir の nSector 番目のセクタへのポインタを得る
  BYTE* GetFilesNthSector(DIRECTORY* pDir, int nSector)
  {
    int c = pDir->chain;
    while(nSector--) c = m_pMSB->fat[c].chain;
    return m_pfiFile.GetPFFSTop() + (c << 12);
  }
  // ファイルシステム先頭から数えたセクタ番号をポインタへ変換
  BYTE* SectorToPointer(int nSector)
  {
    return m_pfiFile.GetPFFSTop() + (nSector << 12);
  }
  // i 番目以降から空き FAT を探す
  DWORD GetFreeFAT(FAT* pFat, int i)
  {
    int MAXFAT2 = ((m_pfiFile.GetSystemInfo()->pffs_end - m_pfiFile.GetPFFSTop()) >> 12);
    if(MAXFAT2 > MAXFAT) MAXFAT2 = MAXFAT;
    for(; i < MAXFAT2; i++ )
    {
      if(pFat[i].chain == FAT_FREE) return i;
    }
    return -1;
  }
public:
  CAuPFFS() : m_pfiFile() {}
  ~CAuPFFS() { Close(); }
  CAuPFIFile* GetPFIFile() { return &m_pfiFile; }
  BOOL Open(LPSTR szFileName)
  {
    if(!m_pfiFile.Open(szFileName)) return FALSE;
    LoadMasterBlock();
    return TRUE;

  }
  VOID Close()
  {
    m_pfiFile.Close();
  }
  DWORD PFFSDirCount()
  {
    DIRECTORY* pDir = NULL;
    DIRECTORY* pWork = m_pMSB->dir;
    int n = 0;
    while(pDir = FindNextDirEntry(&pWork)) n++;
    return n;
  }
  // 0 <= nIndex <= PFFSDirCount()
  DIRECTORY* PFFSDir(DWORD nIndex)
  {
    DIRECTORY* pDir = NULL;
    DIRECTORY* pWork = m_pMSB->dir;
    do { if(!(pDir = FindNextDirEntry(&pWork))) return NULL; } while(nIndex-- > 0);
    return pDir;
  }

  // 空き FAT の個数を返す。<<12 するとバイト単位になる。
  DWORD PFFSFree()
  {
    int n = 0;
    int MAXFAT2 = ((m_pfiFile.GetSystemInfo()->pffs_end - m_pfiFile.GetPFFSTop()) >> 12);
    if(MAXFAT2 > MAXFAT) MAXFAT2 = MAXFAT;
    for(int i = 0; i < MAXFAT2; i++)
      if(m_pMSB->fat[i].chain == FAT_FREE) n++;
    return n;
  }
  // for debugging
  VOID DumpDirEntries()
  {
    printf  ("idx: %-24s attr chain %10s\n", "filename", "size");
    int n = PFFSDirCount();
    for(int i = 0; i < n; i++)
    {
      DIRECTORY* pDir = PFFSDir(i);
      if(!pDir) break;
      printf("%3d: %-24s  %02x  %04x  %10d\n", pDir - m_pMSB->dir, pDir->name,
      pDir->attr, pDir->chain, pDir->size);
    }
    printf("\n%d sectors (%d bytes) free\n", PFFSFree(), PFFSFree() << 12);
  }
  BOOL ExtractFile(LPSTR szPFFSFileName, LPSTR szDiskFileName = NULL)
  {
    DIRECTORY* pDir = FindFile(szPFFSFileName);
    if(!pDir) return FALSE;

    if(!szDiskFileName) szDiskFileName = szPFFSFileName;

    FILE* fp = fopen(szDiskFileName, "wb");
    if(!fp) return FALSE;

    DWORD dwFileSize = pDir->size;
    int nSector = pDir->chain;
    while(dwFileSize && nSector != FAT_END)
    {
      int nBytesToRead = (dwFileSize > 4096) ? 4096 : dwFileSize;
      int nBytesRead = fwrite(SectorToPointer(nSector), 1, nBytesToRead, fp);
      dwFileSize -= nBytesRead;
      nSector = m_pMSB->fat[nSector].chain;
    }
    fclose(fp);
    return TRUE;
  }
  BOOL DeleteFile(LPSTR szPFFSFileName)
  {
    pffsMASTERBLOCK new_msb;
    pffsMASTERBLOCK *old_msb;

    old_msb = m_pMSB;
    new_msb = *m_pMSB;
    m_pMSB = &new_msb;
    // この pDir は msb の中を指すので↑のようにポインタごと差し替える必要がある
    DIRECTORY* pDir = FindFile(szPFFSFileName);
    if(!pDir)
      return FALSE;
    int nSector = pDir->chain;
    while(nSector != FAT_END)
    {
      int n = new_msb.fat[nSector].chain;
      new_msb.fat[nSector].chain = FAT_FREE;
      nSector = n;
    }
    memset(pDir, 0xff, sizeof(DIRECTORY));
    *old_msb = new_msb;
    m_pMSB = old_msb;
    return TRUE;
  }
  BOOL AddFile(LPSTR szFileName)
  {
    BOOL r = FALSE;
    if(CheckFileName(szFileName))
      return FALSE;

    FILE* fp = fopen(szFileName, "rb");
    if(!fp) return FALSE;
    fseek(fp, 0, SEEK_END);
    DWORD dwSize = ftell(fp);
    fclose(fp);

    pffsMASTERBLOCK new_msb;
    pffsMASTERBLOCK *old_msb;
    int nSectors = ((dwSize + 4095) >> 12), nOldSectors = 0;

    old_msb = m_pMSB;
    new_msb = *m_pMSB;
    m_pMSB = &new_msb;

    DIRECTORY* pDir = FindFile(szFileName);
    if(!pDir)
    {
      pDir = m_pMSB->dir;
      for(int i = 0; i < MAXDIR; i++)
      {
        if((BYTE)pDir->name[0] == 0xff)
          goto CreateNew;
        pDir++;
      }
      goto CreateEnd;
CreateNew:
      memset(pDir, 0, sizeof(DIRECTORY));
      strncpy(pDir->name, szFileName, (sizeof pDir->name) - 1);
      pDir->chain = FAT_FREE;
    }
    else
    {
      nOldSectors = (pDir->size + 4095) >> 12;
    }
    pDir->size = dwSize;

    // 新旧ファイルのクラスタチェイン先頭
    WORD* pChain = &pDir->chain;
    // FAT 先頭
    FAT* pFat = m_pMSB->fat;
    int nn = 0, nSector = 0;

    // 準備したいセクタ数だけ回る
    for(nSector = 0; nSector < nSectors; nSector++)
    {
      int n = *pChain;
      if(n == FAT_FREE || n == FAT_END)
      {
        // 旧ファイルよりもたくさんセクタがほしい
        n = GetFreeFAT(pFat, nn); // nn は最後に未使用セクタを見つけたところ
        if(n < 0)                 // 空きなし
          goto CreateEnd;
        *pChain = n;                  // チェインの次の要素にする
        nn = n + 1;
      }
      pChain = &pFat[n].chain;         // その次の要素があればそれを使う
                                  // なければまた割り当てる
    }

    nn = *pChain;                     // 最後の新規セクタ
    *pChain = FAT_END;

    // 余った未使用セクタにマークをつける
    for(; nSector < nOldSectors; nSector++)
    {
      pChain = &pFat[nn].chain;
      nn = *pChain;
      *pChain = FAT_FREE;
      if(nn == FAT_FREE || nn == FAT_END) break;
    }

    fp = fopen(szFileName, "rb");
    if(!fp) goto CreateEnd;
    nSector = pDir->chain;

    // ようやくファイル書き込み
    for(int i = 0; i < nSectors; i++)
    {
      fread(SectorToPointer(nSector), 1, 4096, fp);
      nSector = m_pMSB->fat[nSector].chain;
    }

    fclose(fp);

CreateSuccess:
    r = TRUE;
    *old_msb = new_msb;
CreateEnd:
    m_pMSB = old_msb;
    return r;
  }
};



#endif // !CAUPFFS_H

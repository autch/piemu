
#ifndef CAUPFIFILE_H
#define CAUPFIFILE_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>

#include "psi.h"
#include "../pfi.h"

#define FLASH_TOP 0x0c00000

// pfi ファイルイメージそのもの
class CAuPFIFile
{
private:
  PFIHEADER m_pfiHeader;
  BYTE* m_pbyBuffer;
  DWORD m_dwSize;
public:
  CAuPFIFile() : m_pbyBuffer(NULL) { }
  ~CAuPFIFile() { Close(); }
  BOOL Create(SYSTEMINFO* pInfo, DWORD dwPFIVersion = 1);
  BOOL Open(LPSTR szFileName)
  {
    BOOL r = FALSE;
    FILE* fp = fopen(szFileName, "rb");
    if(fp && fread(&m_pfiHeader, sizeof(PFIHEADER), 1, fp))
    {
      if((m_pfiHeader.dwSignature & 0xffffff00) == ('PFI0' & 0xffffff00))
      {
        fseek(fp, m_pfiHeader.dwOffsetToFlash, SEEK_SET);

        int nSize = (DWORD)GetSystemInfo()->pffs_end - FLASH_TOP;
        int nBits;
        for(nBits = 0; (1 << nBits) < nSize; nBits++);
        m_dwSize = 1 << nBits;
        m_pbyBuffer = new BYTE[m_dwSize];
        if(m_pbyBuffer)
        {
          for(BYTE* p = m_pbyBuffer; p < m_pbyBuffer + m_dwSize; p += 1 << 12)
            fread(p, 1, 1 << 12, fp);
          r = TRUE;
        }
      }
    }
    fclose(fp);
    return r;
  }
  VOID Close()
  {
    delete[] m_pbyBuffer;
  }

  // 現在の内容をファイルに書き出す
  BOOL SaveToFile(LPSTR szFileName)
  {
    FILE* fp = fopen(szFileName, "wb");
    if(!fp) return FALSE;
    m_pfiHeader.dwOffsetToFlash = sizeof(PFIHEADER);
    fwrite(&m_pfiHeader, sizeof(PFIHEADER), 1, fp);
    fwrite(m_pbyBuffer, 1, m_dwSize, fp);
    fclose(fp);
    return TRUE;
  }
  SYSTEMINFO* GetSystemInfo() { return &m_pfiHeader.siSysInfo; }
  PFIHEADER* GetPFIHeader() { return &m_pfiHeader; }
  BYTE* GetMemory() { return m_pbyBuffer; }
  BYTE* GetPFFSTop() { return m_pbyBuffer + ((DWORD)m_pfiHeader.siSysInfo.pffs_top - FLASH_TOP); }

// 読み系
  // dwOffset から dwSize バイトだけ読む
  DWORD Read(DWORD dwOffset, LPVOID pBuffer, DWORD dwSize)
  {
    MoveMemory(pBuffer, (m_pbyBuffer + dwOffset), dwSize);
    return dwSize;
  }
  // dwSectorOffset 番目のセクタから dwSectorsToRead 個のセクタを読む
  DWORD ReadSector(DWORD dwSectorOffset, LPVOID pBuffer, DWORD dwSectorsToRead)
  {
    MoveMemory(pBuffer, m_pbyBuffer + (dwSectorOffset << 12), dwSectorsToRead << 12);
    return dwSectorsToRead;
  }
  // 必ずしも ReadByte() で読んだ値を BYTE の変数に格納するとは限らないので
  // オーバーロードでなく明示的に呼ぶ
  template<class T>T Read(DWORD dwOffset) { return *(T*)(m_pbyBuffer + dwOffset); }
  BYTE ReadByte(DWORD dwOffset) { return Read<BYTE>(dwOffset); }
  WORD ReadHWord(DWORD dwOffset) { return Read<WORD>(dwOffset); }
  DWORD ReadWord(DWORD dwOffset) { return Read<DWORD>(dwOffset); }
// 書き系
  DWORD Write(DWORD dwOffset, LPVOID pBuffer, DWORD dwSize)
  {
    MoveMemory(m_pbyBuffer + dwOffset, pBuffer, dwSize);
    return dwSize;
  }
  DWORD WriteSector(DWORD dwSectorOffset, LPVOID pBuffer, DWORD dwSectorsToWrite)
  {
    MoveMemory(m_pbyBuffer + (dwSectorOffset << 12), pBuffer, dwSectorsToWrite << 12);
    return dwSectorsToWrite;
  }
  template<class T>VOID Write(DWORD dwOffset, T v) { *(T*)(m_pbyBuffer + dwOffset) = v; }
  VOID WriteByte(DWORD dwOffset, BYTE byByte) { Write<BYTE>(dwOffset, byByte); }
  VOID WriteHWord(DWORD dwOffset, WORD wWord) { Write<WORD>(dwOffset, wWord); }
  VOID WriteWord(DWORD dwOffset, DWORD dwDWord) { Write<DWORD>(dwOffset, dwDWord); }

  static VOID DumpSystemInfo(SYSTEMINFO* psi)
  {
    printf("P/ECE H/W v%d.%02d, BIOS v%d.%02d (%d.%02d.%02d), "
           "running at %5.3f MHz, %5.3f V\n",
           psi->hard_ver>>8, psi->hard_ver&255,
           psi->bios_ver >> 8, psi->bios_ver & 255,
           2000+(psi->bios_date>>9), (psi->bios_date>>5)&15, psi->bios_date&31,
           psi->sys_clock / 1e6,psi->vdde_voltage / 1e3);
    printf("SRAM addr 0x%p - 0x%p, %4d KB\n", psi->sram_top, psi->sram_end-1, (psi->sram_end - psi->sram_top)>>10);
    printf("PFFS addr 0x%p - 0x%p, %4d KB\n", psi->pffs_top, psi->pffs_end-1, (psi->pffs_end - psi->pffs_top)>>10);
  }
};

#endif // !CAUPFIFILE_H

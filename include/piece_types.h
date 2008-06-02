/** @file piece_types.h
 * P/ECE arch-specific structures
 */

#ifndef PIECE_TYPES_H
#define PIECE_TYPES_H

#include "types.h"

// derived from P/ECE SDK
typedef struct systeminfo
{
  c33hword size;         //  0 この構造体のサイズ
  c33hword hard_ver;     //  2 ハードウエア・バージョン
  c33hword bios_ver;     //  4 BIOSバージョン
  c33hword bios_date;    //  6 BIOS更新日 YY(7):MM(4):DD(5)
  c33word  sys_clock;    //  8 システム・クロック(Hz)
  c33hword vdde_voltage; // 12 VDDE(周辺)電圧(mV)
  c33hword resv1;        // 14 予約
  c33word sram_top;     // 16 SRAM 開始アドレス
  c33word sram_end;     // 20 SRAM 終了アドレス+1
  c33word pffs_top;     // 24 pffs 開始アドレス
  c33word pffs_end;     // 28 pffs 終了アドレス
} SYSTEMINFO;

#endif

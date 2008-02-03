
#if !defined(PSI_H)
#define PSI_H

// from piece.h
typedef struct tagSYSTEMINFO
{
	unsigned short size;          //  0 この構造体のサイズ
	unsigned short hard_ver;      //  2 ハードウエア・バージョン
	unsigned short bios_ver;      //  4 BIOSバージョン
	unsigned short bios_date;     //  6 BIOS更新日 YY(7):MM(4):DD(5)
	unsigned long sys_clock;      //  8 システム・クロック(Hz)
	unsigned short vdde_voltage;  // 12 VDDE(周辺)電圧(mV)
	unsigned short resv1;         // 14 予約
	unsigned char *sram_top;      // 16 SRAM 開始アドレス
	unsigned char *sram_end;      // 20 SRAM 終了アドレス+1
	unsigned char *pffs_top;      // 24 pffs 開始アドレス
	unsigned char *pffs_end;      // 28 pffs 終了アドレス
} SYSTEMINFO;

#endif // !PSI_H

/*
 *  app.h
 *
 *  P/EMU - P/ECE Emulator
 *  Copyright (C) 2003 Naoyuki Sawa
 *
 *  * Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *  - 作成開始。
 */
#ifndef __APP_H__
#define __APP_H__

#include <SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#include "sleep.h"

//#include "resource.h"

#define APPNAME   "P/EMU"
#define VERSION   "20030430"

#define ABOUT_CAPTION "P/EMUのバージョン情報"
#define ABOUT_TEXT  "P/EMU - P/ECE Emulator (" VERSION ")\n" \
      "Copyright(C) 2003 Naoyuki Sawa"

typedef int64_t  int64;
typedef uint64_t uint64;
#define INLINE

/* /usr/PIECE/include/piece.hより… */
typedef struct tagSYSTEMINFO {    // (piece.hより)
  uint16_t size;    //  0 この構造体のサイズ
  uint16_t hard_ver;  //  2 ハードウエア・バージョン
  uint16_t bios_ver;  //  4 BIOSバージョン
  uint16_t bios_date; //  6 BIOS更新日 YY(7):MM(4):DD(5)
  uint32_t sys_clock;  //  8 システム・クロック(Hz)
  uint16_t vdde_voltage;  // 12 VDDE(周辺)電圧(mV)
  uint16_t resv1;   // 14 予約
  uint32_t sram_top;  // 16 SRAM 開始アドレス
  uint32_t sram_end;  // 20 SRAM 終了アドレス+1
  uint32_t pffs_top;  // 24 pffs 開始アドレス
  uint32_t pffs_end;  // 28 pffs 終了アドレス
} SYSTEMINFO;

struct tagPIEMU_CONTEXT;

#include "emu.h"
//
#include "fram.h"
#include "iomem.h"
#include "sram.h"
#include "usbc.h"
#include "flash.h"
#include "lcdc.h"
//
#include "bcu.h"
#include "core.h"
//
#include "c33209e.h"
#include "vector.h"
//#include "iomap.h"

#include "piemu.h"

/* main.c */
//
//void dbg(const char* fmt, ...);
//void die(const char* fmt, ...);
//#define DIE() die(__FILE__ "(%d)", __LINE__)
#define DIE()  { *(int*)(0xdeadbeef) = 0xdeadbeef; exit(-1); }
#define dbg()  ((void)0)

#define KEY_UP    SDLK_UP
#define KEY_DOWN  SDLK_DOWN
#define KEY_LEFT  SDLK_LEFT
#define KEY_RIGHT SDLK_RIGHT
#define KEY_A   SDLK_z
#define KEY_B   SDLK_x
#define KEY_START SDLK_a
#define KEY_SELECT  SDLK_s

#ifdef PSP
#define JOY_CIRCLE  1  // as A
#define JOY_CROSS   2  // as B
#define JOY_START  11  // as START
#define JOY_SELECT 10  // as SELECT
#define JOY_LEFT    7  // as LEFT
#define JOY_RIGHT   9  // as RIGHT
#define JOY_UP      8  // as UP
#define JOY_DOWN    6  // as DOWN
#endif // PSP

// 汎用メモリリード。パーシャルレジスタストールを防げるし、
// エンディアンネスの違うマシンへ移植するときはここだけ書き換えればよい
// 戻り値はすべて 32bit int, プロトタイプも片っ端から変更の必要あり

// little endian 用
#define MEM_MASK(type) ((1 << (sizeof(type) << 3)) - 1)
#define READ_MEM(mem, type) ((*(int*)(mem)) & MEM_MASK(type))
#define READ_MEM_B(mem) READ_MEM((mem), char)
#define READ_MEM_H(mem) READ_MEM((mem), short)
#define READ_MEM_W(mem) (*(int*)(mem))

#endif /*__APP_H__*/

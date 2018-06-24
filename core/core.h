/*
 *  core.h
 *
 *  P/EMU - P/ECE Emulator
 *  Copyright (C) 2003 Naoyuki Sawa
 *
 *  * Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *  - 作成開始。
 */
#ifndef __CORE_H__
#define __CORE_H__

#include <SDL.h>

#include "c33inst.h"

/****************************************************************************
 *
 ****************************************************************************/

#define SYSTEM_CLOCK  (24 * 1000 * 1000)

/* このシンボルを定義すると、不正なコードをアサートします。（遅いです） */
#undef CORE_DEBUG /* まず確実に未定義にしてから… */
//#define CORE_DEBUG  /* 必要に応じて定義してください */

/****************************************************************************
 *
 ****************************************************************************/

typedef struct _CORE {
/* public */
  uint32_t r[16];
  uint32_t s[ 4];
  uint32_t pc;
/* private */
  INST ext[2];
  uint32_t d;
  uint32_t clk;

  int in_halt;        // halt 中インクリメント, core_trap() でデクリメント

  SDL_mutex* mut_halt;
  SDL_cond* cond_halt;

  SDL_mutex* mut_core;
} CORE;

typedef struct _tPSR {
  uint32_t n  : 1;
  uint32_t z  : 1;
  uint32_t v  : 1;
  uint32_t c  : 1;
  uint32_t ie : 1;
  uint32_t  : 1;
  uint32_t ds : 1;
  uint32_t mo : 1;
  uint32_t il : 4;
  uint32_t  :20;
} tPSR;
#define PSR (*(tPSR*)&context->core.s[0])
#define SP  (context->core.s[1])
#define ALR (context->core.s[2])
#define AHR (context->core.s[3])
#define AR  (*(int64*)&context->core.s[2])
#define PC  (context->core.pc)
#define EXT1  (context->core.ext[0])
#define EXT2  (context->core.ext[1])
#define CLK (context->core.clk)

/****************************************************************************
 *
 ****************************************************************************/

void core_init(struct tagPIEMU_CONTEXT* context);
void core_work(struct tagPIEMU_CONTEXT* context);
void core_handle_hlt(struct tagPIEMU_CONTEXT* context);
void core_trap_from_core(struct tagPIEMU_CONTEXT* context, int no, int level);
void core_trap_from_devices(struct tagPIEMU_CONTEXT* context, int no, int level);
void core_inst(struct tagPIEMU_CONTEXT* context, INST inst);

// もっと速く
uint32_t core_workex(struct tagPIEMU_CONTEXT* context, uint32_t mils_org, uint32_t nClocksDivBy1k);

#endif /*__CORE_H__*/

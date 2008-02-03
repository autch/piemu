/*
 *  emu.c
 *
 *  P/EMU - P/ECE Emulator
 *  Copyright (C) 2003 Naoyuki Sawa
 *
 *  * Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *  - 作成開始。
 */
#include "app.h"

/****************************************************************************
 *  グローバル変数
 ****************************************************************************/

//EMU emu;

/****************************************************************************
 *  モジュール定義
 ****************************************************************************/

#define MOD(n) { mod.init = n##_init; mod.work = n##_work; }

/****************************************************************************
 *  グローバル関数
 ****************************************************************************/

void
emu_init(PIEMU_CONTEXT* context)
{
  MODULE* module;
  MODULE mod;

  memset(&context->emu, 0, sizeof context->emu);

  context->emu.module_tbl = (MODULE*)calloc(MODTBL_SIZE, sizeof(MODULE));
  module = context->emu.module_tbl;
  MOD(fram); *module++ = mod;
  MOD(iomem); *module++ = mod;
  MOD(sram); *module++ = mod;
  MOD(usbc); *module++ = mod;
  MOD(flash); *module++ = mod;
  MOD(lcdc); *module++ = mod;
  MOD(bcu); *module++ = mod;

  context->pfnSetEmuParameters(context, &context->emu, context->pUser);
  printf("BIOS version = %d.%02d\n", context->emu.sysinfo.bios_ver >> 8, context->emu.sysinfo.bios_ver & 255);
  printf("BIOS date    = %d.%02d.%02d\n", 2000+(context->emu.sysinfo.bios_date>>9), (context->emu.sysinfo.bios_date>>5)&15, context->emu.sysinfo.bios_date&31);
  printf("SRAM top adr = 0x%p\n", context->emu.sysinfo.sram_top);
  printf("SRAM end adr = 0x%p\n", context->emu.sysinfo.sram_end-1);
  printf("SRAM size    = %d KB\n", (context->emu.sysinfo.sram_end - context->emu.sysinfo.sram_top)>>10);
  printf("HW version   = %d.%02d\n", context->emu.sysinfo.hard_ver>>8, context->emu.sysinfo.hard_ver&255);
  printf("SYSTEM clock = %5.3f MHz\n", context->emu.sysinfo.sys_clock / 1e6);
  printf("VDDE voltage = %5.3f V\n", context->emu.sysinfo.vdde_voltage / 1e3);
  printf("PFFS top adr = 0x%p\n", context->emu.sysinfo.pffs_top);
  printf("PFFS end adr = 0x%p\n", context->emu.sysinfo.pffs_end-1);

  /* 各モジュールの初期化。 */
  for(module = context->emu.module_tbl; module->init || module->work; module++)
  {
    if(module->init) module->init(context);
  }
  core_init(context); /* この中から他のモジュールを使うので、最後に初期化 */

}

int emu_work(void* ctx)
{
  PIEMU_CONTEXT* context = (PIEMU_CONTEXT*)ctx;
  MODULE* module;
  unsigned real_org;
  unsigned mils_org;
  unsigned frame_org;
  unsigned nIPF = 0;
  unsigned nFrames = 0;

  // きついループ内で計算などさせない
  unsigned nSystemClock = context->emu.sysinfo.sys_clock;
  unsigned nClocksDivBy1k = nSystemClock / 1000;
  unsigned nMSecPerFrame = 1000 / context->o_fps;
  unsigned nClocksPerFrame = nSystemClock / context->o_fps * context->o_oc / 100;
  unsigned nClocksShr14 = nSystemClock >> 14; // nClocks SHift to Right 14bits
  unsigned nHalfFPS = context->o_fps >> 1;
#ifdef PSP
  SceKernelThreadInfo info = { sizeof(SceKernelThreadInfo) };
  if(sceKernelReferThreadStatus(sceKernelGetThreadId(), &info) == 0)
  {
    // 優先度 sage.  数値が小さいほど優先。
    sceKernelChangeThreadPriority(sceKernelGetThreadId(), info.currentPriority + 8);
  }
#endif
#ifdef WINDOWS
  // 優先度 sage.  数値が小さいほど優先。
  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);
#endif
  do
  {
#ifdef PSP
    scePowerSetCpuClockFrequency(333);
#endif
    real_org = SDL_GetTicks();
#ifdef WINDOWS
    if(nFrames == 0)
    {
//      fprintf(stderr, "ipf(%d frames avg.)\t%d\n", nHalfFPS, nIPF / nHalfFPS);
      nIPF = 0;
    }
    nFrames = (nFrames + 1) % nHalfFPS;
#else
    nIPF = 0;
#endif
    frame_org = CLK;
    do
    {
      mils_org = CLK;
      /* 命令実行。 */
      nIPF += core_workex(context, mils_org, nClocksDivBy1k);

      /*{{仮*/
      pCLK_TCD += 1;
      //nSystemClock / 256 / 64 = nSystemClock >> 8 >> 6 = nSystemClock >> (8 + 6)
      pT16_TC0 += nClocksShr14; /* GetSysClock()に24MHzに見せかけるためのつじつま合わせ */
      /*}}仮*/

      /* ※要注意！
       * 必ず、モジュール処理→NMI生成の順で行ってください！
       * そうしないと、モジュール処理からの割り込み発行が常にNMIにブロックされてしまいます。
       */
      /* 各モジュールの処理。 */
      for(module = context->emu.module_tbl; module->init || module->work; module++)
      {
        if(module->work) module->work(context);
      }
      /* コンテキストスイッチ用の16bitタイマ0コンペアB割り込み生成 */
      if(bINT_E16T01_E16TU0) core_trap(context, TRAP_16TU0, bINT_P16T01_P16T0);
      /* P/ECEシステムタイマ用のNMI生成。 */
      if(bWD_EN_EWD) core_trap(context, TRAP_NMI, 0);

      if(context->bEndFlag)
        return 0;
    } while((CLK - frame_org) < nClocksPerFrame); /* 1フレーム分の処理 */

    /* 画面更新。 */
    lcdc_conv(context, context->vbuff);
    context->pfnUpdateScreen(context, context->pUser);

    /* 実時間との同期 */
    if(!context->o_nowait)
    {
      int nExpectedWait = (real_org + nMSecPerFrame) - SDL_GetTicks();
//      fprintf(stderr, "wait: %d\n", nExpectedWait);
//#ifdef PSP
//      scePowerSetCpuClockFrequency(222);
//#endif
      if(nExpectedWait > 0)
        SDL_Delay(nExpectedWait);
      else
        SDL_Delay(1);
    }
  }while(!context->bEndFlag);

  return 0;
}

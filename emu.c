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

#ifdef USE_PROF
#include <gperftools/profiler.h>
#include <sys/time.h>

#endif

#include <time.h>

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
emu_init(PIEMU_CONTEXT *context)
{
    MODULE *module;
    MODULE mod;

    memset(&context->emu, 0, sizeof context->emu);

    context->emu.module_tbl = (MODULE *) calloc(MODTBL_SIZE, sizeof(MODULE));
    module = context->emu.module_tbl;
    MOD(fram);
    *module++ = mod;
    MOD(iomem);
    *module++ = mod;
    MOD(sram);
    *module++ = mod;
    MOD(usbc);
    *module++ = mod;
    MOD(flash);
    *module++ = mod;
    MOD(lcdc);
    *module++ = mod;
    MOD(bcu);
    *module++ = mod;

    context->pfnSetEmuParameters(context, &context->emu, context->pUser);
    printf("BIOS version = %d.%02d\n", context->emu.sysinfo.bios_ver >> 8, context->emu.sysinfo.bios_ver & 255);
    printf("BIOS date    = %d.%02d.%02d\n", 2000 + (context->emu.sysinfo.bios_date >> 9),
           (context->emu.sysinfo.bios_date >> 5) & 15, context->emu.sysinfo.bios_date & 31);
    printf("SRAM top adr = 0x%08x\n", context->emu.sysinfo.sram_top);
    printf("SRAM end adr = 0x%08x\n", context->emu.sysinfo.sram_end - 1);
    printf("SRAM size    = %d KB\n", (context->emu.sysinfo.sram_end - context->emu.sysinfo.sram_top) >> 10);
    printf("HW version   = %d.%02d\n", context->emu.sysinfo.hard_ver >> 8, context->emu.sysinfo.hard_ver & 255);
    printf("SYSTEM clock = %5.3f MHz\n", context->emu.sysinfo.sys_clock / 1e6);
    printf("VDDE voltage = %5.3f V\n", context->emu.sysinfo.vdde_voltage / 1e3);
    printf("PFFS top adr = 0x%08x\n", context->emu.sysinfo.pffs_top);
    printf("PFFS end adr = 0x%08x\n", context->emu.sysinfo.pffs_end - 1);

    /* 各モジュールの初期化。 */
    for (module = context->emu.module_tbl; module->init || module->work; module++) {
        if (module->init) module->init(context);
    }
    core_init(context); /* この中から他のモジュールを使うので、最後に初期化 */

}

int emu_work(void *ctx)
{
#ifdef USE_PROF
    ProfilerRegisterThread();
#endif
    PIEMU_CONTEXT* context = (PIEMU_CONTEXT*)ctx;

    // きついループ内で計算などさせない
    unsigned nSystemClock = context->emu.sysinfo.sys_clock;
    unsigned nClocksDivBy1k = nSystemClock / 1000;
    unsigned nMSecPerFrame = 1000 / context->o_fps;
    unsigned nClocksShr14 = nSystemClock >> 14; // nClocks SHift to Right 14bits

    SDL_TimerID clock_keeper = SDL_AddTimer(500, emu_clockkeeper_work, context);

    do {
        unsigned real_org = SDL_GetTicks();

        do {
            /* 命令実行。 */
            if(!context->core.in_halt) {
                core_workex(context, nClocksDivBy1k);
            }

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
            for (MODULE *module = context->emu.module_tbl; module->init || module->work; module++) {
                if (module->work) module->work(context);
            }

            /* コンテキストスイッチ用の16bitタイマ0コンペアB割り込み生成 */
            if (bINT_E16T01_E16TU0) core_trap_from_devices(context, TRAP_16TU0, bINT_P16T01_P16T0);
            /* P/ECEシステムタイマ用のNMI生成。 */
            if (bWD_EN_EWD) core_trap_from_devices(context, TRAP_NMI, 0);

            SDL_Delay(1);
        }while(!context->bEndFlag && (SDL_GetTicks() - real_org) < nMSecPerFrame);

        /* 実時間との同期 */
        if (!context->o_nowait) {
            int nExpectedWait = (real_org + nMSecPerFrame) - SDL_GetTicks();
            if (nExpectedWait > 0)
                SDL_Delay((unsigned)nExpectedWait);
        }

    } while (!context->bEndFlag);

    SDL_RemoveTimer(clock_keeper);


    return 0;
}

Uint32 emu_clockkeeper_work(Uint32 interval, void* ctx)
{
    PIEMU_CONTEXT *context = (PIEMU_CONTEXT *) ctx;
    time_t now = time(NULL);
    struct tm ltime;

#ifdef _WIN32
    localtime_s(&ltime, &now);
#else
    localtime_r(&now, &ltime);
#endif

    pCLK_TCDD = ltime.tm_hour & 0x1f;
    pCLK_TCHD = ltime.tm_min & 0x3f;
    pCLK_TCMD = ltime.tm_sec & 0x3f;

    {
        int yy = ltime.tm_year + 1900;
        int years = ltime.tm_year + 1900 - 2000;
        int days = years * 365 + ltime.tm_yday + years / 4 - years / 100 + years / 400 + 1;
        if(ltime.tm_mon < 2 && (yy % 4 == 0 && yy % 100 != 0 && yy % 400 == 0))
            days--;

        pCLK_TCNDL = days & 0xff;
        pCLK_TCNDH = (days >> 8) & 0xff;
    }

    return interval;
}


#ifndef SLEEP_H
#define SLEEP_H

// 現在のタイムスライスを放棄するマクロ OS_YIELD() を定義する。
// ミリ秒単位でのディレイには SDL_Delay() を使う。

#ifdef WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define OS_YIELD() Sleep(0)
#endif

#ifdef LINUX
#include <sched.h>
#define OS_YIELD() sched_yield()
#endif

#ifdef PSP
#include <pspthreadman.h>
#define OS_YIELD() sceKernelDelayThreadCB(0)
#endif

#endif // !SLEEP_H

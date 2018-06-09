
#ifndef SLEEP_H
#define SLEEP_H

// 現在のタイムスライスを放棄するマクロ OS_YIELD() を定義する。
// ミリ秒単位でのディレイには SDL_Delay() を使う。

#ifndef _WINDOWS
#include <sched.h>
#define OS_YIELD() sched_yield()
#else
#include <windows.h>
#define OS_YIELD() Sleep(0)
#endif 

#endif // !SLEEP_H

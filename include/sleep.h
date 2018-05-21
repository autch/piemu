
#ifndef SLEEP_H
#define SLEEP_H

// 現在のタイムスライスを放棄するマクロ OS_YIELD() を定義する。
// ミリ秒単位でのディレイには SDL_Delay() を使う。

#include <sched.h>
#define OS_YIELD() sched_yield()

#endif // !SLEEP_H

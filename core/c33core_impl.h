#ifndef PIEMU_C33CORE_IMPL_H
#define PIEMU_C33CORE_IMPL_H

#include "c33core.h"

/* 関数の先頭で使えるように、ダミー変数を初期化します。 */

// C33 には無効命令例外がないので、不正なオペコードを検知したとしてもできることはない。
// マニュアルでは未定義になっている。
// #ifdef CORE_DEBUG
// static int no_ext()  { if(EXT1.s) DIE(); return 0; }
// static int no_delay()  { if(core.d) DIE(); return 0; }
// #define NO_EXT   int __no_ext__ = no_ext();
// #define NO_DELAY int __no_delay__ = no_delay();
// #else /*CORE_DEBUG*/
#define NO_EXT
#define NO_DELAY
// #endif /*CORE_DEBUG*/

/* インデクス検査付きの汎用/システムレジスタアクセス　*/
// #ifdef CORE_DEBUG
// static int check_r(int n) { if(n < 0 || n > 15) DIE(); return n; }
// static int check_s(int n) { if(n < 0 || n >  3) DIE(); return n; }
// #define R(n) this->r[check_r(n)]
// #define S(n) this->s[check_s(n)]
// #else /*CORE_DEBUG*/
#define R(n)  this->r[n]
#define S(n)  this->s[n]

#define RZ this->rZ
// #endif /*CORE_DEBUG*/

#define PSR (*(PSR*)&this->s[0])
#define SP  (this->s[1])
#define ALR (this->s[2])
#define AHR (this->s[3])
#define AR  (*(int64_t*)&this->s[2])
#define PC  (this->pc)
#define EXT1  (this->ext[0])
#define EXT2  (this->ext[1])
#define CLK (this->clk)

#endif //PIEMU_C33CORE_IMPL_H

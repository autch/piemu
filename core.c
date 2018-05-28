/*
 *  core.c
 *
 *  P/EMU - P/ECE Emulator
 *  Copyright (C) 2003 Naoyuki Sawa
 *
 *  * Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *  - 作成開始。
 *  * Sat Apr 19 12:30:00 JST 2003 Naoyuki Sawa
 *  - scan0/scan1のバグ修正。（走査方向が逆でした）
 *
 *  * Aug 23 ... 30, 2005.
 *  * こんなもん全部マクロじゃー
 *
 *  * May 29, 2018
 *  * マクロとかやめやめ
 */
#include "app.h"
#include "tools/types.h"

/****************************************************************************
 *  グローバル変数
 ****************************************************************************/

//CORE core;

/****************************************************************************
 *  マクロ・補助関数
 ****************************************************************************/

// (1 << NOP_WAIT) 回 nop が呼ばれたら SDL_Delay(1)
#define NOP_WAIT 9

#define NOP_CLK_MULTIPLY      2
#define HALT_CLK_MULTIPLY   240
#define HALT_INT_CLK      24000

#if 1
#define STD_NOP {  \
  context->core.nop_count++; \
  context->core.nop_count &= (1 << NOP_WAIT) - 1; \
  if(!context->core.nop_count) \
  { \
    SDL_Delay(1); \
  } \
}
#else // 0
#define STD_NOP OS_YIELD() //__asm { nop }
#endif

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
// #define R(n) context->core.r[check_r(n)]
// #define S(n) context->core.s[check_s(n)]
// #else /*CORE_DEBUG*/
#define R(n)  context->core.r[n]
#define S(n)  context->core.s[n]

#define RZ context->core.rZ
// #endif /*CORE_DEBUG*/

/* 値dataのビット(bits-1)を、ビット31〜bitsに符号拡張します。 */
INLINE int
sign_ext(int data, int bits)
{
    data <<= 32 - bits;
    data >>= 32 - bits;
    return data;
}

/* ディレイド分岐命令の中から使用。引数dに1が指定されたら、ディレイド命令を実行します。 */
INLINE void exec_delay(PIEMU_CONTEXT *context, int dflag)
{
    if (dflag) {
        INST d_inst;

        if (context->core.d) DIE();  /* 念のため、ディレイドが二重になっていないことを検査 */
        context->core.d = 1;    /* ディレイド開始 */
        d_inst.s = mem_readH(context, PC + 2);
        core_inst(context, d_inst);
        if (!context->core.d) DIE(); /* 念のため、予期しないディレイド解除がないことを検査 */
        context->core.d = 0;    /* ディレイド終了 */
    }
}

/****************************************************************************
 *  即値拡張
 ****************************************************************************/

INLINE int
ext_imm6(PIEMU_CONTEXT *context, int imm6)
{
    int data;
    imm6 &= (1 << 6) - 1;
    if (EXT2.s) {
        data = imm6 | (EXT2.c6.imm13 << 6) | (EXT1.c6.imm13 << 19);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else if (EXT1.s) {
        data = imm6 | (EXT1.c6.imm13 << 6);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else {
        data = imm6;
    }
    return data;
}

INLINE int
ext_sign6(PIEMU_CONTEXT *context, int sign6)
{
    int data, bits;
    sign6 &= (1 << 6) - 1;
    if (EXT2.s) {
        data = sign6 | (EXT2.c6.imm13 << 6) | (EXT1.c6.imm13 << 19);
        bits = 6 + 13 + 13;
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else if (EXT1.s) {
        data = sign6 | (EXT1.c6.imm13 << 6);
        bits = 6 + 13;
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else {
        data = sign6;
        bits = 6;
    }
    return sign_ext(data, bits);
}

INLINE int
ext_RB(PIEMU_CONTEXT *context, int rb)
{
    int disp;
    if (EXT2.s) {
        disp = (EXT2.c6.imm13 << 0) | (EXT1.c6.imm13 << 13);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else if (EXT1.s) {
        disp = (EXT1.c6.imm13 << 0);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else {
        disp = 0;
    }
    return R(rb) + disp;
}

INLINE int
ext_SPxIMM6(PIEMU_CONTEXT *context, int imm6, int size)
{
    int disp;
    imm6 &= (1 << 6) - 1;
    if (EXT2.s) {
        disp = imm6 | (EXT2.c6.imm13 << 6) | (EXT1.c6.imm13 << 19);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else if (EXT1.s) {
        disp = imm6 | (EXT1.c6.imm13 << 6);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else {
        disp = imm6 * size;
/*
    switch(size) {
    case 1: disp = imm6 * 1; break;
    case 2: disp = imm6 * 2; break;
    case 4: disp = imm6 * 4; break;
    default: DIE();
    }
*/
    }
    return SP + disp;
}

INLINE int
ext_3op(PIEMU_CONTEXT *context)
{
    /* NOTE1: 少なくともEXT1が存在することを呼び出し側で確認してください。 */
    /* NOTE2: cmp/and/or/xor/notも、3op拡張時はsignではなくimmとなります。 */
    int data;
    if (EXT2.s) {
        data = (EXT2.c6.imm13 << 0) | (EXT1.c6.imm13 << 13);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else if (EXT1.s) {
        data = (EXT1.c6.imm13 << 0);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else {
        DIE();
    }
    return data;
}

INLINE int
ext_PCxSIGN8(PIEMU_CONTEXT *context, int sign8)
{
    int disp, bits;
    sign8 &= (1 << 8) - 1;
    if (EXT2.s) {
        disp = sign8 | (EXT2.c6.imm13 << 8) | (EXT1.c6.imm13 >> 3 << 21);
        bits = 8 + 13 + 10;
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else if (EXT1.s) {
        disp = sign8 | (EXT1.c6.imm13 << 8);
        bits = 8 + 13;
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else {
        disp = sign8;
        bits = 8;
    }
    return PC + sign_ext(disp, bits) * 2;
}

/****************************************************************************
 *  共通演算とPSR設定
 ****************************************************************************/

c33int add(tPSR *psr, c33int a, c33int b)
{
    c33int c = a + b;
    psr->n = c < 0;
    psr->z = !c;
    psr->v = (a < 0 && b < 0 && c >= 0) || (a >= 0 && b >= 0 && c < 0);
    psr->c = (c33word) c < (c33word) a;
    return c;
}

c33int adc(tPSR *psr, c33int a, c33int b)
{
    c33int c = a + b;
    c33int d = c + psr->c;
    psr->n = d < 0;
    psr->z = !d;
    psr->v = (a < 0 && b < 0 && d >= 0) || (a >= 0 && b >= 0 && d < 0);
    psr->c = ((c33word) c < (c33word) a) || ((c33word) d < (c33word) c);
    return d;
}

c33int sub(tPSR *psr, c33int a, c33int b)
{
    c33int c = a - b;
    psr->n = c < 0;
    psr->z = !c;
    psr->v = (a >= 0 && b < 0 && c < 0) || (a < 0 && b >= 0 && c >= 0);
    psr->c = (c33word) c > (c33word) a;
    return c;
}

c33int sbc(tPSR *psr, c33int a, c33int b)
{
    c33int c = a - b;
    c33int d = c - psr->c;
    psr->n = d < 0;
    psr->z = !d;
    psr->v = (a >= 0 && b < 0 && d < 0) || (a < 0 && b >= 0 && d >= 0);
    psr->c = ((c33word) c > (c33word) a) || ((c33word) d > (c33word) c);
    return d;
}

c33int and(tPSR *psr, c33int a, c33int b)
{
    c33int r = a & b;
    psr->n = r < 0;
    psr->z = !r;
    return r;
}

c33int or(tPSR *psr, c33int a, c33int b)
{
    c33int r = a | b;
    psr->n = r < 0;
    psr->z = !r;
    return r;
}

c33int xor(tPSR *psr, c33int a, c33int b)
{
    c33int r = a ^b;
    psr->n = r < 0;
    psr->z = !r;
    return r;
}

c33int not(tPSR *psr, c33int a)
{
    c33int r = ~a;
    psr->n = r < 0;
    psr->z = !r;
    return r;
}

c33int srl(tPSR *psr, c33int a, c33int b)
{
    if (b < 0 || b > 8) DIE();
    c33int r = (c33word) a >> b;
    psr->n = r < 0;
    psr->z = !r;
    return r;
}

c33int sll(tPSR *psr, c33int a, c33int b)
{
    if (b < 0 || b > 8) DIE();
    c33int r = (c33word) a << b;
    psr->n = r < 0;
    psr->z = !r;
    return r;
}

c33int sra(tPSR *psr, c33int a, c33int b)
{
    if (b < 0 || b > 8) DIE();
    c33int r = (c33int) a >> b;
    psr->n = r < 0;
    psr->z = !r;
    return r;
}

c33int sla(tPSR *psr, c33int a, c33int b)
{
    if (b < 0 || b > 8) DIE();
    c33int r = (c33int) a << b;
    psr->n = r < 0;
    psr->z = !r;
    return r;
}

c33int rr(tPSR *psr, c33int a, c33int b)
{
    if (b < 0 || b > 8) DIE();
    c33int r = ((c33word) a >> b) | ((c33word) a << (32 - b));
    psr->n = r < 0;
    psr->z = !r;
    return r;
}

c33int rl(tPSR *psr, c33int a, c33int b)
{
    if (b < 0 || b > 8) DIE();
    c33int r = ((c33word) a << b) | ((c33word) a >> (32 - b));
    psr->n = r < 0;
    psr->z = r;
    return r;
}

/****************************************************************************
 *  CLASS 0A
 ****************************************************************************/

INLINE void exec_nop(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    STD_NOP;
    PC += 2;
    CLK += 1 * NOP_CLK_MULTIPLY;
}

// 呼ばれてないらしい？
INLINE void exec_slp(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    STD_NOP;
    /* ※TODO: */
    context->core.in_halt = 1;
    PC += 2;
    CLK += 1 * NOP_CLK_MULTIPLY;

}

INLINE void exec_halt(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    STD_NOP;
    /* ※TODO: */
    /*context->core.in_halt = 1;*/
    PC += 2;
    CLK += 1 * HALT_CLK_MULTIPLY;

}

INLINE void exec_pushn_rs(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    int rs;
    for (rs = inst.imm2_rd_rs; rs >= 0; rs--) {
        SP -= 4;
        mem_writeW(context, SP, R(rs));
        CLK += 1;

    }
    PC += 2;

}

INLINE void exec_popn_rd(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    int rd;
    for (rd = 0; rd <= inst.imm2_rd_rs; rd++) {
        R(rd) = mem_readW(context, SP);
        SP += 4;
        CLK += 1;
    }
    PC += 2;

}

INLINE void exec_brk(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    DIE();
}

INLINE void exec_retd(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    DIE();
}

INLINE void exec_int_imm2(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    if (inst.imm2_rd_rs < 0 || inst.imm2_rd_rs > 3) DIE();
    PC += 2;
    CLK += 10;
    core_trap(context, TRAP_SOFTINT0 + inst.imm2_rd_rs, 0);

}

INLINE void exec_reti(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    S(0) = mem_readW(context, SP);
    SP += 4;
    PC = mem_readW(context, SP);
    SP += 4;
    CLK += 5;
}

INLINE void exec_call_rb(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    c33word addr = R(inst.imm2_rd_rs);
    exec_delay(context, inst.d);
    SP -= 4;
    mem_writeW(context, SP, PC + 2);
    PC = addr;
    CLK += !inst.d ? 3 : 2;
}

INLINE void exec_ret(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    c33word addr = mem_readW(context, SP);
    SP += 4;
    exec_delay(context, inst.d);
    PC = addr;
    CLK += !inst.d ? 4 : 3;

}

INLINE void exec_jp_rb(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    c33word addr = R(inst.imm2_rd_rs);
    exec_delay(context, inst.d);
    PC = addr;
    CLK += !inst.d ? 2 : 1;
}

/****************************************************************************
 *  CLASS 0B
 ****************************************************************************/

INLINE void exec_jrgt_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = !(PSR.n ^ PSR.v) && !PSR.z;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

INLINE void exec_jrge_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = !(PSR.n ^ PSR.v);
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

INLINE void exec_jrlt_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = (PSR.n ^ PSR.v);
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

INLINE void exec_jrle_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = (PSR.n ^ PSR.v) || PSR.z;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

INLINE void exec_jrugt_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = !PSR.c && !PSR.z;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

INLINE void exec_jruge_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = !PSR.c;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

INLINE void exec_jrult_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = PSR.c;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

INLINE void exec_jrule_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = PSR.c || PSR.z;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

INLINE void exec_jreq_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = PSR.z;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

INLINE void exec_jrne_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = !PSR.z;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

INLINE void exec_call_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    SP -= 4;
    mem_writeW(context, SP, PC + 2);
    PC = addr;
    CLK += !inst.d ? 3 : 2;

}

INLINE void exec_jp_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = addr;
    CLK += !inst.d ? 2 : 1;

}

/****************************************************************************
 *  CLASS 1A
 ****************************************************************************/

INLINE void exec_ld_b_rd_RB(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (char) mem_readB(context, ext_RB(context, inst.rb));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_ub_rd_RB(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (unsigned char) mem_readB(context, ext_RB(context, inst.rb));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_h_rd_RB(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (short) mem_readH(context, ext_RB(context, inst.rb));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_uh_rd_RB(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (unsigned short) mem_readH(context, ext_RB(context, inst.rb));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_w_rd_RB(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (int) mem_readW(context, ext_RB(context, inst.rb));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_b_RB_rs(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    mem_writeB(context, ext_RB(context, inst.rb), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_h_RB_rs(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    mem_writeH(context, ext_RB(context, inst.rb), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_w_RB_rs(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    mem_writeW(context, ext_RB(context, inst.rb), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_b_rd_RBx(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = (char) mem_readB(context, R(inst.rb));
    R(inst.rb) += 1;
    PC += 2;
    CLK += 2;

}

INLINE void exec_ld_ub_rd_RBx(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = (unsigned char) mem_readB(context, R(inst.rb));
    R(inst.rb) += 1;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

INLINE void exec_ld_h_rd_RBx(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = (short) mem_readH(context, R(inst.rb));
    R(inst.rb) += 2;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

INLINE void exec_ld_uh_rd_RBx(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = (unsigned short) mem_readH(context, R(inst.rb));
    R(inst.rb) += 2;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

INLINE void exec_ld_w_rd_RBx(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = mem_readW(context, R(inst.rb));
    R(inst.rb) += 4;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

INLINE void exec_ld_b_RBx_rs(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    mem_writeB(context, R(inst.rb), R(inst.rs_rd));
    R(inst.rb) += 1;
    PC += 2;
    CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */

}

INLINE void exec_ld_h_RBx_rs(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    mem_writeH(context, R(inst.rb), R(inst.rs_rd));
    R(inst.rb) += 2;
    PC += 2;
    CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */

}

INLINE void exec_ld_w_RBx_rs(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    mem_writeW(context, R(inst.rb), R(inst.rs_rd));
    R(inst.rb) += 4;
    PC += 2;
    CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */

}

/****************************************************************************
 *  CLASS 1B
 ****************************************************************************/

INLINE void exec_add_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = add(&PSR, R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = add(&PSR, R(inst.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

INLINE void exec_sub_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = sub(&PSR, R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = sub(&PSR, R(inst.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

// RZ はダミーレジスタ
INLINE void exec_cmp_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    if (!EXT1.s) {
        sub(&PSR, R(inst.rd), R(inst.rs));
    } else {
        sub(&PSR, R(inst.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

INLINE void exec_ld_w_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    NO_EXT
    R(inst.rd) = R(inst.rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_and_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = and(&PSR, R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = and(&PSR, R(inst.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

INLINE void exec_or_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = or(&PSR, R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = or(&PSR, R(inst.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

INLINE void exec_xor_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = xor(&PSR, R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = xor(&PSR, R(inst.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

INLINE void exec_not_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    NO_EXT
    R(inst.rd) = not(&PSR, R(inst.rs));
    PC += 2;
    CLK += 1;
}

/****************************************************************************
 *  CLASS 2
 ****************************************************************************/

INLINE void exec_ld_b_rd_SPxIMM6(PIEMU_CONTEXT *context, CLASS_2 inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33char) mem_readB(context, ext_SPxIMM6(context, inst.imm6, 1));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_ub_rd_SPxIMM6(PIEMU_CONTEXT *context, CLASS_2 inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33byte) mem_readB(context, ext_SPxIMM6(context, inst.imm6, 1));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_h_rd_SPxIMM6(PIEMU_CONTEXT *context, CLASS_2 inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33hint) mem_readH(context, ext_SPxIMM6(context, inst.imm6, 2));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_uh_rd_SPxIMM6(PIEMU_CONTEXT *context, CLASS_2 inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33hword) mem_readH(context, ext_SPxIMM6(context, inst.imm6, 2));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_w_rd_SPxIMM6(PIEMU_CONTEXT *context, CLASS_2 inst)
{
    NO_DELAY
    R(inst.rs_rd) = mem_readW(context, ext_SPxIMM6(context, inst.imm6, 4));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_b_SPxIMM6_rs(PIEMU_CONTEXT *context, CLASS_2 inst)
{
    NO_DELAY
    mem_writeB(context, ext_SPxIMM6(context, inst.imm6, 1), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_h_SPxIMM6_rs(PIEMU_CONTEXT *context, CLASS_2 inst)
{
    NO_DELAY
    mem_writeH(context, ext_SPxIMM6(context, inst.imm6, 2), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_w_SPxIMM6_rs(PIEMU_CONTEXT *context, CLASS_2 inst)
{
    NO_DELAY
    mem_writeW(context, ext_SPxIMM6(context, inst.imm6, 4), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

/****************************************************************************
 *  CLASS 3
 ****************************************************************************/

INLINE void exec_add_rd_imm6(PIEMU_CONTEXT *context, CLASS_3 inst)
{
    c33word un = ext_imm6(context, inst.imm6_sign6);
    R(inst.rd) = add(&PSR, R(inst.rd), un);
    PC += 2;
    CLK += 1;
}

INLINE void exec_sub_rd_imm6(PIEMU_CONTEXT *context, CLASS_3 inst)
{
    c33word un = ext_imm6(context, inst.imm6_sign6);
    R(inst.rd) = sub(&PSR, R(inst.rd), un);
    PC += 2;
    CLK += 1;
}

INLINE void exec_cmp_rd_sign6(PIEMU_CONTEXT *context, CLASS_3 inst)
{
    c33int sn = ext_sign6(context, inst.imm6_sign6);
    sub(&PSR, R(inst.rd), sn); /* 要注意！imm6ではなくsign6です！ */
    PC += 2;
    CLK += 1;
}

INLINE void exec_ld_w_rd_sign6(PIEMU_CONTEXT *context, CLASS_3 inst)
{
    R(inst.rd) = ext_sign6(context, inst.imm6_sign6);
    PC += 2;
    CLK += 1;
}

INLINE void exec_and_rd_sign6(PIEMU_CONTEXT *context, CLASS_3 inst)
{
    c33int sn = ext_sign6(context, inst.imm6_sign6);
    R(inst.rd) = and(&PSR, R(inst.rd), sn);
    PC += 2;
    CLK += 1;
}

INLINE void exec_or_rd_sign6(PIEMU_CONTEXT *context, CLASS_3 inst)
{
    c33int sn = ext_sign6(context, inst.imm6_sign6);
    R(inst.rd) = or(&PSR, R(inst.rd), sn);
    PC += 2;
    CLK += 1;
}

INLINE void exec_xor_rd_sign6(PIEMU_CONTEXT *context, CLASS_3 inst)
{
    c33int sn = ext_sign6(context, inst.imm6_sign6);
    R(inst.rd) = xor(&PSR, R(inst.rd), sn);
    PC += 2;
    CLK += 1;
}

INLINE void exec_not_rd_sign6(PIEMU_CONTEXT *context, CLASS_3 inst)
{
    c33int sn = ext_sign6(context, inst.imm6_sign6);
    R(inst.rd) = not(&PSR, sn);
    PC += 2;
    CLK += 1;
}

/****************************************************************************
 *  CLASS 4A
 ****************************************************************************/

INLINE void exec_add_sp_imm10(PIEMU_CONTEXT *context, CLASS_4A inst)
{
    NO_EXT
    SP += inst.imm10 * 4;
    PC += 2;
    CLK += 1;

}

INLINE void exec_sub_sp_imm10(PIEMU_CONTEXT *context, CLASS_4A inst)
{
    NO_EXT
    SP -= inst.imm10 * 4;
    PC += 2;
    CLK += 1;

}

/****************************************************************************
 *  CLASS 4B
 ****************************************************************************/

INLINE void exec_srl_rd_imm4(PIEMU_CONTEXT *context, CLASS_4B inst)
{
    NO_EXT
    R(inst.rd) = srl(&PSR, R(inst.rd), inst.imm4_rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_sll_rd_imm4(PIEMU_CONTEXT *context, CLASS_4B inst)
{
    NO_EXT
    R(inst.rd) = sll(&PSR, R(inst.rd), inst.imm4_rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_sra_rd_imm4(PIEMU_CONTEXT *context, CLASS_4B inst)
{
    NO_EXT
    R(inst.rd) = sra(&PSR, R(inst.rd), inst.imm4_rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_sla_rd_imm4(PIEMU_CONTEXT *context, CLASS_4B inst)
{
    NO_EXT
    R(inst.rd) = sla(&PSR, R(inst.rd), inst.imm4_rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_rr_rd_imm4(PIEMU_CONTEXT *context, CLASS_4B inst)
{
    NO_EXT
    R(inst.rd) = rr(&PSR, R(inst.rd), inst.imm4_rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_rl_rd_imm4(PIEMU_CONTEXT *context, CLASS_4B inst)
{
    NO_EXT
    R(inst.rd) = rl(&PSR, R(inst.rd), inst.imm4_rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_srl_rd_rs(PIEMU_CONTEXT *context, CLASS_4B inst)
{
    NO_EXT
    R(inst.rd) = srl(&PSR, R(inst.rd), R(inst.imm4_rs));
    PC += 2;
    CLK += 1;
}

INLINE void exec_sll_rd_rs(PIEMU_CONTEXT *context, CLASS_4B inst)
{
    NO_EXT
    R(inst.rd) = sll(&PSR, R(inst.rd), R(inst.imm4_rs));
    PC += 2;
    CLK += 1;
}

INLINE void exec_sra_rd_rs(PIEMU_CONTEXT *context, CLASS_4B inst)
{
    NO_EXT
    R(inst.rd) = sra(&PSR, R(inst.rd), R(inst.imm4_rs));
    PC += 2;
    CLK += 1;
}

INLINE void exec_sla_rd_rs(PIEMU_CONTEXT *context, CLASS_4B inst)
{
    NO_EXT
    R(inst.rd) = sla(&PSR, R(inst.rd), R(inst.imm4_rs));
    PC += 2;
    CLK += 1;
}

INLINE void exec_rr_rd_rs(PIEMU_CONTEXT *context, CLASS_4B inst)
{
    NO_EXT
    R(inst.rd) = rr(&PSR, R(inst.rd), R(inst.imm4_rs));
    PC += 2;
    CLK += 1;
}

INLINE void exec_rl_rd_rs(PIEMU_CONTEXT *context, CLASS_4B inst)
{
    NO_EXT
    R(inst.rd) = rl(&PSR, R(inst.rd), R(inst.imm4_rs));
    PC += 2;
    CLK += 1;
}

/****************************************************************************
 *  CLASS 4C
 ****************************************************************************/

INLINE void exec_scan0_rd_rs(PIEMU_CONTEXT *context, CLASS_4C inst)
{
    NO_EXT
    c33word ua = R(inst.rs);
    c33word ub;
    for (ub = 0; ub < 8; ub++) {
        if (!(ua & (c33word) (1 << 31))) break;
        ua <<= 1;
    }
    R(inst.rd) = ub;
    PSR.z = ub == 0;
    PSR.c = ub == 8;
    PC += 2;
    CLK += 1;
}

INLINE void exec_scan1_rd_rs(PIEMU_CONTEXT *context, CLASS_4C inst)
{
    NO_EXT
    c33word ua = R(inst.rs);
    c33word ub;
    for (ub = 0; ub < 8; ub++) {
        if (ua & (c33word) (1 << 31)) break;
        ua <<= 1;
    }
    R(inst.rd) = ub;
    PSR.z = ub == 0;
    PSR.c = ub == 8;
    PC += 2;
    CLK += 1;
}

INLINE void exec_swap_rd_rs(PIEMU_CONTEXT *context, CLASS_4C inst)
{
    NO_EXT
    c33word ua = R(inst.rs);
    R(inst.rd) = ((ua & 0x000000ffu) << 24) |
                 ((ua & 0x0000ff00u) << 8) |
                 ((ua & 0x00ff0000u) >> 8) |
                 ((ua & 0xff000000u) >> 24);
    PC += 2;
    CLK += 1;
}

INLINE void exec_mirror_rd_rs(PIEMU_CONTEXT *context, CLASS_4C inst)
{
    NO_EXT
    c33word ua = R(inst.rs);
    R(inst.rd) = ((ua & 0x01010101u) << 7) |
                 ((ua & 0x02020202u) << 5) |
                 ((ua & 0x04040404u) << 3) |
                 ((ua & 0x08080808u) << 1) |
                 ((ua & 0x10101010u) >> 1) |
                 ((ua & 0x20202020u) >> 3) |
                 ((ua & 0x40404040u) >> 5) |
                 ((ua & 0x80808080u) >> 7);
    PC += 2;
    CLK += 1;
}

INLINE void exec_div0s_rs(PIEMU_CONTEXT *context, CLASS_4C inst)
{
    NO_EXT NO_DELAY
    if (!R(inst.rs)) DIE()/*core_trap(TRAP_ZERODIV, 0)*/;
    AHR = (int) ALR >> 31;
    PSR.ds = ALR >> 31;
    PSR.n = R(inst.rs) >> 31;
    PC += 2;
    CLK += 1;
}

INLINE void exec_div0u_rs(PIEMU_CONTEXT *context, CLASS_4C inst)
{
    NO_EXT NO_DELAY
    if (!R(inst.rs)) DIE()/*core_trap(TRAP_ZERODIV, 0)*/;
    AHR = 0;
    PSR.ds = 0;
    PSR.n = 0;
    PC += 2;
    CLK += 1;
}

INLINE void exec_div1_rs(PIEMU_CONTEXT *context, CLASS_4C inst)
{
    NO_EXT NO_DELAY
    c33int tmp;

    /* div0x以外では、ゼロ除算例外は発生しません。 */
    AR <<= 1;
    if (!PSR.ds) {
        if (!PSR.n) { /* 正÷正 */
            tmp = AHR - R(inst.rs);
            if (tmp <= AHR) { /* !C */
                AHR = tmp;
                ALR |= 1;
            }
        } else { /* 正÷負 */
            tmp = AHR + R(inst.rs);
            if (tmp < AHR) { /* C */
                AHR = tmp;
                ALR |= 1;
            }
        }
    } else {
        if (!PSR.n) { /* 負÷正 */
            tmp = AHR + R(inst.rs);
            if (tmp >= AHR) { /* !C */
                AHR = tmp;
                ALR |= 1;
            }
        } else { /* 負÷負 */
            tmp = AHR - R(inst.rs);
            if (tmp > AHR) { /* !C */
                AHR = tmp;
                ALR |= 1;
            }
        }
    }
    PC += 2;
    CLK += 1;
}

INLINE void exec_div2s_rs(PIEMU_CONTEXT *context, CLASS_4C inst)
{
    NO_EXT NO_DELAY
    c33int tmp;

    /* div0x以外では、ゼロ除算例外は発生しません。 */
    if (PSR.ds) {
        if (!PSR.n) {
            tmp = AHR + R(inst.rs);
        } else {
            tmp = AHR - R(inst.rs);
        }
        if (!tmp) {
            AHR = tmp;
            ALR += 1;
        }
    }
    PC += 2;
    CLK += 1;
}

INLINE void exec_div3s(PIEMU_CONTEXT *context, CLASS_4C inst)
{
    NO_EXT NO_DELAY
    /* div0x以外では、ゼロ除算例外は発生しません。 */
    if (PSR.ds != PSR.n) {
        ALR = 0 - ALR;  /* ALR = -ALR では警告になるので… */
    }
    PC += 2;
    CLK += 1;
}

/****************************************************************************
 *  CLASS 5A
 ****************************************************************************/

INLINE void exec_ld_w_sd_rs(PIEMU_CONTEXT *context, CLASS_5A inst)
{
    NO_EXT /*NO_DELAY 実はディレイド可能！EPSONライブラリの除算ルーチンが使ってる*/
    S(inst.sd_rd) = R(inst.rs_ss);
    PC += 2;
    CLK += 1;

}

INLINE void exec_ld_w_rd_ss(PIEMU_CONTEXT *context, CLASS_5A inst)
{
    NO_EXT /*NO_DELAY 実はディレイド可能！EPSONライブラリの除算ルーチンが使ってる*/
    R(inst.sd_rd) = S(inst.rs_ss);
    PC += 2;
    CLK += 1;

}

/****************************************************************************
 *  CLASS 5B
 ****************************************************************************/

INLINE void exec_btst_RB_imm3(PIEMU_CONTEXT *context, CLASS_5B inst)
{
    NO_DELAY
    PSR.z = !((mem_readB(context, ext_RB(context, inst.rb)) >> inst.imm3) & 1);
    PC += 2;
    CLK += 3;

}

INLINE void exec_bclr_RB_imm3(PIEMU_CONTEXT *context, CLASS_5B inst)
{
    NO_DELAY
    c33word addr = ext_RB(context, inst.rb);
    mem_writeB(context, addr, mem_readB(context, addr) & ~(1 << inst.imm3));
    PC += 2;
    CLK += 3;
}

INLINE void exec_bset_RB_imm3(PIEMU_CONTEXT *context, CLASS_5B inst)
{
    NO_DELAY
    c33word addr = ext_RB(context, inst.rb);
    mem_writeB(context, addr, mem_readB(context, addr) | (1 << inst.imm3));
    PC += 2;
    CLK += 3;

}

INLINE void exec_bnot_RB_imm3(PIEMU_CONTEXT *context, CLASS_5B inst)
{
    NO_DELAY
    c33word addr = ext_RB(context, inst.rb);
    mem_writeB(context, addr, mem_readB(context, addr) ^ (1 << inst.imm3));
    PC += 2;
    CLK += 3;

}

/****************************************************************************
 *  CLASS 5C
 ****************************************************************************/

INLINE void exec_adc_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT
    R(inst.rd) = adc(&PSR, R(inst.rd), R(inst.rs));
    PC += 2;
    CLK += 1;
}

INLINE void exec_sbc_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT
    R(inst.rd) = sbc(&PSR, R(inst.rd), R(inst.rs));
    PC += 2;
    CLK += 1;
}

INLINE void exec_ld_b_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY /*要注意！ディレイド不可！*/
    R(inst.rd) = (c33char) R(inst.rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_ld_ub_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY /*要注意！ディレイド不可！*/
    R(inst.rd) = (c33byte) R(inst.rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_ld_h_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY /*要注意！ディレイド不可！*/
    R(inst.rd) = (c33hint) R(inst.rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_ld_uh_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY /*要注意！ディレイド不可！*/
    R(inst.rd) = (c33hword) R(inst.rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_mlt_h_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT
    ALR = (c33hint) R(inst.rd) * (c33hint) R(inst.rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_mltu_h_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT
    ALR = (c33hword) R(inst.rd) * (c33hword) R(inst.rs);
    PC += 2;
    CLK += 1;
}

INLINE void exec_mlt_w_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY
    AR = (int64_t) R(inst.rd) * (int64_t) R(inst.rs);
    PC += 2;
    CLK += 5;
}

INLINE void exec_mltu_w_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY
    AR = (uint64_t) R(inst.rd) * (uint64_t) R(inst.rs);
    PC += 2;
    CLK += 5;
}

//int64 a, b, c;
//while(R(inst.rs) != 0) {
//  a = AR;
//  b = (short)mem_read(R(inst.rs + 1), 2) * (short)mem_read(R(inst.rs + 2), 2);
//  c = a + b;
//  AR = a;
//  if(!PSR.mo) { /* 1→0には変化しない */
//    PSR.mo = (a < 0  && b <  0 && c >= 0) ||
//             (a >= 0 && b >= 0 && c <  0);
//  }
//  R(inst.rs)--;
//  R(inst.rs + 1) += 2;
//  R(inst.rs + 2) += 2;
//  CLK += 2;
//}
//PC += 2;
//CLK += 4;
//
//↑本当はこうですが、mac命令実行中に割り込みを受け付けるために、次のように変更しました。
//↓実機と違い毎回mac命令をフェッチしてしまうため、実機よりも実行サイクルが増えています。
//

INLINE void exec_mac_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY
    int64_t a64, b64, c64;

    if (R(inst.rs)) {
        a64 = AR;
        b64 = mem_readH(context, R(inst.rs + 1)) * mem_readH(context, R(inst.rs + 2));
        c64 = a64 + b64;
        AR = a64;
        if (!PSR.mo) { /* 1→0には変化しない */
            PSR.mo = (a64 < 0 && b64 < 0 && c64 >= 0) ||
                     (a64 >= 0 && b64 >= 0 && c64 < 0);
        }

        R(inst.rs)--;
        R(inst.rs + 1) += 2;
        R(inst.rs + 2) += 2;
        /* PCはそのまま。次回もこのmac命令を実行します。 */
        CLK += 2;
    } else {
        PC += 2;
        CLK += 4;
    }
}

/****************************************************************************
 *  CLASS 6
 ****************************************************************************/

INLINE void exec_ext_imm13(PIEMU_CONTEXT *context, CLASS_6 inst)
{
    NO_DELAY
    INST inst2;

    if (!EXT1.s) {
        EXT1.c6 = inst;
    } else if (!EXT2.s) {
        EXT2.c6 = inst;
    } else {
        DIE();
    }
    PC += 2;
    CLK += 1;

    /* 拡張される命令を実行。（この間の割り込みを禁止するため） */
    inst2.s = mem_readH(context, PC);
    core_inst(context, inst2);
    if (EXT1.s) DIE(); /* 確実にextが消費されていること */
}

/****************************************************************************
 *  グローバル関数
 ****************************************************************************/

void
core_init(PIEMU_CONTEXT *context)
{
    memset(&context->core, 0, sizeof context->core);
    PC = mem_readW(context, 0x0c00000);
}

void
core_work(PIEMU_CONTEXT *context)
{
    INST inst;

    if (context->core.in_halt) {
        STD_NOP;
        CLK += HALT_INT_CLK; //1 * NOP_CLK_MULTIPLY;
        return;
    }

    inst.s = mem_readH(context, PC);
    core_inst(context, inst);
}

unsigned
core_workex(PIEMU_CONTEXT *context, unsigned mils_org, unsigned nClocksDivBy1k)
{
    INST inst;
    unsigned insts = 0;
    do {
        inst.s = mem_readH(context, PC);
        core_inst(context, inst);
//    inst.s = mem_readH(context, PC); core_inst(context, inst);
//    inst.s = mem_readH(context, PC); core_inst(context, inst);
//    inst.s = mem_readH(context, PC); core_inst(context, inst);
        insts++;
    } while (!context->bEndFlag && (CLK - mils_org) < nClocksDivBy1k); /* 1ミリ秒分の処理 */
    return insts;
}

void
core_trap(PIEMU_CONTEXT *context, int no, int level)
{
    c33word addr;

    /* マスク可能な割り込みの場合のみ、割り込み可能条件を検査します。 */
    if (no >= 16) {
        if (!PSR.ie) return;
        if ((unsigned) level <= PSR.il) return;
    }

    if (context->core.in_halt)
        context->core.in_halt = 0;

    addr = mem_readW(context, pTTBR_REG + no * 4);  /* ※要検討:トラップテーブル直読みしてます */
    SP -= 4;
    mem_writeW(context, SP, PC); /* 要注意！PC+2じゃないよ！ */
    SP -= 4;
    mem_writeW(context, SP, S(0));
    PC = addr;
    PSR.ie = 0;

    /* マスク可能な割り込みの場合のみ、割り込みレベルを変化させます。 */
    if (no >= 16) {
        PSR.il = level;
    }
}

#if 1
#define MASK(op, shr, and) ((inst.s >> (16 - (shr + and))) & ((1 << and) - 1))
#else
#define MASK(op, shr, and) (op)
#endif

void
core_inst(PIEMU_CONTEXT *context, INST inst)
{
    switch (MASK(inst.c0a.cls, 0, 3)) {
        case 0:
            switch (MASK(inst.c0a.op1, 3, 4)) {
                /* CLASS 0A */
                case 0:
                    switch (MASK(inst.c0a.op2, 8, 2)) {
                        case 0:
                            exec_nop(context, inst.c0a);
                            return;
                        case 1:
                            exec_slp(context, inst.c0a);
                            return;
                        case 2:
                            exec_halt(context, inst.c0a);
                            return;
                        case 3:
                            return;
                    }
                    break;
                case 1:
                    switch (MASK(inst.c0a.op2, 8, 2)) {
                        case 0:
                            exec_pushn_rs(context, inst.c0a);
                            return;
                        case 1:
                            exec_popn_rd(context, inst.c0a);
                            return;
                        case 2:
                            return;
                        case 3:
                            return;
                    }
                    break;
                case 2:
                    switch (MASK(inst.c0a.op2, 8, 2)) {
                        case 0:
                            exec_brk(context, inst.c0a);
                            return;
                        case 1:
                            exec_retd(context, inst.c0a);
                            return;
                        case 2:
                            exec_int_imm2(context, inst.c0a);
                            return;
                        case 3:
                            exec_reti(context, inst.c0a);
                            return;
                    }
                    break;
                case 3:
                    switch (MASK(inst.c0a.op2, 8, 2)) {
                        case 0:
                            exec_call_rb(context, inst.c0a);
                            return;
                        case 1:
                            exec_ret(context, inst.c0a);
                            return;
                        case 2:
                            exec_jp_rb(context, inst.c0a);
                            return;
                        case 3:
                            return;
                    }
                    break;
                    /* CLASS 0B */
                case 4:
                    exec_jrgt_sign8(context, inst.c0b);
                    return;
                case 5:
                    exec_jrge_sign8(context, inst.c0b);
                    return;
                case 6:
                    exec_jrlt_sign8(context, inst.c0b);
                    return;
                case 7:
                    exec_jrle_sign8(context, inst.c0b);
                    return;
                case 8:
                    exec_jrugt_sign8(context, inst.c0b);
                    return;
                case 9:
                    exec_jruge_sign8(context, inst.c0b);
                    return;
                case 10:
                    exec_jrult_sign8(context, inst.c0b);
                    return;
                case 11:
                    exec_jrule_sign8(context, inst.c0b);
                    return;
                case 12:
                    exec_jreq_sign8(context, inst.c0b);
                    return;
                case 13:
                    exec_jrne_sign8(context, inst.c0b);
                    return;
                case 14:
                    exec_call_sign8(context, inst.c0b);
                    return;
                case 15:
                    exec_jp_sign8(context, inst.c0b);
                    return;
            }
            break;
        case 1:
            switch (MASK(inst.c1a.op2, 6, 2)) {
                /* CLASS 1A */
                case 0:
                    switch (MASK(inst.c1a.op1, 3, 3)) {
                        case 0:
                            exec_ld_b_rd_RB(context, inst.c1a);
                            return;
                        case 1:
                            exec_ld_ub_rd_RB(context, inst.c1a);
                            return;
                        case 2:
                            exec_ld_h_rd_RB(context, inst.c1a);
                            return;
                        case 3:
                            exec_ld_uh_rd_RB(context, inst.c1a);
                            return;
                        case 4:
                            exec_ld_w_rd_RB(context, inst.c1a);
                            return;
                        case 5:
                            exec_ld_b_RB_rs(context, inst.c1a);
                            return;
                        case 6:
                            exec_ld_h_RB_rs(context, inst.c1a);
                            return;
                        case 7:
                            exec_ld_w_RB_rs(context, inst.c1a);
                            return;
                    }
                    break;
                case 1:
                    switch (MASK(inst.c1a.op1, 3, 3)) {
                        case 0:
                            exec_ld_b_rd_RBx(context, inst.c1a);
                            return;
                        case 1:
                            exec_ld_ub_rd_RBx(context, inst.c1a);
                            return;
                        case 2:
                            exec_ld_h_rd_RBx(context, inst.c1a);
                            return;
                        case 3:
                            exec_ld_uh_rd_RBx(context, inst.c1a);
                            return;
                        case 4:
                            exec_ld_w_rd_RBx(context, inst.c1a);
                            return;
                        case 5:
                            exec_ld_b_RBx_rs(context, inst.c1a);
                            return;
                        case 6:
                            exec_ld_h_RBx_rs(context, inst.c1a);
                            return;
                        case 7:
                            exec_ld_w_RBx_rs(context, inst.c1a);
                            return;
                    }
                    break;
                    /* CLASS 1B */
                case 2:
                    switch (MASK(inst.c1a.op1, 3, 3)) {
                        case 0:
                            exec_add_rd_rs(context, inst.c1b);
                            return;
                        case 1:
                            exec_sub_rd_rs(context, inst.c1b);
                            return;
                        case 2:
                            exec_cmp_rd_rs(context, inst.c1b);
                            return;
                        case 3:
                            exec_ld_w_rd_rs(context, inst.c1b);
                            return;
                        case 4:
                            exec_and_rd_rs(context, inst.c1b);
                            return;
                        case 5:
                            exec_or_rd_rs(context, inst.c1b);
                            return;
                        case 6:
                            exec_xor_rd_rs(context, inst.c1b);
                            return;
                        case 7:
                            exec_not_rd_rs(context, inst.c1b);
                            return;
                    }
                    break;
            }
            break;
        case 2:
            switch (MASK(inst.c2.op1, 3, 3)) {
                /* CLASS 2 */
                case 0:
                    exec_ld_b_rd_SPxIMM6(context, inst.c2);
                    return;
                case 1:
                    exec_ld_ub_rd_SPxIMM6(context, inst.c2);
                    return;
                case 2:
                    exec_ld_h_rd_SPxIMM6(context, inst.c2);
                    return;
                case 3:
                    exec_ld_uh_rd_SPxIMM6(context, inst.c2);
                    return;
                case 4:
                    exec_ld_w_rd_SPxIMM6(context, inst.c2);
                    return;
                case 5:
                    exec_ld_b_SPxIMM6_rs(context, inst.c2);
                    return;
                case 6:
                    exec_ld_h_SPxIMM6_rs(context, inst.c2);
                    return;
                case 7:
                    exec_ld_w_SPxIMM6_rs(context, inst.c2);
                    return;
            }
            break;
        case 3:
            switch (MASK(inst.c3.op1, 3, 3)) {
                /* CLASS 3 */
                case 0:
                    exec_add_rd_imm6(context, inst.c3);
                    return;
                case 1:
                    exec_sub_rd_imm6(context, inst.c3);
                    return;
                case 2:
                    exec_cmp_rd_sign6(context, inst.c3);
                    return;
                case 3:
                    exec_ld_w_rd_sign6(context, inst.c3);
                    return;
                case 4:
                    exec_and_rd_sign6(context, inst.c3);
                    return;
                case 5:
                    exec_or_rd_sign6(context, inst.c3);
                    return;
                case 6:
                    exec_xor_rd_sign6(context, inst.c3);
                    return;
                case 7:
                    exec_not_rd_sign6(context, inst.c3);
                    return;
            }
            break;
        case 4:
            switch (MASK(inst.c4a.op1, 3, 3)) {
                /* CLASS 4A */
                case 0:
                    exec_add_sp_imm10(context, inst.c4a);
                    return;
                case 1:
                    exec_sub_sp_imm10(context, inst.c4a);
                    return;
                default:
                    switch (MASK(inst.c4b.op2, 6, 2)) {
                        /* CLASS 4B */
                        case 0:
                            switch (MASK(inst.c4b.op1, 3, 3)) {
                                case 0:
                                    return;
                                case 1:
                                    return;
                                case 2:
                                    exec_srl_rd_imm4(context, inst.c4b);
                                    return;
                                case 3:
                                    exec_sll_rd_imm4(context, inst.c4b);
                                    return;
                                case 4:
                                    exec_sra_rd_imm4(context, inst.c4b);
                                    return;
                                case 5:
                                    exec_sla_rd_imm4(context, inst.c4b);
                                    return;
                                case 6:
                                    exec_rr_rd_imm4(context, inst.c4b);
                                    return;
                                case 7:
                                    exec_rl_rd_imm4(context, inst.c4b);
                                    return;
                            }
                            break;
                        case 1:
                            switch (MASK(inst.c4b.op1, 3, 3)) {
                                case 0:
                                    return;
                                case 1:
                                    return;
                                case 2:
                                    exec_srl_rd_rs(context, inst.c4b);
                                    return;
                                case 3:
                                    exec_sll_rd_rs(context, inst.c4b);
                                    return;
                                case 4:
                                    exec_sra_rd_rs(context, inst.c4b);
                                    return;
                                case 5:
                                    exec_sla_rd_rs(context, inst.c4b);
                                    return;
                                case 6:
                                    exec_rr_rd_rs(context, inst.c4b);
                                    return;
                                case 7:
                                    exec_rl_rd_rs(context, inst.c4b);
                                    return;
                            }
                            break;
                            /* CLASS 4C */
                        case 2:
                            switch (MASK(inst.c4c.op1, 3, 3)) {
                                case 0:
                                    return;
                                case 1:
                                    return;
                                case 2:
                                    exec_scan0_rd_rs(context, inst.c4c);
                                    return;
                                case 3:
                                    exec_scan1_rd_rs(context, inst.c4c);
                                    return;
                                case 4:
                                    exec_swap_rd_rs(context, inst.c4c);
                                    return;
                                case 5:
                                    exec_mirror_rd_rs(context, inst.c4c);
                                    return;
                            }
                            break;
                        case 3:
                            switch (MASK(inst.c4c.op1, 3, 3)) {
                                case 0:
                                    return;
                                case 1:
                                    return;
                                case 2:
                                    exec_div0s_rs(context, inst.c4c);
                                    return;
                                case 3:
                                    exec_div0u_rs(context, inst.c4c);
                                    return;
                                case 4:
                                    exec_div1_rs(context, inst.c4c);
                                    return;
                                case 5:
                                    exec_div2s_rs(context, inst.c4c);
                                    return;
                                case 6:
                                    exec_div3s(context, inst.c4c);
                                    return;
                            }
                            break;
                    }
                    break;
            }
            break;
        case 5:
            switch (MASK(inst.c5a.op2, 6, 2)) {
                case 0:
                    switch (MASK(inst.c5a.op1, 3, 3)) {
                        /* CLASS 5A */
                        case 0:
                            exec_ld_w_sd_rs(context, inst.c5a);
                            return;
                        case 1:
                            exec_ld_w_rd_ss(context, inst.c5a);
                            return;
                            /* CLASS 5B */
                        case 2:
                            exec_btst_RB_imm3(context, inst.c5b);
                            return;
                        case 3:
                            exec_bclr_RB_imm3(context, inst.c5b);
                            return;
                        case 4:
                            exec_bset_RB_imm3(context, inst.c5b);
                            return;
                        case 5:
                            exec_bnot_RB_imm3(context, inst.c5b);
                            return;
                            /* CLASS 5C */
                        case 6:
                            exec_adc_rd_rs(context, inst.c5c);
                            return;
                        case 7:
                            exec_sbc_rd_rs(context, inst.c5c);
                            return;
                    }
                    break;
                case 1:
                    switch (MASK(inst.c5c.op1, 3, 3)) {
                        case 0:
                            exec_ld_b_rd_rs(context, inst.c5c);
                            return;
                        case 1:
                            exec_ld_ub_rd_rs(context, inst.c5c);
                            return;
                        case 2:
                            exec_ld_h_rd_rs(context, inst.c5c);
                            return;
                        case 3:
                            exec_ld_uh_rd_rs(context, inst.c5c);
                            return;
                        case 4:
                            return;
                        case 5:
                            return;
                        case 6:
                            return;
                        case 7:
                            return;
                    }
                    break;
                case 2:
                    switch (MASK(inst.c5c.op1, 3, 3)) {
                        case 0:
                            exec_mlt_h_rd_rs(context, inst.c5c);
                            return;
                        case 1:
                            exec_mltu_h_rd_rs(context, inst.c5c);
                            return;
                        case 2:
                            exec_mlt_w_rd_rs(context, inst.c5c);
                            return;
                        case 3:
                            exec_mltu_w_rd_rs(context, inst.c5c);
                            return;
                        case 4:
                            exec_mac_rs(context, inst.c5c);
                            return;
                        case 5:
                            return;
                        case 6:
                            return;
                        case 7:
                            return;
                    }
                    break;
            }
            break;
        case 6:
            /* CLASS 6 */
            exec_ext_imm13(context, inst.c6);
            return;
            break;
        case 7:
            return;
        case 8:
            return;
    }
    DIE();
}


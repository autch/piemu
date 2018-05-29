#include "classes.h"

/* 値dataのビット(bits-1)を、ビット31〜bitsに符号拡張します。 */
int
sign_ext(int data, int bits)
{
    data <<= 32 - bits;
    data >>= 32 - bits;
    return data;
}

/* ディレイド分岐命令の中から使用。引数dに1が指定されたら、ディレイド命令を実行します。 */
void exec_delay(PIEMU_CONTEXT *context, int dflag)
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

#include <stdexcept>
#include "c33core_impl.h"

/* 値dataのビット(bits-1)を、ビット31〜bitsに符号拡張します。 */
int C33Core::sign_ext(int data, int bits)
{
    data <<= 32 - bits;
    data >>= 32 - bits;
    return data;
}

/* ディレイド分岐命令の中から使用。引数dに1が指定されたら、ディレイド命令を実行します。 */
void C33Core::exec_delay(int dflag)
{
    if (dflag) {
        INST d_inst;

        if (this->d) throw std::logic_error("nested delayed insn");  /* 念のため、ディレイドが二重になっていないことを検査 */
        this->d = 1;    /* ディレイド開始 */
        d_inst.s = mem_readH(PC + 2);
        core_inst(d_inst);
        if (!this->d) throw std::logic_error("unexpected exit from delay slot"); /* 念のため、予期しないディレイド解除がないことを検査 */
        this->d = 0;    /* ディレイド終了 */
    }
}


/****************************************************************************
 *  共通演算とPSR設定
 ****************************************************************************/

c33int C33Core::c33add(c33int a, c33int b)
{
    c33int c = a + b;
    PSR.n = c < 0;
    PSR.z = !c;
    PSR.v = (a < 0 && b < 0 && c >= 0) || (a >= 0 && b >= 0 && c < 0);
    PSR.c = (c33word) c < (c33word) a;
    return c;
}

c33int C33Core::c33adc(c33int a, c33int b)
{
    c33int c = a + b;
    c33int d = c + PSR.c;
    PSR.n = d < 0;
    PSR.z = !d;
    PSR.v = (a < 0 && b < 0 && d >= 0) || (a >= 0 && b >= 0 && d < 0);
    PSR.c = ((c33word) c < (c33word) a) || ((c33word) d < (c33word) c);
    return d;
}

c33int C33Core::c33sub(c33int a, c33int b)
{
    c33int c = a - b;
    PSR.n = c < 0;
    PSR.z = !c;
    PSR.v = (a >= 0 && b < 0 && c < 0) || (a < 0 && b >= 0 && c >= 0);
    PSR.c = (c33word) c > (c33word) a;
    return c;
}

c33int C33Core::c33sbc(c33int a, c33int b)
{
    c33int c = a - b;
    c33int d = c - PSR.c;
    PSR.n = d < 0;
    PSR.z = !d;
    PSR.v = (a >= 0 && b < 0 && d < 0) || (a < 0 && b >= 0 && d >= 0);
    PSR.c = ((c33word) c > (c33word) a) || ((c33word) d > (c33word) c);
    return d;
}

c33int C33Core::c33and(c33int a, c33int b)
{
    c33int r = a & b;
    PSR.n = r < 0;
    PSR.z = !r;
    return r;
}

c33int C33Core::c33or(c33int a, c33int b)
{
    c33int r = a | b;
    PSR.n = r < 0;
    PSR.z = !r;
    return r;
}

c33int C33Core::c33xor(c33int a, c33int b)
{
    c33int r = a ^b;
    PSR.n = r < 0;
    PSR.z = !r;
    return r;
}

c33int C33Core::c33not(c33int a)
{
    c33int r = ~a;
    PSR.n = r < 0;
    PSR.z = !r;
    return r;
}

c33int C33Core::c33srl(c33int a, c33int b)
{
    if (b < 0 || b > 8) throw std::invalid_argument("bits not in [0..8]");
    c33int r = (c33word) a >> b;
    PSR.n = r < 0;
    PSR.z = !r;
    return r;
}

c33int C33Core::c33sll(c33int a, c33int b)
{
    if (b < 0 || b > 8) throw std::invalid_argument("bits not in [0..8]");
    c33int r = (c33word) a << b;
    PSR.n = r < 0;
    PSR.z = !r;
    return r;
}

c33int C33Core::c33sra(c33int a, c33int b)
{
    if (b < 0 || b > 8) throw std::invalid_argument("bits not in [0..8]");
    c33int r = (c33int) a >> b;
    PSR.n = r < 0;
    PSR.z = !r;
    return r;
}

c33int C33Core::c33sla(c33int a, c33int b)
{
    if (b < 0 || b > 8) throw std::invalid_argument("bits not in [0..8]");
    c33int r = (c33int) a << b;
    PSR.n = r < 0;
    PSR.z = !r;
    return r;
}

c33int C33Core::c33rr(c33int a, c33int b)
{
    if (b < 0 || b > 8) throw std::invalid_argument("bits not in [0..8]");
    c33int r = ((c33word) a >> b) | ((c33word) a << (32 - b));
    PSR.n = r < 0;
    PSR.z = !r;
    return r;
}

c33int C33Core::c33rl(c33int a, c33int b)
{
    if (b < 0 || b > 8) throw std::invalid_argument("bits not in [0..8]");
    c33int r = ((c33word) a << b) | ((c33word) a >> (32 - b));
    PSR.n = r < 0;
    PSR.z = r;
    return r;
}

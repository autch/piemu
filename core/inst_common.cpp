
#include "c33core_impl.h"

// operations used in insns

inline c33word c33core::sign_ext(c33word data, c33word bits)
{
  data <<= 32 - bits;
  data >>= 32 - bits;
  return data;
}

inline void c33core::exec_delay(c33word dflag)
{
  C33INST d_inst;

  if(dflag)
  {
    /* 念のため、ディレイドが二重になっていないことを検査 */
    if(D) DIE();  
    D = 1;    /* ディレイド開始 */
    d_inst.s = mem_read(PC.w + 2, 2);
    core_decode(d_inst);
    if(!D) DIE(); /* 念のため、予期しないディレイド解除がないことを検査 */
    D = 0;    /* ディレイド終了 */
  }
}

/****************************************************************************
 *  即値拡張
 ****************************************************************************/

inline c33int c33core::ext_imm6(c33int imm6)
{
  c33int data;
  imm6 &= (1 << 6) - 1;
  if(EXT2.s)
  {
    data = imm6 | (EXT2.c6.imm13 << 6) | (EXT1.c6.imm13 << 19);
    EXT1.s = EXT2.s = 0;
  }
  else if(EXT1.s)
  {
    data = imm6 | (EXT1.c6.imm13 << 6);
    EXT1.s = EXT2.s = 0;
  }
  else
  {
    data = imm6;
  }
  return data;
}

inline c33int c33core::ext_sign6(c33int sign6)
{
  c33int data, bits;
  sign6 &= (1 << 6) - 1;
  if(EXT2.s)
  {
    data = sign6 | (EXT2.c6.imm13 << 6) | (EXT1.c6.imm13 << 19);
    bits = 6 + 13 + 13;
    EXT1.s = EXT2.s = 0;
  }
  else if(EXT1.s)
  {
    data = sign6 | (EXT1.c6.imm13 << 6);
    bits = 6 + 13;
    EXT1.s = EXT2.s = 0;
  }
  else
  {
    data = sign6;
    bits = 6;
  }
  return sign_ext(data, bits);
}

inline c33int c33core::ext_RB(c33int rb)
{
  c33int disp; // displacement
  if(EXT2.s)
  {
    disp = (EXT2.c6.imm13 << 0) | (EXT1.c6.imm13 << 13);
    EXT1.s = EXT2.s = 0;
  }
  else if(EXT1.s)
  {
    disp = (EXT1.c6.imm13 << 0);
    EXT1.s = EXT2.s = 0;
  }
  else
  {
    disp = 0;
  }
  return R(rb).i + disp;
}

inline c33int c33core::ext_SPxIMM6(c33int imm6, c33int size)
{
  c33int disp;
  imm6 &= (1 << 6) - 1;
  if(EXT2.s)
  {
    disp = imm6 | (EXT2.c6.imm13 << 6) | (EXT1.c6.imm13 << 19);
    EXT1.s = EXT2.s = 0;
  }
  else if(EXT1.s)
  {
    disp = imm6 | (EXT1.c6.imm13 << 6);
    EXT1.s = EXT2.s = 0;
  }
  else
  {
    disp = imm6 * size;
  }
  return SP.w + disp;
}

inline c33int c33core::ext_3op()
{
  /* NOTE1: 少なくともEXT1が存在することを呼び出し側で確認してください。 */
  /* NOTE2: cmp/and/or/xor/notも、3op拡張時はsignではなくimmとなります。 */
  c33int data = 0;
  if(EXT2.s)
  {
    data = (EXT2.c6.imm13 << 0) | (EXT1.c6.imm13 << 13);
    EXT1.s = EXT2.s = 0;
  }
  else if(EXT1.s)
  {
    data = (EXT1.c6.imm13 << 0);
    EXT1.s = EXT2.s = 0;
  }
  else
  {
    DIE();
  }
  return data;
}

inline c33int c33core::ext_PCxSIGN8(c33int sign8)
{
  c33int disp, bits;
  sign8 &= (1 << 8) - 1;
  if(EXT2.s)
  {
    disp = sign8 | (EXT2.c6.imm13 << 8) | (EXT1.c6.imm13 >> 3 << 21);
    bits = 8 + 13 + 10;
    EXT1.s = EXT2.s = 0;
  }
  else if(EXT1.s)
  {
    disp = sign8 | (EXT1.c6.imm13 << 8);
    bits = 8 + 13;
    EXT1.s = EXT2.s = 0;
  }
  else
  {
    disp = sign8;
    bits = 8;
  }
  return PC.w + sign_ext(disp, bits) * 2;
}

/****************************************************************************
 *  共通演算とPSR設定
 ****************************************************************************/

inline c33int c33core::c33_add(c33int a, c33int b)
{
  c33int c;
  c = a + b;
  PSR.n = c < 0;
  PSR.z = !c;
  PSR.v = (a < 0  && b <  0 && c >= 0)
    || (a >= 0 && b >= 0 && c <  0);
  PSR.c = (c33word)c < (c33word)a;
  return c;
}

inline c33int c33core::c33_adc(c33int a, c33int b)
{
  c33int c, d;
  c = a + b;
  d = c + PSR.c;
  PSR.n = d < 0;
  PSR.z = !d;
  PSR.v = (a < 0  && b <  0 && d >= 0)
    || (a >= 0 && b >= 0 && d <  0);
  PSR.c = ((c33word)c < (c33word)a)
    || ((c33word)d < (c33word)c);
  return d;
}

inline c33int c33core::c33_sub(c33int a, c33int b)
{
  c33int c;
  c = a - b;
  PSR.n = c < 0;
  PSR.z = !c;
  PSR.v = (a >= 0 && b <  0 && c <  0)
    || (a <  0 && b >= 0 && c >= 0);
  PSR.c = (c33word)c > (c33word)a;
  return c;
}

inline c33int c33core::c33_sbc(c33int a, c33int b)
{
  c33int c, d;
  c = a - b;
  d = c - PSR.c;
  PSR.n = d < 0;
  PSR.z = !d;
  PSR.v = (a >= 0 && b <  0 && d <  0)
    || (a <  0 && b >= 0 && d >= 0);
  PSR.c = ((c33word)c > (c33word)a)
    || ((c33word)d > (c33word)c);
  return d;
}

inline c33int c33core::c33_and(c33int a, c33int b)
{
  c33int r;
  r = a & b;
  PSR.n = r < 0;
  PSR.z = !r;
  return r;
}

inline c33int c33core::c33_or(c33int a, c33int b)
{
  c33int r;
  r = a | b;
  PSR.n = r < 0;
  PSR.z = !r;
  return r;
}

inline c33int c33core::c33_xor(c33int a, c33int b)
{
  c33int r;
  r = a ^ b;
  PSR.n = r < 0;
  PSR.z = !r;
  return r;
}

inline c33int c33core::c33_not(c33int a)
{
  c33int r;
  r = ~a;
  PSR.n = r < 0;
  PSR.z = !r;
  return r;
}

inline c33word c33core::c33_srl(c33word a, c33word b)
{
  c33word r;
  r = a >> b;
  PSR.n = (c33int)r < 0;
  PSR.z = !r;
  return r;
}

inline c33word c33core::c33_sll(c33word a, c33word b)
{
  c33word r;
  r = a << b;
  PSR.n = (c33int)r < 0;
  PSR.z = !r;
  return r;
}

inline c33int c33core::c33_sra(c33int a, c33int b)
{
  c33int r;
  r = a >> b;
  PSR.n = r < 0;
  PSR.z = !r;
  return r;
}

inline c33int c33core::c33_sla(c33int a, c33int b)
{
  c33int r;
  r = a << b;
  PSR.n = r < 0;
  PSR.z = !r;
  return r;
}

inline c33word c33core::c33_rr(c33word a, c33word b)
{
  c33word r;
  r = (a >> b) | (a << (32 - b));
  PSR.n = (c33int)r < 0;
  PSR.z = !r;
  return r;
}

inline c33word c33core::c33_rl(c33word a, c33word b)
{
  c33word r;
  r = (a << b) | (a >> (32 - b));
  PSR.n = (c33int)r < 0;
  PSR.z = !r;
  return r;
}


#include "inst_types.h"
#include "core_types.h"

#define DIE() { (void)0; }

#define R(n) (core->r[n])
#define S(n) (core->s[n])

#define PSR  (*(C33PSR*)&S(0))
#define SP   (S(1))
#define AR   (*(c33lint*)&S(2))
#define ALR  (S(2))
#define AHR  (S(3))
#define D    (core->d)
#define PC   (core->pc)
#define EXT1 (core->ext[0])
#define EXT2 (core->ext[1])

#define Rs   (R(inst.rs))
#define Rd   (R(inst.rd))
#define Rb   (R(inst.rb))
#define Rsd  (R(inst.rs_rd))

// shortcut for definition
#define C33_OP(name)      void c33op_##name(C33CORE* core, INST inst)
// shortcut for calling
#define CALL_C33_OP(name, core, inst) c33op_##name(core, inst)
#define NAME_C33_OP(name) c33op_##name

c33word sign_ext(c33word data, c33word bits)
{
  data <<= 32 - bits;
  data >>= 32 - bits;
  return data;
}

void exec_delay(C33CORE* core, c33word dflag)
{
  C33INST d_inst;

  if(dflag)
  {
    /* 念のため、ディレイドが二重になっていないことを検査 */
    if(D) DIE();  
    D = 1;    /* ディレイド開始 */
    d_inst.s = mem_readH(core, PC.w + 2);
    inst_exec(core, d_inst);
    if(!D) DIE(); /* 念のため、予期しないディレイド解除がないことを検査 */
    D = 0;    /* ディレイド終了 */
  }
}

/****************************************************************************
 *  即値拡張
 ****************************************************************************/

c33int ext_imm6(C33CORE* core, c33int imm6)
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

c33int ext_sign6(C33CORE* core, c33int sign6)
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

c33int ext_RB(C33CORE* core, c33int rb)
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

c33int ext_SPxIMM6(C33CORE* core, c33int imm6, c33int size)
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

c33int ext_3op(C33CORE* core)
{
  /* NOTE1: 少なくともEXT1が存在することを呼び出し側で確認してください。 */
  /* NOTE2: cmp/and/or/xor/notも、3op拡張時はsignではなくimmとなります。 */
  c33int data;
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

c33int ext_PCxSIGN8(C33CORE* core, c33int sign8)
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

c33int add(C33CORE* core, c33int a, c33int b)
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

c33int adc(C33CORE* core, c33int a, c33int b)
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

c33int sub(C33CORE* core, c33int a, c33int b)
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

c33int sbc(C33CORE* core, c33int a, c33int b)
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

c33int and(C33CORE* core, c33int a, c33int b)
{
  c33int r;
  r = a & b;
  PSR.n = r < 0;
  PSR.z = !r;
  return r;
}

c33int or(C33CORE* core, c33int a, c33int b)
{
  c33int r;
  r = a | b;
  PSR.n = r < 0;
  PSR.z = !r;
  return r;
}

c33int xor(C33CORE* core, c33int a, c33int b)
{
  c33int r;
  r = a ^ b;
  PSR.n = r < 0;
  PSR.z = !r;
  return r;
}

c33int not(C33CORE* core, c33int a)
{
  c33int r;
  r = ~a;
  PSR.n = r < 0;
  PSR.z = !r;
  return r;
}

c33word srl(C33CORE* core, c33word a, c33word b)
{
  c33word r;
  r = a >> b;
  PSR.n = (c33int)r < 0;
  PSR.z = !r;
  return r;
}

c33word sll(C33CORE* core, c33word a, c33word b)
{
  c33word r;
  r = a << b;
  PSR.n = (c33int)r < 0;
  PSR.z = !r;
  return r;
}

c33int sra(C33CORE* core, c33int a, c33int b)
{
  c33int r;
  r = a >> b;
  PSR.n = r < 0;
  PSR.z = !r;
  return r;
}

c33int sla(C33CORE* core, c33int a, c33int b)
{
  c33int r;
  r = a << b;
  PSR.n = r < 0;
  PSR.z = !r;
  return r;
}

c33word rr(C33CORE* core, c33word a, c33word b)
{
  c33word r;
  r = (a >> b) | (a << (32 - b));
  PSR.n = (c33int)r < 0;
  PSR.z = !r;
  return r;
}

c33word rl(C33CORE* core, c33word a, c33word b)
{
  c33word r;
  r = (a << b) | (a >> (32 - b));
  PSR.n = (c33int)r < 0;
  PSR.z = !r;
  return r;
}

/****************************************************************************
 *  CLASS 0A
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_0A

C33_OP(nop)
{
  PC.w += 2;
}

C33_OP(slp)
{
  /* ※TODO: */
  PC.w += 2;
}

C33_OP(halt)
{
  /* ※TODO: */
  PC.w += 2;
}

C33_OP(pushn_rs)
{
  c33int rs;
  for(rs = inst.imm2_rd_rs; rs >= 0; rs--)
  {
    SP.w -= 4;
    mem_writeW(core, SP.w, R(rs).w);
  }
  PC.w += 2;
}

C33_OP(popn_rd)
{
  c33int rd;
  for(rd = 0; rd <= inst.imm2_rd_rs; rd++)
  {
    R(rd).w = mem_readW(core, SP.w);
    SP.w += 4;
  }
  PC.w += 2;
}

C33_OP(brk)
{
  // do nothing
}

C33_OP(retd)
{
  // do nothing
}

C33_OP(int_imm2)
{
  if(inst.imm2_rd_rs < 0 || inst.imm2_rd_rs > 3) DIE();
  PC.w += 2;
  core_trap(core, TRAP_SOFTINT0 + inst.imm2_rd_rs, 0);
}

C33_OP(reti)
{
  S(0).w = mem_readW(core, SP.w);
  SP.w += 4;
  PC.w = mem_readW(core, SP.w);
  SP.w += 4;
}

C33_OP(call_rb)
{
  c33word addr;
  addr = R(inst.imm2_rd_rs).w;
  exec_delay(core, inst.d);
  SP.w -= 4;
  mem_writeW(core, SP.w, PC.w + 2);
  PC.w = addr;
}

C33_OP(ret)
{
  c33word addr;
  addr = mem_readW(core, SP.w);
  SP.w += 4;
  exec_delay(core, inst.d);
  PC.w = addr;
}

C33_OP(jp_rb)
{
  c33word addr;
  addr = R(inst.imm2_rd_rs).w;
  exec_delay(core, inst.d);
  PC.w = addr;
}

/****************************************************************************
 *  CLASS 0B
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_0B

C33_OP(jrgt_sign8)
{
  c33word cc, addr;
  cc = !(PSR.n ^ PSR.v) && !PSR.z;
  addr = ext_PCxSIGN8(core, inst.sign8);
  exec_delay(core, inst.d);
  PC.w = cc ? addr : PC.w + 2;
}

C33_OP(jrge_sign8)
{
  c33word cc, addr;
  cc = !(PSR.n ^ PSR.v);
  addr = ext_PCxSIGN8(core, inst.sign8);
  exec_delay(core, inst.d);
  PC.w = cc ? addr : PC.w + 2;
}

C33_OP(jrlt_sign8)
{
  c33word cc, addr;
  cc = (PSR.n ^ PSR.v);
  addr = ext_PCxSIGN8(core, inst.sign8);
  exec_delay(core, inst.d);
  PC.w = cc ? addr : PC.w + 2;
}

C33_OP(jrle_sign8)
{
  c33word cc, addr;
  cc = (PSR.n ^ PSR.v) || PSR.z;
  addr = ext_PCxSIGN8(core, inst.sign8);
  exec_delay(core, inst.d);
  PC.w = cc ? addr : PC.w + 2;
}

C33_OP(jrugt_sign8)
{
  c33word cc, addr;
  cc = !PSR.c && !PSR.z;
  addr = ext_PCxSIGN8(core, inst.sign8);
  exec_delay(core, inst.d);
  PC.w = cc ? addr : PC.w + 2;
}

C33_OP(jruge_sign8)
{
  c33word cc, addr;
  cc = !PSR.c;
  addr = ext_PCxSIGN8(core, inst.sign8);
  exec_delay(core, inst.d);
  PC.w = cc ? addr : PC.w + 2;
}

C33_OP(jrult_sign8)
{
  c33word cc, addr;
  cc = PSR.c;
  addr = ext_PCxSIGN8(core, inst.sign8);
  exec_delay(core, inst.d);
  PC.w = cc ? addr : PC.w + 2;
}

C33_OP(jrule_sign8)
{
  c33word cc, addr;
  cc = PSR.c || PSR.z;
  addr = ext_PCxSIGN8(core, inst.sign8);
  exec_delay(core, inst.d);
  PC.w = cc ? addr : PC.w + 2;
}

C33_OP(jreq_sign8)
{
  c33word cc, addr;
  cc = PSR.z;
  addr = ext_PCxSIGN8(core, inst.sign8);
  exec_delay(core, inst.d);
  PC.w = cc ? addr : PC.w + 2;
}

C33_OP(jrne_sign8)
{
  c33word cc, addr;
  cc = !PSR.z;
  addr = ext_PCxSIGN8(core, inst.sign8);
  exec_delay(core, inst.d);
  PC.w = cc ? addr : PC.w + 2;
}

C33_OP(call_sign8)
{
  c33word addr;
  addr = ext_PCxSIGN8(core, inst.sign8);
  exec_delay(core, inst.d);
  SP.w -= 4;
  mem_writeW(core, SP.w, PC.w + 2);
  PC.w = addr;
}

C33_OP(jp_sign8)
{
  c33word addr;
  addr = ext_PCxSIGN8(core, inst.sign8);
  exec_delay(core, inst.d);
  PC.w = addr;
}

/****************************************************************************
 *  CLASS 1A
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_1A

C33_OP(ld_b_rd_RB)
{
  Rsd.i = (c33char)mem_readB(core, ext_RB(core, inst.rb));
  PC.w += 2;
}

C33_OP(ld_ub_rd_RB)
{
  Rsd.w = (c33byte)mem_readB(core, ext_RB(core, inst.rb));
  PC.w += 2;
}

C33_OP(ld_h_rd_RB)
{
  Rsd.i = (c33hint)mem_readH(core, ext_RB(core, inst.rb));
  PC.w += 2;
}

C33_OP(ld_uh_rd_RB)
{
  Rsd.w = (c33hword)mem_readH(core, ext_RB(core, inst.rb));
  PC.w += 2;
}

C33_OP(ld_w_rd_RB)
{
  Rsd.i = (c33int)mem_readW(core, ext_RB(core, inst.rb));
  PC.w += 2;
}

C33_OP(ld_b_RB_rs)
{
  mem_writeB(core, ext_RB(core, inst.rb), Rsd.w);
  PC.w += 2;
}

C33_OP(ld_h_RB_rs)
{
  mem_writeH(core, ext_RB(core, inst.rb), Rsd.w);
  PC.w += 2;
}

C33_OP(ld_w_RB_rs)
{
  mem_writeW(core, ext_RB(core, inst.rb), Rsd.w);
  PC.w += 2;
}

C33_OP(ld_b_rd_RBx)
{
  Rsd.i = (c33char)mem_readB(core, Rb.w);
  Rb.w += 1;
  PC.w += 2;
}

C33_OP(ld_ub_rd_RBx)
{
  Rsd.w = (c33byte)mem_readB(core, Rb.w);
  Rb.w += 1;
  PC.w += 2;
}

C33_OP(ld_h_rd_RBx)
{
  Rsd.i = (c33hint)mem_readH(core, Rb.w);
  Rb.w += 2;
  PC.w += 2;
}

C33_OP(ld_uh_rd_RBx)
{
  Rsd.w = (c33hword)mem_readH(core, Rb.w);
  Rb.w += 2;
  PC.w += 2;
}

C33_OP(ld_w_rd_RBx)
{
  Rsd.i = mem_readW(core, Rb.w);
  Rb.w += 4;
  PC.w += 2;
}

C33_OP(ld_b_RBx_rs)
{
  mem_writeB(core, Rb.w, Rsd.w);
  Rb.w += 1;
  PC.w += 2;
}

C33_OP(ld_h_RBx_rs)
{
  mem_writeH(core, Rb.w, Rsd.w);
  Rb.w += 2;
  PC.w += 2;
}

C33_OP(ld_w_RBx_rs)
{
  mem_writeW(core, Rb.w, Rsd.w);
  Rb.w += 4;
  PC.w += 2;
}

/****************************************************************************
 *  CLASS 1B
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_1B

C33_OP(add_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.i = add(core, Rd.i, Rs.i);
  }
  else
  {
    un = ext_3op(core);
    Rd.i = add(core, Rs.i, un);
  }
  PC.w += 2;
}

C33_OP(sub_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.i = sub(core, Rd.i, Rs.i);
  }
  else
  {
    un = ext_3op(core);
    Rd.i = sub(core, Rs.i, un);
  }
  PC.w += 2;
}

C33_OP(cmp_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    sub(core, Rd.i, Rs.i);
  }
  else
  {
    un = ext_3op(core);
    sub(core, Rs.i, un);
  }
  PC.w += 2;
}

C33_OP(ld_w_rd_rs)
{
  Rd.i = Rs.i;
  PC.w += 2;
}

C33_OP(and_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.w = and(core, Rd.w, Rs.w);
  }
  else
  {
    un = ext_3op(core);
    Rd.w = and(core, Rs.w, un);
  }
  PC.w += 2;
}

C33_OP(or_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.w = or(core, Rd.w, Rs.w);
  }
  else
  {
    un = ext_3op(core);
    Rd.w = or(core, Rs.w, un);
  }
  PC.w += 2;
}

C33_OP(xor_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.w = xor(core, Rd.w, Rs.w);
  }
  else
  {
    un = ext_3op(core);
    Rd.w = xor(core, Rs.w, un);
  }
  PC.w += 2;
}

C33_OP(not_rd_rs)
{
  Rd.w = not(core, Rs.w);
  PC.w += 2;
}

/****************************************************************************
 *  CLASS 2
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_2

C33_OP(ld_b_rd_SPxIMM6)
{
  Rsd.i = (c33char)mem_readB(core, ext_SPxIMM6(core, inst.imm6, 1));
  PC.w += 2;
}

C33_OP(ld_ub_rd_SPxIMM6)
{
  Rsd.w = (c33byte)mem_readB(core, ext_SPxIMM6(core, inst.imm6, 1));
  PC.w += 2;
}

C33_OP(ld_h_rd_SPxIMM6)
{
  Rsd.i = (c33hint)mem_readH(core, ext_SPxIMM6(core, inst.imm6, 2));
  PC.w += 2;
}

C33_OP(ld_uh_rd_SPxIMM6)
{
  Rsd.w = (c33hword)mem_readH(core, ext_SPxIMM6(core, inst.imm6, 2));
  PC.w += 2;
}

C33_OP(ld_w_rd_SPxIMM6)
{
  Rsd.i = mem_readW(core, ext_SPxIMM6(core, inst.imm6, 4));
  PC.w += 2;
}

C33_OP(ld_b_SPxIMM6_rs)
{
  mem_writeB(core, ext_SPxIMM6(core, inst.imm6, 1), Rsd.i);
  PC.w += 2;
}

C33_OP(ld_h_SPxIMM6_rs)
{
  mem_writeH(core, ext_SPxIMM6(core, inst.imm6, 2), Rsd.i);
  PC.w += 2;
}

C33_OP(ld_w_SPxIMM6_rs)
{
  mem_writeW(core, ext_SPxIMM6(core, inst.imm6, 4), Rsd.i);
  PC.w += 2;
}

/****************************************************************************
 *  CLASS 3
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_3

C33_OP(add_rd_imm6)
{
  c33word un;
  un = ext_imm6(core, inst.imm6_sign6);
  Rd.i = add(core, Rd.i, un);
  PC.w += 2;
}

C33_OP(sub_rd_imm6)
{
  c33word un;
  un = ext_imm6(core, inst.imm6_sign6);
  Rd.i = sub(core, Rd.i, un);
  PC.w += 2;
}

C33_OP(cmp_rd_sign6)
{
  c33int sn;
  sn = ext_sign6(core, inst.imm6_sign6);
  sub(core, Rd.i, sn); /* 要注意！imm6ではなくsign6です！ */
  PC.w += 2;
}

C33_OP(ld_w_rd_sign6)
{
  Rd.i = ext_sign6(core, inst.imm6_sign6);
  PC.w += 2;
}

C33_OP(and_rd_sign6)
{
  c33int sn;
  sn = ext_sign6(core, inst.imm6_sign6);
  Rd.w = and(core, Rd.w, sn);
  PC.w += 2;
}

C33_OP(or_rd_sign6)
{
  c33int sn;
  sn = ext_sign6(core, inst.imm6_sign6);
  Rd.w = or(core, Rd.w, sn);
  PC.w += 2;
}

C33_OP(xor_rd_sign6)
{
  c33int sn;
  sn = ext_sign6(core, inst.imm6_sign6);
  Rd.w = xor(core, Rd.w, sn);
  PC.w += 2;
}

C33_OP(not_rd_sign6)
{
  c33int sn;
  sn = ext_sign6(core, inst.imm6_sign6);
  Rd.w = not(core, sn);
  PC.w += 2;
}

/****************************************************************************
 *  CLASS 4A
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_4A

C33_OP(add_sp_imm10)
{
  SP.w += inst.imm10 * 4;
  PC.w += 2;
}

C33_OP(sub_sp_imm10)
{
  SP.w -= inst.imm10 * 4;
  PC.w += 2;
}

/****************************************************************************
 *  CLASS 4B
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_4B

C33_OP(srl_rd_imm4)
{
  Rd.w = srl(core, Rd.w, inst.imm4_rs);
  PC.w += 2;
}

C33_OP(sll_rd_imm4)
{
  Rd.w = sll(core, Rd.w, inst.imm4_rs);
  PC.w += 2;
}

C33_OP(sra_rd_imm4)
{
  Rd.i = sra(core, Rd.i, inst.imm4_rs);
  PC.w += 2;
}

C33_OP(sla_rd_imm4)
{
  Rd.i = sla(core, Rd.i, inst.imm4_rs);
  PC.w += 2;
}

C33_OP(rr_rd_imm4)
{
  Rd.w = rr(core, Rd.w, inst.imm4_rs);
  PC.w += 2;
}

C33_OP(rl_rd_imm4)
{
  Rd.w = rl(core, Rd.w, inst.imm4_rs);
  PC.w += 2;
}

C33_OP(srl_rd_rs)
{
  Rd.w = srl(core, Rd.w, R(inst.imm4_rs).w);
  PC.w += 2;
}

C33_OP(sll_rd_rs)
{
  Rd.w = sll(core, Rd.w, R(inst.imm4_rs).w);
  PC.w += 2;
}

C33_OP(sra_rd_rs)
{
  Rd.i = sra(core, Rd.i, R(inst.imm4_rs).i);
  PC.w += 2;
}

C33_OP(sla_rd_rs)
{
  Rd.i = sla(core, Rd.i, R(inst.imm4_rs).i);
  PC.w += 2;
}

C33_OP(rr_rd_rs)
{
  Rd.w = rr(core, Rd.w, R(inst.imm4_rs).w);
  PC.w += 2;
}

C33_OP(rl_rd_rs)
{
  Rd.w = rl(core, Rd.w, R(inst.imm4_rs).w);
  PC.w += 2;
}

/****************************************************************************
 *  CLASS 4C
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_4C

C33_OP(scan0_rd_rs)
{
  c33word ua, ub;
  ua = Rs.w;
  for(ub = 0; ub < 8; ub++)
  {
    if(!(ua & (1 << 31))) break;
    ua <<= 1;
  }
  Rd.w = ub;
  PSR.z = ub == 0;
  PSR.c = ub == 8;
  PC.w += 2;
}

C33_OP(scan1_rd_rs)
{
  c33word ua, ub;
  ua = Rs.w;
  for(ub = 0; ub < 8; ub++)
  {
    if(ua & (1 << 31)) break;
    ua <<= 1;
  }
  Rd.w = ub;
  PSR.z = ub == 0;
  PSR.c = ub == 8;
  PC.w += 2;
}

C33_OP(swap_rd_rs)
{
  c33word ua;
  ua = Rs.w;
  Rd.w = ((ua & 0x000000ff) << 24)
    | ((ua & 0x0000ff00) <<  8)
    | ((ua & 0x00ff0000) >>  8)
    | ((ua & 0xff000000) >> 24);
  PC.w += 2;
}

C33_OP(mirror_rd_rs)
{
  c33word ua;
  ua = Rs.w;
  Rd.w = ((ua & 0x01010101) << 7)
    | ((ua & 0x02020202) << 5)
    | ((ua & 0x04040404) << 3)
    | ((ua & 0x08080808) << 1)
    | ((ua & 0x10101010) >> 1)
    | ((ua & 0x20202020) >> 3)
    | ((ua & 0x40404040) >> 5)
    | ((ua & 0x80808080) >> 7);
  PC.w += 2;
}

C33_OP(div0s_rs)
{
  if(!Rs.i) core_trap(TRAP_ZERODIV, 0);
  AHR.w = (c33int)ALR.w >> 31;
  PSR.ds = ALR.w >> 31;
  PSR.n = Rs.w >> 31;
  PC.w += 2;
}

C33_OP(div0u_rs)
{
  if(!Rs.i) core_trap(TRAP_ZERODIV, 0);
  AHR.w = 0;
  PSR.ds = 0;
  PSR.n = 0;
  PC.w += 2;
}

C33_OP(div1_rs)
{
  c33word tmp;
  /* div0x以外では、ゼロ除算例外は発生しません。 */
  AR <<= 1;
  if(!PSR.ds)
  {
    if(!PSR.n)
    {
      /* 正÷正 */
      tmp = AHR.i - Rs.i;
      if(tmp <= AHR.i)
      { 
        /* !C */
        AHR.i = tmp;
        ALR.i |= 1;
      }
    }
    else
    {
      /* 正÷負 */
      tmp = AHR.i + Rs.i;
      if(tmp < AHR.i)
      {
        /* C */
        AHR.i = tmp;
        ALR.i |= 1;
      }
    }
  }
  else
  {
    if(!PSR.n)
    { 
      /* 負÷正 */
      tmp = AHR.i + Rs.i;
      if(tmp >= AHR.i)
      {
        /* !C */
        AHR.i = tmp;
        ALR.i |= 1;
      }
    }
    else
    {
      /* 負÷負 */
      tmp = AHR.i - Rs.i;
      if(tmp > AHR.i)
      {
        /* !C */
        AHR.i = tmp;
        ALR.i |= 1;
      }
    }
  }
  PC.w += 2;
}

C33_OP(div2s_rs)
{
  c33word tmp;
  /* div0x以外では、ゼロ除算例外は発生しません。 */
  if(PSR.ds)
  {
    if(!PSR.n)
    {
      tmp = AHR.i + Rs.i;
    }
    else
    {
      tmp = AHR.i - Rs.i;
    }
    if(!tmp)
    {
      AHR.i = tmp;
      ALR.i += 1;
    }
  }
  PC.w += 2;
}

C33_OP(div3s)
{
  /* div0x以外では、ゼロ除算例外は発生しません。 */
  if(PSR.ds != PSR.n)
  {
    ALR.i = 0 - ALR.i;  /* ALR = -ALR では警告になるので… */
  }
  PC.w += 2;
}

/****************************************************************************
 *  CLASS 5A
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_5A

C33_OP(ld_w_sd_rs)
{
  /* 実はディレイド可能！EPSONライブラリの除算ルーチンが使ってる*/
  S(inst.sd_rd).w = R(inst.rs_ss).w;
  PC.w += 2;
}

C33_OP(ld_w_rd_ss)
{
  /* 実はディレイド可能！EPSONライブラリの除算ルーチンが使ってる*/
  R(inst.sd_rd).w = S(inst.rs_ss).w;
  PC.w += 2;
}

/****************************************************************************
 *  CLASS 5B
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_5B

C33_OP(btst_RB_imm3)
{
  PSR.z = !((mem_readB(core, ext_RB(core, inst.rb)) >> inst.imm3) & 1);
  PC.w += 2;
}

C33_OP(bclr_RB_imm3)
{
  c33word addr;
  addr = ext_RB(core, inst.rb);
  mem_writeB(core, addr, mem_readB(core, addr) & ~(1 << inst.imm3));
  PC.w += 2;
}

C33_OP(bset_RB_imm3)
{
  c33word addr;
  addr = ext_RB(core, inst.rb);
  mem_writeB(core, addr, mem_readB(core, addr) | (1 << inst.imm3));
  PC.w += 2;
}

C33_OP(bnot_RB_imm3)
{
  c33word addr;
  addr = ext_RB(core, inst.rb);
  mem_writeB(core, addr, mem_readB(core, addr) ^ (1 << inst.imm3));
  PC.w += 2;
}

/****************************************************************************
 *  CLASS 5C
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_5C

C33_OP(adc_rd_rs)
{
  Rd.i = adc(core, Rd.i, Rs.i);
  PC.w += 2;
}

C33_OP(sbc_rd_rs)
{
  Rd.i = sbc(core, Rd.i, Rs.i);
  PC.w += 2;
}

C33_OP(ld_b_rd_rs)   /*要注意！ディレイド不可！*/
{
  Rd.i = (c33char)Rs.i;
  PC.w += 2;
}

C33_OP(ld_ub_rd_rs)    /*要注意！ディレイド不可！*/
{
  Rd.w = (c33byte)Rs.w;
  PC.w += 2;
}

C33_OP(ld_h_rd_rs)    /*要注意！ディレイド不可！*/
{
  Rd.i = (c33hint)Rs.i;
  PC.w += 2;
}

C33_OP(ld_uh_rd_rs)    /*要注意！ディレイド不可！*/
{
  Rd.w = (c33hword)Rs.w;
  PC.w += 2;
}

C33_OP(mlt_h_rd_rs)
{
  ALR.i = (c33hint)Rd.i * (c33hint)Rs.i;
  PC.w += 2;
}

C33_OP(mltu_h_rd_rs)
{
  ALR.w = (c33hword)Rd.w * (c33hword)Rs.w;
  PC.w += 2;
}

C33_OP(mlt_w_rd_rs)
{
  AR = (c33lint)Rd.i * (c33lint)Rs.i;
  PC.w += 2;
}

C33_OP(mltu_w_rd_rs)
{
  AR = (c33lword)Rd.w * (c33lword)Rs.w;
  PC.w += 2;
}

//int64 a, b, c;
//while(Rs != 0) {
//  a = AR;
//  b = (short)mem_read(R(inst.rs + 1), 2) * (short)mem_read(R(inst.rs + 2), 2);
//  c = a + b;
//  AR = a;
//  if(!PSR.mo) { /* 1→0には変化しない */
//    PSR.mo = (a < 0  && b <  0 && c >= 0) ||
//             (a >= 0 && b >= 0 && c <  0);
//  }
//  Rs--;
//  R(inst.rs + 1) += 2;
//  R(inst.rs + 2) += 2;
//  CLK += 2;
//}
//PC.w += 2;
//CLK += 4;
//
//↑本当はこうですが、mac命令実行中に割り込みを受け付けるために、次のように変更しました。
//↓実機と違い毎回mac命令をフェッチしてしまうため、実機よりも実行サイクルが増えています。
//

C33_OP(mac_rs)
{
  c33lint a, b, c;
  if(Rs.i)
  {
    a = AR;
    b = mem_readH(core, R(inst.rs + 1).w) * mem_readH(core, R(inst.rs + 2).w);
    c = a + b;
    AR = a;
    if(!PSR.mo)
    {
      /* 1→0には変化しない */
      PSR.mo = (a < 0  && b <  0 && c >= 0)
	|| (a >= 0 && b >= 0 && c <  0);
    }
    Rs.i--;
    R(inst.rs + 1).i += 2;
    R(inst.rs + 2).i += 2;
    /* PCはそのまま。次回もこのmac命令を実行します。 */
  }
  else
  {
    PC.w += 2;
  }
}

/****************************************************************************
 *  CLASS 6
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_6

C33_OP(ext_imm13)
{
  C33INST inst2;
  if(!EXT1.s)
  {
    EXT1.c6 = inst;
  }
  else if(!EXT2.s)
  {
    EXT2.c6 = inst;
  }
  else
  {
    DIE();
  }

  PC.w += 2;

  /* 拡張される命令を実行。（この間の割り込みを禁止するため） */
  inst2.s = mem_readH(core, PC);
  core_inst(core, inst2);
  if(EXT1.s) DIE(); /* 確実にextが消費されていること */
}

#undef INST


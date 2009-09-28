
#include "inst.h"

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
  CLK += 1;
}

C33_OP(sub_rd_imm6)
{
  c33word un;
  un = ext_imm6(core, inst.imm6_sign6);
  Rd.i = sub(core, Rd.i, un);
  PC.w += 2;
  CLK += 1;
}

C33_OP(cmp_rd_sign6)
{
  c33int sn;
  sn = ext_sign6(core, inst.imm6_sign6);
  sub(core, Rd.i, sn); /* 要注意！imm6ではなくsign6です！ */
  PC.w += 2;
  CLK += 1;
}

C33_OP(ld_w_rd_sign6)
{
  Rd.i = ext_sign6(core, inst.imm6_sign6);
  PC.w += 2;
  CLK += 1;
}

C33_OP(and_rd_sign6)
{
  c33int sn;
  sn = ext_sign6(core, inst.imm6_sign6);
  Rd.w = and(core, Rd.w, sn);
  PC.w += 2;
  CLK += 1;
}

C33_OP(or_rd_sign6)
{
  c33int sn;
  sn = ext_sign6(core, inst.imm6_sign6);
  Rd.w = or(core, Rd.w, sn);
  PC.w += 2;
  CLK += 1;
}

C33_OP(xor_rd_sign6)
{
  c33int sn;
  sn = ext_sign6(core, inst.imm6_sign6);
  Rd.w = xor(core, Rd.w, sn);
  PC.w += 2;
  CLK += 1;
}

C33_OP(not_rd_sign6)
{
  c33int sn;
  sn = ext_sign6(core, inst.imm6_sign6);
  Rd.w = not(core, sn);
  PC.w += 2;
  CLK += 1;
}

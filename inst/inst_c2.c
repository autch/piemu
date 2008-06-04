
#include "inst.h"

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


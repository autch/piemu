
#include "inst.h"

/****************************************************************************
 *  CLASS 4B
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_4B

C33_OP(srl_rd_imm4)
{
  Rd.w = c33_srl(Rd.w, inst.imm4_rs);
  PC.w += 2;
  CLK += 1;
}

C33_OP(sll_rd_imm4)
{
  Rd.w = c33_sll(Rd.w, inst.imm4_rs);
  PC.w += 2;
  CLK += 1;
}

C33_OP(sra_rd_imm4)
{
  Rd.i = c33_sra(Rd.i, inst.imm4_rs);
  PC.w += 2;
  CLK += 1;
}

C33_OP(sla_rd_imm4)
{
  Rd.i = c33_sla(Rd.i, inst.imm4_rs);
  PC.w += 2;
  CLK += 1;
}

C33_OP(rr_rd_imm4)
{
  Rd.w = c33_rr(Rd.w, inst.imm4_rs);
  PC.w += 2;
  CLK += 1;
}

C33_OP(rl_rd_imm4)
{
  Rd.w = c33_rl(Rd.w, inst.imm4_rs);
  PC.w += 2;
  CLK += 1;
}

C33_OP(srl_rd_rs)
{
  Rd.w = c33_srl(Rd.w, R(inst.imm4_rs).w);
  PC.w += 2;
  CLK += 1;
}

C33_OP(sll_rd_rs)
{
  Rd.w = c33_sll(Rd.w, R(inst.imm4_rs).w);
  PC.w += 2;
  CLK += 1;
}

C33_OP(sra_rd_rs)
{
  Rd.i = c33_sra(Rd.i, R(inst.imm4_rs).i);
  PC.w += 2;
  CLK += 1;
}

C33_OP(sla_rd_rs)
{
  Rd.i = c33_sla(Rd.i, R(inst.imm4_rs).i);
  PC.w += 2;
  CLK += 1;
}

C33_OP(rr_rd_rs)
{
  Rd.w = c33_rr(Rd.w, R(inst.imm4_rs).w);
  PC.w += 2;
  CLK += 1;
}

C33_OP(rl_rd_rs)
{
  Rd.w = c33_rl(Rd.w, R(inst.imm4_rs).w);
  PC.w += 2;
  CLK += 1;
}



#include "inst.h"

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


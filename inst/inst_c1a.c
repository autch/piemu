
#include "inst.h"

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


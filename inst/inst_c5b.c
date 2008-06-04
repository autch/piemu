
#include "inst.h"

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


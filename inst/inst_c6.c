
#include "inst.h"

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

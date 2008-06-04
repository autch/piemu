
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

  /* ��ĥ�����̿���¹ԡ��ʤ��δ֤γ����ߤ�ػߤ��뤿��� */
  inst2.s = mem_readH(core, PC);
  core_inst(core, inst2);
  if(EXT1.s) DIE(); /* �μ¤�ext�����񤵤�Ƥ��뤳�� */
}

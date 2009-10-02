
#include "inst.h"

/****************************************************************************
 *  CLASS 4A
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_4A

C33_OP(add_sp_imm10)
{
  SP.w += inst.imm10 * 4;
  PC.w += 2;
  CLK += 1;
}

C33_OP(sub_sp_imm10)
{
  SP.w -= inst.imm10 * 4;
  PC.w += 2;
  CLK += 1;
}


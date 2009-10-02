
#include "inst.h"

/****************************************************************************
 *  CLASS 5B
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_5B

C33_OP(btst_RB_imm3)
{
	PSR.z = !((mem_read(ext_RB(inst.rb), 1) >> inst.imm3) & 1);
	PC.w += 2;
	CLK += 3;
}

C33_OP(bclr_RB_imm3)
{
	c33word addr;
	addr = ext_RB(inst.rb);
	mem_write(addr, 1, mem_read(addr, 1) & ~(1 << inst.imm3));
	PC.w += 2;
	CLK += 3;
}

C33_OP(bset_RB_imm3)
{
	c33word addr;
	addr = ext_RB(inst.rb);
	mem_write(addr, 1, mem_read(addr, 1) | (1 << inst.imm3));
	PC.w += 2;
	CLK += 3;
}

C33_OP(bnot_RB_imm3)
{
	c33word addr;
	addr = ext_RB(inst.rb);
	mem_write(addr, 1, mem_read(addr, 1) ^ (1 << inst.imm3));
	PC.w += 2;
	CLK += 3;
}


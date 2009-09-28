
#include "inst.h"

/****************************************************************************
 *  CLASS 2
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_2

C33_OP(ld_b_rd_SPxIMM6)
{
	Rsd.i = (c33char)core->mem_read(core, ext_SPxIMM6(core, inst.imm6, 1), 1);
	PC.w += 2;
}

C33_OP(ld_ub_rd_SPxIMM6)
{
	Rsd.w = (c33byte)core->mem_read(core, ext_SPxIMM6(core, inst.imm6, 1), 1);
	PC.w += 2;
}

C33_OP(ld_h_rd_SPxIMM6)
{
	Rsd.i = (c33hint)core->mem_read(core, ext_SPxIMM6(core, inst.imm6, 2), 2);
	PC.w += 2;
}

C33_OP(ld_uh_rd_SPxIMM6)
{
	Rsd.w = (c33hword)core->mem_read(core, ext_SPxIMM6(core, inst.imm6, 2), 2);
	PC.w += 2;
}

C33_OP(ld_w_rd_SPxIMM6)
{
	Rsd.i = core->mem_read(core, ext_SPxIMM6(core, inst.imm6, 4), 4);
	PC.w += 2;
}

C33_OP(ld_b_SPxIMM6_rs)
{
	core->mem_write(core, ext_SPxIMM6(core, inst.imm6, 1), 1, Rsd.i);
	PC.w += 2;
}

C33_OP(ld_h_SPxIMM6_rs)
{
	core->mem_write(core, ext_SPxIMM6(core, inst.imm6, 2), 2, Rsd.i);
	PC.w += 2;
}

C33_OP(ld_w_SPxIMM6_rs)
{
	core->mem_write(core, ext_SPxIMM6(core, inst.imm6, 4), 4, Rsd.i);
	PC.w += 2;
}


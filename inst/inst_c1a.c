
#include "inst.h"

/****************************************************************************
 *  CLASS 1A
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_1A

C33_OP(ld_b_rd_RB)
{
	Rsd.i = (c33char)core->mem_read(core, ext_RB(core, inst.rb), 1);
	PC.w += 2;
	CLK += 1;
}

C33_OP(ld_ub_rd_RB)
{
	Rsd.w = (c33byte)core->mem_read(core, ext_RB(core, inst.rb), 1);
	PC.w += 2;
	CLK += 1;
}

C33_OP(ld_h_rd_RB)
{
	Rsd.i = (c33hint)core->mem_read(core, ext_RB(core, inst.rb), 2);
	PC.w += 2;
	CLK += 1;
}

C33_OP(ld_uh_rd_RB)
{
	Rsd.w = (c33hword)core->mem_read(core, ext_RB(core, inst.rb), 2);
	PC.w += 2;
	CLK += 1;
}

C33_OP(ld_w_rd_RB)
{
	Rsd.i = (c33int)core->mem_read(core, ext_RB(core, inst.rb), 4);
	PC.w += 2;
	CLK += 1;
}

C33_OP(ld_b_RB_rs)
{
	core->mem_write(core, ext_RB(core, inst.rb), 1, Rsd.w);
	PC.w += 2;
	CLK += 1;
}

C33_OP(ld_h_RB_rs)
{
	core->mem_write(core, ext_RB(core, inst.rb), 2, Rsd.w);
	PC.w += 2;
	CLK += 1;
}

C33_OP(ld_w_RB_rs)
{
	core->mem_write(core, ext_RB(core, inst.rb), 4, Rsd.w);
	PC.w += 2;
	CLK += 1;
}

C33_OP(ld_b_rd_RBx)
{
	Rsd.i = (c33char)core->mem_read(core, Rb.w, 1);
	Rb.w += 1;
	PC.w += 2;
	CLK += 2;
}

C33_OP(ld_ub_rd_RBx)
{
	Rsd.w = (c33byte)core->mem_read(core, Rb.w, 1);
	Rb.w += 1;
	PC.w += 2;
	CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */
}

C33_OP(ld_h_rd_RBx)
{
	Rsd.i = (c33hint)core->mem_read(core, Rb.w, 2);
	Rb.w += 2;
	PC.w += 2;
	CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */
}

C33_OP(ld_uh_rd_RBx)
{
	Rsd.w = (c33hword)core->mem_read(core, Rb.w, 2);
	Rb.w += 2;
	PC.w += 2;
	CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */
}

C33_OP(ld_w_rd_RBx)
{
	Rsd.i = core->mem_read(core, Rb.w, 4);
	Rb.w += 4;
	PC.w += 2;
	CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */
}

C33_OP(ld_b_RBx_rs)
{
	core->mem_write(core, Rb.w, 1, Rsd.w);
	Rb.w += 1;
	PC.w += 2;
	CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */
}

C33_OP(ld_h_RBx_rs)
{
	core->mem_write(core, Rb.w, 2, Rsd.w);
	Rb.w += 2;
	PC.w += 2;
	CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */
}

C33_OP(ld_w_RBx_rs)
{
	core->mem_write(core, Rb.w, 4, Rsd.w);
	Rb.w += 4;
	PC.w += 2;
	CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */
}


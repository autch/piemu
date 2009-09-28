
#include "inst.h"

/****************************************************************************
 *  CLASS 0A
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_0A

C33_OP(nop)
{
  PC.w += 2;
  CLK += 1;
}

C33_OP(slp)
{
  /* ※TODO: */
  PC.w += 2;
  CLK += 1;
}

C33_OP(halt)
{
  /* ※TODO: */
  PC.w += 2;
  CLK += 1;
}

C33_OP(pushn_rs)
{
  c33int rs;
  for(rs = inst.imm2_rd_rs; rs >= 0; rs--)
  {
    SP.w -= 4;
    core->mem_write(core, SP.w, 4, R(rs).w);
	CLK += 1;
  }
  PC.w += 2;
}

C33_OP(popn_rd)
{
  c33int rd;
  for(rd = 0; rd <= inst.imm2_rd_rs; rd++)
  {
	  R(rd).w = core->mem_read(core, SP.w, 4);
	  SP.w += 4;
	  CLK += 1;
  }
  PC.w += 2;
}

C33_OP(brk)
{
  // do nothing
}

C33_OP(retd)
{
  // do nothing
}

C33_OP(int_imm2)
{
  if(inst.imm2_rd_rs < 0 || inst.imm2_rd_rs > 3) DIE();
  PC.w += 2;
  CLK += 10;
  core_trap(core, TRAP_SOFTINT0 + inst.imm2_rd_rs, 0);
}

C33_OP(reti)
{
	S(0).w = core->mem_read(core, SP.w, 4);
	SP.w += 4;
	PC.w = core->mem_read(core, SP.w, 4);
	SP.w += 4;
	CLK += 5;
}

C33_OP(call_rb)
{
  c33word addr;
  addr = R(inst.imm2_rd_rs).w;
  exec_delay(core, inst.d);
  SP.w -= 4;
  core->mem_write(core, SP.w, 4, PC.w + 2);
  PC.w = addr;
  CLK += !D ? 3 : 2;
}

C33_OP(ret)
{
  c33word addr;
  addr = core->mem_read(core, SP.w, 4);
  SP.w += 4;
  exec_delay(core, inst.d);
  PC.w = addr;
  CLK += !D ? 4 : 3;
}

C33_OP(jp_rb)
{
  c33word addr;
  addr = R(inst.imm2_rd_rs).w;
  exec_delay(core, inst.d);
  PC.w = addr;
  CLK += !D ? 2 : 1;
}


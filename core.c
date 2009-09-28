
#include "core.h"
#include <memory.h>

void core_init(C33CORE* core)
{
  memset(core, 0, sizeof core);
  PC.w = core->mem_read(core, BOOT_ADDRESS, 4);
}

void core_work(C33CORE* core)
{
  C33INST inst;

//  if(core.in_halt)
//  {
//    STD_NOP;
//    CLK += HALT_INT_CLK;
//    return;
//  }

  inst.s = core->mem_read(core, PC.w, 2);
  inst_decode(core, inst);
}

uint32_t core_workex(C33CORE* core, uint32_t mils_org, uint32_t nClocksPerMSec)
{
  C33INST inst;
  uint32_t insts = 0;
  do
  {
	  inst.s = core->mem_read(core, PC.w, 2);
	  inst_decode(core, inst);
	  insts++;
  }while((CLK - mils_org) < nClocksPerMSec); /* 1ミリ秒分の処理 */
  return insts;
}

void core_trap(C33CORE* core, c33word no, c33word level)
{
  c33word addr;

  /* マスク可能な割り込みの場合のみ、割り込み可能条件を検査します。 */
  if(no >= 16) {
    if(!PSR.ie) return;
    if(level <= PSR.il) return;
  }

//  if(context->core.in_halt)
//    context->core.in_halt = 0;

  /* ※要検討:トラップテーブル直読みしてます */
  addr = core->mem_read(core, pTTBR_REG + no * 4, 4);  
  SP.w -= 4;
  core->mem_write(core, SP.w, 4, PC.w); /* 要注意！PC+2じゃないよ！ */
  SP.w -= 4;
  core->mem_write(core, SP.w, 4, S(0).w);
  PC.w = addr;
  PSR.ie = 0;

  /* マスク可能な割り込みの場合のみ、割り込みレベルを変化させます。 */
  if(no >= 16) {
    PSR.il = level;
  }
}

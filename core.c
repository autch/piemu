
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
  }while((CLK - mils_org) < nClocksPerMSec); /* 1�ߥ���ʬ�ν��� */
  return insts;
}

void core_trap(C33CORE* core, c33word no, c33word level)
{
  c33word addr;

  /* �ޥ�����ǽ�ʳ����ߤξ��Τߡ������߲�ǽ���򸡺����ޤ��� */
  if(no >= 16) {
    if(!PSR.ie) return;
    if(level <= PSR.il) return;
  }

//  if(context->core.in_halt)
//    context->core.in_halt = 0;

  /* ���׸�Ƥ:�ȥ�åץơ��֥�ľ�ɤߤ��Ƥޤ� */
  addr = core->mem_read(core, pTTBR_REG + no * 4, 4);  
  SP.w -= 4;
  core->mem_write(core, SP.w, 4, PC.w); /* ����ա�PC+2����ʤ��衪 */
  SP.w -= 4;
  core->mem_write(core, SP.w, 4, S(0).w);
  PC.w = addr;
  PSR.ie = 0;

  /* �ޥ�����ǽ�ʳ����ߤξ��Τߡ������ߥ�٥���Ѳ������ޤ��� */
  if(no >= 16) {
    PSR.il = level;
  }
}

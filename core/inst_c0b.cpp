
#include "inst.h"

/****************************************************************************
 *  CLASS 0B
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_0B

C33_OP(jrgt_sign8)
{
  c33word cc, addr;
  cc = !(PSR.n ^ PSR.v) && !PSR.z;
  addr = ext_PCxSIGN8(inst.sign8);
  exec_delay(inst.d);
  PC.w = cc ? addr : PC.w + 2;
  CLK += (cc && !D) ? 2 : 1;
}

C33_OP(jrge_sign8)
{
  c33word cc, addr;
  cc = !(PSR.n ^ PSR.v);
  addr = ext_PCxSIGN8(inst.sign8);
  exec_delay(inst.d);
  PC.w = cc ? addr : PC.w + 2;
  CLK += (cc && !D) ? 2 : 1;
}

C33_OP(jrlt_sign8)
{
  c33word cc, addr;
  cc = (PSR.n ^ PSR.v);
  addr = ext_PCxSIGN8(inst.sign8);
  exec_delay(inst.d);
  PC.w = cc ? addr : PC.w + 2;
  CLK += (cc && !D) ? 2 : 1;
}

C33_OP(jrle_sign8)
{
  c33word cc, addr;
  cc = (PSR.n ^ PSR.v) || PSR.z;
  addr = ext_PCxSIGN8(inst.sign8);
  exec_delay(inst.d);
  PC.w = cc ? addr : PC.w + 2;
  CLK += (cc && !D) ? 2 : 1;
}

C33_OP(jrugt_sign8)
{
  c33word cc, addr;
  cc = !PSR.c && !PSR.z;
  addr = ext_PCxSIGN8(inst.sign8);
  exec_delay(inst.d);
  PC.w = cc ? addr : PC.w + 2;
  CLK += (cc && !D) ? 2 : 1;
}

C33_OP(jruge_sign8)
{
  c33word cc, addr;
  cc = !PSR.c;
  addr = ext_PCxSIGN8(inst.sign8);
  exec_delay(inst.d);
  PC.w = cc ? addr : PC.w + 2;
  CLK += (cc && !D) ? 2 : 1;
}

C33_OP(jrult_sign8)
{
  c33word cc, addr;
  cc = PSR.c;
  addr = ext_PCxSIGN8(inst.sign8);
  exec_delay(inst.d);
  PC.w = cc ? addr : PC.w + 2;
  CLK += (cc && !D) ? 2 : 1;
}

C33_OP(jrule_sign8)
{
  c33word cc, addr;
  cc = PSR.c || PSR.z;
  addr = ext_PCxSIGN8(inst.sign8);
  exec_delay(inst.d);
  PC.w = cc ? addr : PC.w + 2;
  CLK += (cc && !D) ? 2 : 1;
}

C33_OP(jreq_sign8)
{
  c33word cc, addr;
  cc = PSR.z;
  addr = ext_PCxSIGN8(inst.sign8);
  exec_delay(inst.d);
  PC.w = cc ? addr : PC.w + 2;
  CLK += (cc && !D) ? 2 : 1;
}

C33_OP(jrne_sign8)
{
  c33word cc, addr;
  cc = !PSR.z;
  addr = ext_PCxSIGN8(inst.sign8);
  exec_delay(inst.d);
  PC.w = cc ? addr : PC.w + 2;
  CLK += (cc && !D) ? 2 : 1;
}

C33_OP(call_sign8)
{
  c33word addr;
  addr = ext_PCxSIGN8(inst.sign8);
  exec_delay(inst.d);
  SP.w -= 4;
  mem_write(SP.w, 4, PC.w + 2);
  PC.w = addr;
  CLK += !D ? 3 : 2;
}

C33_OP(jp_sign8)
{
  c33word addr;
  addr = ext_PCxSIGN8(inst.sign8);
  exec_delay(inst.d);
  PC.w = addr;
  CLK += !D ? 2 : 1;
}


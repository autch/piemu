
#include "inst.h"

/****************************************************************************
 *  CLASS 1B
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_1B

C33_OP(add_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.i = add(core, Rd.i, Rs.i);
  }
  else
  {
    un = ext_3op(core);
    Rd.i = add(core, Rs.i, un);
  }
  PC.w += 2;
}

C33_OP(sub_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.i = sub(core, Rd.i, Rs.i);
  }
  else
  {
    un = ext_3op(core);
    Rd.i = sub(core, Rs.i, un);
  }
  PC.w += 2;
}

C33_OP(cmp_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    sub(core, Rd.i, Rs.i);
  }
  else
  {
    un = ext_3op(core);
    sub(core, Rs.i, un);
  }
  PC.w += 2;
}

C33_OP(ld_w_rd_rs)
{
  Rd.i = Rs.i;
  PC.w += 2;
}

C33_OP(and_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.w = and(core, Rd.w, Rs.w);
  }
  else
  {
    un = ext_3op(core);
    Rd.w = and(core, Rs.w, un);
  }
  PC.w += 2;
}

C33_OP(or_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.w = or(core, Rd.w, Rs.w);
  }
  else
  {
    un = ext_3op(core);
    Rd.w = or(core, Rs.w, un);
  }
  PC.w += 2;
}

C33_OP(xor_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.w = xor(core, Rd.w, Rs.w);
  }
  else
  {
    un = ext_3op(core);
    Rd.w = xor(core, Rs.w, un);
  }
  PC.w += 2;
}

C33_OP(not_rd_rs)
{
  Rd.w = not(core, Rs.w);
  PC.w += 2;
}


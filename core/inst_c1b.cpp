
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
    Rd.i = c33_add(Rd.i, Rs.i);
  }
  else
  {
    un = ext_3op();
    Rd.i = c33_add(Rs.i, un);
  }
  PC.w += 2;
  CLK += 1;
}

C33_OP(sub_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.i = c33_sub(Rd.i, Rs.i);
  }
  else
  {
    un = ext_3op();
    Rd.i = c33_sub(Rs.i, un);
  }
  PC.w += 2;
  CLK += 1;
}

C33_OP(cmp_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    c33_sub(Rd.i, Rs.i);
  }
  else
  {
    un = ext_3op();
    c33_sub(Rs.i, un);
  }
  PC.w += 2;
  CLK += 1;
}

C33_OP(ld_w_rd_rs)
{
  Rd.i = Rs.i;
  PC.w += 2;
  CLK += 1;
}

C33_OP(and_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.w = c33_and(Rd.w, Rs.w);
  }
  else
  {
    un = ext_3op();
    Rd.w = c33_and(Rs.w, un);
  }
  PC.w += 2;
  CLK += 1;
}

C33_OP(or_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.w = c33_or(Rd.w, Rs.w);
  }
  else
  {
    un = ext_3op();
    Rd.w = c33_or(Rs.w, un);
  }
  PC.w += 2;
  CLK += 1;
}

C33_OP(xor_rd_rs)
{
  c33word un;
  if(!EXT1.s)
  {
    Rd.w = c33_xor(Rd.w, Rs.w);
  }
  else
  {
    un = ext_3op();
    Rd.w = c33_xor(Rs.w, un);
  }
  PC.w += 2;
  CLK += 1;
}

C33_OP(not_rd_rs)
{
  Rd.w = c33_not(Rs.w);
  PC.w += 2;
  CLK += 1;
}


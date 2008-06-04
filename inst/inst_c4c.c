
#include "inst.h"

/****************************************************************************
 *  CLASS 4C
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_4C

C33_OP(scan0_rd_rs)
{
  c33word ua, ub;
  ua = Rs.w;
  for(ub = 0; ub < 8; ub++)
  {
    if(!(ua & (1 << 31))) break;
    ua <<= 1;
  }
  Rd.w = ub;
  PSR.z = ub == 0;
  PSR.c = ub == 8;
  PC.w += 2;
}

C33_OP(scan1_rd_rs)
{
  c33word ua, ub;
  ua = Rs.w;
  for(ub = 0; ub < 8; ub++)
  {
    if(ua & (1 << 31)) break;
    ua <<= 1;
  }
  Rd.w = ub;
  PSR.z = ub == 0;
  PSR.c = ub == 8;
  PC.w += 2;
}

C33_OP(swap_rd_rs)
{
  c33word ua;
  ua = Rs.w;
  Rd.w = ((ua & 0x000000ff) << 24)
    | ((ua & 0x0000ff00) <<  8)
    | ((ua & 0x00ff0000) >>  8)
    | ((ua & 0xff000000) >> 24);
  PC.w += 2;
}

C33_OP(mirror_rd_rs)
{
  c33word ua;
  ua = Rs.w;
  Rd.w = ((ua & 0x01010101) << 7)
    | ((ua & 0x02020202) << 5)
    | ((ua & 0x04040404) << 3)
    | ((ua & 0x08080808) << 1)
    | ((ua & 0x10101010) >> 1)
    | ((ua & 0x20202020) >> 3)
    | ((ua & 0x40404040) >> 5)
    | ((ua & 0x80808080) >> 7);
  PC.w += 2;
}

C33_OP(div0s_rs)
{
  if(!Rs.i) core_trap(TRAP_ZERODIV, 0);
  AHR.w = (c33int)ALR.w >> 31;
  PSR.ds = ALR.w >> 31;
  PSR.n = Rs.w >> 31;
  PC.w += 2;
}

C33_OP(div0u_rs)
{
  if(!Rs.i) core_trap(TRAP_ZERODIV, 0);
  AHR.w = 0;
  PSR.ds = 0;
  PSR.n = 0;
  PC.w += 2;
}

C33_OP(div1_rs)
{
  c33word tmp;
  /* div0x以外では、ゼロ除算例外は発生しません。 */
  AR <<= 1;
  if(!PSR.ds)
  {
    if(!PSR.n)
    {
      /* 正÷正 */
      tmp = AHR.i - Rs.i;
      if(tmp <= AHR.i)
      { 
        /* !C */
        AHR.i = tmp;
        ALR.i |= 1;
      }
    }
    else
    {
      /* 正÷負 */
      tmp = AHR.i + Rs.i;
      if(tmp < AHR.i)
      {
        /* C */
        AHR.i = tmp;
        ALR.i |= 1;
      }
    }
  }
  else
  {
    if(!PSR.n)
    { 
      /* 負÷正 */
      tmp = AHR.i + Rs.i;
      if(tmp >= AHR.i)
      {
        /* !C */
        AHR.i = tmp;
        ALR.i |= 1;
      }
    }
    else
    {
      /* 負÷負 */
      tmp = AHR.i - Rs.i;
      if(tmp > AHR.i)
      {
        /* !C */
        AHR.i = tmp;
        ALR.i |= 1;
      }
    }
  }
  PC.w += 2;
}

C33_OP(div2s_rs)
{
  c33word tmp;
  /* div0x以外では、ゼロ除算例外は発生しません。 */
  if(PSR.ds)
  {
    if(!PSR.n)
    {
      tmp = AHR.i + Rs.i;
    }
    else
    {
      tmp = AHR.i - Rs.i;
    }
    if(!tmp)
    {
      AHR.i = tmp;
      ALR.i += 1;
    }
  }
  PC.w += 2;
}

C33_OP(div3s)
{
  /* div0x以外では、ゼロ除算例外は発生しません。 */
  if(PSR.ds != PSR.n)
  {
    ALR.i = 0 - ALR.i;  /* ALR = -ALR では警告になるので… */
  }
  PC.w += 2;
}



#include "inst.h"

/****************************************************************************
 *  CLASS 5A
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_5A

C33_OP(ld_w_sd_rs)
{
  /* 実はディレイド可能！EPSONライブラリの除算ルーチンが使ってる*/
  S(inst.sd_rd).w = R(inst.rs_ss).w;
  PC.w += 2;
}

C33_OP(ld_w_rd_ss)
{
  /* 実はディレイド可能！EPSONライブラリの除算ルーチンが使ってる*/
  R(inst.sd_rd).w = S(inst.rs_ss).w;
  PC.w += 2;
}


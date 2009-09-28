
#include "inst.h"

/****************************************************************************
 *  CLASS 5C
 ****************************************************************************/

#undef INST
#define INST INST_CLASS_5C

C33_OP(adc_rd_rs)
{
	Rd.i = adc(core, Rd.i, Rs.i);
	PC.w += 2;
	CLK += 1;
}

C33_OP(sbc_rd_rs)
{
	Rd.i = sbc(core, Rd.i, Rs.i);
	PC.w += 2;
	CLK += 1;
}

C33_OP(ld_b_rd_rs)   /*要注意！ディレイド不可！*/
{
	Rd.i = (c33char)Rs.i;
	PC.w += 2;
	CLK += 1;
}

C33_OP(ld_ub_rd_rs)    /*要注意！ディレイド不可！*/
{
	Rd.w = (c33byte)Rs.w;
	PC.w += 2;
	CLK += 1;
}

C33_OP(ld_h_rd_rs)    /*要注意！ディレイド不可！*/
{
	Rd.i = (c33hint)Rs.i;
	PC.w += 2;
	CLK += 1;
}

C33_OP(ld_uh_rd_rs)    /*要注意！ディレイド不可！*/
{
	Rd.w = (c33hword)Rs.w;
	PC.w += 2;
	CLK += 1;
}

C33_OP(mlt_h_rd_rs)
{
	ALR.i = (c33hint)Rd.i * (c33hint)Rs.i;
	PC.w += 2;
	CLK += 1;
}

C33_OP(mltu_h_rd_rs)
{
	ALR.w = (c33hword)Rd.w * (c33hword)Rs.w;
	PC.w += 2;
	CLK += 1;
}

C33_OP(mlt_w_rd_rs)
{
	AR = (c33lint)Rd.i * (c33lint)Rs.i;
	PC.w += 2;
	CLK += 5;
}

C33_OP(mltu_w_rd_rs)
{
	AR = (c33lword)Rd.w * (c33lword)Rs.w;
	PC.w += 2;
	CLK += 5;
}

//int64 a, b, c;
//while(Rs != 0) {
//  a = AR;
//  b = (short)mem_read(R(inst.rs + 1), 2) * (short)mem_read(R(inst.rs + 2), 2);
//  c = a + b;
//  AR = a;
//  if(!PSR.mo) { /* 1→0には変化しない */
//    PSR.mo = (a < 0  && b <  0 && c >= 0) ||
//             (a >= 0 && b >= 0 && c <  0);
//  }
//  Rs--;
//  R(inst.rs + 1) += 2;
//  R(inst.rs + 2) += 2;
//  CLK += 2;
//}
//PC.w += 2;
//CLK += 4;
//
//↑本当はこうですが、mac命令実行中に割り込みを受け付けるために、次のように変更しました。
//↓実機と違い毎回mac命令をフェッチしてしまうため、実機よりも実行サイクルが増えています。
//

C33_OP(mac_rs)
{
	c33lint a, b, c;
	if(Rs.i)
	{
		a = AR;
		b = core->mem_read(core, R(inst.rs + 1).w, 2)
				* core->mem_read(core, R(inst.rs + 2).w, 2);
		c = a + b;
		AR = a;
		if(!PSR.mo)
		{
			/* 1→0には変化しない */
			PSR.mo = (a < 0  && b <  0 && c >= 0)
					|| (a >= 0 && b >= 0 && c <  0);
		}
		Rs.i--;
		R(inst.rs + 1).i += 2;
		R(inst.rs + 2).i += 2;
		/* PCはそのまま。次回もこのmac命令を実行します。 */
		CLK += 2;
	}
	else
	{
		PC.w += 2;
		CLK += 4;
	}
}


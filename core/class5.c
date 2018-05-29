#include "classes.h"

/****************************************************************************
 *  CLASS 5A
 ****************************************************************************/

void exec_ld_w_sd_rs(PIEMU_CONTEXT *context, CLASS_5A inst)
{
    NO_EXT /*NO_DELAY 実はディレイド可能！EPSONライブラリの除算ルーチンが使ってる*/
    S(inst.sd_rd) = R(inst.rs_ss);
    PC += 2;
    CLK += 1;

}

void exec_ld_w_rd_ss(PIEMU_CONTEXT *context, CLASS_5A inst)
{
    NO_EXT /*NO_DELAY 実はディレイド可能！EPSONライブラリの除算ルーチンが使ってる*/
    R(inst.sd_rd) = S(inst.rs_ss);
    PC += 2;
    CLK += 1;

}

/****************************************************************************
 *  CLASS 5B
 ****************************************************************************/

void exec_btst_RB_imm3(PIEMU_CONTEXT *context, CLASS_5B inst)
{
    NO_DELAY
    PSR.z = !((mem_readB(context, ext_RB(context, inst.rb)) >> inst.imm3) & 1);
    PC += 2;
    CLK += 3;

}

void exec_bclr_RB_imm3(PIEMU_CONTEXT *context, CLASS_5B inst)
{
    NO_DELAY
    c33word addr = ext_RB(context, inst.rb);
    mem_writeB(context, addr, mem_readB(context, addr) & ~(1 << inst.imm3));
    PC += 2;
    CLK += 3;
}

void exec_bset_RB_imm3(PIEMU_CONTEXT *context, CLASS_5B inst)
{
    NO_DELAY
    c33word addr = ext_RB(context, inst.rb);
    mem_writeB(context, addr, mem_readB(context, addr) | (1 << inst.imm3));
    PC += 2;
    CLK += 3;

}

void exec_bnot_RB_imm3(PIEMU_CONTEXT *context, CLASS_5B inst)
{
    NO_DELAY
    c33word addr = ext_RB(context, inst.rb);
    mem_writeB(context, addr, mem_readB(context, addr) ^ (1 << inst.imm3));
    PC += 2;
    CLK += 3;

}

/****************************************************************************
 *  CLASS 5C
 ****************************************************************************/

void exec_adc_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT
    R(inst.rd) = adc(&PSR, R(inst.rd), R(inst.rs));
    PC += 2;
    CLK += 1;
}

void exec_sbc_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT
    R(inst.rd) = sbc(&PSR, R(inst.rd), R(inst.rs));
    PC += 2;
    CLK += 1;
}

void exec_ld_b_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY /*要注意！ディレイド不可！*/
    R(inst.rd) = (c33char) R(inst.rs);
    PC += 2;
    CLK += 1;
}

void exec_ld_ub_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY /*要注意！ディレイド不可！*/
    R(inst.rd) = (c33byte) R(inst.rs);
    PC += 2;
    CLK += 1;
}

void exec_ld_h_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY /*要注意！ディレイド不可！*/
    R(inst.rd) = (c33hint) R(inst.rs);
    PC += 2;
    CLK += 1;
}

void exec_ld_uh_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY /*要注意！ディレイド不可！*/
    R(inst.rd) = (c33hword) R(inst.rs);
    PC += 2;
    CLK += 1;
}

void exec_mlt_h_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT
            ALR = (c33hint) R(inst.rd) * (c33hint) R(inst.rs);
    PC += 2;
    CLK += 1;
}

void exec_mltu_h_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT
            ALR = (c33hword) R(inst.rd) * (c33hword) R(inst.rs);
    PC += 2;
    CLK += 1;
}

void exec_mlt_w_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY
    AR = (int64_t) R(inst.rd) * (int64_t) R(inst.rs);
    PC += 2;
    CLK += 5;
}

void exec_mltu_w_rd_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY
    AR = (uint64_t) R(inst.rd) * (uint64_t) R(inst.rs);
    PC += 2;
    CLK += 5;
}

//int64 a, b, c;
//while(R(inst.rs) != 0) {
//  a = AR;
//  b = (short)mem_read(R(inst.rs + 1), 2) * (short)mem_read(R(inst.rs + 2), 2);
//  c = a + b;
//  AR = a;
//  if(!PSR.mo) { /* 1→0には変化しない */
//    PSR.mo = (a < 0  && b <  0 && c >= 0) ||
//             (a >= 0 && b >= 0 && c <  0);
//  }
//  R(inst.rs)--;
//  R(inst.rs + 1) += 2;
//  R(inst.rs + 2) += 2;
//  CLK += 2;
//}
//PC += 2;
//CLK += 4;
//
//↑本当はこうですが、mac命令実行中に割り込みを受け付けるために、次のように変更しました。
//↓実機と違い毎回mac命令をフェッチしてしまうため、実機よりも実行サイクルが増えています。
//

void exec_mac_rs(PIEMU_CONTEXT *context, CLASS_5C inst)
{
    NO_EXT NO_DELAY
    int64_t a64, b64, c64;

    if (R(inst.rs)) {
        a64 = AR;
        b64 = mem_readH(context, R(inst.rs + 1)) * mem_readH(context, R(inst.rs + 2));
        c64 = a64 + b64;
        AR = a64;
        if (!PSR.mo) { /* 1→0には変化しない */
            PSR.mo = (a64 < 0 && b64 < 0 && c64 >= 0) ||
                     (a64 >= 0 && b64 >= 0 && c64 < 0);
        }

        R(inst.rs)--;
        R(inst.rs + 1) += 2;
        R(inst.rs + 2) += 2;
        /* PCはそのまま。次回もこのmac命令を実行します。 */
        CLK += 2;
    } else {
        PC += 2;
        CLK += 4;
    }
}

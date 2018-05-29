#include "classes.h"

/****************************************************************************
 *  CLASS 1A
 ****************************************************************************/

void exec_ld_b_rd_RB(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (char) mem_readB(context, ext_RB(context, inst.rb));
    PC += 2;
    CLK += 1;

}

void exec_ld_ub_rd_RB(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (unsigned char) mem_readB(context, ext_RB(context, inst.rb));
    PC += 2;
    CLK += 1;

}

void exec_ld_h_rd_RB(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (short) mem_readH(context, ext_RB(context, inst.rb));
    PC += 2;
    CLK += 1;

}

void exec_ld_uh_rd_RB(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (unsigned short) mem_readH(context, ext_RB(context, inst.rb));
    PC += 2;
    CLK += 1;

}

void exec_ld_w_rd_RB(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (int) mem_readW(context, ext_RB(context, inst.rb));
    PC += 2;
    CLK += 1;

}

void exec_ld_b_RB_rs(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    mem_writeB(context, ext_RB(context, inst.rb), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

void exec_ld_h_RB_rs(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    mem_writeH(context, ext_RB(context, inst.rb), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

void exec_ld_w_RB_rs(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_DELAY
    mem_writeW(context, ext_RB(context, inst.rb), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

void exec_ld_b_rd_RBx(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = (char) mem_readB(context, R(inst.rb));
    R(inst.rb) += 1;
    PC += 2;
    CLK += 2;

}

void exec_ld_ub_rd_RBx(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = (unsigned char) mem_readB(context, R(inst.rb));
    R(inst.rb) += 1;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

void exec_ld_h_rd_RBx(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = (short) mem_readH(context, R(inst.rb));
    R(inst.rb) += 2;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

void exec_ld_uh_rd_RBx(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = (unsigned short) mem_readH(context, R(inst.rb));
    R(inst.rb) += 2;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

void exec_ld_w_rd_RBx(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = mem_readW(context, R(inst.rb));
    R(inst.rb) += 4;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

void exec_ld_b_RBx_rs(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    mem_writeB(context, R(inst.rb), R(inst.rs_rd));
    R(inst.rb) += 1;
    PC += 2;
    CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */

}

void exec_ld_h_RBx_rs(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    mem_writeH(context, R(inst.rb), R(inst.rs_rd));
    R(inst.rb) += 2;
    PC += 2;
    CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */

}

void exec_ld_w_RBx_rs(PIEMU_CONTEXT *context, CLASS_1A inst)
{
    NO_EXT NO_DELAY
    mem_writeW(context, R(inst.rb), R(inst.rs_rd));
    R(inst.rb) += 4;
    PC += 2;
    CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */

}

/****************************************************************************
 *  CLASS 1B
 ****************************************************************************/

void exec_add_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = add(&PSR, R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = add(&PSR, R(inst.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

void exec_sub_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = sub(&PSR, R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = sub(&PSR, R(inst.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

// RZ はダミーレジスタ
void exec_cmp_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    if (!EXT1.s) {
        sub(&PSR, R(inst.rd), R(inst.rs));
    } else {
        sub(&PSR, R(inst.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

void exec_ld_w_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    NO_EXT
    R(inst.rd) = R(inst.rs);
    PC += 2;
    CLK += 1;
}

void exec_and_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = and(&PSR, R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = and(&PSR, R(inst.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

void exec_or_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = or(&PSR, R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = or(&PSR, R(inst.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

void exec_xor_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = xor(&PSR, R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = xor(&PSR, R(inst.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

void exec_not_rd_rs(PIEMU_CONTEXT *context, CLASS_1B inst)
{
    NO_EXT
    R(inst.rd) = not(&PSR, R(inst.rs));
    PC += 2;
    CLK += 1;
}

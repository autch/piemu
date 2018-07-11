#include "classes.h"

/****************************************************************************
 *  CLASS 1A
 ****************************************************************************/

void exec_ld_b_rd_RB(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    R(inst.c1a.rs_rd) = (char) mem_readB(context, ext_RB(context, inst.c1a.rb));
    PC += 2;
    CLK += 1;

}

void exec_ld_ub_rd_RB(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    R(inst.c1a.rs_rd) = (unsigned char) mem_readB(context, ext_RB(context, inst.c1a.rb));
    PC += 2;
    CLK += 1;

}

void exec_ld_h_rd_RB(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    R(inst.c1a.rs_rd) = (short) mem_readH(context, ext_RB(context, inst.c1a.rb));
    PC += 2;
    CLK += 1;

}

void exec_ld_uh_rd_RB(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    R(inst.c1a.rs_rd) = (unsigned short) mem_readH(context, ext_RB(context, inst.c1a.rb));
    PC += 2;
    CLK += 1;

}

void exec_ld_w_rd_RB(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    R(inst.c1a.rs_rd) = (int) mem_readW(context, ext_RB(context, inst.c1a.rb));
    PC += 2;
    CLK += 1;

}

void exec_ld_b_RB_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    mem_writeB(context, ext_RB(context, inst.c1a.rb), R(inst.c1a.rs_rd));
    PC += 2;
    CLK += 1;

}

void exec_ld_h_RB_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    mem_writeH(context, ext_RB(context, inst.c1a.rb), R(inst.c1a.rs_rd));
    PC += 2;
    CLK += 1;

}

void exec_ld_w_RB_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    mem_writeW(context, ext_RB(context, inst.c1a.rb), R(inst.c1a.rs_rd));
    PC += 2;
    CLK += 1;

}

void exec_ld_b_rd_RBx(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    R(inst.c1a.rs_rd) = (char) mem_readB(context, R(inst.c1a.rb));
    R(inst.c1a.rb) += 1;
    PC += 2;
    CLK += 2;

}

void exec_ld_ub_rd_RBx(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    R(inst.c1a.rs_rd) = (unsigned char) mem_readB(context, R(inst.c1a.rb));
    R(inst.c1a.rb) += 1;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

void exec_ld_h_rd_RBx(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    R(inst.c1a.rs_rd) = (short) mem_readH(context, R(inst.c1a.rb));
    R(inst.c1a.rb) += 2;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

void exec_ld_uh_rd_RBx(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    R(inst.c1a.rs_rd) = (unsigned short) mem_readH(context, R(inst.c1a.rb));
    R(inst.c1a.rb) += 2;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

void exec_ld_w_rd_RBx(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    R(inst.c1a.rs_rd) = mem_readW(context, R(inst.c1a.rb));
    R(inst.c1a.rb) += 4;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

void exec_ld_b_RBx_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    mem_writeB(context, R(inst.c1a.rb), R(inst.c1a.rs_rd));
    R(inst.c1a.rb) += 1;
    PC += 2;
    CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */

}

void exec_ld_h_RBx_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    mem_writeH(context, R(inst.c1a.rb), R(inst.c1a.rs_rd));
    R(inst.c1a.rb) += 2;
    PC += 2;
    CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */

}

void exec_ld_w_RBx_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    mem_writeW(context, R(inst.c1a.rb), R(inst.c1a.rs_rd));
    R(inst.c1a.rb) += 4;
    PC += 2;
    CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */

}

/****************************************************************************
 *  CLASS 1B
 ****************************************************************************/

void exec_add_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    if (!EXT1.s) {
        R(inst.c1b.rd) = add(&PSR, R(inst.c1b.rd), R(inst.c1b.rs));
    } else {
        R(inst.c1b.rd) = add(&PSR, R(inst.c1b.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

void exec_sub_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    if (!EXT1.s) {
        R(inst.c1b.rd) = sub(&PSR, R(inst.c1b.rd), R(inst.c1b.rs));
    } else {
        R(inst.c1b.rd) = sub(&PSR, R(inst.c1b.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

void exec_cmp_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    if (!EXT1.s) {
        sub(&PSR, R(inst.c1b.rd), R(inst.c1b.rs));
    } else {
        sub(&PSR, R(inst.c1b.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

void exec_ld_w_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c1b.rd) = R(inst.c1b.rs);
    PC += 2;
    CLK += 1;
}

void exec_and_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    if (!EXT1.s) {
        R(inst.c1b.rd) = and(&PSR, R(inst.c1b.rd), R(inst.c1b.rs));
    } else {
        R(inst.c1b.rd) = and(&PSR, R(inst.c1b.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

void exec_or_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    if (!EXT1.s) {
        R(inst.c1b.rd) = or(&PSR, R(inst.c1b.rd), R(inst.c1b.rs));
    } else {
        R(inst.c1b.rd) = or(&PSR, R(inst.c1b.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

void exec_xor_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    if (!EXT1.s) {
        R(inst.c1b.rd) = xor(&PSR, R(inst.c1b.rd), R(inst.c1b.rs));
    } else {
        R(inst.c1b.rd) = xor(&PSR, R(inst.c1b.rs), ext_3op(context));
    }
    PC += 2;
    CLK += 1;
}

void exec_not_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c1b.rd) = not(&PSR, R(inst.c1b.rs));
    PC += 2;
    CLK += 1;
}

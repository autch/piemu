#include "classes.h"

/****************************************************************************
 *  CLASS 2
 ****************************************************************************/

void exec_ld_b_rd_SPxIMM6(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    R(inst.c2.rs_rd) = (c33char) mem_readB(context, ext_SPxIMM6(context, inst.c2.imm6, 1));
    PC += 2;
    CLK += 1;

}

void exec_ld_ub_rd_SPxIMM6(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    R(inst.c2.rs_rd) = (c33byte) mem_readB(context, ext_SPxIMM6(context, inst.c2.imm6, 1));
    PC += 2;
    CLK += 1;

}

void exec_ld_h_rd_SPxIMM6(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    R(inst.c2.rs_rd) = (c33hint) mem_readH(context, ext_SPxIMM6(context, inst.c2.imm6, 2));
    PC += 2;
    CLK += 1;

}

void exec_ld_uh_rd_SPxIMM6(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    R(inst.c2.rs_rd) = (c33hword) mem_readH(context, ext_SPxIMM6(context, inst.c2.imm6, 2));
    PC += 2;
    CLK += 1;

}

void exec_ld_w_rd_SPxIMM6(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    R(inst.c2.rs_rd) = mem_readW(context, ext_SPxIMM6(context, inst.c2.imm6, 4));
    PC += 2;
    CLK += 1;

}

void exec_ld_b_SPxIMM6_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    mem_writeB(context, ext_SPxIMM6(context, inst.c2.imm6, 1), R(inst.c2.rs_rd));
    PC += 2;
    CLK += 1;

}

void exec_ld_h_SPxIMM6_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    mem_writeH(context, ext_SPxIMM6(context, inst.c2.imm6, 2), R(inst.c2.rs_rd));
    PC += 2;
    CLK += 1;

}

void exec_ld_w_SPxIMM6_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    mem_writeW(context, ext_SPxIMM6(context, inst.c2.imm6, 4), R(inst.c2.rs_rd));
    PC += 2;
    CLK += 1;

}

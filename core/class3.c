#include "classes.h"

/****************************************************************************
 *  CLASS 3
 ****************************************************************************/

void exec_add_rd_imm6(PIEMU_CONTEXT *context, INST inst)
{
    c33word un = ext_imm6(context, inst.c3.imm6_sign6);
    R(inst.c3.rd) = add(&PSR, R(inst.c3.rd), un);
    PC += 2;
    CLK += 1;
}

void exec_sub_rd_imm6(PIEMU_CONTEXT *context, INST inst)
{
    c33word un = ext_imm6(context, inst.c3.imm6_sign6);
    R(inst.c3.rd) = sub(&PSR, R(inst.c3.rd), un);
    PC += 2;
    CLK += 1;
}

void exec_cmp_rd_sign6(PIEMU_CONTEXT *context, INST inst)
{
    c33int sn = ext_sign6(context, inst.c3.imm6_sign6);
    sub(&PSR, R(inst.c3.rd), sn); /* 要注意！imm6ではなくsign6です！ */
    PC += 2;
    CLK += 1;
}

void exec_ld_w_rd_sign6(PIEMU_CONTEXT *context, INST inst)
{
    R(inst.c3.rd) = ext_sign6(context, inst.c3.imm6_sign6);
    PC += 2;
    CLK += 1;
}

void exec_and_rd_sign6(PIEMU_CONTEXT *context, INST inst)
{
    c33int sn = ext_sign6(context, inst.c3.imm6_sign6);
    R(inst.c3.rd) = and(&PSR, R(inst.c3.rd), sn);
    PC += 2;
    CLK += 1;
}

void exec_or_rd_sign6(PIEMU_CONTEXT *context, INST inst)
{
    c33int sn = ext_sign6(context, inst.c3.imm6_sign6);
    R(inst.c3.rd) = or(&PSR, R(inst.c3.rd), sn);
    PC += 2;
    CLK += 1;
}

void exec_xor_rd_sign6(PIEMU_CONTEXT *context, INST inst)
{
    c33int sn = ext_sign6(context, inst.c3.imm6_sign6);
    R(inst.c3.rd) = xor(&PSR, R(inst.c3.rd), sn);
    PC += 2;
    CLK += 1;
}

void exec_not_rd_sign6(PIEMU_CONTEXT *context, INST inst)
{
    c33int sn = ext_sign6(context, inst.c3.imm6_sign6);
    R(inst.c3.rd) = not(&PSR, sn);
    PC += 2;
    CLK += 1;
}


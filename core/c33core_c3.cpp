#include "c33core_impl.h"

/****************************************************************************
 *  CLASS 3
 ****************************************************************************/

void C33Core::exec_add_rd_imm6(CLASS_3 inst)
{
    c33word un = ext_imm6(inst.imm6_sign6);
    R(inst.rd) = c33add(R(inst.rd), un);
    PC += 2;
    CLK += 1;
}

void C33Core::exec_sub_rd_imm6(CLASS_3 inst)
{
    c33word un = ext_imm6(inst.imm6_sign6);
    R(inst.rd) = c33sub(R(inst.rd), un);
    PC += 2;
    CLK += 1;
}

void C33Core::exec_cmp_rd_sign6(CLASS_3 inst)
{
    c33int sn = ext_sign6(inst.imm6_sign6);
    c33sub(R(inst.rd), sn); /* 要注意！imm6ではなくsign6です！ */
    PC += 2;
    CLK += 1;
}

void C33Core::exec_ld_w_rd_sign6(CLASS_3 inst)
{
    R(inst.rd) = ext_sign6(inst.imm6_sign6);
    PC += 2;
    CLK += 1;
}

void C33Core::exec_and_rd_sign6(CLASS_3 inst)
{
    c33int sn = ext_sign6(inst.imm6_sign6);
    R(inst.rd) = c33and(R(inst.rd), sn);
    PC += 2;
    CLK += 1;
}

void C33Core::exec_or_rd_sign6(CLASS_3 inst)
{
    c33int sn = ext_sign6(inst.imm6_sign6);
    R(inst.rd) = c33or(R(inst.rd), sn);
    PC += 2;
    CLK += 1;
}

void C33Core::exec_xor_rd_sign6(CLASS_3 inst)
{
    c33int sn = ext_sign6(inst.imm6_sign6);
    R(inst.rd) = c33xor(R(inst.rd), sn);
    PC += 2;
    CLK += 1;
}

void C33Core::exec_not_rd_sign6(CLASS_3 inst)
{
    c33int sn = ext_sign6(inst.imm6_sign6);
    R(inst.rd) = c33not(sn);
    PC += 2;
    CLK += 1;
}


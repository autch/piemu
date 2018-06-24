#include "c33core_impl.h"

/****************************************************************************
 *  CLASS 2
 ****************************************************************************/

void C33Core::exec_ld_b_rd_SPxIMM6(CLASS_2 inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33char) mem_readB(ext_SPxIMM6(inst.imm6, 1));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_ub_rd_SPxIMM6(CLASS_2 inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33byte) mem_readB(ext_SPxIMM6(inst.imm6, 1));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_h_rd_SPxIMM6(CLASS_2 inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33hint) mem_readH(ext_SPxIMM6(inst.imm6, 2));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_uh_rd_SPxIMM6(CLASS_2 inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33hword) mem_readH(ext_SPxIMM6(inst.imm6, 2));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_w_rd_SPxIMM6(CLASS_2 inst)
{
    NO_DELAY
    R(inst.rs_rd) = mem_readW(ext_SPxIMM6(inst.imm6, 4));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_b_SPxIMM6_rs(CLASS_2 inst)
{
    NO_DELAY
    mem_writeB(ext_SPxIMM6(inst.imm6, 1), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_h_SPxIMM6_rs(CLASS_2 inst)
{
    NO_DELAY
    mem_writeH(ext_SPxIMM6(inst.imm6, 2), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_w_SPxIMM6_rs(CLASS_2 inst)
{
    NO_DELAY
    mem_writeW(ext_SPxIMM6(inst.imm6, 4), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

#include "c33core_impl.h"

/****************************************************************************
 *  CLASS 1A
 ****************************************************************************/

void C33Core::exec_ld_b_rd_RB(CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33char) mem_readB(ext_RB(inst.rb));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_ub_rd_RB(CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33byte) mem_readB(ext_RB(inst.rb));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_h_rd_RB(CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33hint) mem_readH(ext_RB(inst.rb));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_uh_rd_RB(CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33hword) mem_readH(ext_RB(inst.rb));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_w_rd_RB(CLASS_1A inst)
{
    NO_DELAY
    R(inst.rs_rd) = (c33int) mem_readW(ext_RB(inst.rb));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_b_RB_rs(CLASS_1A inst)
{
    NO_DELAY
    mem_writeB(ext_RB(inst.rb), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_h_RB_rs(CLASS_1A inst)
{
    NO_DELAY
    mem_writeH(ext_RB(inst.rb), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_w_RB_rs(CLASS_1A inst)
{
    NO_DELAY
    mem_writeW(ext_RB(inst.rb), R(inst.rs_rd));
    PC += 2;
    CLK += 1;

}

void C33Core::exec_ld_b_rd_RBx(CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = (c33char) mem_readB(R(inst.rb));
    R(inst.rb) += 1;
    PC += 2;
    CLK += 2;

}

void C33Core::exec_ld_ub_rd_RBx(CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = (c33byte) mem_readB(R(inst.rb));
    R(inst.rb) += 1;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

void C33Core::exec_ld_h_rd_RBx(CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = (c33hint) mem_readH(R(inst.rb));
    R(inst.rb) += 2;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

void C33Core::exec_ld_uh_rd_RBx(CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = (c33hword) mem_readH(R(inst.rb));
    R(inst.rb) += 2;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

void C33Core::exec_ld_w_rd_RBx(CLASS_1A inst)
{
    NO_EXT NO_DELAY
    R(inst.rs_rd) = mem_readW(R(inst.rb));
    R(inst.rb) += 4;
    PC += 2;
    CLK += 2; /* 読み込み側の[%rb]+は2クロック要する */

}

void C33Core::exec_ld_b_RBx_rs(CLASS_1A inst)
{
    NO_EXT NO_DELAY
    mem_writeB(R(inst.rb), R(inst.rs_rd));
    R(inst.rb) += 1;
    PC += 2;
    CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */

}

void C33Core::exec_ld_h_RBx_rs(CLASS_1A inst)
{
    NO_EXT NO_DELAY
    mem_writeH(R(inst.rb), R(inst.rs_rd));
    R(inst.rb) += 2;
    PC += 2;
    CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */

}

void C33Core::exec_ld_w_RBx_rs(CLASS_1A inst)
{
    NO_EXT NO_DELAY
    mem_writeW(R(inst.rb), R(inst.rs_rd));
    R(inst.rb) += 4;
    PC += 2;
    CLK += 1; /* 書き込み側の[%rb]+は1クロックで済む */

}

/****************************************************************************
 *  CLASS 1B
 ****************************************************************************/

void C33Core::exec_add_rd_rs(CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = c33add(R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = c33add(R(inst.rs), ext_3op());
    }
    PC += 2;
    CLK += 1;
}

void C33Core::exec_sub_rd_rs(CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = c33sub(R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = c33sub(R(inst.rs), ext_3op());
    }
    PC += 2;
    CLK += 1;
}

// RZ はダミーレジスタ
void C33Core::exec_cmp_rd_rs(CLASS_1B inst)
{
    if (!EXT1.s) {
        c33sub(R(inst.rd), R(inst.rs));
    } else {
        c33sub(R(inst.rs), ext_3op());
    }
    PC += 2;
    CLK += 1;
}

void C33Core::exec_ld_w_rd_rs(CLASS_1B inst)
{
    NO_EXT
    R(inst.rd) = R(inst.rs);
    PC += 2;
    CLK += 1;
}

void C33Core::exec_and_rd_rs(CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = c33and(R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = c33and(R(inst.rs), ext_3op());
    }
    PC += 2;
    CLK += 1;
}

void C33Core::exec_or_rd_rs(CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = c33or(R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = c33or(R(inst.rs), ext_3op());
    }
    PC += 2;
    CLK += 1;
}

void C33Core::exec_xor_rd_rs(CLASS_1B inst)
{
    if (!EXT1.s) {
        R(inst.rd) = c33xor(R(inst.rd), R(inst.rs));
    } else {
        R(inst.rd) = c33xor(R(inst.rs), ext_3op());
    }
    PC += 2;
    CLK += 1;
}

void C33Core::exec_not_rd_rs(CLASS_1B inst)
{
    NO_EXT
    R(inst.rd) = c33not(R(inst.rs));
    PC += 2;
    CLK += 1;
}

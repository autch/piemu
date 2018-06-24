#include <stdexcept>
#include "c33209e.h"
#include "c33core_impl.h"

/****************************************************************************
 *  CLASS 0A
 ****************************************************************************/

void C33Core::exec_nop(CLASS_0A inst)
{
    NO_EXT NO_DELAY
    PC += 2;
    CLK += 1;
}

// 呼ばれてないらしい？
void C33Core::exec_slp(CLASS_0A inst)
{
    NO_EXT NO_DELAY

    /* ※TODO: */
    PC += 2;
    CLK += 1;

    this->in_halt = 1;
}

void C33Core::exec_halt(CLASS_0A inst)
{
    NO_EXT NO_DELAY

    /* ※TODO: */
    PC += 2;
    CLK += 1;

    this->in_halt = 1;
}

void C33Core::exec_pushn_rs(CLASS_0A inst)
{
    NO_EXT NO_DELAY
    int rs;
    for (rs = inst.imm2_rd_rs; rs >= 0; rs--) {
        SP -= 4;
        mem_writeW(SP, R(rs));
        CLK += 1;

    }
    PC += 2;

}

void C33Core::exec_popn_rd(CLASS_0A inst)
{
    NO_EXT NO_DELAY
    int rd;
    for (rd = 0; rd <= inst.imm2_rd_rs; rd++) {
        R(rd) = mem_readW(SP);
        SP += 4;
        CLK += 1;
    }
    PC += 2;

}

void C33Core::exec_brk(CLASS_0A inst)
{
    throw std::logic_error("BRK: not implemented");
}

void C33Core::exec_retd(CLASS_0A inst)
{
    throw std::logic_error("RETD: not implemented");
}

void C33Core::exec_int_imm2(CLASS_0A inst)
{
    NO_EXT NO_DELAY
    if (inst.imm2_rd_rs < 0 || inst.imm2_rd_rs > 3) throw std::range_error("%%rs out of range: %x");
    PC += 2;
    CLK += 10;
    core_trap_from_core(TRAP_SOFTINT0 + inst.imm2_rd_rs, 0);
}

void C33Core::exec_reti(CLASS_0A inst)
{
    NO_EXT NO_DELAY
    S(0) = mem_readW(SP);
    SP += 4;
    PC = mem_readW(SP);
    SP += 4;
    CLK += 5;
}

void C33Core::exec_call_rb(CLASS_0A inst)
{
    NO_EXT NO_DELAY
    c33word addr = R(inst.imm2_rd_rs);
    exec_delay(inst.d);
    SP -= 4;
    mem_writeW(SP, PC + 2);
    PC = addr;
    CLK += !inst.d ? 3 : 2;
}

void C33Core::exec_ret(CLASS_0A inst)
{
    NO_EXT NO_DELAY
    c33word addr = mem_readW(SP);
    SP += 4;
    exec_delay(inst.d);
    PC = addr;
    CLK += !inst.d ? 4 : 3;

}

void C33Core::exec_jp_rb(CLASS_0A inst)
{
    NO_EXT NO_DELAY
    c33word addr = R(inst.imm2_rd_rs);
    exec_delay(inst.d);
    PC = addr;
    CLK += !inst.d ? 2 : 1;
}

/****************************************************************************
 *  CLASS 0B
 ****************************************************************************/

void C33Core::exec_jrgt_sign8(CLASS_0B inst)
{
    NO_DELAY
    int cc = !(PSR.n ^ PSR.v) && !PSR.z;
    c33word addr = ext_PCxSIGN8(inst.sign8);
    exec_delay(inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void C33Core::exec_jrge_sign8(CLASS_0B inst)
{
    NO_DELAY
    int cc = !(PSR.n ^ PSR.v);
    c33word addr = ext_PCxSIGN8(inst.sign8);
    exec_delay(inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void C33Core::exec_jrlt_sign8(CLASS_0B inst)
{
    NO_DELAY
    int cc = (PSR.n ^ PSR.v);
    c33word addr = ext_PCxSIGN8(inst.sign8);
    exec_delay(inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void C33Core::exec_jrle_sign8(CLASS_0B inst)
{
    NO_DELAY
    int cc = (PSR.n ^ PSR.v) || PSR.z;
    c33word addr = ext_PCxSIGN8(inst.sign8);
    exec_delay(inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void C33Core::exec_jrugt_sign8(CLASS_0B inst)
{
    NO_DELAY
    int cc = !PSR.c && !PSR.z;
    c33word addr = ext_PCxSIGN8(inst.sign8);
    exec_delay(inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void C33Core::exec_jruge_sign8(CLASS_0B inst)
{
    NO_DELAY
    int cc = !PSR.c;
    c33word addr = ext_PCxSIGN8(inst.sign8);
    exec_delay(inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void C33Core::exec_jrult_sign8(CLASS_0B inst)
{
    NO_DELAY
    int cc = PSR.c;
    c33word addr = ext_PCxSIGN8(inst.sign8);
    exec_delay(inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void C33Core::exec_jrule_sign8(CLASS_0B inst)
{
    NO_DELAY
    int cc = PSR.c || PSR.z;
    c33word addr = ext_PCxSIGN8(inst.sign8);
    exec_delay(inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void C33Core::exec_jreq_sign8(CLASS_0B inst)
{
    NO_DELAY
    int cc = PSR.z;
    c33word addr = ext_PCxSIGN8(inst.sign8);
    exec_delay(inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void C33Core::exec_jrne_sign8(CLASS_0B inst)
{
    NO_DELAY
    int cc = !PSR.z;
    c33word addr = ext_PCxSIGN8(inst.sign8);
    exec_delay(inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void C33Core::exec_call_sign8(CLASS_0B inst)
{
    NO_DELAY
    c33word addr = ext_PCxSIGN8(inst.sign8);
    exec_delay(inst.d);
    SP -= 4;
    mem_writeW(SP, PC + 2);
    PC = addr;
    CLK += !inst.d ? 3 : 2;

}

void C33Core::exec_jp_sign8(CLASS_0B inst)
{
    NO_DELAY
    c33word addr = ext_PCxSIGN8(inst.sign8);
    exec_delay(inst.d);
    PC = addr;
    CLK += !inst.d ? 2 : 1;

}

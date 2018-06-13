#include "classes.h"

/****************************************************************************
 *  CLASS 0A
 ****************************************************************************/

void exec_nop(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    PC += 2;
    CLK += 1;
}

// 呼ばれてないらしい？
void exec_slp(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY

    /* ※TODO: */
    PC += 2;
    CLK += 1;

    context->core.in_halt = 1;
}

void exec_halt(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY

    /* ※TODO: */
    PC += 2;
    CLK += 1;

    context->core.in_halt = 1;
}

void exec_pushn_rs(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    int rs;
    for (rs = inst.imm2_rd_rs; rs >= 0; rs--) {
        SP -= 4;
        mem_writeW(context, SP, R(rs));
        CLK += 1;

    }
    PC += 2;

}

void exec_popn_rd(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    int rd;
    for (rd = 0; rd <= inst.imm2_rd_rs; rd++) {
        R(rd) = mem_readW(context, SP);
        SP += 4;
        CLK += 1;
    }
    PC += 2;

}

void exec_brk(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    DIE("BRK: not implemented");
}

void exec_retd(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    DIE("RETD: not implemented");
}

void exec_int_imm2(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    if (inst.imm2_rd_rs < 0 || inst.imm2_rd_rs > 3) DIE("%%rs out of range: %x", inst.imm2_rd_rs);
    PC += 2;
    CLK += 10;
    core_trap_from_core(context, TRAP_SOFTINT0 + inst.imm2_rd_rs, 0);
}

void exec_reti(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    S(0) = mem_readW(context, SP);
    SP += 4;
    PC = mem_readW(context, SP);
    SP += 4;
    CLK += 5;
}

void exec_call_rb(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    c33word addr = R(inst.imm2_rd_rs);
    exec_delay(context, inst.d);
    SP -= 4;
    mem_writeW(context, SP, PC + 2);
    PC = addr;
    CLK += !inst.d ? 3 : 2;
}

void exec_ret(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    c33word addr = mem_readW(context, SP);
    SP += 4;
    exec_delay(context, inst.d);
    PC = addr;
    CLK += !inst.d ? 4 : 3;

}

void exec_jp_rb(PIEMU_CONTEXT *context, CLASS_0A inst)
{
    NO_EXT NO_DELAY
    c33word addr = R(inst.imm2_rd_rs);
    exec_delay(context, inst.d);
    PC = addr;
    CLK += !inst.d ? 2 : 1;
}

/****************************************************************************
 *  CLASS 0B
 ****************************************************************************/

void exec_jrgt_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = !(PSR.n ^ PSR.v) && !PSR.z;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void exec_jrge_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = !(PSR.n ^ PSR.v);
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void exec_jrlt_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = (PSR.n ^ PSR.v);
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void exec_jrle_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = (PSR.n ^ PSR.v) || PSR.z;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void exec_jrugt_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = !PSR.c && !PSR.z;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void exec_jruge_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = !PSR.c;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void exec_jrult_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = PSR.c;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void exec_jrule_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = PSR.c || PSR.z;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void exec_jreq_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = PSR.z;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void exec_jrne_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    int cc = !PSR.z;
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = cc ? addr : PC + 2;
    CLK += cc && !inst.d ? 2 : 1;

}

void exec_call_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    SP -= 4;
    mem_writeW(context, SP, PC + 2);
    PC = addr;
    CLK += !inst.d ? 3 : 2;

}

void exec_jp_sign8(PIEMU_CONTEXT *context, CLASS_0B inst)
{
    NO_DELAY
    c33word addr = ext_PCxSIGN8(context, inst.sign8);
    exec_delay(context, inst.d);
    PC = addr;
    CLK += !inst.d ? 2 : 1;

}

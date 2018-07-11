#include "classes.h"

/****************************************************************************
 *  CLASS 4A
 ****************************************************************************/

void exec_add_sp_imm10(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    SP += inst.c4a.imm10 * 4;
    PC += 2;
    CLK += 1;

}

void exec_sub_sp_imm10(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    SP -= inst.c4a.imm10 * 4;
    PC += 2;
    CLK += 1;

}

/****************************************************************************
 *  CLASS 4B
 ****************************************************************************/

void exec_srl_rd_imm4(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c4b.rd) = srl(&PSR, R(inst.c4b.rd), inst.c4b.imm4_rs);
    PC += 2;
    CLK += 1;
}

void exec_sll_rd_imm4(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c4b.rd) = sll(&PSR, R(inst.c4b.rd), inst.c4b.imm4_rs);
    PC += 2;
    CLK += 1;
}

void exec_sra_rd_imm4(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c4b.rd) = sra(&PSR, R(inst.c4b.rd), inst.c4b.imm4_rs);
    PC += 2;
    CLK += 1;
}

void exec_sla_rd_imm4(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c4b.rd) = sla(&PSR, R(inst.c4b.rd), inst.c4b.imm4_rs);
    PC += 2;
    CLK += 1;
}

void exec_rr_rd_imm4(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c4b.rd) = rr(&PSR, R(inst.c4b.rd), inst.c4b.imm4_rs);
    PC += 2;
    CLK += 1;
}

void exec_rl_rd_imm4(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c4b.rd) = rl(&PSR, R(inst.c4b.rd), inst.c4b.imm4_rs);
    PC += 2;
    CLK += 1;
}

void exec_srl_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c4b.rd) = srl(&PSR, R(inst.c4b.rd), R(inst.c4b.imm4_rs));
    PC += 2;
    CLK += 1;
}

void exec_sll_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c4b.rd) = sll(&PSR, R(inst.c4b.rd), R(inst.c4b.imm4_rs));
    PC += 2;
    CLK += 1;
}

void exec_sra_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c4b.rd) = sra(&PSR, R(inst.c4b.rd), R(inst.c4b.imm4_rs));
    PC += 2;
    CLK += 1;
}

void exec_sla_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c4b.rd) = sla(&PSR, R(inst.c4b.rd), R(inst.c4b.imm4_rs));
    PC += 2;
    CLK += 1;
}

void exec_rr_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c4b.rd) = rr(&PSR, R(inst.c4b.rd), R(inst.c4b.imm4_rs));
    PC += 2;
    CLK += 1;
}

void exec_rl_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    R(inst.c4b.rd) = rl(&PSR, R(inst.c4b.rd), R(inst.c4b.imm4_rs));
    PC += 2;
    CLK += 1;
}

/****************************************************************************
 *  CLASS 4C
 ****************************************************************************/

void exec_scan0_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    c33word ua = R(inst.c4c.rs);
    c33word ub;
    for (ub = 0; ub < 8; ub++) {
        if (!(ua & (c33word) (1 << 31))) break;
        ua <<= 1;
    }
    R(inst.c4c.rd) = ub;
    PSR.z = ub == 0;
    PSR.c = ub == 8;
    PC += 2;
    CLK += 1;
}

void exec_scan1_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    c33word ua = R(inst.c4c.rs);
    c33word ub;
    for (ub = 0; ub < 8; ub++) {
        if (ua & (c33word) (1 << 31)) break;
        ua <<= 1;
    }
    R(inst.c4c.rd) = ub;
    PSR.z = ub == 0;
    PSR.c = ub == 8;
    PC += 2;
    CLK += 1;
}

void exec_swap_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    c33word ua = R(inst.c4c.rs);
    R(inst.c4c.rd) = ((ua & 0x000000ffu) << 24) |
                 ((ua & 0x0000ff00u) << 8) |
                 ((ua & 0x00ff0000u) >> 8) |
                 ((ua & 0xff000000u) >> 24);
    PC += 2;
    CLK += 1;
}

void exec_mirror_rd_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT;
    c33word ua = R(inst.c4c.rs);
    R(inst.c4c.rd) = ((ua & 0x01010101u) << 7) |
                 ((ua & 0x02020202u) << 5) |
                 ((ua & 0x04040404u) << 3) |
                 ((ua & 0x08080808u) << 1) |
                 ((ua & 0x10101010u) >> 1) |
                 ((ua & 0x20202020u) >> 3) |
                 ((ua & 0x40404040u) >> 5) |
                 ((ua & 0x80808080u) >> 7);
    PC += 2;
    CLK += 1;
}

void exec_div0s_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    if (!R(inst.c4c.rs)) {
        core_trap_from_core(context, TRAP_ZERODIV, 0);
        return;
    }
    AHR = (int) ALR >> 31;
    PSR.ds = ALR >> 31;
    PSR.n = R(inst.c4c.rs) >> 31;
    PC += 2;
    CLK += 1;
}

void exec_div0u_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    if (!R(inst.c4c.rs)) {
        core_trap_from_core(context, TRAP_ZERODIV, 0);
        return;
    }
    AHR = 0;
    PSR.ds = 0;
    PSR.n = 0;
    PC += 2;
    CLK += 1;
}

void exec_div1_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    c33int tmp;

    /* div0x以外では、ゼロ除算例外は発生しません。 */
    AR <<= 1;
    if (!PSR.ds) {
        if (!PSR.n) { /* 正÷正 */
            tmp = AHR - R(inst.c4c.rs);
            if (tmp <= AHR) { /* !C */
                AHR = tmp;
                ALR |= 1;
            }
        } else { /* 正÷負 */
            tmp = AHR + R(inst.c4c.rs);
            if (tmp < AHR) { /* C */
                AHR = tmp;
                ALR |= 1;
            }
        }
    } else {
        if (!PSR.n) { /* 負÷正 */
            tmp = AHR + R(inst.c4c.rs);
            if (tmp >= AHR) { /* !C */
                AHR = tmp;
                ALR |= 1;
            }
        } else { /* 負÷負 */
            tmp = AHR - R(inst.c4c.rs);
            if (tmp > AHR) { /* !C */
                AHR = tmp;
                ALR |= 1;
            }
        }
    }
    PC += 2;
    CLK += 1;
}

void exec_div2s_rs(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    c33int tmp;

    /* div0x以外では、ゼロ除算例外は発生しません。 */
    if (PSR.ds) {
        if (!PSR.n) {
            tmp = AHR + R(inst.c4c.rs);
        } else {
            tmp = AHR - R(inst.c4c.rs);
        }
        if (!tmp) {
            AHR = tmp;
            ALR += 1;
        }
    }
    PC += 2;
    CLK += 1;
}

void exec_div3s(PIEMU_CONTEXT *context, INST inst)
{
    NO_EXT; NO_DELAY;
    /* div0x以外では、ゼロ除算例外は発生しません。 */
    if (PSR.ds != PSR.n) {
        ALR = 0 - ALR;  /* ALR = -ALR では警告になるので… */
    }
    PC += 2;
    CLK += 1;
}

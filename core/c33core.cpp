/*
 *  core.c
 *
 *  P/EMU - P/ECE Emulator
 *  Copyright (C) 2003 Naoyuki Sawa
 *
 *  * Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *  - 作成開始。
 *  * Sat Apr 19 12:30:00 JST 2003 Naoyuki Sawa
 *  - scan0/scan1のバグ修正。（走査方向が逆でした）
 *
 *  * Aug 23 ... 30, 2005.
 *  * こんなもん全部マクロじゃー
 *
 *  * May 29, 2018
 *  * マクロとかやめやめ
 *
 *  * Jun 13, 2018
 *  * C++ 化
 */

#include <stdexcept>
#include "c33core.h"
#include "c33core_impl.h"
#include "c33209e.h"

/****************************************************************************
 *  グローバル関数
 ****************************************************************************/

void C33Core::core_init()
{
    PC = mem_readW(0x0c00000);
}

void C33Core::core_work()
{
    INST inst;

    inst.s = mem_readH(PC);
    core_inst(inst);
}

void C33Core::core_handle_hlt()
{
    if(in_halt == 0)
        return;

    SDL_LockMutex(mut_halt);
    {
        while(in_halt != 0) {
            SDL_CondWait(cond_halt, mut_halt);
        }
    }
    SDL_UnlockMutex(mut_halt);
}

unsigned C33Core::core_workex(unsigned mils_org, unsigned nClocksDivBy1k)
{
    unsigned insts = 0;
    do {
        SDL_LockMutex(mut_core);
        core_work();
        SDL_UnlockMutex(mut_core);
        insts++;
        if(in_halt)
            break;
    } while ((CLK - mils_org) < nClocksDivBy1k); /* 1ミリ秒分の処理 */
    return insts;
}

void C33Core::core_trap_from_core(int no, int level)
{
    c33word addr;

    /* マスク可能な割り込みの場合のみ、割り込み可能条件を検査します。 */
    if (no >= 16) {
        if (!PSR.ie) return;
        if ((unsigned) level <= PSR.il) return;
    }

    // FIXME: 0x48134 を記号定数にする
    addr = mem_readW(0x48134 + no * 4);  /* ※要検討:トラップテーブル直読みしてます */
//    addr = mem_readW(pTTBR_REG + no * 4);  /* ※要検討:トラップテーブル直読みしてます */
    SP -= 4;
    mem_writeW(SP, PC); /* 要注意！PC+2じゃないよ！ */
    SP -= 4;
    mem_writeW(SP, S(0));
    PC = addr;
    PSR.ie = 0;

    /* マスク可能な割り込みの場合のみ、割り込みレベルを変化させます。 */
    if (no >= 16) {
        PSR.il = level;
    }
}

void C33Core::core_trap_from_devices(int no, int level)
{
    SDL_LockMutex(mut_core);
    core_trap_from_core(no, level);
    SDL_UnlockMutex(mut_core);

    SDL_LockMutex(mut_halt);
    {
        in_halt = 0;
        SDL_CondBroadcast(cond_halt);
    }
    SDL_UnlockMutex(mut_halt);
}

#if 1
#define MASK(op, shr_bits, and_bits) ((inst.s >> (16 - (shr_bits + and_bits))) & ((1 << and_bits) - 1))
#else
#define MASK(op, shr, and) (op)
#endif

void C33Core::core_inst(INST inst)
{
    switch (MASK(inst.c0a.cls, 0, 3)) {
        case 0:
            switch (MASK(inst.c0a.op1, 3, 4)) {
                /* CLASS 0A */
                case 0:
                    switch (MASK(inst.c0a.op2, 8, 2)) {
                        case 0:
                            exec_nop(inst.c0a);
                            return;
                        case 1:
                            exec_slp(inst.c0a);
                            return;
                        case 2:
                            exec_halt(inst.c0a);
                            return;
                        case 3:
                            return;
                    }
                    break;
                case 1:
                    switch (MASK(inst.c0a.op2, 8, 2)) {
                        case 0:
                            exec_pushn_rs(inst.c0a);
                            return;
                        case 1:
                            exec_popn_rd(inst.c0a);
                            return;
                        case 2:
                            return;
                        case 3:
                            return;
                    }
                    break;
                case 2:
                    switch (MASK(inst.c0a.op2, 8, 2)) {
                        case 0:
                            exec_brk(inst.c0a);
                            return;
                        case 1:
                            exec_retd(inst.c0a);
                            return;
                        case 2:
                            exec_int_imm2(inst.c0a);
                            return;
                        case 3:
                            exec_reti(inst.c0a);
                            return;
                    }
                    break;
                case 3:
                    switch (MASK(inst.c0a.op2, 8, 2)) {
                        case 0:
                            exec_call_rb(inst.c0a);
                            return;
                        case 1:
                            exec_ret(inst.c0a);
                            return;
                        case 2:
                            exec_jp_rb(inst.c0a);
                            return;
                        case 3:
                            return;
                    }
                    break;
                    /* CLASS 0B */
                case 4:
                    exec_jrgt_sign8(inst.c0b);
                    return;
                case 5:
                    exec_jrge_sign8(inst.c0b);
                    return;
                case 6:
                    exec_jrlt_sign8(inst.c0b);
                    return;
                case 7:
                    exec_jrle_sign8(inst.c0b);
                    return;
                case 8:
                    exec_jrugt_sign8(inst.c0b);
                    return;
                case 9:
                    exec_jruge_sign8(inst.c0b);
                    return;
                case 10:
                    exec_jrult_sign8(inst.c0b);
                    return;
                case 11:
                    exec_jrule_sign8(inst.c0b);
                    return;
                case 12:
                    exec_jreq_sign8(inst.c0b);
                    return;
                case 13:
                    exec_jrne_sign8(inst.c0b);
                    return;
                case 14:
                    exec_call_sign8(inst.c0b);
                    return;
                case 15:
                    exec_jp_sign8(inst.c0b);
                    return;
            }
            break;
        case 1:
            switch (MASK(inst.c1a.op2, 6, 2)) {
                /* CLASS 1A */
                case 0:
                    switch (MASK(inst.c1a.op1, 3, 3)) {
                        case 0:
                            exec_ld_b_rd_RB(inst.c1a);
                            return;
                        case 1:
                            exec_ld_ub_rd_RB(inst.c1a);
                            return;
                        case 2:
                            exec_ld_h_rd_RB(inst.c1a);
                            return;
                        case 3:
                            exec_ld_uh_rd_RB(inst.c1a);
                            return;
                        case 4:
                            exec_ld_w_rd_RB(inst.c1a);
                            return;
                        case 5:
                            exec_ld_b_RB_rs(inst.c1a);
                            return;
                        case 6:
                            exec_ld_h_RB_rs(inst.c1a);
                            return;
                        case 7:
                            exec_ld_w_RB_rs(inst.c1a);
                            return;
                    }
                    break;
                case 1:
                    switch (MASK(inst.c1a.op1, 3, 3)) {
                        case 0:
                            exec_ld_b_rd_RBx(inst.c1a);
                            return;
                        case 1:
                            exec_ld_ub_rd_RBx(inst.c1a);
                            return;
                        case 2:
                            exec_ld_h_rd_RBx(inst.c1a);
                            return;
                        case 3:
                            exec_ld_uh_rd_RBx(inst.c1a);
                            return;
                        case 4:
                            exec_ld_w_rd_RBx(inst.c1a);
                            return;
                        case 5:
                            exec_ld_b_RBx_rs(inst.c1a);
                            return;
                        case 6:
                            exec_ld_h_RBx_rs(inst.c1a);
                            return;
                        case 7:
                            exec_ld_w_RBx_rs(inst.c1a);
                            return;
                    }
                    break;
                    /* CLASS 1B */
                case 2:
                    switch (MASK(inst.c1a.op1, 3, 3)) {
                        case 0:
                            exec_add_rd_rs(inst.c1b);
                            return;
                        case 1:
                            exec_sub_rd_rs(inst.c1b);
                            return;
                        case 2:
                            exec_cmp_rd_rs(inst.c1b);
                            return;
                        case 3:
                            exec_ld_w_rd_rs(inst.c1b);
                            return;
                        case 4:
                            exec_and_rd_rs(inst.c1b);
                            return;
                        case 5:
                            exec_or_rd_rs(inst.c1b);
                            return;
                        case 6:
                            exec_xor_rd_rs(inst.c1b);
                            return;
                        case 7:
                            exec_not_rd_rs(inst.c1b);
                            return;
                    }
                    break;
            }
            break;
        case 2:
            switch (MASK(inst.c2.op1, 3, 3)) {
                /* CLASS 2 */
                case 0:
                    exec_ld_b_rd_SPxIMM6(inst.c2);
                    return;
                case 1:
                    exec_ld_ub_rd_SPxIMM6(inst.c2);
                    return;
                case 2:
                    exec_ld_h_rd_SPxIMM6(inst.c2);
                    return;
                case 3:
                    exec_ld_uh_rd_SPxIMM6(inst.c2);
                    return;
                case 4:
                    exec_ld_w_rd_SPxIMM6(inst.c2);
                    return;
                case 5:
                    exec_ld_b_SPxIMM6_rs(inst.c2);
                    return;
                case 6:
                    exec_ld_h_SPxIMM6_rs(inst.c2);
                    return;
                case 7:
                    exec_ld_w_SPxIMM6_rs(inst.c2);
                    return;
            }
            break;
        case 3:
            switch (MASK(inst.c3.op1, 3, 3)) {
                /* CLASS 3 */
                case 0:
                    exec_add_rd_imm6(inst.c3);
                    return;
                case 1:
                    exec_sub_rd_imm6(inst.c3);
                    return;
                case 2:
                    exec_cmp_rd_sign6(inst.c3);
                    return;
                case 3:
                    exec_ld_w_rd_sign6(inst.c3);
                    return;
                case 4:
                    exec_and_rd_sign6(inst.c3);
                    return;
                case 5:
                    exec_or_rd_sign6(inst.c3);
                    return;
                case 6:
                    exec_xor_rd_sign6(inst.c3);
                    return;
                case 7:
                    exec_not_rd_sign6(inst.c3);
                    return;
            }
            break;
        case 4:
            switch (MASK(inst.c4a.op1, 3, 3)) {
                /* CLASS 4A */
                case 0:
                    exec_add_sp_imm10(inst.c4a);
                    return;
                case 1:
                    exec_sub_sp_imm10(inst.c4a);
                    return;
                default:
                    switch (MASK(inst.c4b.op2, 6, 2)) {
                        /* CLASS 4B */
                        case 0:
                            switch (MASK(inst.c4b.op1, 3, 3)) {
                                case 0:
                                    return;
                                case 1:
                                    return;
                                case 2:
                                    exec_srl_rd_imm4(inst.c4b);
                                    return;
                                case 3:
                                    exec_sll_rd_imm4(inst.c4b);
                                    return;
                                case 4:
                                    exec_sra_rd_imm4(inst.c4b);
                                    return;
                                case 5:
                                    exec_sla_rd_imm4(inst.c4b);
                                    return;
                                case 6:
                                    exec_rr_rd_imm4(inst.c4b);
                                    return;
                                case 7:
                                    exec_rl_rd_imm4(inst.c4b);
                                    return;
                            }
                            break;
                        case 1:
                            switch (MASK(inst.c4b.op1, 3, 3)) {
                                case 0:
                                    return;
                                case 1:
                                    return;
                                case 2:
                                    exec_srl_rd_rs(inst.c4b);
                                    return;
                                case 3:
                                    exec_sll_rd_rs(inst.c4b);
                                    return;
                                case 4:
                                    exec_sra_rd_rs(inst.c4b);
                                    return;
                                case 5:
                                    exec_sla_rd_rs(inst.c4b);
                                    return;
                                case 6:
                                    exec_rr_rd_rs(inst.c4b);
                                    return;
                                case 7:
                                    exec_rl_rd_rs(inst.c4b);
                                    return;
                            }
                            break;
                            /* CLASS 4C */
                        case 2:
                            switch (MASK(inst.c4c.op1, 3, 3)) {
                                case 0:
                                    return;
                                case 1:
                                    return;
                                case 2:
                                    exec_scan0_rd_rs(inst.c4c);
                                    return;
                                case 3:
                                    exec_scan1_rd_rs(inst.c4c);
                                    return;
                                case 4:
                                    exec_swap_rd_rs(inst.c4c);
                                    return;
                                case 5:
                                    exec_mirror_rd_rs(inst.c4c);
                                    return;
                            }
                            break;
                        case 3:
                            switch (MASK(inst.c4c.op1, 3, 3)) {
                                case 0:
                                    return;
                                case 1:
                                    return;
                                case 2:
                                    exec_div0s_rs(inst.c4c);
                                    return;
                                case 3:
                                    exec_div0u_rs(inst.c4c);
                                    return;
                                case 4:
                                    exec_div1_rs(inst.c4c);
                                    return;
                                case 5:
                                    exec_div2s_rs(inst.c4c);
                                    return;
                                case 6:
                                    exec_div3s(inst.c4c);
                                    return;
                            }
                            break;
                    }
                    break;
            }
            break;
        case 5:
            switch (MASK(inst.c5a.op2, 6, 2)) {
                case 0:
                    switch (MASK(inst.c5a.op1, 3, 3)) {
                        /* CLASS 5A */
                        case 0:
                            exec_ld_w_sd_rs(inst.c5a);
                            return;
                        case 1:
                            exec_ld_w_rd_ss(inst.c5a);
                            return;
                            /* CLASS 5B */
                        case 2:
                            exec_btst_RB_imm3(inst.c5b);
                            return;
                        case 3:
                            exec_bclr_RB_imm3(inst.c5b);
                            return;
                        case 4:
                            exec_bset_RB_imm3(inst.c5b);
                            return;
                        case 5:
                            exec_bnot_RB_imm3(inst.c5b);
                            return;
                            /* CLASS 5C */
                        case 6:
                            exec_adc_rd_rs(inst.c5c);
                            return;
                        case 7:
                            exec_sbc_rd_rs(inst.c5c);
                            return;
                    }
                    break;
                case 1:
                    switch (MASK(inst.c5c.op1, 3, 3)) {
                        case 0:
                            exec_ld_b_rd_rs(inst.c5c);
                            return;
                        case 1:
                            exec_ld_ub_rd_rs(inst.c5c);
                            return;
                        case 2:
                            exec_ld_h_rd_rs(inst.c5c);
                            return;
                        case 3:
                            exec_ld_uh_rd_rs(inst.c5c);
                            return;
                        case 4:
                            return;
                        case 5:
                            return;
                        case 6:
                            return;
                        case 7:
                            return;
                    }
                    break;
                case 2:
                    switch (MASK(inst.c5c.op1, 3, 3)) {
                        case 0:
                            exec_mlt_h_rd_rs(inst.c5c);
                            return;
                        case 1:
                            exec_mltu_h_rd_rs(inst.c5c);
                            return;
                        case 2:
                            exec_mlt_w_rd_rs(inst.c5c);
                            return;
                        case 3:
                            exec_mltu_w_rd_rs(inst.c5c);
                            return;
                        case 4:
                            exec_mac_rs(inst.c5c);
                            return;
                        case 5:
                            return;
                        case 6:
                            return;
                        case 7:
                            return;
                    }
                    break;
            }
            break;
        case 6:
            /* CLASS 6 */
            exec_ext_imm13(inst.c6);
            return;
            break;
        case 7:
            return;
        case 8:
            return;
    }
    throw std::invalid_argument("Unexpected opcode");
}


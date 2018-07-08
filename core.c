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
 */
#include "app.h"
#include "c33types.h"
#include "core/classes.h"

/****************************************************************************
 *  グローバル変数
 ****************************************************************************/

//CORE core;

/****************************************************************************
 *  グローバル関数
 ****************************************************************************/

void
core_init(PIEMU_CONTEXT *context)
{
    memset(&context->core, 0, sizeof context->core);
    PC = mem_readW(context, 0x0c00000);

    context->core.cond_halt = SDL_CreateCond();
    context->core.mut_halt = SDL_CreateMutex();
    context->core.mut_trap = SDL_CreateMutex();
    context->core.sem_trap_free = SDL_CreateSemaphore(8);
    context->core.sem_trap_queued = SDL_CreateSemaphore(0);
}

void
core_work(PIEMU_CONTEXT *context)
{
    INST inst;

    inst.s = mem_readH(context, PC);
    core_inst(context, inst);
}

void core_handle_hlt(PIEMU_CONTEXT* context)
{
    if(context->core.in_halt == 0)
        return;

    SDL_LockMutex(context->core.mut_halt);
    {
        while(context->core.in_halt != 0) {
            SDL_CondWait(context->core.cond_halt, context->core.mut_halt);
        }
    }
    SDL_UnlockMutex(context->core.mut_halt);
}

void core_handle_trap(PIEMU_CONTEXT* context)
{
    if(SDL_SemTryWait(context->core.sem_trap_queued) == 0) {
        SDL_LockMutex(context->core.mut_trap);
        if (context->core.trap_no > -1) {
            int trap_no = context->core.trap_no;
            struct core_trap_list *trapentry = context->core.traplist + trap_no;

            core_trap_from_core(context, trapentry->no, trapentry->level);

            context->core.trap_no--;
        }
        SDL_UnlockMutex(context->core.mut_trap);
        SDL_SemPost(context->core.sem_trap_free);
    }
}

void core_assert_trap(PIEMU_CONTEXT* context, int no, int level)
{
    SDL_SemWait(context->core.sem_trap_free);
    SDL_LockMutex(context->core.mut_trap);
    {
        int trap_no = context->core.trap_no + 1;
        if (trap_no >= NUM_PENDING_TRAPS) {
            DIE("pending traps list overflow");
        }

        struct core_trap_list *trapentry = context->core.traplist + trap_no;
        trapentry->no = no;
        trapentry->level = level;

        context->core.trap_no = trap_no;
    }
    SDL_UnlockMutex(context->core.mut_trap);
    SDL_SemPost(context->core.sem_trap_queued);
}


unsigned
core_workex(PIEMU_CONTEXT *context, unsigned mils_org, unsigned nClocksDivBy1k)
{
    unsigned insts = 0;
    do {
        core_work(context);
        insts++;
        if(context->core.in_halt)
            break;
    } while (!context->bEndFlag && (CLK - mils_org) < nClocksDivBy1k); /* 1ミリ秒分の処理 */
    return insts;
}

void
core_trap_from_core(PIEMU_CONTEXT *context, int no, int level)
{
    c33word addr;

    /* マスク可能な割り込みの場合のみ、割り込み可能条件を検査します。 */
    if (no >= 16) {
        if (!PSR.ie) return;
        if ((unsigned) level <= PSR.il) return;
    }

    addr = mem_readW(context, pTTBR_REG + no * 4);  /* ※要検討:トラップテーブル直読みしてます */
    SP -= 4;
    mem_writeW(context, SP, PC); /* 要注意！PC+2じゃないよ！ */
    SP -= 4;
    mem_writeW(context, SP, S(0));
    PC = addr;
    PSR.ie = 0;

    /* マスク可能な割り込みの場合のみ、割り込みレベルを変化させます。 */
    if (no >= 16) {
        PSR.il = level;
    }
}

void
core_trap_from_devices(PIEMU_CONTEXT *context, int no, int level)
{
    core_assert_trap(context, no, level);

    SDL_LockMutex(context->core.mut_halt);
    {
        context->core.in_halt = 0;
        SDL_CondBroadcast(context->core.cond_halt);
    }
    SDL_UnlockMutex(context->core.mut_halt);
}

#if 1
#define MASK(op, shr, and) ((inst.s >> (16 - (shr + and))) & ((1 << and) - 1))
#else
#define MASK(op, shr, and) (op)
#endif

void
core_inst(PIEMU_CONTEXT *context, INST inst)
{
    switch (MASK(inst.c0a.cls, 0, 3)) {
        case 0:
            switch (MASK(inst.c0a.op1, 3, 4)) {
                /* CLASS 0A */
                case 0:
                    switch (MASK(inst.c0a.op2, 8, 2)) {
                        case 0:
                            exec_nop(context, inst.c0a);
                            return;
                        case 1:
                            exec_slp(context, inst.c0a);
                            return;
                        case 2:
                            exec_halt(context, inst.c0a);
                            return;
                        case 3:
                            return;
                    }
                    break;
                case 1:
                    switch (MASK(inst.c0a.op2, 8, 2)) {
                        case 0:
                            exec_pushn_rs(context, inst.c0a);
                            return;
                        case 1:
                            exec_popn_rd(context, inst.c0a);
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
                            exec_brk(context, inst.c0a);
                            return;
                        case 1:
                            exec_retd(context, inst.c0a);
                            return;
                        case 2:
                            exec_int_imm2(context, inst.c0a);
                            return;
                        case 3:
                            exec_reti(context, inst.c0a);
                            return;
                    }
                    break;
                case 3:
                    switch (MASK(inst.c0a.op2, 8, 2)) {
                        case 0:
                            exec_call_rb(context, inst.c0a);
                            return;
                        case 1:
                            exec_ret(context, inst.c0a);
                            return;
                        case 2:
                            exec_jp_rb(context, inst.c0a);
                            return;
                        case 3:
                            return;
                    }
                    break;
                    /* CLASS 0B */
                case 4:
                    exec_jrgt_sign8(context, inst.c0b);
                    return;
                case 5:
                    exec_jrge_sign8(context, inst.c0b);
                    return;
                case 6:
                    exec_jrlt_sign8(context, inst.c0b);
                    return;
                case 7:
                    exec_jrle_sign8(context, inst.c0b);
                    return;
                case 8:
                    exec_jrugt_sign8(context, inst.c0b);
                    return;
                case 9:
                    exec_jruge_sign8(context, inst.c0b);
                    return;
                case 10:
                    exec_jrult_sign8(context, inst.c0b);
                    return;
                case 11:
                    exec_jrule_sign8(context, inst.c0b);
                    return;
                case 12:
                    exec_jreq_sign8(context, inst.c0b);
                    return;
                case 13:
                    exec_jrne_sign8(context, inst.c0b);
                    return;
                case 14:
                    exec_call_sign8(context, inst.c0b);
                    return;
                case 15:
                    exec_jp_sign8(context, inst.c0b);
                    return;
            }
            break;
        case 1:
            switch (MASK(inst.c1a.op2, 6, 2)) {
                /* CLASS 1A */
                case 0:
                    switch (MASK(inst.c1a.op1, 3, 3)) {
                        case 0:
                            exec_ld_b_rd_RB(context, inst.c1a);
                            return;
                        case 1:
                            exec_ld_ub_rd_RB(context, inst.c1a);
                            return;
                        case 2:
                            exec_ld_h_rd_RB(context, inst.c1a);
                            return;
                        case 3:
                            exec_ld_uh_rd_RB(context, inst.c1a);
                            return;
                        case 4:
                            exec_ld_w_rd_RB(context, inst.c1a);
                            return;
                        case 5:
                            exec_ld_b_RB_rs(context, inst.c1a);
                            return;
                        case 6:
                            exec_ld_h_RB_rs(context, inst.c1a);
                            return;
                        case 7:
                            exec_ld_w_RB_rs(context, inst.c1a);
                            return;
                    }
                    break;
                case 1:
                    switch (MASK(inst.c1a.op1, 3, 3)) {
                        case 0:
                            exec_ld_b_rd_RBx(context, inst.c1a);
                            return;
                        case 1:
                            exec_ld_ub_rd_RBx(context, inst.c1a);
                            return;
                        case 2:
                            exec_ld_h_rd_RBx(context, inst.c1a);
                            return;
                        case 3:
                            exec_ld_uh_rd_RBx(context, inst.c1a);
                            return;
                        case 4:
                            exec_ld_w_rd_RBx(context, inst.c1a);
                            return;
                        case 5:
                            exec_ld_b_RBx_rs(context, inst.c1a);
                            return;
                        case 6:
                            exec_ld_h_RBx_rs(context, inst.c1a);
                            return;
                        case 7:
                            exec_ld_w_RBx_rs(context, inst.c1a);
                            return;
                    }
                    break;
                    /* CLASS 1B */
                case 2:
                    switch (MASK(inst.c1a.op1, 3, 3)) {
                        case 0:
                            exec_add_rd_rs(context, inst.c1b);
                            return;
                        case 1:
                            exec_sub_rd_rs(context, inst.c1b);
                            return;
                        case 2:
                            exec_cmp_rd_rs(context, inst.c1b);
                            return;
                        case 3:
                            exec_ld_w_rd_rs(context, inst.c1b);
                            return;
                        case 4:
                            exec_and_rd_rs(context, inst.c1b);
                            return;
                        case 5:
                            exec_or_rd_rs(context, inst.c1b);
                            return;
                        case 6:
                            exec_xor_rd_rs(context, inst.c1b);
                            return;
                        case 7:
                            exec_not_rd_rs(context, inst.c1b);
                            return;
                    }
                    break;
            }
            break;
        case 2:
            switch (MASK(inst.c2.op1, 3, 3)) {
                /* CLASS 2 */
                case 0:
                    exec_ld_b_rd_SPxIMM6(context, inst.c2);
                    return;
                case 1:
                    exec_ld_ub_rd_SPxIMM6(context, inst.c2);
                    return;
                case 2:
                    exec_ld_h_rd_SPxIMM6(context, inst.c2);
                    return;
                case 3:
                    exec_ld_uh_rd_SPxIMM6(context, inst.c2);
                    return;
                case 4:
                    exec_ld_w_rd_SPxIMM6(context, inst.c2);
                    return;
                case 5:
                    exec_ld_b_SPxIMM6_rs(context, inst.c2);
                    return;
                case 6:
                    exec_ld_h_SPxIMM6_rs(context, inst.c2);
                    return;
                case 7:
                    exec_ld_w_SPxIMM6_rs(context, inst.c2);
                    return;
            }
            break;
        case 3:
            switch (MASK(inst.c3.op1, 3, 3)) {
                /* CLASS 3 */
                case 0:
                    exec_add_rd_imm6(context, inst.c3);
                    return;
                case 1:
                    exec_sub_rd_imm6(context, inst.c3);
                    return;
                case 2:
                    exec_cmp_rd_sign6(context, inst.c3);
                    return;
                case 3:
                    exec_ld_w_rd_sign6(context, inst.c3);
                    return;
                case 4:
                    exec_and_rd_sign6(context, inst.c3);
                    return;
                case 5:
                    exec_or_rd_sign6(context, inst.c3);
                    return;
                case 6:
                    exec_xor_rd_sign6(context, inst.c3);
                    return;
                case 7:
                    exec_not_rd_sign6(context, inst.c3);
                    return;
            }
            break;
        case 4:
            switch (MASK(inst.c4a.op1, 3, 3)) {
                /* CLASS 4A */
                case 0:
                    exec_add_sp_imm10(context, inst.c4a);
                    return;
                case 1:
                    exec_sub_sp_imm10(context, inst.c4a);
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
                                    exec_srl_rd_imm4(context, inst.c4b);
                                    return;
                                case 3:
                                    exec_sll_rd_imm4(context, inst.c4b);
                                    return;
                                case 4:
                                    exec_sra_rd_imm4(context, inst.c4b);
                                    return;
                                case 5:
                                    exec_sla_rd_imm4(context, inst.c4b);
                                    return;
                                case 6:
                                    exec_rr_rd_imm4(context, inst.c4b);
                                    return;
                                case 7:
                                    exec_rl_rd_imm4(context, inst.c4b);
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
                                    exec_srl_rd_rs(context, inst.c4b);
                                    return;
                                case 3:
                                    exec_sll_rd_rs(context, inst.c4b);
                                    return;
                                case 4:
                                    exec_sra_rd_rs(context, inst.c4b);
                                    return;
                                case 5:
                                    exec_sla_rd_rs(context, inst.c4b);
                                    return;
                                case 6:
                                    exec_rr_rd_rs(context, inst.c4b);
                                    return;
                                case 7:
                                    exec_rl_rd_rs(context, inst.c4b);
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
                                    exec_scan0_rd_rs(context, inst.c4c);
                                    return;
                                case 3:
                                    exec_scan1_rd_rs(context, inst.c4c);
                                    return;
                                case 4:
                                    exec_swap_rd_rs(context, inst.c4c);
                                    return;
                                case 5:
                                    exec_mirror_rd_rs(context, inst.c4c);
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
                                    exec_div0s_rs(context, inst.c4c);
                                    return;
                                case 3:
                                    exec_div0u_rs(context, inst.c4c);
                                    return;
                                case 4:
                                    exec_div1_rs(context, inst.c4c);
                                    return;
                                case 5:
                                    exec_div2s_rs(context, inst.c4c);
                                    return;
                                case 6:
                                    exec_div3s(context, inst.c4c);
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
                            exec_ld_w_sd_rs(context, inst.c5a);
                            return;
                        case 1:
                            exec_ld_w_rd_ss(context, inst.c5a);
                            return;
                            /* CLASS 5B */
                        case 2:
                            exec_btst_RB_imm3(context, inst.c5b);
                            return;
                        case 3:
                            exec_bclr_RB_imm3(context, inst.c5b);
                            return;
                        case 4:
                            exec_bset_RB_imm3(context, inst.c5b);
                            return;
                        case 5:
                            exec_bnot_RB_imm3(context, inst.c5b);
                            return;
                            /* CLASS 5C */
                        case 6:
                            exec_adc_rd_rs(context, inst.c5c);
                            return;
                        case 7:
                            exec_sbc_rd_rs(context, inst.c5c);
                            return;
                    }
                    break;
                case 1:
                    switch (MASK(inst.c5c.op1, 3, 3)) {
                        case 0:
                            exec_ld_b_rd_rs(context, inst.c5c);
                            return;
                        case 1:
                            exec_ld_ub_rd_rs(context, inst.c5c);
                            return;
                        case 2:
                            exec_ld_h_rd_rs(context, inst.c5c);
                            return;
                        case 3:
                            exec_ld_uh_rd_rs(context, inst.c5c);
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
                            exec_mlt_h_rd_rs(context, inst.c5c);
                            return;
                        case 1:
                            exec_mltu_h_rd_rs(context, inst.c5c);
                            return;
                        case 2:
                            exec_mlt_w_rd_rs(context, inst.c5c);
                            return;
                        case 3:
                            exec_mltu_w_rd_rs(context, inst.c5c);
                            return;
                        case 4:
                            exec_mac_rs(context, inst.c5c);
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
            exec_ext_imm13(context, inst.c6);
            return;
            break;
        case 7:
            return;
        case 8:
            return;
    }
    DIE("Unexpected opcode: %02x", inst.s);
}


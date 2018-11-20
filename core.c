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

void exec_DIE(PIEMU_CONTEXT* context, INST inst)
{
    DIE("invalid opcode: %04x", inst.s);
}

/****
 * 命令表
 ****/

// [op1][op2]
static const C33INST_EXEC C33INST_TABLE_C0A[4][4] = {
    { exec_nop, exec_slp, exec_halt, exec_DIE },
    { exec_pushn_rs, exec_popn_rd, exec_DIE, exec_DIE },
    { exec_brk, exec_retd, exec_int_imm2, exec_reti },
    { exec_call_rb, exec_ret, exec_jp_rb, exec_DIE }
};

static const C33INST_EXEC C33INST_TABLE_C0B[16] = {
    exec_DIE, exec_DIE, exec_DIE, exec_DIE,

    exec_jrgt_sign8, exec_jrge_sign8, exec_jrlt_sign8, exec_jrle_sign8,
    exec_jrugt_sign8, exec_jruge_sign8, exec_jrult_sign8, exec_jrule_sign8,
    exec_jreq_sign8, exec_jrne_sign8, exec_call_sign8, exec_jp_sign8
};

// [op2][op1]
static const C33INST_EXEC C33INST_TABLE_C1[4][8] = {
    { exec_ld_b_rd_RB, exec_ld_ub_rd_RB, exec_ld_h_rd_RB, exec_ld_uh_rd_RB,
      exec_ld_w_rd_RB, exec_ld_b_RB_rs, exec_ld_h_RB_rs, exec_ld_w_RB_rs },
    { exec_ld_b_rd_RBx, exec_ld_ub_rd_RBx, exec_ld_h_rd_RBx, exec_ld_uh_rd_RBx,
      exec_ld_w_rd_RBx, exec_ld_b_RBx_rs, exec_ld_h_RBx_rs, exec_ld_w_RBx_rs },
    { exec_add_rd_rs, exec_sub_rd_rs, exec_cmp_rd_rs, exec_ld_w_rd_rs,
      exec_and_rd_rs, exec_or_rd_rs, exec_xor_rd_rs, exec_not_rd_rs },
    { exec_DIE, exec_DIE, exec_DIE, exec_DIE, exec_DIE, exec_DIE, exec_DIE, exec_DIE },
};

static const C33INST_EXEC C33INST_TABLE_C2[8] = {
    exec_ld_b_rd_SPxIMM6, exec_ld_ub_rd_SPxIMM6, exec_ld_h_rd_SPxIMM6, exec_ld_uh_rd_SPxIMM6,
    exec_ld_w_rd_SPxIMM6, exec_ld_b_SPxIMM6_rs, exec_ld_h_SPxIMM6_rs, exec_ld_w_SPxIMM6_rs
};

static const C33INST_EXEC C33INST_TABLE_C3[8] = {
    exec_add_rd_imm6, exec_sub_rd_imm6, exec_cmp_rd_sign6, exec_ld_w_rd_sign6,
    exec_and_rd_sign6, exec_or_rd_sign6, exec_xor_rd_sign6, exec_not_rd_sign6
};

static const C33INST_EXEC C33INST_TABLE_C4A[8] = {
    exec_add_sp_imm10, exec_sub_sp_imm10, exec_DIE, exec_DIE,
    exec_DIE, exec_DIE, exec_DIE, exec_DIE
};

// [op2][op1]
static const C33INST_EXEC C33INST_TABLE_C4BC[4][8] = {
    { exec_DIE, exec_DIE, exec_srl_rd_imm4, exec_sll_rd_imm4,
        exec_sra_rd_imm4, exec_sla_rd_imm4, exec_rr_rd_imm4, exec_rl_rd_imm4 },
    { exec_DIE, exec_DIE, exec_srl_rd_rs, exec_sll_rd_rs,
        exec_sra_rd_rs, exec_sla_rd_rs, exec_rr_rd_rs, exec_rl_rd_rs },
    { exec_DIE, exec_DIE, exec_scan0_rd_rs, exec_scan1_rd_rs,
        exec_swap_rd_rs, exec_mirror_rd_rs, exec_DIE, exec_DIE },
    { exec_DIE, exec_DIE, exec_div0s_rs, exec_div0u_rs,
        exec_div1_rs, exec_div2s_rs, exec_div3s, exec_DIE }
};

// [op2][op1]
static const C33INST_EXEC C33INST_TABLE_C5[4][8] = {
    { exec_ld_w_sd_rs, exec_ld_w_rd_ss, exec_btst_RB_imm3, exec_bclr_RB_imm3,
        exec_bset_RB_imm3, exec_bnot_RB_imm3, exec_adc_rd_rs, exec_sbc_rd_rs  },
    { exec_ld_b_rd_rs, exec_ld_ub_rd_rs, exec_ld_h_rd_rs, exec_ld_uh_rd_rs,
        exec_DIE, exec_DIE, exec_DIE, exec_DIE },
    { exec_mlt_h_rd_rs, exec_mltu_h_rd_rs, exec_mlt_w_rd_rs, exec_mltu_w_rd_rs,
        exec_mac_rs, exec_DIE, exec_DIE, exec_DIE },
    { exec_DIE, exec_DIE, exec_DIE, exec_DIE, exec_DIE, exec_DIE, exec_DIE, exec_DIE },
};


/****************************************************************************
 *  グローバル関数
 ****************************************************************************/

void core_init(PIEMU_CONTEXT *context)
{
    memset(&context->core, 0, sizeof context->core);
    PC = mem_readW(context, 0x0c00000);

    context->core.cond_halt = SDL_CreateCond();
    context->core.mut_halt = SDL_CreateMutex();
    context->core.mut_trap = SDL_CreateMutex();
    context->core.sem_trap_free = SDL_CreateSemaphore(8);
    context->core.sem_trap_queued = SDL_CreateSemaphore(0);
}

void core_work(PIEMU_CONTEXT *context)
{
    INST inst;

    inst.s = mem_readH(context, PC);
    core_inst(context, inst);
}

// HLT が実行中なら割り込みがあるまでここで寝る
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

// コアがデバイスから通知された割り込みを取り扱う
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

// デバイスがコアへ割り込みを通知する
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

unsigned core_workex(PIEMU_CONTEXT *context, unsigned nClocksDivBy1k)
{
    unsigned insts = 0;
    unsigned mils_org = CLK;
    do {
        core_work(context);
        insts++;
        core_handle_trap(context);
        if(context->core.in_halt)
            break;
    } while (!context->bEndFlag && (CLK - mils_org) < nClocksDivBy1k); /* 1ミリ秒分の処理 */
    return insts;
}

// コア内から割り込みを発生（デバイスからは trap_from_device を使う）
void core_trap_from_core(PIEMU_CONTEXT *context, int no, int level)
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

// デバイスから割り込みを発生する（コアが HLT/SLP 中なら起こす）
void core_trap_from_devices(PIEMU_CONTEXT *context, int no, int level)
{
    core_assert_trap(context, no, level);

    SDL_LockMutex(context->core.mut_halt);
    {
        context->core.in_halt = 0;
        SDL_CondBroadcast(context->core.cond_halt);
    }
    SDL_UnlockMutex(context->core.mut_halt);
}

// 命令デコード
void core_inst(PIEMU_CONTEXT* context, INST inst)
{
    switch(inst.c0a.cls) {
        case 0:
            if(inst.c0a.op1 < 4)
                (C33INST_TABLE_C0A[inst.c0a.op1][inst.c0a.op2])(context, inst);
            else
                (C33INST_TABLE_C0B[inst.c0b.op1])(context, inst);
            return;
        case 1:
            (C33INST_TABLE_C1[inst.c1a.op2][inst.c1a.op1])(context, inst);
            return;
        case 2:
            (C33INST_TABLE_C2[inst.c2.op1])(context, inst);
            return;
        case 3:
            (C33INST_TABLE_C3[inst.c3.op1])(context, inst);
            return;
        case 4:
            if(inst.c4a.op1 < 2)
                (C33INST_TABLE_C4A[inst.c4a.op1])(context, inst);
            else
                (C33INST_TABLE_C4BC[inst.c4b.op2][inst.c4b.op1])(context, inst);
            return;
        case 5:
            (C33INST_TABLE_C5[inst.c5a.op2][inst.c5a.op1])(context, inst);
            return;
        case 6:
            exec_ext_imm13(context, inst);
            return;
        case 7:
        default:
            return;
    }
}

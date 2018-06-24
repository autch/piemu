#ifndef PIEMU_C33CORE_H
#define PIEMU_C33CORE_H

#include <SDL_mutex.h>
#include "c33types.h"
#include "c33inst.h"

struct PSR {
    uint32_t n  : 1;
    uint32_t z  : 1;
    uint32_t v  : 1;
    uint32_t c  : 1;
    uint32_t ie : 1;
    uint32_t  : 1;
    uint32_t ds : 1;
    uint32_t mo : 1;
    uint32_t il : 4;
    uint32_t  :20;
};

class C33Core
{

private:
    uint32_t r[16];         // general register
    uint32_t s[ 4];         // PSR, SP, ALR, AHR
    uint32_t pc;            // program counter
    INST     ext[2];        // immediate extension
    uint32_t d;             // delay slot
    uint32_t clk;           // current core clock counter

    SDL_mutex* mut_halt;
    SDL_cond* cond_halt;
    int in_halt;            // protected by mut/cond_halt, 1 indicates core is waiting for interrupt

    SDL_mutex* mut_core;

    c33byte mem_readB(c33word addr);
    c33hword mem_readH(c33word addr);
    c33word mem_readW(c33word addr);

    void mem_writeB(c33word addr, c33byte value);
    void mem_writeH(c33word addr, c33hword value);
    void mem_writeW(c33word addr, c33word value);

    // ext
    int ext_3op();
    int ext_PCxSIGN8(int sign8);
    int ext_RB(int rb);
    int ext_SPxIMM6(int imm6, int size);
    int ext_imm6(int imm6);
    int ext_sign6(int sign6);

    // common operations
    int sign_ext(int data, int bits);

    c33int c33adc(c33int a, c33int b);
    c33int c33add(c33int a, c33int b);
    c33int c33and(c33int a, c33int b);
    c33int c33not(c33int a);
    c33int c33or(c33int a, c33int b);
    c33int c33rl(c33int a, c33int b);
    c33int c33rr(c33int a, c33int b);
    c33int c33sbc(c33int a, c33int b);
    c33int c33sla(c33int a, c33int b);
    c33int c33sll(c33int a, c33int b);
    c33int c33sra(c33int a, c33int b);
    c33int c33srl(c33int a, c33int b);
    c33int c33sub(c33int a, c33int b);
    c33int c33xor(c33int a, c33int b);

    void exec_delay(int dflag);

    // CLASS 0A
    void exec_brk(CLASS_0A inst);
    void exec_halt(CLASS_0A inst);
    void exec_jp_rb(CLASS_0A inst);
    void exec_call_rb(CLASS_0A inst);
    void exec_int_imm2(CLASS_0A inst);
    void exec_nop(CLASS_0A inst);
    void exec_popn_rd(CLASS_0A inst);
    void exec_pushn_rs(CLASS_0A inst);
    void exec_ret(CLASS_0A inst);
    void exec_retd(CLASS_0A inst);
    void exec_reti(CLASS_0A inst);
    void exec_slp(CLASS_0A inst);

    // CLASS 0B
    void exec_call_sign8(CLASS_0B inst);
    void exec_jp_sign8(CLASS_0B inst);
    void exec_jreq_sign8(CLASS_0B inst);
    void exec_jrge_sign8(CLASS_0B inst);
    void exec_jrgt_sign8(CLASS_0B inst);
    void exec_jrle_sign8(CLASS_0B inst);
    void exec_jrlt_sign8(CLASS_0B inst);
    void exec_jrne_sign8(CLASS_0B inst);
    void exec_jruge_sign8(CLASS_0B inst);
    void exec_jrugt_sign8(CLASS_0B inst);
    void exec_jrule_sign8(CLASS_0B inst);
    void exec_jrult_sign8(CLASS_0B inst);

    // CLASS 1A
    void exec_ld_b_RB_rs(CLASS_1A inst);
    void exec_ld_b_RBx_rs(CLASS_1A inst);
    void exec_ld_b_rd_RB(CLASS_1A inst);
    void exec_ld_b_rd_RBx(CLASS_1A inst);
    void exec_ld_h_RB_rs(CLASS_1A inst);
    void exec_ld_h_RBx_rs(CLASS_1A inst);
    void exec_ld_h_rd_RB(CLASS_1A inst);
    void exec_ld_h_rd_RBx(CLASS_1A inst);
    void exec_ld_ub_rd_RB(CLASS_1A inst);
    void exec_ld_ub_rd_RBx(CLASS_1A inst);
    void exec_ld_uh_rd_RB(CLASS_1A inst);
    void exec_ld_uh_rd_RBx(CLASS_1A inst);
    void exec_ld_w_RB_rs(CLASS_1A inst);
    void exec_ld_w_RBx_rs(CLASS_1A inst);
    void exec_ld_w_rd_RB(CLASS_1A inst);
    void exec_ld_w_rd_RBx(CLASS_1A inst);

    // CLASS 1B
    void exec_add_rd_rs(CLASS_1B inst);
    void exec_and_rd_rs(CLASS_1B inst);
    void exec_cmp_rd_rs(CLASS_1B inst);
    void exec_ld_w_rd_rs(CLASS_1B inst);
    void exec_not_rd_rs(CLASS_1B inst);
    void exec_or_rd_rs(CLASS_1B inst);
    void exec_sub_rd_rs(CLASS_1B inst);
    void exec_xor_rd_rs(CLASS_1B inst);

    // CLASS 2
    void exec_ld_b_SPxIMM6_rs(CLASS_2 inst);
    void exec_ld_b_rd_SPxIMM6(CLASS_2 inst);
    void exec_ld_h_SPxIMM6_rs(CLASS_2 inst);
    void exec_ld_h_rd_SPxIMM6(CLASS_2 inst);
    void exec_ld_ub_rd_SPxIMM6(CLASS_2 inst);
    void exec_ld_uh_rd_SPxIMM6(CLASS_2 inst);
    void exec_ld_w_SPxIMM6_rs(CLASS_2 inst);
    void exec_ld_w_rd_SPxIMM6(CLASS_2 inst);

    // CLASS 3
    void exec_add_rd_imm6(CLASS_3 inst);
    void exec_and_rd_sign6(CLASS_3 inst);
    void exec_cmp_rd_sign6(CLASS_3 inst);
    void exec_ld_w_rd_sign6(CLASS_3 inst);
    void exec_not_rd_sign6(CLASS_3 inst);
    void exec_or_rd_sign6(CLASS_3 inst);
    void exec_sub_rd_imm6(CLASS_3 inst);
    void exec_xor_rd_sign6(CLASS_3 inst);

    // CLASS 4A
    void exec_add_sp_imm10(CLASS_4A inst);
    void exec_sub_sp_imm10(CLASS_4A inst);

    // CLASS 4B
    void exec_rl_rd_imm4(CLASS_4B inst);
    void exec_rl_rd_rs(CLASS_4B inst);
    void exec_rr_rd_imm4(CLASS_4B inst);
    void exec_rr_rd_rs(CLASS_4B inst);
    void exec_sla_rd_imm4(CLASS_4B inst);
    void exec_sla_rd_rs(CLASS_4B inst);
    void exec_sll_rd_imm4(CLASS_4B inst);
    void exec_sll_rd_rs(CLASS_4B inst);
    void exec_sra_rd_imm4(CLASS_4B inst);
    void exec_sra_rd_rs(CLASS_4B inst);
    void exec_srl_rd_imm4(CLASS_4B inst);
    void exec_srl_rd_rs(CLASS_4B inst);

    // CLASS 4C
    void exec_scan0_rd_rs(CLASS_4C inst);
    void exec_scan1_rd_rs(CLASS_4C inst);
    void exec_div0s_rs(CLASS_4C inst);
    void exec_div0u_rs(CLASS_4C inst);
    void exec_div1_rs(CLASS_4C inst);
    void exec_div2s_rs(CLASS_4C inst);
    void exec_div3s(CLASS_4C inst);
    void exec_mirror_rd_rs(CLASS_4C inst);
    void exec_swap_rd_rs(CLASS_4C inst);

    // CLASS 5A
    void exec_ld_w_rd_ss(CLASS_5A inst);
    void exec_ld_w_sd_rs(CLASS_5A inst);

    // CLASS 5B
    void exec_bclr_RB_imm3(CLASS_5B inst);
    void exec_bnot_RB_imm3(CLASS_5B inst);
    void exec_bset_RB_imm3(CLASS_5B inst);
    void exec_btst_RB_imm3(CLASS_5B inst);

    // CLASS 5C
    void exec_adc_rd_rs(CLASS_5C inst);
    void exec_ld_b_rd_rs(CLASS_5C inst);
    void exec_ld_h_rd_rs(CLASS_5C inst);
    void exec_ld_ub_rd_rs(CLASS_5C inst);
    void exec_ld_uh_rd_rs(CLASS_5C inst);
    void exec_mac_rs(CLASS_5C inst);
    void exec_mlt_h_rd_rs(CLASS_5C inst);
    void exec_mlt_w_rd_rs(CLASS_5C inst);
    void exec_mltu_h_rd_rs(CLASS_5C inst);
    void exec_mltu_w_rd_rs(CLASS_5C inst);
    void exec_sbc_rd_rs(CLASS_5C inst);

    // CLASS 6
    void exec_ext_imm13(CLASS_6 inst);

    void core_trap_from_core(int no, int level);
    void core_inst(INST inst);

public:
    C33Core()
    {
        cond_halt = SDL_CreateCond();
        mut_halt = SDL_CreateMutex();
        mut_core = SDL_CreateMutex();
        SDL_memset(r, 0, sizeof r);
        SDL_memset(s, 0, sizeof s);
        SDL_memset(ext, 0, sizeof ext);
        in_halt = 0;
        pc = 0x00000000;
        clk = 0;
        d = 0;
    }

    ~C33Core() {
        SDL_DestroyCond(cond_halt);
        SDL_DestroyMutex(mut_halt);
        SDL_DestroyMutex(mut_core);
    }

    void core_init();
    void core_work();
    unsigned core_workex(unsigned mils_org, unsigned nClocksDivBy1k);
    void core_handle_hlt();

    void core_trap_from_devices(int no, int level);

    unsigned advance_clock(unsigned clock) {
        clk += clock;
        return clk;
    }
};


#endif //PIEMU_C33CORE_H

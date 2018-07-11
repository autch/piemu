#ifndef classes_h
#define classes_h

#include "c33types.h"
#include "../app.h"

/****************************************************************************
 *  マクロ・補助関数
 ****************************************************************************/

/* 関数の先頭で使えるように、ダミー変数を初期化します。 */

// C33 には無効命令例外がないので、不正なオペコードを検知したとしてもできることはない。
// マニュアルでは未定義になっている。
#ifdef CORE_DEBUG
#define NO_EXT          { if(EXT1.s) DIE("NO_EXT violation"); }
#define NO_DELAY        { if(context->core.d) DIE("NO_DELAY violation"); }
#else /*CORE_DEBUG*/
#define NO_EXT          ((void)0)
#define NO_DELAY        ((void)0)
#endif /*CORE_DEBUG*/

/* インデクス検査付きの汎用/システムレジスタアクセス　*/
#ifdef CORE_DEBUG
static int check_r(int n) { if(n < 0 || n > 15) DIE(); return n; }
static int check_s(int n) { if(n < 0 || n >  3) DIE(); return n; }
#define R(n) context->core.r[check_r(n)]
#define S(n) context->core.s[check_s(n)]
#else /*CORE_DEBUG*/
#define R(n)  context->core.r[n]
#define S(n)  context->core.s[n]

#endif /*CORE_DEBUG*/

int sign_ext(int data, int bits);
void exec_delay(PIEMU_CONTEXT *context, int dflag);

c33int adc(tPSR *psr, c33int a, c33int b);
c33int add(tPSR *psr, c33int a, c33int b);
c33int and(tPSR *psr, c33int a, c33int b);
void exec_adc_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_add_rd_imm6(PIEMU_CONTEXT *context, INST inst);
void exec_add_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_add_sp_imm10(PIEMU_CONTEXT *context, INST inst);
void exec_and_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_and_rd_sign6(PIEMU_CONTEXT *context, INST inst);
void exec_bclr_RB_imm3(PIEMU_CONTEXT *context, INST inst);
void exec_bnot_RB_imm3(PIEMU_CONTEXT *context, INST inst);
void exec_brk(PIEMU_CONTEXT *context, INST inst);
void exec_bset_RB_imm3(PIEMU_CONTEXT *context, INST inst);
void exec_btst_RB_imm3(PIEMU_CONTEXT *context, INST inst);
void exec_call_rb(PIEMU_CONTEXT *context, INST inst);
void exec_call_sign8(PIEMU_CONTEXT *context, INST inst);
void exec_cmp_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_cmp_rd_sign6(PIEMU_CONTEXT *context, INST inst);
void exec_div0s_rs(PIEMU_CONTEXT *context, INST inst);
void exec_div0u_rs(PIEMU_CONTEXT *context, INST inst);
void exec_div1_rs(PIEMU_CONTEXT *context, INST inst);
void exec_div2s_rs(PIEMU_CONTEXT *context, INST inst);
void exec_div3s(PIEMU_CONTEXT *context, INST inst);
void exec_ext_imm13(PIEMU_CONTEXT *context, INST inst);
void exec_halt(PIEMU_CONTEXT *context, INST inst);
void exec_int_imm2(PIEMU_CONTEXT *context, INST inst);
void exec_jp_rb(PIEMU_CONTEXT *context, INST inst);
void exec_jp_sign8(PIEMU_CONTEXT *context, INST inst);
void exec_jreq_sign8(PIEMU_CONTEXT *context, INST inst);
void exec_jrge_sign8(PIEMU_CONTEXT *context, INST inst);
void exec_jrgt_sign8(PIEMU_CONTEXT *context, INST inst);
void exec_jrle_sign8(PIEMU_CONTEXT *context, INST inst);
void exec_jrlt_sign8(PIEMU_CONTEXT *context, INST inst);
void exec_jrne_sign8(PIEMU_CONTEXT *context, INST inst);
void exec_jruge_sign8(PIEMU_CONTEXT *context, INST inst);
void exec_jrugt_sign8(PIEMU_CONTEXT *context, INST inst);
void exec_jrule_sign8(PIEMU_CONTEXT *context, INST inst);
void exec_jrult_sign8(PIEMU_CONTEXT *context, INST inst);
void exec_ld_b_RB_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_b_RBx_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_b_SPxIMM6_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_b_rd_RB(PIEMU_CONTEXT *context, INST inst);
void exec_ld_b_rd_RBx(PIEMU_CONTEXT *context, INST inst);
void exec_ld_b_rd_SPxIMM6(PIEMU_CONTEXT *context, INST inst);
void exec_ld_b_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_h_RB_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_h_RBx_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_h_SPxIMM6_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_h_rd_RB(PIEMU_CONTEXT *context, INST inst);
void exec_ld_h_rd_RBx(PIEMU_CONTEXT *context, INST inst);
void exec_ld_h_rd_SPxIMM6(PIEMU_CONTEXT *context, INST inst);
void exec_ld_h_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_ub_rd_RB(PIEMU_CONTEXT *context, INST inst);
void exec_ld_ub_rd_RBx(PIEMU_CONTEXT *context, INST inst);
void exec_ld_ub_rd_SPxIMM6(PIEMU_CONTEXT *context, INST inst);
void exec_ld_ub_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_uh_rd_RB(PIEMU_CONTEXT *context, INST inst);
void exec_ld_uh_rd_RBx(PIEMU_CONTEXT *context, INST inst);
void exec_ld_uh_rd_SPxIMM6(PIEMU_CONTEXT *context, INST inst);
void exec_ld_uh_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_w_RB_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_w_RBx_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_w_SPxIMM6_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_w_rd_RB(PIEMU_CONTEXT *context, INST inst);
void exec_ld_w_rd_RBx(PIEMU_CONTEXT *context, INST inst);
void exec_ld_w_rd_SPxIMM6(PIEMU_CONTEXT *context, INST inst);
void exec_ld_w_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ld_w_rd_sign6(PIEMU_CONTEXT *context, INST inst);
void exec_ld_w_rd_ss(PIEMU_CONTEXT *context, INST inst);
void exec_ld_w_sd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_mac_rs(PIEMU_CONTEXT *context, INST inst);
void exec_mirror_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_mlt_h_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_mlt_w_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_mltu_h_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_mltu_w_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_nop(PIEMU_CONTEXT *context, INST inst);
void exec_not_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_not_rd_sign6(PIEMU_CONTEXT *context, INST inst);
void exec_or_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_or_rd_sign6(PIEMU_CONTEXT *context, INST inst);
void exec_popn_rd(PIEMU_CONTEXT *context, INST inst);
void exec_pushn_rs(PIEMU_CONTEXT *context, INST inst);
void exec_ret(PIEMU_CONTEXT *context, INST inst);
void exec_retd(PIEMU_CONTEXT *context, INST inst);
void exec_reti(PIEMU_CONTEXT *context, INST inst);
void exec_rl_rd_imm4(PIEMU_CONTEXT *context, INST inst);
void exec_rl_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_rr_rd_imm4(PIEMU_CONTEXT *context, INST inst);
void exec_rr_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_sbc_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_scan0_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_scan1_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_sla_rd_imm4(PIEMU_CONTEXT *context, INST inst);
void exec_sla_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_sll_rd_imm4(PIEMU_CONTEXT *context, INST inst);
void exec_sll_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_slp(PIEMU_CONTEXT *context, INST inst);
void exec_sra_rd_imm4(PIEMU_CONTEXT *context, INST inst);
void exec_sra_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_srl_rd_imm4(PIEMU_CONTEXT *context, INST inst);
void exec_srl_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_sub_rd_imm6(PIEMU_CONTEXT *context, INST inst);
void exec_sub_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_sub_sp_imm10(PIEMU_CONTEXT *context, INST inst);
void exec_swap_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_xor_rd_rs(PIEMU_CONTEXT *context, INST inst);
void exec_xor_rd_sign6(PIEMU_CONTEXT *context, INST inst);
int ext_3op(PIEMU_CONTEXT *context);
int ext_PCxSIGN8(PIEMU_CONTEXT *context, int sign8);
int ext_RB(PIEMU_CONTEXT *context, int rb);
int ext_SPxIMM6(PIEMU_CONTEXT *context, int imm6, int size);
int ext_imm6(PIEMU_CONTEXT *context, int imm6);
int ext_sign6(PIEMU_CONTEXT *context, int sign6);
c33int not(tPSR *psr, c33int a);
c33int or(tPSR *psr, c33int a, c33int b);
c33int rl(tPSR *psr, c33int a, c33int b);
c33int rr(tPSR *psr, c33int a, c33int b);
c33int sbc(tPSR *psr, c33int a, c33int b);
c33int sla(tPSR *psr, c33int a, c33int b);
c33int sll(tPSR *psr, c33int a, c33int b);
c33int sra(tPSR *psr, c33int a, c33int b);
c33int srl(tPSR *psr, c33int a, c33int b);
c33int sub(tPSR *psr, c33int a, c33int b);
c33int xor(tPSR *psr, c33int a, c33int b);

#endif // !classes_h

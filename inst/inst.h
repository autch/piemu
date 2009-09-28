
#ifndef INST_H
#define INST_H

#include "inst_types.h"
#include "core_types.h"
#include "c33209e.h"

#define DIE() { (void)0; }

#define R(n) (core->r[n])
#define S(n) (core->s[n])

#define PSR  (*(C33PSR*)&S(0))
#define SP   (S(1))
#define AR   (*(c33lint*)&S(2))
#define ALR  (S(2))
#define AHR  (S(3))
#define D    (core->d)
#define PC   (core->pc)
#define EXT1 (core->ext[0])
#define EXT2 (core->ext[1])

#define Rs   (R(inst.rs))
#define Rd   (R(inst.rd))
#define Rb   (R(inst.rb))
#define Rsd  (R(inst.rs_rd))

#define CLK  (core->clk)

// shortcut for definition
#define C33_OP(name)      void c33op_##name(C33CORE* core, INST inst)
// shortcut for calling
#define CALL_C33_OP(name, core, inst) c33op_##name(core, inst)
#define NAME_C33_OP(name) c33op_##name

#ifdef INST
#undef INST
#endif

// C33 insns
#define INST INST_CLASS_0A
C33_OP(nop);
C33_OP(slp);
C33_OP(halt);
C33_OP(pushn_rs);
C33_OP(popn_rd);
C33_OP(brk);
C33_OP(retd);
C33_OP(int_imm2);
C33_OP(reti);
C33_OP(call_rb);
C33_OP(ret);
C33_OP(jp_rb);
#undef INST
#define INST INST_CLASS_0B
C33_OP(jrgt_sign8);
C33_OP(jrge_sign8);
C33_OP(jrlt_sign8);
C33_OP(jrle_sign8);
C33_OP(jrugt_sign8);
C33_OP(jruge_sign8);
C33_OP(jrult_sign8);
C33_OP(jrule_sign8);
C33_OP(jreq_sign8);
C33_OP(jrne_sign8);
C33_OP(call_sign8);
C33_OP(jp_sign8);
#undef INST
#define INST INST_CLASS_1A
C33_OP(ld_b_rd_RB);
C33_OP(ld_ub_rd_RB);
C33_OP(ld_h_rd_RB);
C33_OP(ld_uh_rd_RB);
C33_OP(ld_w_rd_RB);
C33_OP(ld_b_RB_rs);
C33_OP(ld_h_RB_rs);
C33_OP(ld_w_RB_rs);
C33_OP(ld_b_rd_RBx);
C33_OP(ld_ub_rd_RBx);
C33_OP(ld_h_rd_RBx);
C33_OP(ld_uh_rd_RBx);
C33_OP(ld_w_rd_RBx);
C33_OP(ld_b_RBx_rs);
C33_OP(ld_h_RBx_rs);
C33_OP(ld_w_RBx_rs);
#undef INST
#define INST INST_CLASS_1B
C33_OP(add_rd_rs);
C33_OP(sub_rd_rs);
C33_OP(cmp_rd_rs);
C33_OP(ld_w_rd_rs);
C33_OP(and_rd_rs);
C33_OP(or_rd_rs);
C33_OP(xor_rd_rs);
C33_OP(not_rd_rs);
#undef INST
#define INST INST_CLASS_2
C33_OP(ld_b_rd_SPxIMM6);
C33_OP(ld_ub_rd_SPxIMM6);
C33_OP(ld_h_rd_SPxIMM6);
C33_OP(ld_uh_rd_SPxIMM6);
C33_OP(ld_w_rd_SPxIMM6);
C33_OP(ld_b_SPxIMM6_rs);
C33_OP(ld_h_SPxIMM6_rs);
C33_OP(ld_w_SPxIMM6_rs);
#undef INST
#define INST INST_CLASS_3
C33_OP(add_rd_imm6);
C33_OP(sub_rd_imm6);
C33_OP(cmp_rd_sign6);
C33_OP(ld_w_rd_sign6);
C33_OP(and_rd_sign6);
C33_OP(or_rd_sign6);
C33_OP(xor_rd_sign6);
C33_OP(not_rd_sign6);
#undef INST
#define INST INST_CLASS_4A
C33_OP(add_sp_imm10);
C33_OP(sub_sp_imm10);
#undef INST
#define INST INST_CLASS_4B
C33_OP(srl_rd_imm4);
C33_OP(sll_rd_imm4);
C33_OP(sra_rd_imm4);
C33_OP(sla_rd_imm4);
C33_OP(rr_rd_imm4);
C33_OP(rl_rd_imm4);
C33_OP(srl_rd_rs);
C33_OP(sll_rd_rs);
C33_OP(sra_rd_rs);
C33_OP(sla_rd_rs);
C33_OP(rr_rd_rs);
C33_OP(rl_rd_rs);
#undef INST
#define INST INST_CLASS_4C
C33_OP(scan0_rd_rs);
C33_OP(scan1_rd_rs);
C33_OP(swap_rd_rs);
C33_OP(mirror_rd_rs);
C33_OP(div0s_rs);
C33_OP(div0u_rs);
C33_OP(div1_rs);
C33_OP(div2s_rs);
C33_OP(div3s);
#undef INST
#define INST INST_CLASS_5A
C33_OP(ld_w_sd_rs);
C33_OP(ld_w_rd_ss);
#undef INST
#define INST INST_CLASS_5B
C33_OP(btst_RB_imm3);
C33_OP(bclr_RB_imm3);
C33_OP(bset_RB_imm3);
C33_OP(bnot_RB_imm3);
#undef INST
#define INST INST_CLASS_5C
C33_OP(adc_rd_rs);
C33_OP(sbc_rd_rs);
C33_OP(ld_b_rd_rs);
C33_OP(ld_ub_rd_rs);
C33_OP(ld_h_rd_rs);
C33_OP(ld_uh_rd_rs);
C33_OP(mlt_h_rd_rs);
C33_OP(mltu_h_rd_rs);
C33_OP(mlt_w_rd_rs);
C33_OP(mltu_w_rd_rs);
C33_OP(mac_rs);
#undef INST
#define INST INST_CLASS_6
C33_OP(ext_imm13);
#undef INST

// operations used in insns
c33word sign_ext(c33word data, c33word bits);
void exec_delay(C33CORE* core, c33word dflag);
c33int ext_imm6(C33CORE* core, c33int imm6);
c33int ext_sign6(C33CORE* core, c33int sign6);
c33int ext_RB(C33CORE* core, c33int rb);
c33int ext_SPxIMM6(C33CORE* core, c33int imm6, c33int size);
c33int ext_3op(C33CORE* core);
c33int ext_PCxSIGN8(C33CORE* core, c33int sign8);
c33int add(C33CORE* core, c33int a, c33int b);
c33int adc(C33CORE* core, c33int a, c33int b);
c33int sub(C33CORE* core, c33int a, c33int b);
c33int sbc(C33CORE* core, c33int a, c33int b);
c33int and(C33CORE* core, c33int a, c33int b);
c33int or(C33CORE* core, c33int a, c33int b);
c33int xor(C33CORE* core, c33int a, c33int b);
c33int not(C33CORE* core, c33int a);
c33word srl(C33CORE* core, c33word a, c33word b);
c33word sll(C33CORE* core, c33word a, c33word b);
c33int sra(C33CORE* core, c33int a, c33int b);
c33int sla(C33CORE* core, c33int a, c33int b);
c33word rr(C33CORE* core, c33word a, c33word b);
c33word rl(C33CORE* core, c33word a, c33word b);

void inst_decode(C33CORE* core, C33INST inst);

#endif

// -*- mode: c++; coding: utf-8-unix; -*-

#ifndef c33core_h
#define c33core_h

#include "inst_types.h"

#define DIE(v) {}

#define R(n) (r[n])
#define S(n) (s[n])

#define PSR  (s[0].p)
#define SP   (s[1])
#define AR   (*(c33lint*)&s[2])
#define ALR  (s[2])
#define AHR  (s[3])
#define D    (d)
#define PC   (pc)
#define EXT1 (ext[0])
#define EXT2 (ext[1])

#define Rs   (r[inst.rs])
#define Rd   (r[inst.rd])
#define Rb   (r[inst.rb])
#define Rsd  (r[inst.rs_rd])

#define CLK  (clk)

#ifdef INST
#undef INST
#endif

class c33core
{
private:
  union c33psr
  {
    c33word w;
    struct
    {
      c33word n  : 1;
      c33word z  : 1;
      c33word v  : 1;
      c33word c  : 1;
      c33word ie : 1;
    c33word    : 1;
      c33word ds : 1;
      c33word mo : 1;
      c33word il : 4;
    c33word    :20;
    };
  };

  union c33reg
  {
    c33int  i;
    c33word w;
    c33psr  p;
  } r[16], s[4], pc;

  union c33lreg
  {
    c33lint  i;
    c33lword w;
  };
  

	C33INST ext[2];

	int     d;             // delayed
	uint32_t clk;

// shortcut for definition
#define C33_OP(name)      void c33op_##name(INST inst)
// shortcut for calling
#define CALL_C33_OP(name, inst) c33op_##name(inst)
#define NAME_C33_OP(name) c33op_##name

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
#undef C33_OP

  void exec_delay(c33word dflag);

  c33word sign_ext(c33word data, c33word bits);

  c33int ext_imm6(c33int imm6);
  c33int ext_sign6(c33int sign6);
  c33int ext_RB(c33int rb);
  c33int ext_SPxIMM6(c33int imm6, c33int size);
  c33int ext_3op();
  c33int ext_PCxSIGN8(c33int sign8);

  c33int c33_add(c33int a, c33int b);
  c33int c33_adc(c33int a, c33int b);
  c33int c33_sub(c33int a, c33int b);
  c33int c33_sbc(c33int a, c33int b);
  c33int c33_and(c33int a, c33int b);
  c33int c33_or(c33int a, c33int b);
  c33int c33_xor(c33int a, c33int b);
  c33int c33_not(c33int a);
  c33word c33_srl(c33word a, c33word b);
  c33word c33_sll(c33word a, c33word b);
  c33int c33_sra(c33int a, c33int b);
  c33int c33_sla(c33int a, c33int b);
  c33word c33_rr(c33word a, c33word b);
  c33word c33_rl(c33word a, c33word b);

public:

  void core_decode(C33INST inst);
  void core_trap(c33word no, c33word level);

  c33word mem_read(c33word offset, c33word size);
  void mem_write(c33word offset, c33word size, c33word value);

};

#endif // !c33core_h

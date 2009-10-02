
#include "c33core_impl.h"


#if 1
// endianess-independent decoder
#define MASK(op, shr, bits) ((inst.s >> (16 - (shr + bits))) & ((1 << bits) - 1))
#else
// endianess-dependent
#define MASK(op, shr, bits) (op)
#endif

void c33core::core_decode(C33INST inst)
{
	switch(MASK(inst.c0a.cls, 0, 3))
	{
		case 0:
			switch(MASK(inst.c0a.op1, 3, 4))
			{
				/* CLASS 0A */
				case  0:
					switch(MASK(inst.c0a.op2, 8, 2))
					{
						case 0: CALL_C33_OP(nop, inst.c0a); return;
						case 1: CALL_C33_OP(slp, inst.c0a); return;
						case 2: CALL_C33_OP(halt, inst.c0a); return;
						case 3: return;
					}
					break;
				case  1:
					switch(MASK(inst.c0a.op2, 8, 2))
					{
						case 0: CALL_C33_OP(pushn_rs, inst.c0a); return;
						case 1: CALL_C33_OP(popn_rd, inst.c0a); return;
						case 2: return;
						case 3: return;
					}
					break;
				case  2:
					switch(MASK(inst.c0a.op2, 8, 2))
					{
						case 0: CALL_C33_OP(brk, inst.c0a); return;
						case 1: CALL_C33_OP(retd, inst.c0a); return;
						case 2: CALL_C33_OP(int_imm2, inst.c0a); return;
						case 3: CALL_C33_OP(reti, inst.c0a); return;
					}
					break;
				case  3:
					switch(MASK(inst.c0a.op2, 8, 2))
					{
						case 0: CALL_C33_OP(call_rb, inst.c0a); return;
						case 1: CALL_C33_OP(ret, inst.c0a); return;
						case 2: CALL_C33_OP(jp_rb, inst.c0a); return;
						case 3: return;
					}
					break;
					/* CLASS 0B */
				case  4: CALL_C33_OP(jrgt_sign8, inst.c0b); return;
				case  5: CALL_C33_OP(jrge_sign8, inst.c0b); return;
				case  6: CALL_C33_OP(jrlt_sign8, inst.c0b); return;
				case  7: CALL_C33_OP(jrle_sign8, inst.c0b); return;
				case  8: CALL_C33_OP(jrugt_sign8, inst.c0b); return;
				case  9: CALL_C33_OP(jruge_sign8, inst.c0b); return;
				case 10: CALL_C33_OP(jrult_sign8, inst.c0b); return;
				case 11: CALL_C33_OP(jrule_sign8, inst.c0b); return;
				case 12: CALL_C33_OP(jreq_sign8, inst.c0b); return;
				case 13: CALL_C33_OP(jrne_sign8, inst.c0b); return;
				case 14: CALL_C33_OP(call_sign8, inst.c0b); return;
				case 15: CALL_C33_OP(jp_sign8, inst.c0b); return;
			}
			break;
		case 1:
			switch(MASK(inst.c1a.op2, 6, 2))
			{
				/* CLASS 1A */
				case  0:
					switch(MASK(inst.c1a.op1, 3, 3))
					{
						case 0: CALL_C33_OP(ld_b_rd_RB, inst.c1a); return;
						case 1: CALL_C33_OP(ld_ub_rd_RB, inst.c1a); return;
						case 2: CALL_C33_OP(ld_h_rd_RB, inst.c1a); return;
						case 3: CALL_C33_OP(ld_uh_rd_RB, inst.c1a); return;
						case 4: CALL_C33_OP(ld_w_rd_RB, inst.c1a); return;
						case 5: CALL_C33_OP(ld_b_RB_rs, inst.c1a); return;
						case 6: CALL_C33_OP(ld_h_RB_rs, inst.c1a); return;
						case 7: CALL_C33_OP(ld_w_RB_rs, inst.c1a); return;
					}
					break;
				case  1:
					switch(MASK(inst.c1a.op1, 3, 3))
					{
						case 0: CALL_C33_OP(ld_b_rd_RBx, inst.c1a); return;
						case 1: CALL_C33_OP(ld_ub_rd_RBx, inst.c1a); return;
						case 2: CALL_C33_OP(ld_h_rd_RBx, inst.c1a); return;
						case 3: CALL_C33_OP(ld_uh_rd_RBx, inst.c1a); return;
						case 4: CALL_C33_OP(ld_w_rd_RBx, inst.c1a); return;
						case 5: CALL_C33_OP(ld_b_RBx_rs, inst.c1a); return;
						case 6: CALL_C33_OP(ld_h_RBx_rs, inst.c1a); return;
						case 7: CALL_C33_OP(ld_w_RBx_rs, inst.c1a); return;
					}
					break;
					/* CLASS 1B */
				case  2:
					switch(MASK(inst.c1a.op1, 3, 3))
					{
						case 0: CALL_C33_OP(add_rd_rs, inst.c1b); return;
						case 1: CALL_C33_OP(sub_rd_rs, inst.c1b); return;
						case 2: CALL_C33_OP(cmp_rd_rs, inst.c1b); return;
						case 3: CALL_C33_OP(ld_w_rd_rs, inst.c1b); return;
						case 4: CALL_C33_OP(and_rd_rs, inst.c1b); return;
						case 5: CALL_C33_OP(or_rd_rs, inst.c1b); return;
						case 6: CALL_C33_OP(xor_rd_rs, inst.c1b); return;
						case 7: CALL_C33_OP(not_rd_rs, inst.c1b); return;
					}
					break;
			}
			break;
		case 2:
			switch(MASK(inst.c2.op1, 3, 3))
			{
				/* CLASS 2 */
				case 0: CALL_C33_OP(ld_b_rd_SPxIMM6, inst.c2); return;
				case 1: CALL_C33_OP(ld_ub_rd_SPxIMM6, inst.c2); return;
				case 2: CALL_C33_OP(ld_h_rd_SPxIMM6, inst.c2); return;
				case 3: CALL_C33_OP(ld_uh_rd_SPxIMM6, inst.c2); return;
				case 4: CALL_C33_OP(ld_w_rd_SPxIMM6, inst.c2); return;
				case 5: CALL_C33_OP(ld_b_SPxIMM6_rs, inst.c2); return;
				case 6: CALL_C33_OP(ld_h_SPxIMM6_rs, inst.c2); return;
				case 7: CALL_C33_OP(ld_w_SPxIMM6_rs, inst.c2); return;
			}
			break;
		case 3:
			switch(MASK(inst.c3.op1, 3, 3))
			{
				/* CLASS 3 */
				case 0: CALL_C33_OP(add_rd_imm6, inst.c3); return;
				case 1: CALL_C33_OP(sub_rd_imm6, inst.c3); return;
				case 2: CALL_C33_OP(cmp_rd_sign6, inst.c3); return;
				case 3: CALL_C33_OP(ld_w_rd_sign6, inst.c3); return;
				case 4: CALL_C33_OP(and_rd_sign6, inst.c3); return;
				case 5: CALL_C33_OP(or_rd_sign6, inst.c3); return;
				case 6: CALL_C33_OP(xor_rd_sign6, inst.c3); return;
				case 7: CALL_C33_OP(not_rd_sign6, inst.c3); return;
			}
			break;
		case 4:
			switch(MASK(inst.c4a.op1, 3, 3))
			{
				/* CLASS 4A */
				case 0: CALL_C33_OP(add_sp_imm10, inst.c4a); return;
				case 1: CALL_C33_OP(sub_sp_imm10, inst.c4a); return;
				default:
					switch(MASK(inst.c4b.op2, 6, 2))
					{
						/* CLASS 4B */
						case 0:
							switch(MASK(inst.c4b.op1, 3, 3))
							{
								case 0: return;
								case 1: return;
								case 2: CALL_C33_OP(srl_rd_imm4, inst.c4b); return;
								case 3: CALL_C33_OP(sll_rd_imm4, inst.c4b); return;
								case 4: CALL_C33_OP(sra_rd_imm4, inst.c4b); return;
								case 5: CALL_C33_OP(sla_rd_imm4, inst.c4b); return;
								case 6: CALL_C33_OP(rr_rd_imm4, inst.c4b); return;
								case 7: CALL_C33_OP(rl_rd_imm4, inst.c4b); return;
							}
							break;
						case 1:
							switch(MASK(inst.c4b.op1, 3, 3))
							{
								case 0: return;
								case 1: return;
								case 2: CALL_C33_OP(srl_rd_rs, inst.c4b); return;
								case 3: CALL_C33_OP(sll_rd_rs, inst.c4b); return;
								case 4: CALL_C33_OP(sra_rd_rs, inst.c4b); return;
								case 5: CALL_C33_OP(sla_rd_rs, inst.c4b); return;
								case 6: CALL_C33_OP(rr_rd_rs, inst.c4b); return;
								case 7: CALL_C33_OP(rl_rd_rs, inst.c4b); return;
							}
							break;
							/* CLASS 4C */
						case 2:
							switch(MASK(inst.c4c.op1, 3, 3))
							{
								case 0: return;
								case 1: return;
								case 2: CALL_C33_OP(scan0_rd_rs, inst.c4c); return;
								case 3: CALL_C33_OP(scan1_rd_rs, inst.c4c); return;
								case 4: CALL_C33_OP(swap_rd_rs, inst.c4c); return;
								case 5: CALL_C33_OP(mirror_rd_rs, inst.c4c); return;
							}
							break;
						case 3:
							switch(MASK(inst.c4c.op1, 3, 3))
							{
								case 0: return;
								case 1: return;
								case 2: CALL_C33_OP(div0s_rs, inst.c4c); return;
								case 3: CALL_C33_OP(div0u_rs, inst.c4c); return;
								case 4: CALL_C33_OP(div1_rs, inst.c4c); return;
								case 5: CALL_C33_OP(div2s_rs, inst.c4c); return;
								case 6: CALL_C33_OP(div3s, inst.c4c); return;
							}
							break;
					}
					break;
			}
			break;
		case 5:
			switch(MASK(inst.c5a.op2, 6, 2))
			{
				case 0:
					switch(MASK(inst.c5a.op1, 3, 3))
					{
						/* CLASS 5A */
						case 0: CALL_C33_OP(ld_w_sd_rs, inst.c5a); return;
						case 1: CALL_C33_OP(ld_w_rd_ss, inst.c5a); return;
							/* CLASS 5B */
						case 2: CALL_C33_OP(btst_RB_imm3, inst.c5b); return;
						case 3: CALL_C33_OP(bclr_RB_imm3, inst.c5b); return;
						case 4: CALL_C33_OP(bset_RB_imm3, inst.c5b); return;
						case 5: CALL_C33_OP(bnot_RB_imm3, inst.c5b); return;
							/* CLASS 5C */
						case 6: CALL_C33_OP(adc_rd_rs, inst.c5c); return;
						case 7: CALL_C33_OP(sbc_rd_rs, inst.c5c); return;
					}
					break;
				case 1:
					switch(MASK(inst.c5c.op1, 3, 3))
					{
						case 0: CALL_C33_OP(ld_b_rd_rs, inst.c5c); return;
						case 1: CALL_C33_OP(ld_ub_rd_rs, inst.c5c); return;
						case 2: CALL_C33_OP(ld_h_rd_rs, inst.c5c); return;
						case 3: CALL_C33_OP(ld_uh_rd_rs, inst.c5c); return;
						case 4: return;
						case 5: return;
						case 6: return;
						case 7: return;
					}
					break;
				case 2:
					switch(MASK(inst.c5c.op1, 3, 3))
					{
						case 0: CALL_C33_OP(mlt_h_rd_rs, inst.c5c); return;
						case 1: CALL_C33_OP(mltu_h_rd_rs, inst.c5c); return;
						case 2: CALL_C33_OP(mlt_w_rd_rs, inst.c5c); return;
						case 3: CALL_C33_OP(mltu_w_rd_rs, inst.c5c); return;
						case 4: CALL_C33_OP(mac_rs, inst.c5c); return;
						case 5: return;
						case 6: return;
						case 7: return;
					}
					break;
			}
			break;
		case 6:
			/* CLASS 6 */
			CALL_C33_OP(ext_imm13, inst.c6); return;
			break;
		case 7:
			return;
		case 8:
			return;
	}
}

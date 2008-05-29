
#ifndef INST_TYPES_H
#define INST_TYPES_H

#include "types.h"

/************************************************
 * derived from p/emu, by Naoyuki Sawa
 ***********************************************/

//////////////////// CLASS 0 ////////////////////
/*  op1 op2 mnemonic
 *  0000  00  nop
 *  0000  01  slp
 *  0000  10  halt
 *  0000  11  ---
 *  0001  00  pushn %rs
 *  0001  01  popn %rd
 *  0001  10  ---
 *  0001  11  ---
 *  0010  00  brk
 *  0010  01  retd
 *  0010  10  int imm2
 *  0010  11  reti
 *  0011  00  call %rb
 *  0011  01  ret
 *  0011  10  jp %rb
 *  0011  11  ---
 */
typedef struct _CLASS_0A
{
  c33hword imm2_rd_rs : 4;
  c33hword            : 2;
  c33hword op2        : 2;
  c33hword d          : 1;
  c33hword op1        : 4;
  c33hword cls        : 3;
} INST_CLASS_0A;

/*  op1   mnemonic
 *  0100    jrgt sign8
 *  0101    jrge sign8
 *  0110    jrlt sign8
 *  0111    jrle sign8
 *  1000    jrugt sign8
 *  1001    jruge sign8
 *  1010    jrult sign8
 *  1011    jrule sign8
 *  1100    jreq sign8
 *  1101    jrne sign8
 *  1110    call sign8
 *  1111    jp sign8
 */
typedef struct _CLASS_0B
{
  c33hword sign8 : 8;
  c33hword d     : 1;
  c33hword op1   : 4;
  c33hword cls   : 3;
} INST_CLASS_0B;

//////////////////// CLASS 1 ////////////////////
/*  op1 op2 mnemonic
 *  000 00  ld.b %rd,[%rb]
 *  001 00  ld.ub %rd,[%rb]
 *  010 00  ld.h %rd,[%rb]
 *  011 00  ld.uh %rd,[%rb]
 *  100 00  ld.w %rd,[%rb]
 *  101 00  ld.b [%rb],%rs
 *  110 00  ld.h [%rb],%rs
 *  111 00  ld.w [%rb],%rs
 *  000 01  ld.b %rd,[%rb]+
 *  001 01  ld.ub %rd,[%rb]+
 *  010 01  ld.h %rd,[%rb]+
 *  011 01  ld.uh %rd,[%rb]+
 *  100 01  ld.w %rd,[%rb]+
 *  101 01  ld.b [%rb]+,%rs
 *  110 01  ld.h [%rb]+,%rs
 *  111 01  ld.w [%rb]+,%rs
 */
typedef struct _CLASS_1A
{
  c33hword rs_rd : 4;
  c33hword rb    : 4;
  c33hword op2   : 2;
  c33hword op1   : 3;
  c33hword cls   : 3;
} INST_CLASS_1A;

/*  op1 op2 mnemonic
 *  000 10  add %rd,%rs
 *  001 10  sub %rd,%rs
 *  010 10  cmp %rd,%rs
 *  011 10  ld.w %rd,%rs
 *  100 10  and %rd,%rs
 *  101 10  or %rd,%rs
 *  110 10  xor %rd,%rs
 *  111 10  not %rd,%rs
 *  000 11  ---
 *  001 11  ---
 *  010 11  ---
 *  011 11  ---
 *  100 11  ---
 *  101 11  ---
 *  110 11  ---
 *  111 11  ---
 */
typedef struct _CLASS_1B
{
  c33hword rd  : 4;
  c33hword rs  : 4;
  c33hword op2 : 2;
  c33hword op1 : 3;
  c33hword cls : 3;
} INST_CLASS_1B;

//////////////////// CLASS 2 ////////////////////
/*  op1   mnemonic
 *  000   ld.b %rd,[%sp+imm6]
 *  001   ld.ub %rd,[%sp+imm6]
 *  010   ld.h %rd,[%sp+imm6]
 *  011   ld.uh %rd,[%sp+imm6]
 *  100   ld.w %rd,[%sp+imm6]
 *  101   ld.b [%sp+imm6],%rs
 *  110   ld.h [%sp+imm6],%rs
 *  111   ld.w [%sp+imm6],%rs
 */
typedef struct _CLASS_2
{
  c33hword rs_rd : 4;
  c33hword imm6  : 6;
  c33hword op1   : 3;
  c33hword cls   : 3;
} INST_CLASS_2;

//////////////////// CLASS 3 ////////////////////
/*  op1   mnemonic
 *  000   add %rd,imm6
 *  001   sub %rd,imm6
 *  010   cmp %rd,sign6
 *  011   ld.w %rd,sign6
 *  100   and %rd,sign6
 *  101   or %rd,sign6
 *  110   xor %rd,sign6
 *  111   not %rd,sign6
 */
typedef struct _CLASS_3
{
  c33hword rd         : 4;
  c33hword imm6_sign6 : 6;
  c33hword op1        : 3;
  c33hword cls        : 3;
} INST_CLASS_3;

//////////////////// CLASS 4 ////////////////////
/*  op1   mnemonic
 *  000   add %sp,imm10
 *  001   sub %sp,imm10
 */
typedef struct _CLASS_4A
{
  c33hword imm10 :10;
  c33hword op1   : 3;
  c33hword cls   : 3;
} INST_CLASS_4A;

/*  op1 op2 mnemonic
 *  000 00  --- (CLASS 4A)
 *  001 00  --- (CLASS 4A)
 *  010 00  srl %rd,imm4
 *  011 00  sll %rd,imm4
 *  100 00  sra %rd,imm4
 *  101 00  sla %rd,imm4
 *  110 00  rr %rd,imm4
 *  111 00  rl %rd,imm4
 *  000 01  --- (CLASS 4A)
 *  001 01  --- (CLASS 4A)
 *  010 01  srl %rd,%rs
 *  011 01  sll %rd,%rs
 *  100 01  sra %rd,%rs
 *  101 01  sla %rd,%rs
 *  110 01  rr %rd,%rs
 *  111 01  rl %rd,%rs
 */
typedef struct _CLASS_4B
{
  c33hword rd      : 4;
  c33hword imm4_rs : 4;
  c33hword op2     : 2;
  c33hword op1     : 3;
  c33hword cls     : 3;
} INST_CLASS_4B;

/*  op1 op2 mnemonic
 *  000 10  --- (CLASS 4A)
 *  001 10  --- (CLASS 4A)
 *  010 10  scan0 %rd,%rs
 *  011 10  scan1 %rd,%rs
 *  100 10  swap %rd,%rs
 *  101 10  mirror %rd,%rs
 *  110 10  ---
 *  111 10  ---
 *  000 11  --- (CLASS 4A)
 *  001 11  --- (CLASS 4A)
 *  010 11  div0s %rs
 *  011 11  div0u %rs
 *  100 11  div1 %rs
 *  101 11  div2s %rs
 *  110 11  div3s
 *  111 11  ---
 */
typedef struct _CLASS_4C
{
  c33hword rd  : 4;
  c33hword rs  : 4;
  c33hword op2 : 2;
  c33hword op1 : 3;
  c33hword cls : 3;
} INST_CLASS_4C;

//////////////////// CLASS 5 ////////////////////
/*  op1 op2 mnemonic
 *  000 00  ld.w %sd,%rs
 *  001 00  ld.w %rd,%ss
 */
typedef struct _CLASS_5A
{
  c33hword sd_rd : 4;
  c33hword rs_ss : 4;
  c33hword op2   : 2;
  c33hword op1   : 3;
  c33hword cls   : 3;
} INST_CLASS_5A;

/*  op1 op2 mnemonic
 *  010 00  btst [%rb],imm3
 *  011 00  bclr [%rb],imm3
 *  100 00  bset [%rb],imm3
 *  101 00  bnot [%rb],imm3
 */
typedef struct _CLASS_5B
{
  c33hword imm3 : 3;
  c33hword      : 1;
  c33hword rb   : 4;
  c33hword op2  : 2;
  c33hword op1  : 3;
  c33hword cls  : 3;
} INST_CLASS_5B;

/*  op1 op2 mnemonic
 *  110 00  adc %rd,%rs
 *  111 00  sbc %rd,%rs
 *  000 01  ld.b %rd,%rs
 *  001 01  ld.ub %rd,%rs
 *  010 01  ld.h %rd,%rs
 *  011 01  ld.uh %rd,%rs
 *  100 01  ---
 *  101 01  ---
 *  110 01  ---
 *  111 01  ---
 *  000 10  mlt.h %rd,%rs
 *  001 10  mltu.h %rd,%rs
 *  010 10  mlt.w %rd,%rs
 *  011 10  mltu.w %rd,%rs
 *  100 10  mac %rs
 *  101 10  ---
 *  110 10  ---
 *  111 10  ---
 *  000 11  ---
 *  001 11  ---
 *  010 11  ---
 *  011 11  ---
 *  100 11  ---
 *  101 11  ---
 *  110 11  ---
 *  111 11  ---
 */
typedef struct _CLASS_5C
{
  c33hword rd  : 4;
  c33hword rs  : 4;
  c33hword op2 : 2;
  c33hword op1 : 3;
  c33hword cls : 3;
} INST_CLASS_5C;

//////////////////// CLASS 6 ////////////////////
/*      mnemonic
 *      ext imm13
 */
typedef struct _CLASS_6
{
  c33hword imm13 :13;
  c33hword cls   : 3;
} INST_CLASS_6;

//////////////////// CLASS 7 ////////////////////
/*      mnemonic
 *      ---
 */
typedef struct _CLASS_7
{
  c33hword     :13;
  c33hword cls : 3;
} INST_CLASS_7;

typedef union c33inst
{
  c33hword       s  ;
  INST_CLASS_0A  c0a;
  INST_CLASS_0B  c0b;
  INST_CLASS_1A  c1a;
  INST_CLASS_1B  c1b;
  INST_CLASS_2   c2 ;
  INST_CLASS_3   c3;
  INST_CLASS_4A  c4a;
  INST_CLASS_4B  c4b;
  INST_CLASS_4C  c4c;
  INST_CLASS_5A  c5a;
  INST_CLASS_5B  c5b;
  INST_CLASS_5C  c5c;
  INST_CLASS_6   c6 ;
  INST_CLASS_7   c7 ;
}C33INST;

#endif

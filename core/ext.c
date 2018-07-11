#include "classes.h"

/****************************************************************************
 *  即値拡張
 ****************************************************************************/

int ext_imm6(PIEMU_CONTEXT *context, int imm6)
{
    int data;
    imm6 &= (1 << 6) - 1;
    if (EXT2.s) {
        data = imm6 | (EXT2.c6.imm13 << 6) | (EXT1.c6.imm13 << 19);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else if (EXT1.s) {
        data = imm6 | (EXT1.c6.imm13 << 6);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else {
        data = imm6;
    }
    return data;
}

int ext_sign6(PIEMU_CONTEXT *context, int sign6)
{
    int data, bits;
    sign6 &= (1 << 6) - 1;
    if (EXT2.s) {
        data = sign6 | (EXT2.c6.imm13 << 6) | (EXT1.c6.imm13 << 19);
        bits = 6 + 13 + 13;
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else if (EXT1.s) {
        data = sign6 | (EXT1.c6.imm13 << 6);
        bits = 6 + 13;
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else {
        data = sign6;
        bits = 6;
    }
    return sign_ext(data, bits);
}

int ext_RB(PIEMU_CONTEXT *context, int rb)
{
    int disp;
    if (EXT2.s) {
        disp = (EXT2.c6.imm13 << 0) | (EXT1.c6.imm13 << 13);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else if (EXT1.s) {
        disp = (EXT1.c6.imm13 << 0);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else {
        disp = 0;
    }
    return R(rb) + disp;
}

int ext_SPxIMM6(PIEMU_CONTEXT *context, int imm6, int size)
{
    int disp;
    imm6 &= (1 << 6) - 1;
    if (EXT2.s) {
        disp = imm6 | (EXT2.c6.imm13 << 6) | (EXT1.c6.imm13 << 19);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else if (EXT1.s) {
        disp = imm6 | (EXT1.c6.imm13 << 6);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else {
        disp = imm6 * size;
/*
    switch(size) {
    case 1: disp = imm6 * 1; break;
    case 2: disp = imm6 * 2; break;
    case 4: disp = imm6 * 4; break;
    default: DIE();
    }
*/
    }
    return SP + disp;
}

int ext_3op(PIEMU_CONTEXT *context)
{
    /* NOTE1: 少なくともEXT1が存在することを呼び出し側で確認してください。 */
    /* NOTE2: cmp/and/or/xor/notも、3op拡張時はsignではなくimmとなります。 */
    int data;
    if (EXT2.s) {
        data = (EXT2.c6.imm13 << 0) | (EXT1.c6.imm13 << 13);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else if (EXT1.s) {
        data = (EXT1.c6.imm13 << 0);
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else {
        DIE();
    }
    return data;
}

int ext_PCxSIGN8(PIEMU_CONTEXT *context, int sign8)
{
    int disp, bits;
    sign8 &= (1 << 8) - 1;
    if (EXT2.s) {
        disp = sign8 | (EXT2.c6.imm13 << 8) | (EXT1.c6.imm13 >> 3 << 21);
        bits = 8 + 13 + 10;
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else if (EXT1.s) {
        disp = sign8 | (EXT1.c6.imm13 << 8);
        bits = 8 + 13;
        context->core.ext[0].s = context->core.ext[1].s = 0;
    } else {
        disp = sign8;
        bits = 8;
    }
    return PC + sign_ext(disp, bits) * 2;
}

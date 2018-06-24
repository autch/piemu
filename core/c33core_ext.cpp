#include <stdexcept>
#include "c33core_impl.h"

/****************************************************************************
 *  即値拡張
 ****************************************************************************/

int C33Core::ext_imm6(int imm6)
{
    int data;
    imm6 &= (1 << 6) - 1;
    if (EXT2.s) {
        data = imm6 | (EXT2.c6.imm13 << 6) | (EXT1.c6.imm13 << 19);
        this->ext[0].s = this->ext[1].s = 0;
    } else if (EXT1.s) {
        data = imm6 | (EXT1.c6.imm13 << 6);
        this->ext[0].s = this->ext[1].s = 0;
    } else {
        data = imm6;
    }
    return data;
}

int C33Core::ext_sign6(int sign6)
{
    int data, bits;
    sign6 &= (1 << 6) - 1;
    if (EXT2.s) {
        data = sign6 | (EXT2.c6.imm13 << 6) | (EXT1.c6.imm13 << 19);
        bits = 6 + 13 + 13;
        this->ext[0].s = this->ext[1].s = 0;
    } else if (EXT1.s) {
        data = sign6 | (EXT1.c6.imm13 << 6);
        bits = 6 + 13;
        this->ext[0].s = this->ext[1].s = 0;
    } else {
        data = sign6;
        bits = 6;
    }
    return sign_ext(data, bits);
}

int C33Core::ext_RB(int rb)
{
    int disp;
    if (EXT2.s) {
        disp = (EXT2.c6.imm13 << 0) | (EXT1.c6.imm13 << 13);
        this->ext[0].s = this->ext[1].s = 0;
    } else if (EXT1.s) {
        disp = (EXT1.c6.imm13 << 0);
        this->ext[0].s = this->ext[1].s = 0;
    } else {
        disp = 0;
    }
    return R(rb) + disp;
}

int C33Core::ext_SPxIMM6(int imm6, int size)
{
    int disp;
    imm6 &= (1 << 6) - 1;
    if (EXT2.s) {
        disp = imm6 | (EXT2.c6.imm13 << 6) | (EXT1.c6.imm13 << 19);
        this->ext[0].s = this->ext[1].s = 0;
    } else if (EXT1.s) {
        disp = imm6 | (EXT1.c6.imm13 << 6);
        this->ext[0].s = this->ext[1].s = 0;
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

int C33Core::ext_3op()
{
    /* NOTE1: 少なくともEXT1が存在することを呼び出し側で確認してください。 */
    /* NOTE2: cmp/and/or/xor/notも、3op拡張時はsignではなくimmとなります。 */
    int data;
    if (EXT2.s) {
        data = (EXT2.c6.imm13 << 0) | (EXT1.c6.imm13 << 13);
        this->ext[0].s = this->ext[1].s = 0;
    } else if (EXT1.s) {
        data = (EXT1.c6.imm13 << 0);
        this->ext[0].s = this->ext[1].s = 0;
    } else {
        throw std::logic_error("invalid ext_3op state");
    }
    return data;
}

int C33Core::ext_PCxSIGN8(int sign8)
{
    int disp, bits;
    sign8 &= (1 << 8) - 1;
    if (EXT2.s) {
        disp = sign8 | (EXT2.c6.imm13 << 8) | (EXT1.c6.imm13 >> 3 << 21);
        bits = 8 + 13 + 10;
        this->ext[0].s = this->ext[1].s = 0;
    } else if (EXT1.s) {
        disp = sign8 | (EXT1.c6.imm13 << 8);
        bits = 8 + 13;
        this->ext[0].s = this->ext[1].s = 0;
    } else {
        disp = sign8;
        bits = 8;
    }
    return PC + sign_ext(disp, bits) * 2;
}

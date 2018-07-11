#include "classes.h"

/****************************************************************************
 *  CLASS 6
 ****************************************************************************/

void exec_ext_imm13(PIEMU_CONTEXT *context, INST inst)
{
    NO_DELAY;
    INST inst2;

    if (!EXT1.s) {
        EXT1.s = inst.s;
    } else if (!EXT2.s) {
        EXT2.s = inst.s;
    } else {
        DIE();
    }
    PC += 2;
    CLK += 1;

    /* 拡張される命令を実行。（この間の割り込みを禁止するため） */
    inst2.s = mem_readH(context, PC);
    core_inst(context, inst2);
    if (EXT1.s) DIE(); /* 確実にextが消費されていること */
}

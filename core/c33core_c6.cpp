#include <stdexcept>
#include "c33core_impl.h"

/****************************************************************************
 *  CLASS 6
 ****************************************************************************/

void C33Core::exec_ext_imm13(CLASS_6 inst)
{
    NO_DELAY
    INST inst2;

    if (!EXT1.s) {
        EXT1.c6 = inst;
    } else if (!EXT2.s) {
        EXT2.c6 = inst;
    } else {
        throw std::logic_error("invalid ext state");
    }
    PC += 2;
    CLK += 1;

    /* 拡張される命令を実行。（この間の割り込みを禁止するため） */
    inst2.s = mem_readH(PC);
    core_inst(inst2);
    if (EXT1.s) throw std::logic_error("ext state not consumed"); /* 確実にextが消費されていること */
}

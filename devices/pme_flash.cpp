#include "pme_flash.h"

#define FLASH_TOP	0x0c00000
#define READ_UNIT	0x1000
#define SECTOR_SIZE	0x01000		/* セクタ  消去単位= 4KB */
#define BLOCK_SIZE	0x10000		/* ブロック消去単位=64KB */

void PMEFlash::init(void* context)
{
    int size, bits;

    /* CFIクエリー情報作成。(とりあえず必要なフィールドだけ) */
    size = (int)context->emu.sysinfo.pffs_end - FLASH_TOP;
    bits = 0;
    for(;;) {
        if(1 << bits >= size) break;
        bits++;
    }
    cfiinfo.device_size = bits;

    /* メモリ割り当て。 */
    mem_size = 1 << cfiinfo.device_size;
    mem.resize(mem_size);

    /* イメージ読み込み。 */
    context->pfnLoadFlashImage(context, &context->flash, context->pUser);
}

void PMEFlash::bus_writeH(c33word ofs, c33hword data)
{
    int cmd;

    ofs &= mem_size - 1;
    cmd = data & 0xff;

    /* WORD PROGRAM以外での0xf0書き込みは、汎用EXITコマンドと見なします。 */
    if(stat != FLASH_WORD_PROGRAM && cmd == 0xf0) {
        stat = FLASH_NORMAL; /* MANUAL EXIT */
        return;
    }

    switch(stat) {
        case FLASH_NORMAL:
            if(ofs == 0x5555 * 2 && cmd == 0xaa) {
                stat = FLASH_ENTER1;
            } else {
                DIE();
            }
            break;
        case FLASH_ENTER1:
            if(ofs == 0x2aaa * 2 && cmd == 0x55) {
                stat = FLASH_ENTER2;
            } else {
                DIE();
            }
            break;
        case FLASH_ENTER2:
            if(ofs == 0x5555 * 2) {
                switch(cmd) {
                    case 0x80:
                        stat = FLASH_ENTER3;
                        break;
                    case 0x90: /* SOFTWARE ID と                     */
                    case 0x98: /* CFI QUERY   は、同じものとして扱う */
                        stat = FLASH_CFI_QUERY;
                        break;
                    case 0xa0:
                        stat = FLASH_WORD_PROGRAM;
                        break;
                    default:
                        DIE();
                }
            } else {
                DIE();
            }
            break;
        case FLASH_ENTER3:
            if(ofs == 0x5555 * 2 && cmd == 0xaa) {
                stat = FLASH_ENTER4;
            } else {
                DIE();
            }
            break;
        case FLASH_ENTER4:
            if(ofs == 0x2aaa * 2 && cmd == 0x55) {
                stat = FLASH_ENTER5;
            } else {
                DIE();
            }
            break;
        case FLASH_ENTER5:
            switch(cmd) {
                case 0x10: /* CHIP ERASE */
                    if(ofs == 0x5555 * 2) {
                        // dbg("CHIP ERASE");
                        std::fill(mem.begin(), mem.end(), (uint8_t)-1);
                        stat = FLASH_NORMAL; /* AUTO EXIT */
                    } else {
                        DIE();
                    }
                    break;
                case 0x30: /* SECTOR ERASE */
                    ofs &= ~(SECTOR_SIZE - 1);
                    // dbg("SECTOR ERASE: %07x", ofs);
                    std::fill_n(mem.begin() + ofs, SECTOR_SIZE, (uint8_t)-1);
                    stat = FLASH_NORMAL; /* AUTO EXIT */
                    break;
                case 0x50: /* BLOCK ERASE */
                    ofs &= ~(BLOCK_SIZE - 1);
                    // dbg("BLOCK ERASE: %07x", ofs);
                    std::fill_n(mem.begin() + ofs, BLOCK_SIZE, (uint8_t)-1);
                    stat = FLASH_NORMAL; /* AUTO EXIT */
                    break;
                default:
                    DIE();
            }
            break;
        case FLASH_CFI_QUERY:
            if((ofs == 0x5555 * 2 && cmd == 0xaa) ||
               (ofs == 0x2aaa * 2 && cmd == 0x55)) {
                /* 長い方のEXITシーケンス三命令(aa-55-f0)のうち、先行する二命令を無視します。
                 * 最後の一命令は汎用EXITコマンドとして、この関数の最初でまとめて処理しています。
                 */
            } else {
                DIE();
            }
            break;
        case FLASH_WORD_PROGRAM:
            // dbg("WORD PROGRAM: %07x=%04x", ofs, data & 0xffff);
            piemu::mem_util::mem_write(mem, ofs, data);
            stat = FLASH_NORMAL; /* AUTO EXIT */
            break;
        default:
            DIE();
    }
}

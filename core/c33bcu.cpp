#include "c33bcu.h"
#include "c33209e.h"

C33BCU::AreaDefinition C33BCU::area_sel(c33word addr, size_t size, RWMode mode /*0:内部RDWR/1:外部RD/2:外部WR*/)
{
    int no, sz, wt;
    int clk;

    // 警告対策
    no = sz = wt = 0;

    /* 整列チェック。 */
    switch(size) {
        case 1:                     break;
        case 2: if(addr & 1) throw std::invalid_argument("bcu: odd-aligned access"); break;
        case 4: if(addr & 3) throw std::invalid_argument("bcu: odd-aligned access"); break;
        default: throw std::invalid_argument("bcu: bad access width");
    }

    /* エリア選択。 */
    addr &= (1 << 28) - 1; /* S1C33のアドレス空間は28ビット */

    // これはうまくジャンプテーブルになるからこれでいい
    switch(addr >> 24)
    {
        case 0x0:
            switch(addr >> 20)
            {
                case 0x00:
                {
                    switch(addr >> 16)
                    {
                        case 0x000:
                        case 0x001:
                        case 0x002: no = 0;               goto NO_WAIT;
                        case 0x003:
                        case 0x004:
                        case 0x005: no = 1;               goto NO_WAIT;
                        case 0x006:
                        case 0x007: no = 2;               goto NO_WAIT;
                        case 0x008:
                        case 0x009:
                        case 0x00a:
                        case 0x00b:
                        case 0x00c:
                        case 0x00d:
                        case 0x00e:
                        case 0x00f: no = 3;               goto NO_WAIT;
                    }
                    DIE();
                }
                case 0x01:    no = 4;   sz = bA6_4_A5SZ;  wt = bA6_4_A5WT;  goto WAIT;
                case 0x02:    no = 5;   sz = bA6_4_A5SZ;  wt = bA6_4_A5WT;  goto WAIT;
                case 0x03:    no = 6;   sz = addr < 0x0380000;  wt = bA6_4_A6WT;  goto WAIT;
                case 0x04:
                case 0x05:    no = 7;   sz = bA8_7_A8SZ;  wt = bA8_7_A8WT;  goto WAIT;
                case 0x06:
                case 0x07:    no = 8;   sz = bA8_7_A8SZ;  wt = bA8_7_A8WT;  goto WAIT;
                case 0x08:
                case 0x09:
                case 0x0a:
                case 0x0b:    no = 9;   sz = bA10_9_A10SZ;  wt = bA10_9_A10WT;  goto WAIT;
                case 0x0c:
                case 0x0d:
                case 0x0e:
                case 0x0f:    no = 10;  sz = bA10_9_A10SZ;  wt = bA10_9_A10WT;  goto WAIT;
            }
            DIE();
        case 0x1:
        {
            switch(addr >> 20)
            {
                case 0x10:
                case 0x11:
                case 0x12:
                case 0x13:
                case 0x14:
                case 0x15:
                case 0x16:
                case 0x17:    no = 11;  sz = bA12_11_A12SZ; wt = bA12_11_A12WT; goto WAIT;
                case 0x18:
                case 0x19:
                case 0x1a:
                case 0x1b:
                case 0x1c:
                case 0x1d:
                case 0x1e:
                case 0x1f:    no = 12;  sz = bA12_11_A12SZ; wt = bA12_11_A12WT; goto WAIT;
            }
            DIE();
        }
        case 0x2:     no = 13;  sz = bA14_13_A14SZ; wt = bA14_13_A14WT; goto WAIT;
        case 0x3:     no = 14;  sz = bA14_13_A14SZ; wt = bA14_13_A14WT; goto WAIT;
        case 0x4:
        case 0x5:     no = 15;  sz = bA18_15_A16SZ; wt = bA18_15_A16WT; goto WAIT;
        case 0x6:
        case 0x7:     no = 16;  sz = bA18_15_A16SZ; wt = bA18_15_A16WT; goto WAIT;
        case 0x8:
        case 0x9:
        case 0xa:
        case 0xb:     no = 17;  sz = bA18_15_A18SZ; wt = bA18_15_A18WT; goto WAIT;
        case 0xc:
        case 0xd:
        case 0xe:
        case 0xf:     no = 18;  sz = bA18_15_A18SZ; wt = bA18_15_A18WT; goto WAIT;
    }
    DIE();

    /* ウェイト加算。 */
WAIT:
    if(!mode) goto NO_WAIT;
    if(sz) { /*  8ビットデバイス */
        clk += wt * size;
//    switch(size) {
//    case 1:  CLK += wt * 1; break;
//    case 2:  CLK += wt * 2; break;
//    case 4:  CLK += wt * 4; break;
//    default: DIE();
//    }
    } else { /* 16ビットデバイス */
        clk += wt << (size >> 2);
//    switch(size) {
//    case 1:  CLK += wt * 1; break;
//    case 2:  CLK += wt * 1; break;
//    case 4:  CLK += wt * 2; break;
//    default: DIE();
//    }
    }
    /* * 内蔵RAM以外へのアクセスは、その領域のウェイトサイクルに加えて、さらに1サイクル必要です。
     *   （S1C33000コアCPUマニュアル 33000Core-J.pdf p.34「3.2.2 命令の実行サイクル数 (2)」参照）
     * * 2003/04/26追記
     *   S1C33000コアCPUマニュアル 33000Core-J.pdf B-II-4-20「外部システムインターフェイスのバスサイクル」を見ると、
     *   外部メモリのREADは前述の通り＋１サイクルですが、WRITEは＋２サイクルみたいなので、そのように修正しました。
     */
    clk += static_cast<int>(mode);

    core->advance_clock(clk);

NO_WAIT:

    return area[no];
}

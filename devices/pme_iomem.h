#ifndef PIEMU_PME_IOMEM_H
#define PIEMU_PME_IOMEM_H

#include "c33types.h"
#include "../core/c33bcu.h"
#include <vector>
#include <array>
#include <list>
#include <SDL_audio.h>
#include <map>

// ホストのサウンドデバイスへ一度に送られるサンプル数
#define WAVEBUFFER_SAMPLES (512)
// ↑の分だけバッファを満たすために必要な HDMA 要求数
#define WAVEBUFFER_MIN_FILLED_BUFFERS (WAVEBUFFER_SAMPLES * 2 * 2 / 256)
// 余裕を持って↑の 2 倍とする
#define BLKN		(WAVEBUFFER_MIN_FILLED_BUFFERS * 2)		/* バッファ数（※調整可） */

#define IOMEM_SIZE	0x10000		/* 0x0040000〜0x004ffff */
#define IOMAP_SIZE	0x10      // I/O mapping テーブルのサイズ

// 実体のあるデバイスをあらわすというよりは、特定アドレスへの読み書きに対するフックと考えるべき
class IOMemHook
{
public:
    virtual ~IOMemHook() = default;

    virtual c33int read(c33word addr, int size) = 0;
    virtual void write(c33word addr, c33int data, int size) = 0;
};


struct WAVEBUFFER
{
    std::vector<int16_t> data;
    size_t bytes_recorded;
    int ready;
};
#define WAVEBUFFER_NOT_READY 0
#define WAVEBUFFER_READY 1
#define WAVEBUFFER_DONE 2

#define HSDMA1_EDGE_NONE		0
#define HSDMA1_EDGE_ENABLED		1
#define HSDMA1_EDGE_DISABLED	2


// iomem は C33 の一部
class C33IOMem: public C33BusDevice
{
private:
//    std::map<c33word, std::shared_ptr<IOMemHook>> iomap;
    std::array<uint8_t, IOMEM_SIZE> iomem;
    MemoryAccessor<decltype(iomem)> acc;

    typedef c33int (C33IOMem::*IOMemHookReadHandler)(c33word ofs, int size);
    typedef void (C33IOMem::*IOMemHookWriteHandler)(c33word ofs, c33int data, int size);

    typedef std::pair<IOMemHookReadHandler, IOMemHookWriteHandler> IOMemHookHandler;
    std::map<c33word, IOMemHookHandler> iomap;

    std::list<WAVEBUFFER> ready_list, free_list;

    SDL_AudioSpec desired, obtained;
    SDL_atomic_t hsdma1_en;
public:
    C33IOMem();

    ~C33IOMem() override = default;

    void init(void* context) override;
    void work() override;

    c33int bus_read(c33word ofs, int size);
    void bus_write(c33word ofs, c33int data, int size);

    virtual c33byte bus_readB(c33word ofs) override;
    virtual void bus_writeB(c33word ofs, c33byte data) override;

    virtual c33hword bus_readH(c33word ofs) override;
    virtual void bus_writeH(c33word ofs, c33hword data) override;

    virtual c33word bus_readW(c33word ofs) override;
    virtual void bus_writeW(c33word ofs, c33word data) override;


    volatile struct c_IOtag* plc_IO() {
        return (volatile struct c_IOtag*)iomem.data();
    }

private:
    /* PAD */
    c33int io_read_pK5_K5D(c33word ofs, int size);
    c33int io_read_pK6_K6D(c33word ofs, int size);

    /* LCDC */
    void io_write_pSIF3_TXD(c33word ofs, c33int data, int size);

    /* SOUND */
    void io_write_pHS1_EN(c33word ofs, c33int data, int size);

    void iomem_write_default(c33word ofs, c33int data, int size);
    c33int iomem_read_default(c33word ofs, int size);

    void iomem_write_dummy(c33word ofs, c33int data, int size) {}
    c33int iomem_read_dummy(c33word ofs, int size) { return -1; }

};

#endif //PIEMU_PME_IOMEM_H

#ifndef PIEMU_PMEFRAM_H
#define PIEMU_PMEFRAM_H

#include "../core/c33bcu.h"
#include <array>

class PMEFRAM : public C33BusDevice
{
private:
    constexpr int FRAM_SIZE = 0x2000;

    std::array<c33byte, FRAM_SIZE> mem;
    MemoryAccessor acc;
public:
    PMEFRAM(size_t size) : mem(), acc(mem)
    {

    }

    ~PMEFRAM() override = default;


    c33byte bus_readB(c33word ofs) override
    {
        return acc.mem_readB(ofs);
    }

    void bus_writeB(c33word ofs, c33byte data) override
    {
        acc.mem_writeB(ofs, data);
    }

    c33hword bus_readH(c33word ofs) override
    {
        return acc.mem_readH(ofs);
    }
    void bus_writeH(c33word ofs, c33hword data) override
    {
        acc.mem_writeH(ofs, data);
    }

    c33word bus_readW(c33word ofs) override
    {
        return acc.mem_readW(ofs);
    }
    void bus_writeW(c33word ofs, c33word data) override
    {
        acc.mem_writeW(ofs, data);
    }
};


#endif //PIEMU_PMEFRAM_H

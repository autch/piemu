#ifndef PIEMU_PMESRAM_H
#define PIEMU_PMESRAM_H

#include "../core/c33bcu.h"
#include <array>

class PMESRAM : public C33BusDevice
{
private:

    std::vector<c33byte> mem;
    MemoryAccessor acc;
public:
    PMESRAM(size_t size) : mem(size), acc(mem)
    {

    }

    ~PMESRAM() override = default;

    c33int bus_read(c33word ofs, int size) override
    {
        return acc.mem_read(ofs, size);
    }

    void bus_write(c33word ofs, c33int data, int size) override
    {
        acc.mem_write(ofs, data, size);
    }

    c33byte bus_readB(c33word ofs) override
    {
        return acc.mem_readB(ofs);
    }

    void bus_writeB(c33word ofs, c33byte data) override
    {
        acc.mem_writeB(ofs, data);
    }

    c33hword bus_readH(c33word ofs) override {
        return acc.mem_readH(ofs);
    }
    void bus_writeH(c33word ofs, c33hword data) override {
        acc.mem_writeH(ofs, data);
    }

    c33word bus_readW(c33word ofs) override {
        return acc.mem_readW(ofs);
    }
    void bus_writeW(c33word ofs, c33word data) override {
        return acc.mem_writeW(ofs, data);
    }
};


#endif //PIEMU_PMESRAM_H

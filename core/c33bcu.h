#ifndef PIEMU_C33BCU_H
#define PIEMU_C33BCU_H

#include "c33types.h"
#include "c33core.h"
#include <c++/v1/vector>

template<class Container>
struct MemoryAccessor
{
    Container& mem;

    explicit MemoryAccessor(Container& parent) : mem(parent)
    {

    }

    c33int mem_read(c33word ofs, int size)
    {
        switch(size) {
            case 1: return mem_readB(ofs);
            case 2: return mem_readH(ofs);
            case 4: return mem_readW(ofs);
        }
        throw std::invalid_argument("size must be 1, 2, 4");
    }

    void mem_write(c33word ofs, c33int data, int size)
    {
        switch(size) {
            case 1: mem_writeB(ofs, data); return;
            case 2: mem_writeH(ofs, data); return;
            case 4: mem_writeW(ofs, data); return;
        }
        throw std::invalid_argument("size must be 1, 2, 4");
    }

    c33byte mem_readB(c33word ofs)
    {
        return mem[ofs];
    }

    void mem_writeB(c33word ofs, c33byte data)
    {
        mem[ofs] = data;
    }

    c33hword mem_readH(c33word ofs)
    {
        return (mem[ofs + 1] << 8) | mem[ofs];
    }

    void mem_writeH(c33word ofs, c33hword data)
    {
        mem[ofs] = data & 0xff;
        mem[ofs + 1] = (data >> 8) & 0xff;
    }

    c33word mem_readW(c33word ofs)
    {
        return (mem[ofs + 3] << 24)
               | (mem[ofs + 2] << 16)
               | (mem[ofs + 1] << 8)
               | mem[ofs];
    }

    void mem_writeW(c33word ofs, c33word data)
    {
        mem[ofs] = data & 0xff;
        mem[ofs + 1] = (data >> 8) & 0xff;
        mem[ofs + 2] = (data >> 16) & 0xff;
        mem[ofs + 3] = (data >> 24) & 0xff;
    }
};

class C33BusDevice {
public:
    virtual ~C33BusDevice() = default;

    virtual void init(void* context) {}
    virtual void work() {}

//    virtual c33int bus_read(c33word ofs, int size) = 0;
//    virtual void bus_write(c33word ofs, c33int data, int size) = 0;

    // default implementation
    virtual c33byte bus_readB(c33word ofs) = 0;
    virtual void bus_writeB(c33word ofs, c33byte data) = 0;

    virtual c33hword bus_readH(c33word ofs) = 0;
    virtual void bus_writeH(c33word ofs, c33hword data) = 0;

    virtual c33word bus_readW(c33word ofs) = 0;
    virtual void bus_writeW(c33word ofs, c33word data) = 0;
};

class C33BCU {
    std::shared_ptr<C33Core> core;

    typedef std::pair<c33word, std::shared_ptr<C33BusDevice> > AreaDefinition;
    enum class RWMode: int {
        InternalRW = 0,
        ExternalRead,
        ExternalWrite
    };

    std::vector<AreaDefinition> area;


    AreaDefinition area_sel(c33word addr, size_t size, RWMode mode);

public:

    void set_core(const std::shared_ptr<C33Core> &core_ptr)
    {
        core = core_ptr;
    }

    // for PIO
    c33int mem_read(c33word addr, size_t size);
    void mem_write(c33word addr, c33int data, size_t size);

    // for DMA or any internal access
    c33int mem_read_nowait(c33word addr, size_t size);
    void mem_write_nowait(c33word addr, c33int data, size_t size);

};


#endif //PIEMU_C33BCU_H

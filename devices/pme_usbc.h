#ifndef PIEMU_USBC_H
#define PIEMU_USBC_H

#include "../core/c33bcu.h"

class PMEUSBC: public C33BusDevice {
public:
    ~PMEUSBC() override = default;

    c33int bus_read(c33word ofs, int size) override
    {
        // TODO: not implemented
        return -1;
    }
    void bus_write(c33word ofs, c33int data, int size) override
    {
        // TODO: not implemented
    }
};

#endif //PIEMU_USBC_H

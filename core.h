
#ifndef CORE_H
#define CORE_H

#include "inst/inst.h"

#define BOOT_ADDRESS (0x0c00000)

void core_init(C33CORE* core);
void core_work(C33CORE* core);
uint32_t core_workex(C33CORE* core, uint32_t mils_org, uint32_t nClocksPerMSec);
void core_trap(C33CORE* core, c33word no, c33word level);

#endif // !CORE_H


#ifndef INST_H
#define INST_H

#include "inst_types.h"
#include "core_types.h"
#include "c33209e.h"

#define DIE() { (void)0; }

#define R(n) (core->r[n])
#define S(n) (core->s[n])

#define PSR  (*(C33PSR*)&S(0))
#define SP   (S(1))
#define AR   (*(c33lint*)&S(2))
#define ALR  (S(2))
#define AHR  (S(3))
#define D    (core->d)
#define PC   (core->pc)
#define EXT1 (core->ext[0])
#define EXT2 (core->ext[1])

#define Rs   (R(inst.rs))
#define Rd   (R(inst.rd))
#define Rb   (R(inst.rb))
#define Rsd  (R(inst.rs_rd))

// shortcut for definition
#define C33_OP(name)      void c33op_##name(C33CORE* core, INST inst)
// shortcut for calling
#define CALL_C33_OP(name, core, inst) c33op_##name(core, inst)
#define NAME_C33_OP(name) c33op_##name

#endif

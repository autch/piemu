// -*- mode: c++; -*-
/** 
 * c33core の実装につかうヘッダ
 */

#ifndef c33core_impl_h
#define c33core_impl_h

#include "c33core.h"

#ifdef C33_OP
#undef C33_OP
#endif

#define C33_OP(name) inline void c33core::c33op_##name(INST inst)

#endif  // !c33core_impl_h

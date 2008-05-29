
#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include "types.h"
#include "inst_types.h"

typedef union c33reg
{
  c33int  i;
  c33word w;
}C33REG;

typedef union c33lreg
{
  c33lint  i;
  c33lword w;
}C33LREG;

typedef union psr
{
  c33word w;
  struct
  {
    c33word n  : 1;
    c33word z  : 1;
    c33word v  : 1;
    c33word c  : 1;
    c33word ie : 1;
    c33word    : 1;
    c33word ds : 1;
    c33word mo : 1;
    c33word il : 4;
    c33word    :20;
  };
}C33PSR;

typedef struct c33core
{
  C33REG r[16];
  C33REG s[4];

  C33REG  pc;
  C33INST ext[2];
  C33REG  acc;

  int     d;
}C33CORE;

#endif

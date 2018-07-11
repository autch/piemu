/*
 *  bcu.c
 *
 *  P/EMU - P/ECE Emulator
 *  Copyright (C) 2003 Naoyuki Sawa
 *
 *  * Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *  - 作成開始。
 *  * Wed Apr 22 05:21:00 JST 2003 Naoyuki Sawa
 *  - DMAからのメモリアクセス用に、mem_read_nowait()/mem_write_nowait()追加。
 *    これまで、DMAからのメモリアクセスでCLKが加算されていたのを修正しました。
 */
#include "app.h"

/****************************************************************************
 *  エリア定義
 ****************************************************************************/

typedef struct _AREA {
  unsigned addr;
  int (*read)(PIEMU_CONTEXT* context, unsigned ofs, int size);
  void (*write)(PIEMU_CONTEXT* context, unsigned ofs, int data, int size);

  int (*readB)(PIEMU_CONTEXT* context, unsigned ofs);
  void (*writeB)(PIEMU_CONTEXT* context, unsigned ofs, int data);

  int (*readH)(PIEMU_CONTEXT* context, unsigned ofs);
  void (*writeH)(PIEMU_CONTEXT* context, unsigned ofs, int data);

  int (*readW)(PIEMU_CONTEXT* context, unsigned ofs);
  void (*writeW)(PIEMU_CONTEXT* context, unsigned ofs, int data);
} AREA;

#define DEF_AREA(addr, mod) \
{ \
  addr, \
  mod##_read, mod##_write, \
  mod##_readB, mod##_writeB, \
  mod##_readH, mod##_writeH, \
  mod##_readW, mod##_writeW \
}

#define DEF_NONE(addr) { addr, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL }

static AREA area_tbl[] = {
/* 0*/  DEF_AREA(0x0000000, fram), //{  0x0000000,  fram_read,  fram_write, fram_readB, fram_writeB, fram_readH, fram_writeH, fram_readW, fram_writeW },
/* 1*/  DEF_AREA(0x0030000, iomem), //{ 0x0030000,  iomem_read, iomem_write },
/* 2*/  DEF_NONE(0x0060000), //{  0x0060000,  NULL,   NULL    },
/* 3*/  DEF_NONE(0x0080000), //{  0x0080000,  NULL,   NULL    },
/* 4*/  DEF_AREA(0x0100000, sram), //{  0x0100000,  sram_read,  sram_write  },
/* 5*/  DEF_NONE(0x0200000), //{  0x0200000,  NULL,   NULL    },
/* 6*/  DEF_NONE(0x0300000), //{  0x0300000,  NULL,   NULL    },
/* 7*/  DEF_AREA(0x0400000, usbc), //{  0x0400000,  usbc_read,  usbc_write  },
/* 8*/  DEF_NONE(0x0600000), //{  0x0600000,  NULL,   NULL    },
/* 9*/  DEF_NONE(0x0800000), //{  0x0800000,  NULL,   NULL    },
/*10*/  DEF_AREA(0x0c00000, flash), //{ 0x0c00000,  flash_read, flash_write },
/*11*/  DEF_NONE(0x1000000), //{  0x1000000,  NULL,   NULL    },
/*12*/  DEF_NONE(0x1800000), //{  0x1800000,  NULL,   NULL    },
/*13*/  DEF_NONE(0x2000000), //{  0x2000000,  NULL,   NULL    },
/*14*/  DEF_NONE(0x3000000), //{  0x3000000,  NULL,   NULL    },
/*15*/  DEF_NONE(0x4000000), //{  0x4000000,  NULL,   NULL    },
/*16*/  DEF_NONE(0x6000000), //{  0x6000000,  NULL,   NULL    },
/*17*/  DEF_NONE(0x8000000), //{  0x8000000,  NULL,   NULL    },
/*18*/  DEF_NONE(0xc000000), //{  0xc000000,  NULL,   NULL    },
};

static AREA*
area_sel(PIEMU_CONTEXT* context, unsigned addr, int size, int mode/*0:内部RDWR/1:外部RD/2:外部WR*/)
{
  int no, sz, wt;

  // 警告対策
  no = sz = wt = 0;

  /* 整列チェック。 */
  switch(size) {
  case 1:                     break;
  case 2: if(addr & 1) DIE("bcu: odd-aligned access"); break;
  case 4: if(addr & 3) DIE("bcu: odd-aligned access"); break;
  default: DIE("bcu: odd-aligned access");
  }

  /* エリア選択。 */
  addr &= (1 << 28) - 1; /* S1C33のアドレス空間は28ビット */

  // これはうまくジャンプテーブルになるからこれでいい
  switch((addr >> 24) & 0x0f)
  {
    case 0x0:
      switch((addr >> 20) & 0x0f)
      {
        case 0x00:
        {
          switch((addr >> 16) & 0x0f)
          {
            case 0x00:
            case 0x01:
            case 0x02: no = 0;               goto NO_WAIT;
            case 0x03:
            case 0x04:
            case 0x05: no = 1;               goto NO_WAIT;
            case 0x06:
            case 0x07: no = 2;               goto NO_WAIT;
            case 0x08:
            case 0x09:
            case 0x0a:
            case 0x0b:
            case 0x0c:
            case 0x0d:
            case 0x0e:
            case 0x0f: no = 3;               goto NO_WAIT;
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
      switch((addr >> 20) & 0x0f)
      {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:    no = 11;  sz = bA12_11_A12SZ; wt = bA12_11_A12WT; goto WAIT;
        case 0x8:
        case 0x9:
        case 0xa:
        case 0xb:
        case 0xc:
        case 0xd:
        case 0xe:
        case 0xf:    no = 12;  sz = bA12_11_A12SZ; wt = bA12_11_A12WT; goto WAIT;
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
    CLK += wt * size;
//    switch(size) {
//    case 1:  CLK += wt * 1; break;
//    case 2:  CLK += wt * 2; break;
//    case 4:  CLK += wt * 4; break;
//    default: DIE();
//    }
  } else { /* 16ビットデバイス */
    CLK += wt << (size >> 2);
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
  CLK += mode;
NO_WAIT:

  return &area_tbl[no];
}

/****************************************************************************
 *  グローバル関数
 ****************************************************************************/

void
bcu_init(PIEMU_CONTEXT* context)
{
  /* TODO */
}

#define READ(ctx, addr, size, wait) \
{ \
  AREA* area = area_sel(context, addr, size, wait); \
  if(!area->read) return -1; \
  return area->read(context, addr - area->addr, size); \
}

#define WRITE(ctx, addr, data, size, wait) \
{ \
  AREA* area = area_sel(context, addr, size, wait); \
  if(!area->write) return; \
  area->write(context, addr - area->addr, data, size); \
}

#define READ_S(type, ctx, addr, size, wait) \
{ \
  AREA* area = area_sel(context, addr, size, wait); \
  if(!area->read##type) READ(ctx, addr, size, wait) \
  return area->read##type(context, addr - area->addr); \
}

#define WRITE_S(type, ctx, addr, data, size, wait) \
{ \
  AREA* area = area_sel(context, addr, size, wait); \
  if(!area->write##type) WRITE(ctx, addr, data, size, wait) \
  area->write##type(context, addr - area->addr, data); \
}

#define DEFUN_READ_S(type, size) \
int mem_read##type(PIEMU_CONTEXT* context, unsigned addr) \
{ \
  READ_S(type, context, addr, size, 1) \
}

#define DEFUN_READ_S_NW(type, size) \
int mem_read##type##_nowait(PIEMU_CONTEXT* context, unsigned addr) \
{ \
  READ_S(type, context, addr, size, 0) \
}

#define DEFUN_WRITE_S(type,  size) \
void mem_write##type(PIEMU_CONTEXT* context, unsigned addr, int data) \
{ \
  WRITE_S(type, context, addr, data, size, 2) \
}

#define DEFUN_WRITE_S_NW(type, size) \
void mem_write##type##_nowait(PIEMU_CONTEXT* context, unsigned addr, int data) \
{ \
  WRITE_S(type, context, addr, data, size, 0) \
}

#define DEFUN_S(type, size) \
  DEFUN_READ_S(type, size) \
  DEFUN_READ_S_NW(type, size) \
  DEFUN_WRITE_S(type, size) \
  DEFUN_WRITE_S_NW(type, size)


DEFUN_S(B, 1);
DEFUN_S(H, 2);
DEFUN_S(W, 4);

int
mem_read(PIEMU_CONTEXT* context, unsigned addr, int size)
{
  READ(context, addr, size, 1);
}

void
mem_write(PIEMU_CONTEXT* context, unsigned addr, int data, int size)
{
  WRITE(context, addr, data, size, 2);
}

int
mem_read_nowait(PIEMU_CONTEXT* context, unsigned addr, int size)
{
  READ(context, addr, size, 0);
}

void
mem_write_nowait(PIEMU_CONTEXT* context, unsigned addr, int data, int size)
{
  WRITE(context, addr, data, size, 0);
}


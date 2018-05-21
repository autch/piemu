/*	
 *	sram.h
 *
 *	P/EMU - P/ECE Emulator
 *	Copyright (C) 2003 Naoyuki Sawa
 *
 *	* Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *	- 作成開始。
 */
#ifndef __SRAM_H__
#define __SRAM_H__

#include "bcu.h"

typedef struct _SRAM {
	int mem_size;
	uint8_t* mem;
} SRAM;

void sram_init(struct tagPIEMU_CONTEXT* context);
#define sram_work	NULL

int sram_read(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size);
void sram_write(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size);

IOHANDLER_R(sram, B);
IOHANDLER_W(sram, B);
IOHANDLER_R(sram, H);
IOHANDLER_W(sram, H);
IOHANDLER_R(sram, W);
IOHANDLER_W(sram, W);

#endif /*__SRAM_H__*/

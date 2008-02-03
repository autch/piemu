/*	
 *	fram.h
 *
 *	P/EMU - P/ECE Emulator
 *	Copyright (C) 2003 Naoyuki Sawa
 *
 *	* Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *	- çÏê¨äJénÅB
 */
#ifndef __FRAM_H__
#define __FRAM_H__

#include "bcu.h"

#define FRAM_SIZE	0x2000		/* 0x0000000Å`0x0001fff */
typedef struct _FRAM {
	unsigned char mem[FRAM_SIZE];
} FRAM;

void fram_init(struct tagPIEMU_CONTEXT* context);
#define fram_work	NULL

int fram_read(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size);
void fram_write(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size);

IOHANDLER_R(fram, B);
IOHANDLER_W(fram, B);
IOHANDLER_R(fram, H);
IOHANDLER_W(fram, H);
IOHANDLER_R(fram, W);
IOHANDLER_W(fram, W);

#endif /*__FRAM_H__*/

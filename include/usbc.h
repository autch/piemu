/*	
 *	usbc.h
 *
 *	P/EMU - P/ECE Emulator
 *	Copyright (C) 2003 Naoyuki Sawa
 *
 *	* Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *	- 作成開始。
 */
#ifndef __USBC_H__
#define __USBC_H__

#include "bcu.h"

typedef struct _USBC {
	int dummy;
} USBC;

void usbc_init(struct tagPIEMU_CONTEXT* context);
void usbc_work(struct tagPIEMU_CONTEXT* context);

int usbc_read(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size);
void usbc_write(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size);

IOHANDLER_R(usbc, B);
IOHANDLER_W(usbc, B);
IOHANDLER_R(usbc, H);
IOHANDLER_W(usbc, H);
IOHANDLER_R(usbc, W);
IOHANDLER_W(usbc, W);

#endif /*__USBC_H__*/

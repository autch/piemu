/*	
 *	usbc.c
 *
 *	P/EMU - P/ECE Emulator
 *	Copyright (C) 2003 Naoyuki Sawa
 *
 *	* Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *	- 作成開始。
 */
#include "app.h"

/****************************************************************************
 *	グローバル変数
 ****************************************************************************/

//USBC usbc;

/****************************************************************************
 *	グローバル関数
 ****************************************************************************/

void
usbc_init(PIEMU_CONTEXT* context)
{
	/* TODO */
}

void
usbc_work(PIEMU_CONTEXT* context)
{
	/* TODO */
}

int usbc_read(PIEMU_CONTEXT* context, unsigned ofs, int size)
{
	/* TODO */
	return -1;
}

void usbc_write(PIEMU_CONTEXT* context, unsigned ofs, int data, int size)
{
	/* TODO */
}

IOHANDLER_R(usbc, B) { return -1; }
IOHANDLER_W(usbc, B) { }
IOHANDLER_R(usbc, H) { return -1; }
IOHANDLER_W(usbc, H) { }
IOHANDLER_R(usbc, W) { return -1; }
IOHANDLER_W(usbc, W) { }

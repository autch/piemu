/*	
 *	fram.c
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

//FRAM fram;

/****************************************************************************
 *	グローバル関数
 ****************************************************************************/

void
fram_init(PIEMU_CONTEXT* context)
{
	memset(&context->fram, 0, sizeof context->fram);
}

int
fram_read(PIEMU_CONTEXT* context, unsigned ofs, int size)
{
	ofs &= FRAM_SIZE - 1;
	switch(size) {
	case 1: return READ_MEM_B(context->fram.mem + ofs);
	case 2: return READ_MEM_H(context->fram.mem + ofs);
	case 4: return READ_MEM_W(context->fram.mem + ofs);
	}
	DIE();
	return -1; /* 警告抑制 */
}

void
fram_write(PIEMU_CONTEXT* context, unsigned ofs, int data, int size)
{
	ofs &= FRAM_SIZE - 1;
	switch(size) {
	case 1: *(char *)&context->fram.mem[ofs] = data; return;
	case 2: *(short*)&context->fram.mem[ofs] = data; return;
	case 4: *(int  *)&context->fram.mem[ofs] = data; return;
	}
	DIE();
}

IOHANDLER_R(fram, B)
{
	ofs &= FRAM_SIZE - 1;
	return READ_MEM_B(context->fram.mem + ofs);
}
IOHANDLER_W(fram, B)
{
	ofs &= FRAM_SIZE - 1;
	*(char *)&context->fram.mem[ofs] = data;
}

IOHANDLER_R(fram, H)
{
	ofs &= FRAM_SIZE - 1;
	return READ_MEM_H(context->fram.mem + ofs);
}
IOHANDLER_W(fram, H)
{
	ofs &= FRAM_SIZE - 1;
	*(short*)&context->fram.mem[ofs] = data;
}

IOHANDLER_R(fram, W)
{
	ofs &= FRAM_SIZE - 1;
	return READ_MEM_W(context->fram.mem + ofs);
}

IOHANDLER_W(fram, W)
{
	ofs &= FRAM_SIZE - 1;
	*(int  *)&context->fram.mem[ofs] = data;
}


/*	
 *	sram.c
 *
 *	P/EMU - P/ECE Emulator
 *	Copyright (C) 2003 Naoyuki Sawa
 *
 *	* Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *	- 作成開始。
 */
#include "../app.h"

/****************************************************************************
 *	グローバル変数
 ****************************************************************************/

//SRAM sram;

/****************************************************************************
 *	グローバル関数
 ****************************************************************************/

void
sram_init(PIEMU_CONTEXT* context)
{
	/* メモリ割り当て。 */
	memset(&context->sram, 0, sizeof context->sram);
	context->sram.mem_size = context->emu.sysinfo.sram_end - context->emu.sysinfo.sram_top;
	context->sram.mem = (unsigned char*)calloc(context->sram.mem_size, 1);
}

int
sram_read(PIEMU_CONTEXT* context, unsigned ofs, int size)
{
	ofs &= context->sram.mem_size - 1;
	switch(size) {
	case 1: return READ_MEM_B(context->sram.mem + ofs);
	case 2: return READ_MEM_H(context->sram.mem + ofs);
	case 4: return READ_MEM_W(context->sram.mem + ofs);
	}
	DIE();
	return -1; /* 警告抑制 */
}

void
sram_write(PIEMU_CONTEXT* context, unsigned ofs, int data, int size)
{
	ofs &= context->sram.mem_size - 1;
	switch(size) {
	case 1: *(char *)&context->sram.mem[ofs] = data; return;
	case 2: *(short*)&context->sram.mem[ofs] = data; return;
	case 4: *(int  *)&context->sram.mem[ofs] = data; return;
	}
	DIE();
}

IOHANDLER_R(sram, B)
{
	ofs &= context->sram.mem_size - 1;
	return READ_MEM_B(context->sram.mem + ofs);
}
IOHANDLER_W(sram, B)
{
	ofs &= context->sram.mem_size - 1;
	*(char *)&context->sram.mem[ofs] = data;
}
IOHANDLER_R(sram, H)
{
	ofs &= context->sram.mem_size - 1;
	return READ_MEM_H(context->sram.mem + ofs);
}
IOHANDLER_W(sram, H)
{
	ofs &= context->sram.mem_size - 1;
	*(short*)&context->sram.mem[ofs] = data;
}
IOHANDLER_R(sram, W)
{
	ofs &= context->sram.mem_size - 1;
	return READ_MEM_W(context->sram.mem + ofs);
}
IOHANDLER_W(sram, W)
{
	ofs &= context->sram.mem_size - 1;
	*(int  *)&context->sram.mem[ofs] = data;
}


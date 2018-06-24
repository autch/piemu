/*	
 *	flash.c
 *
 *	P/EMU - P/ECE Emulator
 *	Copyright (C) 2003 Naoyuki Sawa
 *
 *	* Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *	- 作成開始。
 */
#include "../app.h"

/****************************************************************************
 *	
 ****************************************************************************/

#define SECTOR_SIZE	0x01000		/* セクタ  消去単位= 4KB */
#define BLOCK_SIZE	0x10000		/* ブロック消去単位=64KB */

/****************************************************************************
 *	グローバル変数
 ****************************************************************************/

//FLASH flash;

/****************************************************************************
 *	グローバル関数
 ****************************************************************************/

void
flash_init(PIEMU_CONTEXT* context)
{
#define FLASH_TOP	0x0c00000
#define READ_UNIT	0x1000

	int size, bits;

	memset(&context->flash, 0, sizeof context->flash);

	/* CFIクエリー情報作成。(とりあえず必要なフィールドだけ) */
	size = (int)context->emu.sysinfo.pffs_end - FLASH_TOP;
	bits = 0;
	for(;;) {
		if(1 << bits >= size) break;
		bits++;
	}
	context->flash.cfiinfo.device_size = bits;

	/* メモリ割り当て。 */
	context->flash.mem_size = 1 << context->flash.cfiinfo.device_size;
	context->flash.mem = (unsigned char*)calloc(context->flash.mem_size, 1);

	/* イメージ読み込み。 */
  context->pfnLoadFlashImage(context, &context->flash, context->pUser);

#undef FLASH_TOP
#undef READ_UNIT
}

#define FLASH_READ(ofs, size, type) \
{ \
	ofs &= context->flash.mem_size - 1; \
	switch(context->flash.stat) \
	{ \
		case FLASH_NORMAL: \
			return READ_MEM(context->flash.mem + ofs, type); \
		case FLASH_CFI_QUERY: \
			ofs -= CFIINFO_OFFSET * sizeof(short); \
			if(ofs > sizeof(CFIINFO)) DIE(); \
			return READ_MEM((char*)&context->flash.cfiinfo + ofs, type); \
	} \
	DIE(); \
	return -1; /* 警告抑制 */ \
}

int
flash_read(PIEMU_CONTEXT* context, unsigned ofs, int size)
{
	ofs &= context->flash.mem_size - 1;
	switch(context->flash.stat) {
	case FLASH_NORMAL:
		switch(size) {
		case 1: return READ_MEM_B(context->flash.mem + ofs);
		case 2: return READ_MEM_H(context->flash.mem + ofs);
		case 4: return READ_MEM_W(context->flash.mem + ofs);
		}
		DIE();
	case FLASH_CFI_QUERY:
		/* ※ソフトウェアIDモード未対応 */
		ofs -= CFIINFO_OFFSET * sizeof(short);
		if(ofs > sizeof(CFIINFO)) DIE();
		switch(size) {
		case 1: return READ_MEM_B((char*)&context->flash.cfiinfo + ofs);
		case 2: return READ_MEM_H((char*)&context->flash.cfiinfo + ofs);
		case 4: return READ_MEM_W((char*)&context->flash.cfiinfo + ofs);
		}
		DIE();
	}
	DIE();
	return -1; /* 警告抑制 */
}

void
flash_write(PIEMU_CONTEXT* context, unsigned ofs, int data, int size)
{
	int cmd;

	if(size != 2) DIE(); /* 書き込みはハーフワード単位のみ！ */
	ofs &= context->flash.mem_size - 1;
	cmd = data & 0xff;

	/* WORD PROGRAM以外での0xf0書き込みは、汎用EXITコマンドと見なします。 */
	if(context->flash.stat != FLASH_WORD_PROGRAM && cmd == 0xf0) {
		context->flash.stat = FLASH_NORMAL; /* MANUAL EXIT */
		return;
	}

	switch(context->flash.stat) {
	case FLASH_NORMAL:
		if(ofs == 0x5555 * 2 && cmd == 0xaa) {
			context->flash.stat = FLASH_ENTER1;
		} else {
			DIE();
		}
		break;
	case FLASH_ENTER1:
		if(ofs == 0x2aaa * 2 && cmd == 0x55) {
			context->flash.stat = FLASH_ENTER2;
		} else {
			DIE();
		}
		break;
	case FLASH_ENTER2:
		if(ofs == 0x5555 * 2) {
			switch(cmd) {
			case 0x80:
				context->flash.stat = FLASH_ENTER3;
				break;
			case 0x90: /* SOFTWARE ID と                     */
			case 0x98: /* CFI QUERY   は、同じものとして扱う */
				context->flash.stat = FLASH_CFI_QUERY;
				break;
			case 0xa0:
				context->flash.stat = FLASH_WORD_PROGRAM;
				break;
			default:
				DIE();
			}
		} else {
			DIE();
		}
		break;
	case FLASH_ENTER3:
		if(ofs == 0x5555 * 2 && cmd == 0xaa) {
			context->flash.stat = FLASH_ENTER4;
		} else {
			DIE();
		}
		break;
	case FLASH_ENTER4:
		if(ofs == 0x2aaa * 2 && cmd == 0x55) {
			context->flash.stat = FLASH_ENTER5;
		} else {
			DIE();
		}
		break;
	case FLASH_ENTER5:
		switch(cmd) {
		case 0x10: /* CHIP ERASE */
			if(ofs == 0x5555 * 2) {
				// dbg("CHIP ERASE");
				memset(context->flash.mem, -1, context->flash.mem_size);
				context->flash.stat = FLASH_NORMAL; /* AUTO EXIT */
			} else {
				DIE();
			}
			break;
		case 0x30: /* SECTOR ERASE */
			ofs &= ~(SECTOR_SIZE - 1);
			// dbg("SECTOR ERASE: %07x", ofs);
			memset(&context->flash.mem[ofs], -1, SECTOR_SIZE);
			context->flash.stat = FLASH_NORMAL; /* AUTO EXIT */
			break;
		case 0x50: /* BLOCK ERASE */
			ofs &= ~(BLOCK_SIZE - 1);
			// dbg("BLOCK ERASE: %07x", ofs);
			memset(&context->flash.mem[ofs], -1, BLOCK_SIZE);
			context->flash.stat = FLASH_NORMAL; /* AUTO EXIT */
			break;
		default:
			DIE();
		}
		break;
	case FLASH_CFI_QUERY:
		if((ofs == 0x5555 * 2 && cmd == 0xaa) ||
		   (ofs == 0x2aaa * 2 && cmd == 0x55)) {
			/* 長い方のEXITシーケンス三命令(aa-55-f0)のうち、先行する二命令を無視します。
			 * 最後の一命令は汎用EXITコマンドとして、この関数の最初でまとめて処理しています。
			 */
		} else {
			DIE();
		}
		break;
	case FLASH_WORD_PROGRAM:
		// dbg("WORD PROGRAM: %07x=%04x", ofs, data & 0xffff);
		*(short*)&context->flash.mem[ofs] = data;
		context->flash.stat = FLASH_NORMAL; /* AUTO EXIT */
		break;
	default:
		DIE();
	}
}

IOHANDLER_R(flash, B)
{
	FLASH_READ(ofs, size, char);
}
IOHANDLER_W(flash, B)
{
	DIE();
}

IOHANDLER_R(flash, H)
{
	FLASH_READ(ofs, size, short);
}
IOHANDLER_W(flash, H)
{
	flash_write(context, ofs, data, 2);
}

IOHANDLER_R(flash, W)
{
	ofs &= context->flash.mem_size - 1;
	switch(context->flash.stat)
	{
		case FLASH_NORMAL:
			return READ_MEM_W(context->flash.mem + ofs);
		case FLASH_CFI_QUERY:
			ofs -= CFIINFO_OFFSET * sizeof(short);
			if(ofs > sizeof(CFIINFO)) DIE();
			return READ_MEM_W((char*)&context->flash.cfiinfo + ofs);
	}
	DIE();
	return -1; /* 警告抑制 */
}

IOHANDLER_W(flash, W)
{
	DIE();
}


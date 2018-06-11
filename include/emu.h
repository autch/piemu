/*	
 *	emu.h
 *
 *	P/EMU - P/ECE Emulator
 *	Copyright (C) 2003 Naoyuki Sawa
 *
 *	* Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *	- 作成開始。
 */
#ifndef __EMU_H__
#define __EMU_H__

typedef struct _MODULE {
	void (*init)(struct tagPIEMU_CONTEXT* context);
	void (*work)(struct tagPIEMU_CONTEXT* context);
} MODULE;

#define MODTBL_SIZE 0x10

typedef struct _EMU {
	SYSTEMINFO sysinfo;
	MODULE* module_tbl;
} EMU;
extern EMU emu;

void emu_init(struct tagPIEMU_CONTEXT* context);
//void emu_work(struct tagPIEMU_CONTEXT* context);
int emu_work(void* ctx);
int emu_devices_work(void* ctx);
int emu_lcd_work(void* ctx);

#endif /*__EMU_H__*/

/*	
 *	flash.h
 *
 *	P/EMU - P/ECE Emulator
 *	Copyright (C) 2003 Naoyuki Sawa
 *
 *	* Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *	- 作成開始。
 */
#ifndef __FLASH_H__
#define __FLASH_H__

#define CFIINFO_OFFSET	0x10		/* CFIクエリー情報の先頭オフセット（ハーフワード単位！） */
typedef struct _CFIINFO {
	/* CFI Query Identification String */
	unsigned short query_unique_ascii_string[3];							/* 10-12 */
	unsigned short primary_oem_command_set[2];								/* 13-14 */
	unsigned short address_for_primary_extended_table[2];					/* 15-16 */
	unsigned short alternate_oem_command_set[2];							/* 17-18 */
	unsigned short address_for_alternate_oem_extended_table[2];				/* 19-1a */
	/* System Interface Information */
	unsigned short vdd_min;													/* 1b */
	unsigned short vdd_max;													/* 1c */
	unsigned short vpp_min;													/* 1d */
	unsigned short vpp_max;													/* 1e */
	unsigned short typical_time_out_for_word_program;						/* 1f */
	unsigned short typical_time_out_for_min_size_buffer_program;			/* 20 */
	unsigned short typical_time_out_for_individual_sector_block_erase;		/* 21 */
	unsigned short typical_time_out_for_chip_erase;							/* 22 */
	unsigned short maximum_time_out_for_word_program;						/* 23 */
	unsigned short maximum_time_out_for_buffer_program;						/* 24 */
	unsigned short maximum_time_out_for_individual_sector_block_erase;		/* 25 */
	unsigned short maximum_time_out_for_chip_erase;							/* 26 */
	/* Device Geometry Information */
	unsigned short device_size;												/* 27 */
	unsigned short flash_device_interface_description[2];					/* 28-29 */
	unsigned short maximum_number_of_byte_in_multi_byte_write[2];			/* 2a-2b */
	unsigned short number_of_erase_sector_block_sizes_supported_by_device;	/* 2c */
	unsigned short sector_information[4];									/* 2d-30 */
	unsigned short block_information[4];									/* 31-34 */
} CFIINFO;

typedef struct _FLASH {
	CFIINFO cfiinfo;
	int mem_size;
	unsigned char* mem;
	//
	int stat;
} FLASH;

#define FLASH_NORMAL		0
#define FLASH_ENTER1		1	/* NORMAL -> [5555*2]=??aa */
#define FLASH_ENTER2		2	/* ENTER1 -> [2aaa*2]=??55 */
#define FLASH_ENTER3		3	/* ENTER2 -> [5555*2]=??80 */
#define FLASH_ENTER4		4	/* ENTER3 -> [5555*2]=??aa */
#define FLASH_ENTER5		5	/* ENTER4 -> [2aaa*2]=??55 */
/* CHIP   ERASE */			/* ENTER5 -> [5555*2]=??10 -> EXIT */
/* SECTOR ERASE */			/* ENTER5 -> [??????]=??30 -> EXIT */
/* BLOCK  ERASE */			/* ENTER5 -> [??????]=??50 -> EXIT */
#define FLASH_CFI_QUERY		6	/* ENTER2 -> [5555*2]=??98 */
/* SOFTWARE ID (≒ CFI QUERY) */	/* ENTER2 -> [5555*2]=??90 */
/* EXIT CFI QUERY/SOFTWARE ID */	/* ------------------------------------------> [??????]=??f0 -> EXIT */
					/* または -> [5555*2]=??aa -> [2aaa*2]=??55 -> [5555*2]=??f0 -> EXIT */
#define FLASH_WORD_PROGRAM	7	/* ENTER2 -> [5555*2]=??a0 */
/* LOAD WORD ADDRESS/WORD DATA */	/* --------> [??????]=???? -> EXIT */

void flash_init(struct tagPIEMU_CONTEXT* context);
#define flash_work	NULL

int flash_read(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size);
void flash_write(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size);

IOHANDLER_R(flash, B);
IOHANDLER_W(flash, B);
IOHANDLER_R(flash, H);
IOHANDLER_W(flash, H);
IOHANDLER_R(flash, W);
IOHANDLER_W(flash, W);

#endif /*__FLASH_H__*/

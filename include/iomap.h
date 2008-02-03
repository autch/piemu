/*
 * iomap.h -- Device I/O mapping definitions
 * a part of p/emu - p/ece emulator
 * (c) 2003, Naoyuki Sawa, re-written by Yui N.
 *
 * yui: 2005.08.24: このファイルはもはや不要。
 */

/*	
 *	iomap.h
 *
 *	P/EMU - P/ECE Emulator
 *	Copyright (C) 2003 Naoyuki Sawa
 *
 *	* Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *	- 作成開始。
 */

/* 複数回インクルードして使うので、ガードなしです。 */

struct tagPIEMU_CONTEXT;

/****************************************************************************/
#undef IOR_
#undef IO_W
#undef IORW
#ifdef IOMAP_TBL
typedef struct _IOMAP {
	volatile void* addr;
	int (*read)(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size);
	void (*write)(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size);
} IOMAP;
static IOMAP iomap_tbl[] = {
#define IOR_(p)		{ &(p), p##_read, NULL },
#define IO_W(p)		{ &(p), NULL, p##_write },
#define IORW(p)		{ &(p), p##_read, p##_write },
#else /*IOMAP_TBL*/
#define IOR_(p)		int  p##_read(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size);
#define IO_W(p)		void p##_write(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size);
#define IORW(p)		IOR_(p) IO_W(p)
#endif /*IOMAP_TBL*/
/****************************************************************************/

/* PAD */
IOR_(pK5_K5D)
IOR_(pK6_K6D)

/* LCDC */
IO_W(pSIF3_TXD)

/* SOUND */
IO_W(pHS1_EN)

/****************************************************************************/
#ifdef IOMAP_TBL
};
#define IOMAP_COUNT	(sizeof iomap_tbl / sizeof *iomap_tbl)
#endif /*IOMAP_TBL*/
#undef IOR_
#undef IO_W
#undef IORW
#define IOR_(p)		int  p##_read(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size)
#define IO_W(p)		void p##_write(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size)
/****************************************************************************/

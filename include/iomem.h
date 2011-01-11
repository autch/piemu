/*	
 *	iomem.h
 *
 *	P/EMU - P/ECE Emulator
 *	Copyright (C) 2003 Naoyuki Sawa
 *
 *	* Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *	- 作成開始。
 */
#ifndef __IOMEM_H__
#define __IOMEM_H__

/* SOUND */

// ホストのサウンドデバイスへ一度に送られるサンプル数
#define WAVEBUFFER_SAMPLES 2048
// ↑の分だけバッファを満たすために必要な HDMA 要求数
#define WAVEBUFFER_MIN_FILLED_BUFFERS (WAVEBUFFER_SAMPLES * 2 / 256)
// 余裕を持って↑の 2 倍とする
#define BLKN		(WAVEBUFFER_MIN_FILLED_BUFFERS * 2)		/* バッファ数（※調整可） */

#define IOMEM_SIZE	0x10000		/* 0x0040000～0x004ffff */

#define IOMAP_SIZE	0x10      // I/O mapping テーブルのサイズ

typedef struct _IOMAP {
	volatile void* addr;
	int (*read)(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size);
	void (*write)(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size);
} IOMAP;

#define IOR_(p)		p##_read
#define IO_W(p)		p##_write
#define IORW(p)		IOR_(p); IO_W(p);
#define PIOR_(p)	&(p)
#define PIO_W(p)	&(p)
#define PIORW(p)	&(p)

typedef struct tagWAVEBUFFER
{
	unsigned char* pData;
	unsigned int dwBufferLength;   // size, in bytes, of the pData
	unsigned int dwBytesRecorded;  // size, in bytes, of the actual content of pData
	int nReady; // 0: not ready, 1: ready to play, use #define's below.
	struct tagWAVEBUFFER* next;
}WAVEBUFFER;

#define WAVEBUFFER_NOT_READY 0
#define WAVEBUFFER_READY 1
#define WAVEBUFFER_DONE 2

typedef struct _IOMEM {
	unsigned char mem[IOMEM_SIZE];
	IOMAP* iomap_tbl;
	/* SOUND */
	WAVEBUFFER buffer[BLKN];
	WAVEBUFFER* head;
	WAVEBUFFER* tail;
	int nQueuedBuffers;
} IOMEM;

#define IOOFS(p)	((uint32_t)(p) - (uint32_t)context->iomem.mem)

void iomem_init(struct tagPIEMU_CONTEXT* context);
void iomem_work(struct tagPIEMU_CONTEXT* context);

int iomem_read(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size);
void iomem_write(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size);

int iomem_read_default(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size);
void iomem_write_default(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size);

/* PAD */
int IOR_(pK5_K5D)(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size);
int IOR_(pK6_K6D)(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size);

/* LCDC */
void IO_W(pSIF3_TXD)(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size);

/* SOUND */
void IO_W(pHS1_EN)(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size);

IOHANDLER_R(iomem, B);
IOHANDLER_W(iomem, B);
IOHANDLER_R(iomem, H);
IOHANDLER_W(iomem, H);
IOHANDLER_R(iomem, W);
IOHANDLER_W(iomem, W);

#endif /*__IOMEM_H__*/

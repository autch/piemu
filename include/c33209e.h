//****************************************************************************
//
//	Copyright (C) SEIKO EPSON CORP. 1999
//
//	File name: c33209.h
//	  This is C33209 symbol definition file.
//
//	Revision history
//		1999.07.29	T.Mineshima start c33208 structure and define
//		1999.08.04	T.Mineshima change define name.
//
//	これ以降の修正は、Naoyuki Sawa<nsawa@north.hokkai.net>によるものです。
//		2002.07.04	「〜33208〜」という名称を「〜33209〜」に変更。
//		2002.07.04	IDMA_STのbBLKとbSINの内容が逆だったのを修正。
//		2002.07.05	pINT_PSIO1_PADのコメント欄のアドレス間違いを修正。
//		2002.07.05	plc_IOの定義を追加。
//		2002.10.20	8ビットタイマCh.4-5の定義を追加。
//		2002.10.20	シリアルI/F  Ch.2-3の定義を追加。
//		2002.10.20	ポートSIO機能拡張レジスタの定義を追加。
//		2002.10.24	CLKDIV_8T1_32〜4096のコメントが"8-bit timer 2"
//				となっていたのを"8-bit timer 1"に修正しました。
//		2002.10.24	割り込みベクタ番号の定義を追加しました。
//
//	NOTE:
//		* struct c_IOtagのDummy*は、コメントのアドレス範囲が左右逆です。
//
//****************************************************************************

#ifndef C33209_H
#define C33209_H

/****************************************************************/
/*	c33209.h	: C33209 symbol definition file		*/
/****************************************************************/

#define plc_IO ((volatile struct c_IOtag*)0x40000)
//↓2003/04/12:エミュレータ用に変更
//#define plc_IO ((volatile struct c_IOtag*)context->iomem.mem)

/****************************************************************/
/* [8-bit timer clock select register] c_CLKSEL			*/
/****************************************************************/
union c_CLKSELtag {
	volatile struct {
		unsigned char	P8TPCK0	: 1;		/* 8-bit timer0 clock division ratio selection */
		unsigned char	P8TPCK1	: 1;		/* 8-bit timer1 clock division ratio selection */
		unsigned char	P8TPCK2	: 1;		/* 8-bit timer2 clock division ratio selection */
		unsigned char	P8TPCK3	: 1;		/* 8-bit timer3 clock division ratio selection */
	} bCTL;
	volatile unsigned char	ucCTL;
};

/****************************************************************/
/* [Clock control register] c_CLKCTL				*/
/****************************************************************/
union c_CLKCTLtag {
	volatile struct {
		unsigned char	TSA	: 3;		/* Clock A clock division ratio selection */
		unsigned char	TONA	: 1;		/* Clock A clock control */
		unsigned char	TSB	: 3;		/* Clock B clock division ratio selection */
		unsigned char	TONB	: 1;		/* Clock B clock control */
	} bCTL;
	volatile unsigned char	ucCTL;
};

/****************************************************************/
/* [Clock timer] c_TIMER					*/
/****************************************************************/
struct c_TIMERtag {
	union {						/* Clock timer Run/Stop register */
		volatile struct {
			unsigned char	TCRUN	: 1;	/* Clock timer Run/Stop control */
			unsigned char	TCRST	: 1;	/* Clock timer Reset */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rTCR;

	union {						/* Clock timer interrupt control register */
		volatile struct {
			unsigned char	TCAF	: 1;	/* Alarm factor generation flag */
			unsigned char	TCIF	: 1;	/* Interrupt factor generation flag */
			unsigned char	TCASE	: 3;	/* Clock timer alarm factor selection */
			unsigned char	TCISE	: 3;	/* Clock timer interrupt factor selection */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rTCI;

	volatile unsigned char	rTCD;			/* Clock timer data */
	volatile unsigned char	rTCMD;			/* Clock timer second counter data */
	volatile unsigned char	rTCHD;			/* Clock timer minute counter data */
	volatile unsigned char	rTCDD;			/* Clock timer hour counter data */
	volatile unsigned char	rTCNDL;			/* Clock timer day counter data (low-order 8-bits) */
	volatile unsigned char	rTCNDH;			/* Clock timer day counter data (high-order 8-bits) */
	volatile unsigned char	rTCCH;			/* Clock timer minute comparison data */
	volatile unsigned char	rTCCD;			/* Clock timer hour comparison data */
	volatile unsigned char	rTCCN;			/* Clock timer day comparison data */
};

/****************************************************************/
/* [8-bit timer] c_T8						*/
/****************************************************************/
struct c_T8tag {
	union {						/* 8-bit timer control register */
		volatile struct {
			unsigned char	PTRUN	: 1;	/* 8-bit timer Run/Stop control */
			unsigned char	PSET	: 1;	/* 8-bit timer preset */
			unsigned char	PTOUT	: 1;	/* 8-bit timer clock output control */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rT8CTL;
	volatile unsigned char	rRLD;			/* 8-bit timer reload data register */
	volatile unsigned char	rPTD;			/* 8-bit timer counter data register */
	unsigned char	Dummy;
};

/****************************************************************/
/* [Watchdog timer] c_WDT					*/
/****************************************************************/
struct c_WDTtag {
	union {						/* Watchdog timer write protect register */
		volatile struct {
			unsigned char	Dummy	: 7;
			unsigned char	WRWD	: 1;	/* EWD write protection */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rWRWD;

	union {
		volatile struct {			/* Watchdog timer enable register */
			unsigned char	Dummy	: 1;
			unsigned char	EWD	: 1;	/* Watchdog timer enable */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rEWD;
};

/****************************************************************/
/* [Power control] c_PWRCTL					*/
/****************************************************************/
struct c_PWRCTLtag {
	union {						/* Power control register */
		volatile struct {
			unsigned char	SOSC1	: 1;	/* Low-speed (OSC1) oscillation On/Off */
			unsigned char	SOSC3	: 1;	/* High-speed (OSC3) oscillation On/Off */
			unsigned char	CLKCHG	: 1;	/* CPU operating clock switch */
			unsigned char	Dummy	: 2;	/* D4-3 reserved */
			unsigned char	PSCON	: 1;	/* Prescaler On/Off control */
			unsigned char	CLKDT	: 2;	/* System clock division ratio selection */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rPWRCTL;

	union {						/* Prescaler clock select control register */
		volatile struct {
			unsigned char	PSCDT0	: 1;	/* Prescaler clock selection */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rCLKSEL;

	unsigned char	Dummy1[14];			/* 0x4018f - 0x40182 */

	union {						/* Clock option register */
		volatile struct {
			unsigned char	PF1ON	: 1;	/* OSC1 external output control */
			unsigned char	Dummy	: 1;	/* D1 reserved */
			unsigned char	P8T1ON	: 1;	/* OSC3-stabilize waiting function */
			unsigned char	HLT2OP	: 1;	/* HALT clock option */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rCLKOPT;

	unsigned char	Dummy2[13];			/* 0x4019d - 0x40191 */

	volatile unsigned char	rPWRPRT;		/* Power control protect register */
};

/****************************************************************/
/* [Serial I/F] c_SIF						*/
/****************************************************************/
struct c_SIFtag {
	volatile unsigned char	rTXD;			/* Serial I/F transmit data register */
	volatile unsigned char	rRXD;			/* Serial I/F receive data register */

	union {						/* Serial I/F status register */
		volatile struct {
			unsigned char	RDBF	: 1;	/* Receive data buffer full */
			unsigned char	TDBE	: 1;	/* Transmit data buffer empty */
			unsigned char	OER	: 1;	/* Overrun error flag */
			unsigned char	PER	: 1;	/* Parity error flag */
			unsigned char	FER	: 1;	/* Flaming error flag */
			unsigned char	TEND	: 1;	/* Transmit end flag */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rSTATUS;

	union {						/* Serial I/F control register */
		volatile struct {
			unsigned char	SMD	: 2;	/* Transmit mode selection */
			unsigned char	SSCK	: 1;	/* Input clock selection */
			unsigned char	STPB	: 1;	/* Stop bit selection */
			unsigned char	PMD	: 1;	/* Parity mode selection */
			unsigned char	EPR	: 1;	/* Parity enable */
			unsigned char	RXEN	: 1;	/* Receive enable */
			unsigned char	TXEN	: 1;	/* Transmit enable */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rCONTROL;

	union {						/* Serial I/F IrDA register */
		volatile struct {
			unsigned char	IRMD	: 2;	/* Interface mode selection */
			unsigned char	IRRL	: 1;	/* I/F input logic inversion */
			unsigned char	IRTL	: 1;	/* I/F output logic inversion */
			unsigned char	DIVMD	: 1;	/* Async. clock division ratio */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rIRDA;
};

/****************************************************************/
/* [A/D converter] c_AD						*/
/****************************************************************/
struct c_ADtag {
	volatile unsigned short	rADD;			/* A/D conversion result register */

	union {						/* A/D trigger register */
		volatile struct {
			unsigned char	CH	: 3;	/* A/D conversion channel status */
			unsigned char	TS	: 2;	/* A/D conversion trigger selection */
			unsigned char	MS	: 1;	/* A/D conversion mode selection */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rTRIG;

	union {						/* A/D channel register */
		volatile struct {
			unsigned char	CS	: 3;	/* A/D converter start channel selection */
			unsigned char	CE	: 3;	/* A/D converter end channel selection */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rCHNL;

	union {						/* A/D enable register */
		volatile struct {
			unsigned char	OWE	: 1;	/* Overwrite error flag */
			unsigned char	ADST	: 1;	/* A/D conversion control/status */
			unsigned char	ADE	: 1;	/* A/D enable */
			unsigned char	ADF	: 1;	/* Conversion-complete flag */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rENBL;

	union {						/* A/D sampling register */
		volatile struct {
			unsigned char	ST	: 2;	/* Input sampling time setup */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rSMPL;
};

/****************************************************************/
/* [Interrupt controller] c_INTC				*/
/****************************************************************/
struct c_INTCtag {
	union {						/* Port input 0/1 interrupt priority register */
		volatile struct {
			unsigned char	PP0L	: 3;	/* Port input 0 interrupt priority level */
			unsigned char	Dummy	: 1;
			unsigned char	PP1L	: 3;	/* Port input 1 interrupt priority level */
		} bPP01L;
		volatile unsigned char	ucPP01L;
	} rPP01L;

	union {						/* Port input 2/3 interrupt priority register */
		volatile struct {
			unsigned char	PP2L	: 3;	/* Port input 2 interrupt priority level */
			unsigned char	Dummy	: 1;
			unsigned char	PP3L	: 3;	/* Port input 3 interrupt priority level */
		} bPP23L;
		volatile unsigned char	ucPP23L;
	} rPP23L;

	union {						/* Key input 0/1 interrupt priority register */
		volatile struct {
			unsigned char	PK0L	: 3;	/* Key input 0 interrupt priority level */
			unsigned char	Dummy	: 1;
			unsigned char	PK1L	: 3;	/* Key input 1 interrupt priority level */
		} bPK01L;
		volatile unsigned char	ucPK01L;
	} rPK01L;

	union {						/* High-speed DMA Ch.0/1 interrupt priority register */
		volatile struct {
			unsigned char	PHSD0L	: 3;	/* High-speed DMA Ch.0 interrupt priority level */
			unsigned char	Dummy	: 1;
			unsigned char	PHSD1L	: 3;	/* High-speed DMA Ch.1 interrupt priority level */
		} bPHSD01L;
		volatile unsigned char	ucPHSD01L;
	} rPHSD01L;

	union {						/* High-speed DMA Ch.2/3 interrupt priority register */
		volatile struct {
			unsigned char	PHSD2L	: 3;	/* High-speed DMA Ch.2 interrupt priority level */
			unsigned char	Dummy	: 1;
			unsigned char	PHSD3L	: 3;	/* High-speed DMA Ch.3 interrupt priority level */
		} bPHSD23L;
		volatile unsigned char	ucPHSD23L;
	} rPHSD23L;

	volatile unsigned char	rPDM;			/* IDMA interrupt priority register */

	union {						/* 16-bit timer 0/1 interrupt priority register */
		volatile struct {
			unsigned char	P16T0	: 3;	/* 16-bit timer 0 interrupt priority level */
			unsigned char	Dummy	: 1;
			unsigned char	P16T1	: 3;	/* 16-bit timer 1 interrupt priority level */
		} bP16T01;
		volatile unsigned char	ucP16T01;
	} rP16T01;

	union {						/* 16-bit timer 2/3 interrupt priority register */
		volatile struct {
			unsigned char	P16T2	: 3;	/* 16-bit timer 2 interrupt priority level */
			unsigned char	Dummy	: 1;
			unsigned char	P16T3	: 3;	/* 16-bit timer 3 interrupt priority level */
		} bP16T23;
		volatile unsigned char	ucP16T23;
	} rP16T23;

	union {						/* 16-bit timer 4/5 interrupt priority register */
		volatile struct {
			unsigned char	P16T4	: 3;	/* 16-bit timer 4 interrupt priority level */
			unsigned char	Dummy	: 1;
			unsigned char	P16T5	: 3;	/* 16-bit timer 5 interrupt priority level */
		} bP16T45;
		volatile unsigned char	ucP16T45;
	} rP16T45;

	union {						/* 8-bit timer 0-3, serial I/F Ch.0 interrupt priority register */
		volatile struct {
			unsigned char	P8TM	: 3;	/* 8-bit timer 0-3 interrupt priority level */
			unsigned char	Dummy	: 1;
			unsigned char	PSIO0	: 3;	/* Serial I/F Ch.0 interrupt priority level */
		} bP8TM_PSIO0;
		volatile unsigned char	ucP8TM_PSIO0;
	} rP8TM_PSIO0;

	union {						/* Serial I/F Ch.1 and A/D converter interrupt priority register */
		volatile struct {
			unsigned char	PSIO1	: 3;	/* Serial I/F Ch.1 interrupt priority level */
			unsigned char	Dummy	: 1;
			unsigned char	PAD	: 3;	/* A/D converter interrupt priority level */
		} bPSIO1_PAD;
		volatile unsigned char	ucPSIO1_PAD;
	} rPSIO1_PAD;

	volatile unsigned char	rPCTM;			/* Clock timer interrupt priority register */

	union {						/* Port input 4/5 interrupt priority register */
		volatile struct {
			unsigned char	PP4L	: 3;	/* Port input 4 interrupt priority level */
			unsigned char	Dummy	: 1;
			unsigned char	PP5L	: 3;	/* Port input 5 interrupt priority level */
		} bPP45L;
		volatile unsigned char	ucPP45L;
	} rPP45L;

	union {						/* Port input 6/7 interrupt priority register */
		volatile struct {
			unsigned char	PP6L	: 3;	/* Port input 6 interrupt priority level */
			unsigned char	Dummy	: 1;
			unsigned char	PP7L	: 3;	/* Port input 7 interrupt priority level */
		} bPP67L;
		volatile unsigned char	ucPP67L;
	} rPP67L;

	unsigned char	Dummy0[2];

	union {						/* Key input, port input 0-3 interrupt request flag register */
		volatile struct {
			unsigned char	EP0	: 1;	/* Port input 0 */
			unsigned char	EP1	: 1;	/* Port input 1 */
			unsigned char	EP2	: 1;	/* Port input 2 */
			unsigned char	EP3	: 1;	/* Port input 3 */
			unsigned char	EK0	: 1;	/* Key input 0 */
			unsigned char	EK1	: 1;	/* Key input 1 */
		} bIEN;
		volatile unsigned char	ucIEN;
	} rIEN1;

	union {						/* DMA interrupt enable register */
		volatile struct {
			unsigned char	EHDM0	: 1;	/* High-speed DMA Ch.0 */
			unsigned char	EHDM1	: 1;	/* High-speed DMA Ch.1 */
			unsigned char	EHDM2	: 1;	/* High-speed DMA Ch.2 */
			unsigned char	EHDM3	: 1;	/* High-speed DMA Ch.3 */
			unsigned char	EIDMA	: 1;	/* IDMA */
		} bIEN;
		volatile unsigned char	ucIEN;
	} rIEN2;

	union {						/* 16-bit timer 0/1 interrupt enable register */
		volatile struct {
			unsigned char	Dummy0	: 2;	/* D1-0 reserved */
			unsigned char	E16TU0	: 1;	/* 16-bit timer 0 comparison B */
			unsigned char	E16TC0	: 1;	/* 16-bit timer 0 comparison A */
			unsigned char	Dummy1	: 2;	/* D5-4 reserved */
			unsigned char	E16TU1	: 1;	/* 16-bit timer 1 comparison B */
			unsigned char	E16TC1	: 1;	/* 16-bit timer 1 comparison A */
		} bIEN;
		volatile unsigned char	ucIEN;
	} rIEN3;

	union {						/* 16-bit timer 2/3 interrupt enable register */
		volatile struct {
			unsigned char	Dummy0	: 2;	/* D1-0 reserved */
			unsigned char	E16TU2	: 1;	/* 16-bit timer 2 comparison B */
			unsigned char	E16TC2	: 1;	/* 16-bit timer 2 comparison A */
			unsigned char	Dummy1	: 2;	/* D5-4 reserved */
			unsigned char	E16TU3	: 1;	/* 16-bit timer 3 comparison B */
			unsigned char	E16TC3	: 1;	/* 16-bit timer 3 comparison A */
		} bIEN;
		volatile unsigned char	ucIEN;
	} rIEN4;

	union {						/* 16-bit timer 4/5 interrupt enable register */
		volatile struct {
			unsigned char	Dummy0	: 2;	/* D1-0 reserved */
			unsigned char	E16TU4	: 1;	/* 16-bit timer 4 comparison B */
			unsigned char	E16TC4	: 1;	/* 16-bit timer 4 comparison A */
			unsigned char	Dummy1	: 2;	/* D5-4 reserved */
			unsigned char	E16TU5	: 1;	/* 16-bit timer 5 comparison B */
			unsigned char	E16TC5	: 1;	/* 16-bit timer 5 comparison A */
		} bIEN;
		volatile unsigned char	ucIEN;
	} rIEN5;

	union {						/* 8-bit timer interrupt enable register */
		volatile struct {
			unsigned char	E8TU0	: 1;	/* 8-bit timer 0 underflow */
			unsigned char	E8TU1	: 1;	/* 8-bit timer 1 underflow */
			unsigned char	E8TU2	: 1;	/* 8-bit timer 2 underflow */
			unsigned char	E8TU3	: 1;	/* 8-bit timer 3 underflow */
		} bIEN;
		volatile unsigned char	ucIEN;
	} rIEN6;

	union {						/* Serial I/F interrupt enable register */
		volatile struct {
			unsigned char	ESERR0	: 1;	/* Serial I/F Ch.0 receive error */
			unsigned char	ESRX0	: 1;	/* Serial I/F Ch.0 receive buffer full */
			unsigned char	ESTX0	: 1;	/* Serial I/F Ch.0 transmit buffer empty */
			unsigned char	ESERR1	: 1;	/* Serial I/F Ch.1 receive error */
			unsigned char	ESRX1	: 1;	/* Serial I/F Ch.1 receive buffer full */
			unsigned char	ESTX1	: 1;	/* Serial I/F Ch.1 transmit buffer empty */
		} bIEN;
		volatile unsigned char	ucIEN;
	} rIEN7;

	union {						/* Port input 4-7, clock timer, A/D interrupt enable register */
		volatile struct {
			unsigned char	EADE	: 1;	/* A/D converter */
			unsigned char	ECTM	: 1;	/* Clock timer */
			unsigned char	EP4	: 1;	/* Port input 4 */
			unsigned char	EP5	: 1;	/* Port input 5 */
			unsigned char	EP6	: 1;	/* Port input 6 */
			unsigned char	EP7	: 1;	/* Port input 7 */
		} bIEN;
		volatile unsigned char	ucIEN;
	} rIEN8;

	unsigned char	Dummy1[8];

	union {						/* Key input, port input 0-3 interrupt request flag register */
		volatile struct {
			unsigned char	FP0	: 1;	/* Port input 0 */
			unsigned char	FP1	: 1;	/* Port input 1 */
			unsigned char	FP2	: 1;	/* Port input 2 */
			unsigned char	FP3	: 1;	/* Port input 3 */
			unsigned char	FK0	: 1;	/* Key input 0 */
			unsigned char	FK1	: 1;	/* Key input 1 */
		} bISR;
		volatile unsigned char	ucISR;
	} rISR1;

	union {						/* DMA interrupt request flag register */
		volatile struct {
			unsigned char	FHDM0	: 1;	/* High-speed DMA Ch.0 */
			unsigned char	FHDM1	: 1;	/* High-speed DMA Ch.1 */
			unsigned char	FHDM2	: 1;	/* High-speed DMA Ch.2 */
			unsigned char	FHDM3	: 1;	/* High-speed DMA Ch.3 */
			unsigned char	FIDMA	: 1;	/* IDMA */
		} bISR;
		volatile unsigned char	ucISR;
	} rISR2;

	union {						/* 16-bit timer 0/1 interrupt request flag register */
		volatile struct {
			unsigned char	Dummy0	: 2;	/* D1-0 reserved */
			unsigned char	F16TU0	: 1;	/* 16-bit timer 0 comparison B */
			unsigned char	F16TC0	: 1;	/* 16-bit timer 0 comparison A */
			unsigned char	Dummy1	: 2;	/* D5-4 reserved */
			unsigned char	F16TU1	: 1;	/* 16-bit timer 1 comparison B */
			unsigned char	F16TC1	: 1;	/* 16-bit timer 1 comparison A */
		} bISR;
		volatile unsigned char	ucISR;
	} rISR3;

	union {						/* 16-bit timer 2/3 interrupt request flag register */
		volatile struct {
			unsigned char	Dummy2	: 2;	/* D1-0 reserved */
			unsigned char	F16TU2	: 1;	/* 16-bit timer 2 comparison B */
			unsigned char	F16TC2	: 1;	/* 16-bit timer 2 comparison A */
			unsigned char	Dummy3	: 2;	/* D5-4 reserved */
			unsigned char	F16TU3	: 1;	/* 16-bit timer 3 comparison B */
			unsigned char	F16TC3	: 1;	/* 16-bit timer 3 comparison A */
		} bISR;
		volatile unsigned char	ucISR;
	} rISR4;

	union {						/* 16-bit timer 4/5 interrupt request flag register */
		volatile struct {
			unsigned char	Dummy4	: 2;	/* D1-0 reserved */
			unsigned char	F16TU4	: 1;	/* 16-bit timer 4 comparison B */
			unsigned char	F16TC4	: 1;	/* 16-bit timer 4 comparison A */
			unsigned char	Dummy5	: 2;	/* D5-4 reserved */
			unsigned char	F16TU5	: 1;	/* 16-bit timer 5 comparison B */
			unsigned char	F16TC5	: 1;	/* 16-bit timer 5 comparison A */
		} bISR;
		volatile unsigned char	ucISR;
	} rISR5;

	union {						/* 8-bit timer interrupt request flag register */
		volatile struct {
			unsigned char	F8TU0	: 1;	/* 8-bit timer 0 underflow */
			unsigned char	F8TU1	: 1;	/* 8-bit timer 1 underflow */
			unsigned char	F8TU2	: 1;	/* 8-bit timer 2 underflow */
			unsigned char	F8TU3	: 1;	/* 8-bit timer 3 underflow */
		} bISR;
		volatile unsigned char	ucISR;
	} rISR6;

	union {						/* Serial I/F interrupt request flag register */
		volatile struct {
			unsigned char	FSERR0	: 1;	/* Serial I/F Ch.0 receive error */
			unsigned char	FSRX0	: 1;	/* Serial I/F Ch.0 receive buffer full */
			unsigned char	FSTX0	: 1;	/* Serial I/F Ch.0 transmit buffer empty */
			unsigned char	FSERR1	: 1;	/* Serial I/F Ch.1 receive error */
			unsigned char	FSRX1	: 1;	/* Serial I/F Ch.1 receive buffer full */
			unsigned char	FSTX1	: 1;	/* Serial I/F Ch.1 transmit buffer empty */
		} bISR;
		volatile unsigned char	ucISR;
	} rISR7;

	union {						/* Port input 4-7, clock timer, A/D interrupt request flag enable */
		volatile struct {
			unsigned char	FADE	: 1;	/* A/D converter */
			unsigned char	FCTM	: 1;	/* Clock timer */
			unsigned char	FP4	: 1;	/* Port input 4 */
			unsigned char	FP5	: 1;	/* Port input 5 */
			unsigned char	FP6	: 1;	/* Port input 6 */
			unsigned char	FP7	: 1;	/* Port input 7 */
		} bISR;
		volatile unsigned char	ucISR;
	} rISR8;

	unsigned char	Dummy2[8];

	union {						/* Port input 0-3, high-speed DMA, 16-bit timer 0 IDMA request register */
		volatile struct {
			unsigned char	RP0	: 1;	/* Port input 0 IDMA request */
			unsigned char	RP1	: 1;	/* Port input 1 IDMA request */
			unsigned char	RP2	: 1;	/* Port input 2 IDMA request */
			unsigned char	RP3	: 1;	/* Port input 3 IDMA request */
			unsigned char	RHDM0	: 1;	/* High-speed DMA Ch.0 IDMA request */
			unsigned char	RHDM1	: 1;	/* High-speed DMA Ch.1 IDMA request */
			unsigned char	R16TU0	: 1;	/* 16-bit timer 0 comparison B IDMA request */
			unsigned char	R16TC0	: 1;	/* 16-bit timer 0 comparison A IDMA request */
		} bIDMAREQ;
		volatile unsigned char	ucIDMAREQ;
	} rIDMAREQ1;

	union {						/* 16-bit timer 1-4 IDMA request register */
		volatile struct {
			unsigned char	R16TU1	: 1;	/* 16-bit timer 1 comparison B IDMA request */
			unsigned char	R16TC1	: 1;	/* 16-bit timer 1 comparison A IDMA request */
			unsigned char	R16TU2	: 1;	/* 16-bit timer 2 comparison B IDMA request */
			unsigned char	R16TC2	: 1;	/* 16-bit timer 2 comparison A IDMA request */
			unsigned char	R16TU3	: 1;	/* 16-bit timer 3 comparison B IDMA request */
			unsigned char	R16TC3	: 1;	/* 16-bit timer 3 comparison A IDMA request */
			unsigned char	R16TU4	: 1;	/* 16-bit timer 4 comparison B IDMA request */
			unsigned char	R16TC4	: 1;	/* 16-bit timer 4 comparison A IDMA request */
		} bIDMAREQ;
		volatile unsigned char	ucIDMAREQ;
	} rIDMAREQ2;

	union {						/* 16-bit timer 5, 8-bit timer, serial I/F Ch.0 IDMA request register */
		volatile struct {
			unsigned char	R16TU5	: 1;	/* 16-bit timer 5 comparison B IDMA request */
			unsigned char	R16TC5	: 1;	/* 16-bit timer 5 comparison A IDMA request */
			unsigned char	R8TU0	: 1;	/* 8-bit timer 0 underflow IDMA request */
			unsigned char	R8TU1	: 1;	/* 8-bit timer 1 underflow IDMA request */
			unsigned char	R8TU2	: 1;	/* 8-bit timer 2 underflow IDMA request */
			unsigned char	R8TU3	: 1;	/* 8-bit timer 3 underflow IDMA request */
			unsigned char	RSRX0	: 1;	/* Serial I/F Ch.0 receive buffer full IDMA request */
			unsigned char	RSTX0	: 1;	/* Serial I/F Ch.0 receive buffer empty IDMA request */
		} bIDMAREQ;
		volatile unsigned char	ucIDMAREQ;
	} rIDMAREQ3;

	union {						/* Serial I/F Ch.1, A/D, port input 4-7 IDMA request register */
		volatile struct {
			unsigned char	RSRX1	: 1;	/* Serial I/F Ch.1 receive buffer full IDMA request */
			unsigned char	RSTX1	: 1;	/* Serial I/F Ch.1 receive buffer empty IDMA request */
			unsigned char	RADE	: 1;	/* A/D converter conversion end IDMA request */
			unsigned char	Dummy	: 1;	/* D3 reserved */
			unsigned char	RP4	: 1;	/* Port input 4 IDMA request */
			unsigned char	RP5	: 1;	/* Port input 5 IDMA request */
			unsigned char	RP6	: 1;	/* Port input 6 IDMA request */
			unsigned char	RP7	: 1;	/* Port input 7 IDMA request */
		} bIDMAREQ;
		volatile unsigned char	ucIDMAREQ;
	} rIDMAREQ4;

	union {						/* Port input 0-3, high-speed DMA, 16-bit timer 0 IDMA enable register */
		volatile struct {
			unsigned char	DEP0	: 1;	/* Port input 0 IDMA enable */
			unsigned char	DEP1	: 1;	/* Port input 1 IDMA enable */
			unsigned char	DEP2	: 1;	/* Port input 2 IDMA enable */
			unsigned char	DEP3	: 1;	/* Port input 3 IDMA enable */
			unsigned char	DEHDM0	: 1;	/* High-speed DMA Ch.0 IDMA enable */
			unsigned char	DEHDM1	: 1;	/* High-speed DMA Ch.1 IDMA enable */
			unsigned char	DE16TU0	: 1;	/* 16-bit timer 0 comparison B IDMA enable */
			unsigned char	DE16TC0	: 1;	/* 16-bit timer 0 comparison A IDMA enable */
		} bIDMA_EN;
		volatile unsigned char	ucIDMA_EN;
	} rIDMA_EN1;

	union {						/* 16-bit timer 1-4 IDMA enable register */
		volatile struct {
			unsigned char	DE16TU1	: 1;	/* 16-bit timer 1 comparison B IDMA enable */
			unsigned char	DE16TC1	: 1;	/* 16-bit timer 1 comparison A IDMA enable */
			unsigned char	DE16TU2	: 1;	/* 16-bit timer 2 comparison B IDMA enable */
			unsigned char	DE16TC2	: 1;	/* 16-bit timer 2 comparison A IDMA enable */
			unsigned char	DE16TU3	: 1;	/* 16-bit timer 3 comparison B IDMA enable */
			unsigned char	DE16TC3	: 1;	/* 16-bit timer 3 comparison A IDMA enable */
			unsigned char	DE16TU4	: 1;	/* 16-bit timer 4 comparison B IDMA enable */
			unsigned char	DE16TC4	: 1;	/* 16-bit timer 4 comparison A IDMA enable */
		} bIDMA_EN;
		volatile unsigned char	ucIDMA_EN;
	} rIDMA_EN2;

	union {						/* 16-bit timer 5, 8-bit timer, serial I/F Ch.0 IDMA enable register */
		volatile struct {
			unsigned char	DE16TU5	: 1;	/* 16-bit timer 5 comparison B IDMA enable */
			unsigned char	DE16TC5	: 1;	/* 16-bit timer 5 comparison A IDMA enable */
			unsigned char	DE8TU0	: 1;	/* 8-bit timer 0 underflow IDMA enable */
			unsigned char	DE8TU1	: 1;	/* 8-bit timer 1 underflow IDMA enable */
			unsigned char	DE8TU2	: 1;	/* 8-bit timer 2 underflow IDMA enable */
			unsigned char	DE8TU3	: 1;	/* 8-bit timer 3 underflow IDMA enable */
			unsigned char	DESRX0	: 1;	/* Serial I/F Ch.0 receive buffer full IDMA enable */
			unsigned char	DESTX0	: 1;	/* Serial I/F Ch.0 receive buffer empty IDMA enable */
		} bIDMA_EN;
		volatile unsigned char	ucIDMA_EN;
	} rIDMA_EN3;

	union {						/* Serial I/F Ch.1, A/D, port input 4-7 IDMA enable register */
		volatile struct {
			unsigned char	DESRX1	: 1;	/* Serial I/F Ch.1 receive buffer full IDMA enable */
			unsigned char	DESTX1	: 1;	/* Serial I/F Ch.1 receive buffer empty IDMA enable */
			unsigned char	DEADE	: 1;	/* A/D converter IDMA enable */
			unsigned char	Dummy	: 1;	/* D3 reserved */
			unsigned char	DEP4	: 1;	/* Port input4 IDMA enable */
			unsigned char	DEP5	: 1;	/* Port input5 IDMA enable */
			unsigned char	DEP6	: 1;	/* Port input6 IDMA enable */
			unsigned char	DEP7	: 1;	/* Port input7 IDMA enable */
		} bIDMA_EN;
		volatile unsigned char	ucIDMA_EN;
	} rIDMA_EN4;

	union {						/* High-speed DMA Ch.0/1 trigger select register */
		volatile struct {
			unsigned char	HSD0S	: 4;	/* High-speed DMA Ch.0 trigger selection */
			unsigned char	HSD1S	: 4;	/* High-speed DMA Ch.1 trigger selection */
		} bHTGR;
		volatile unsigned char	ucHTGR;
	} rHTGR1;

	union {						/* High-speed DMA Ch.2/3 trigger select register */
		volatile struct {
			unsigned char	HSD2S	: 4;	/* High-speed DMA Ch.2 trigger selection */
			unsigned char	HSD3S	: 4;	/* High-speed DMA Ch.3 trigger selection */
		} bHTGR;
		volatile unsigned char	ucHTGR;
	} rHTGR2;

	union {						/* High-speed DMA software trigger select register */
		volatile struct {
			unsigned char	HST0	: 1;	/* High-speed DMA Ch.0 software trigger */
			unsigned char	HST1	: 1;	/* High-speed DMA Ch.1 software trigger */
			unsigned char	HST2	: 1;	/* High-speed DMA Ch.2 software trigger */
			unsigned char	HST3	: 1;	/* High-speed DMA Ch.3 software trigger */
		} bHSOFTTGR;
		volatile unsigned char	ucHSOFTTGR;
	} rHSOFTTGR;

	unsigned char	Dummy4[4];

	union {						/* Flag set/reset method select register */
		volatile struct {
			unsigned char	RSTONLY	: 1;	/* Interrupt factor flag reset method selection */
			unsigned char	IDMAONLY	: 1;	/* IDMA factor register set method selection */
			unsigned char	DENONLY	: 1;	/* IDMA enable register set method selection */
		} bRESET;
		volatile unsigned char	ucRESET;
	} rRESET;
};

/****************************************************************/
/* [Input port (K port)] c_KPORT				*/
/****************************************************************/
struct c_KPORTtag {
	union {						/* K5 function select register */
		volatile struct {
			unsigned char	CFK50	: 1;	/* K50 function selection */
			unsigned char	CFK51	: 1;	/* K51 function selection */
			unsigned char	CFK52	: 1;	/* K52 function selection */
			unsigned char	CFK53	: 1;	/* K53 function selection */
			unsigned char	CFK54	: 1;	/* K54 function selection */
		} bCFK5;
		volatile unsigned char	ucCFK5;
	} rCFK5;

	union {						/* K5 input port register */
		volatile struct {
			unsigned char	K50D	: 1;	/* K50 input port data */
			unsigned char	K51D	: 1;	/* K51 input port data */
			unsigned char	K52D	: 1;	/* K52 input port data */
			unsigned char	K53D	: 1;	/* K53 input port data */
			unsigned char	K54D	: 1;	/* K54 input port data */
		} bK5D;
		volatile unsigned char	ucK5D;
	} rK5D;

	unsigned char	Dummy;

	union {						/* K6 function select register */
		volatile struct {
			unsigned char	CFK60	: 1;	/* K60 function selection */
			unsigned char	CFK61	: 1;	/* K61 function selection */
			unsigned char	CFK62	: 1;	/* K62 function selection */
			unsigned char	CFK63	: 1;	/* K63 function selection */
			unsigned char	CFK64	: 1;	/* K64 function selection */
			unsigned char	CFK65	: 1;	/* K65 function selection */
			unsigned char	CFK66	: 1;	/* K66 function selection */
			unsigned char	CFK67	: 1;	/* K67 function selection */
		} bCFK6;
		volatile unsigned char	ucCFK6;
	} rCFK6;

	union {						/* K6 input port register */
		volatile struct {
			unsigned char	K60D	: 1;	/* K60 input port data */
			unsigned char	K61D	: 1;	/* K61 input port data */
			unsigned char	K62D	: 1;	/* K62 input port data */
			unsigned char	K63D	: 1;	/* K63 input port data */
			unsigned char	K64D	: 1;	/* K64 input port data */
			unsigned char	K65D	: 1;	/* K65 input port data */
			unsigned char	K66D	: 1;	/* K66 input port data */
			unsigned char	K67D	: 1;	/* K67 input port data */
		} bK6D;
		volatile unsigned char	ucK6D;
	} rK6D;
};

/****************************************************************/
/* [Port input interrupt] c_PINT				*/
/****************************************************************/
struct c_PINTtag {
	union {						/* Port input interrupt select register1 */
		volatile struct {
			unsigned char SPT0	: 2;	/* FPT0 interrupt input port selection */
			unsigned char SPT1	: 2;	/* FPT1 interrupt input port selection */
			unsigned char SPT2	: 2;	/* FPT2 interrupt input port selection */
			unsigned char SPT3	: 2;	/* FPT3 interrupt input port selection */
		} bSPT03;
		volatile unsigned char	ucSPT03;
	} rSPT1;

	union {						/* Port input interrupt select register2 */
		volatile struct {
			unsigned char SPT4	: 2;	/* FPT4 interrupt input port selection */
			unsigned char SPT5	: 2;	/* FPT5 interrupt input port selection */
			unsigned char SPT6	: 2;	/* FPT6 interrupt input port selection */
			unsigned char SPT7	: 2;	/* FPT7 interrupt input port selection */
		} bSPT47;
		volatile unsigned char	ucSPT47;
	} rSPT2;

	union {						/* Port input interrupt input polarity select register */
		volatile struct {
			unsigned char SPPT0	: 1;	/* FPT0 input polarity selection */
			unsigned char SPPT1	: 1;	/* FPT1 input polarity selection */
			unsigned char SPPT2	: 1;	/* FPT2 input polarity selection */
			unsigned char SPPT3	: 1;	/* FPT3 input polarity selection */
			unsigned char SPPT4	: 1;	/* FPT4 input polarity selection */
			unsigned char SPPT5	: 1;	/* FPT5 input polarity selection */
			unsigned char SPPT6	: 1;	/* FPT6 input polarity selection */
			unsigned char SPPT7	: 1;	/* FPT7 input polarity selection */
		} bSPPT;
		volatile unsigned char	ucSPPT;
	} rSPPT;

	union {						/* Port input interrupt edge/level select register */
		volatile struct {
			unsigned char SEPT0	: 1;	/* FPT0 edge/level selection */
			unsigned char SEPT1	: 1;	/* FPT1 edge/level selection */
			unsigned char SEPT2	: 1;	/* FPT2 edge/level selection */
			unsigned char SEPT3	: 1;	/* FPT3 edge/level selection */
			unsigned char SEPT4	: 1;	/* FPT4 edge/level selection */
			unsigned char SEPT5	: 1;	/* FPT5 edge/level selection */
			unsigned char SEPT6	: 1;	/* FPT6 edge/level selection */
			unsigned char SEPT7	: 1;	/* FPT7 edge/level selection */
		} bSEPT;
		volatile unsigned char	ucSEPT;
	} rSEPT;

	union {						/* Key input interrupt select register */
		volatile struct {
			unsigned char SPPK0	: 2;	/* FPK0 interrupt input port selection */
			unsigned char SPPK1	: 2;	/* FPK1 interrupt input port selection */
		} bSPPK;
		volatile unsigned char	ucSPPK;
	} rSPPK;

	unsigned char	Dummy;

	union {						/* Key input interrupt (FPK0) input comparison register */
		volatile struct {
			unsigned char SCPK00	: 1;	/* FPK00 input comparison */
			unsigned char SCPK01	: 1;	/* FPK01 input comparison */
			unsigned char SCPK02	: 1;	/* FPK02 input comparison */
			unsigned char SCPK03	: 1;	/* FPK03 input comparison */
			unsigned char SCPK04	: 1;	/* FPK04 input comparison */
		} bSCPK0;
		volatile unsigned char	ucSCPK0;
	} rSCPK0;

	union {						/* Key input interrupt (FPK1) input comparison register */
		volatile struct {
			unsigned char SCPK10	: 1;	/* FPK10 input comparison */
			unsigned char SCPK11	: 1;	/* FPK11 input comparison */
			unsigned char SCPK12	: 1;	/* FPK12 input comparison */
			unsigned char SCPK13	: 1;	/* FPK13 input comparison */
		} bSCPK1;
		volatile unsigned char	ucSCPK1;
	} rSCPK1;

	union {						/* Key input interrupt (FPK0) input mask register */
		volatile struct {
			unsigned char SMPK00	: 1;	/* FPK00 input mask */
			unsigned char SMPK01	: 1;	/* FPK01 input mask */
			unsigned char SMPK02	: 1;	/* FPK02 input mask */
			unsigned char SMPK03	: 1;	/* FPK03 input mask */
			unsigned char SMPK04	: 1;	/* FPK04 input mask */
		} bSMPK0;
		volatile unsigned char	ucSMPK0;
	} rSMPK0;

	union {						/* Key input interrupt (FPK1) input mask register */
		volatile struct {
			unsigned char SMPK10	: 1;	/* FPK10 input mask */
			unsigned char SMPK11	: 1;	/* FPK11 input mask */
			unsigned char SMPK12	: 1;	/* FPK12 input mask */
			unsigned char SMPK13	: 1;	/* FPK13 input mask */
		} bSMPK1;
		volatile unsigned char	ucSMPK1;
	} rSMPK1;
};

/****************************************************************/
/* [Input/output port (P port)]	c_PCTL				*/
/****************************************************************/
struct c_PCTLtag {
	union {						/* Function select register */
		volatile struct {
			unsigned char	CFP0	: 1;	/* PX0 */
			unsigned char	CFP1	: 1;	/* PX1 */
			unsigned char	CFP2	: 1;	/* PX2 */
			unsigned char	CFP3	: 1;	/* PX3 */
			unsigned char	CFP4	: 1;	/* PX4 */
			unsigned char	CFP5	: 1;	/* PX5 */
			unsigned char	CFP6	: 1;	/* PX6 */
			unsigned char	CFP7	: 1;	/* PX7 */
		} bCFP;
		volatile unsigned char	ucCFP;
	} rCFP;

	union {						/* Input/output port data register */
		volatile struct {
			unsigned char	P0D	: 1;	/* Bit0 */
			unsigned char	P1D	: 1;	/* Bit1 */
			unsigned char	P2D	: 1;	/* Bit2 */
			unsigned char	P3D	: 1;	/* Bit3 */
			unsigned char	P4D	: 1;	/* Bit4 */
			unsigned char	P5D	: 1;	/* Bit5 */
			unsigned char	P6D	: 1;	/* Bit6 */
			unsigned char	P7D	: 1;	/* Bit7 */
		} bPD;
		volatile unsigned char	ucPD;
	} rPD;

	union {						/* I/O control register */
		volatile struct {
			unsigned char	IOC0	: 1;	/* PX0 1:output 0:input */
			unsigned char	IOC1	: 1;	/* PX1 1:output 0:input */
			unsigned char	IOC2	: 1;	/* PX2 1:output 0:input */
			unsigned char	IOC3	: 1;	/* PX3 1:output 0:input */
			unsigned char	IOC4	: 1;	/* PX4 1:output 0:input */
			unsigned char	IOC5	: 1;	/* PX5 1:output 0:input */
			unsigned char	IOC6	: 1;	/* PX6 1:output 0:input */
			unsigned char	IOC7	: 1;	/* PX7 1:output 0:input */
		} bIOC;
		volatile unsigned char	ucIOC;
	} rIOC;

	union {						/* Port function extension register */
		volatile struct {
			unsigned char	CFEX0	: 1;	/* CFEX0 P12, P14 port extended function */
			unsigned char	CFEX1	: 1;	/* CFEX1 P10, P11, P13 port extended function */
			unsigned char	CFEX2	: 1;	/* CFEX2 P21 port extended function */
			unsigned char	CFEX3	: 1;	/* CFEX3 P31 port extended function */
			unsigned char	CFEX4	: 1;	/* CFEX4 P04 port extended function */
			unsigned char	CFEX5	: 1;	/* CFEX5 P05 port extended function */
			unsigned char	CFEX6	: 1;	/* CFEX6 P06 port extended function */
			unsigned char	CFEX7	: 1;	/* CFEX7 P07 port extended function */
		} bCFEX;
		volatile unsigned char	ucCFEX;
	} rCFEX;

};


/****************************************************************/
/* [BCU Area selection] c_BCUAREA				*/
/****************************************************************/
struct c_BCUAREAtag {
	union {						/* [Areas 18-15 set-up register]  rA18_15 */
		volatile struct {
			unsigned short	A16WT	: 3;	/* Areas 16-15 wait control */
			unsigned short	Dummy0	: 1;	/* D3 reserve */
			unsigned short	A16DF	: 2;	/* Areas 16-15 output disable delay time */
			unsigned short	A16SZ	: 1;	/* Areas 16-15 device size selection */
			unsigned short	Dummy1	: 1;	/* D7 reserve */
			unsigned short	A18WT	: 3;	/* Areas 18-17 wait control */
			unsigned short	Dummy2	: 1;	/* DB reserve */
			unsigned short	A18DF	: 2;	/* Areas 18-17 output disable delay time */
			unsigned short	A18SZ	: 1;	/* Areas 18-17 device size selection */
		} bCTL;
		volatile unsigned short	usCTL;
	} rA18_15;

	union {						/* [Areas 14-13 set-up register] rA14_13 */
		volatile struct {
			unsigned short	A14WT	: 3;	/* Areas 14-13 wait control */
			unsigned short	Dummy	: 1;	/* D3 reserve */
			unsigned short	A14DF	: 2;	/* Areas 14-13 output disable delay time */
			unsigned short	A14SZ	: 1;	/* Areas 14-13 device size selection */
			unsigned short	A13DRA	: 1;	/* Area 13 DRAM selection */
			unsigned short	A14DRA	: 1;	/* Area 14 DRAM selection */
		} bCTL;
		volatile unsigned short	usCTL;
	} rA14_13;

	union {						/* [Areas 12-11 set-up register] rA12_11 */
		volatile struct {
			unsigned short	A12WT	: 3;	/* Areas 12-11 wait control */
			unsigned short	Dummy	: 1;	/* D3 reserve */
			unsigned short	A12DF	: 2;	/* Areas 12-11 output disable delay time */
			unsigned short	A12SZ	: 1;	/* Areas 12-11 device size selection */
		} bCTL;
		volatile unsigned short	usCTL;
	} rA12_11;

	union {						/* [Areas 10-9 set-up register] rA10_9 */
		volatile struct {
			unsigned short	A10WT	: 3;	/* Areas 10-9 wait control */
			unsigned short	Dummy0	: 1;	/* D3 reserve */
			unsigned short	A10DF	: 2;	/* Areas 10-9 output disable delay time */
			unsigned short	A10SZ	: 1;	/* Areas 10-9 device size selection */
			unsigned short	A9DRA	: 1;	/* Area 9 burst ROM selection */
			unsigned short	A10DRA	: 1;	/* Area 10 burst ROM selection */
			unsigned short	A10BW	: 2;	/* Areas 10-9 burst ROM burst read cycle wait control */
			unsigned short	Dummy1	: 1;	/* D7 reserve */
			unsigned short	A10IR	: 3;	/* Area 10 internal ROM capacity selection */
		} bCTL;
		volatile unsigned short	usCTL;
	} rA10_9;

	union {						/* [Areas 8-7 set-up register] rA8_7 */
		volatile struct {
			unsigned short	A8WT	: 3;	/* Areas 8-7 wait control */
			unsigned short	Dummy	: 1;	/* D3 reserve */
			unsigned short	A8DF	: 2;	/* Areas 8-7 output disable delay time */
			unsigned short	A8SZ	: 1;	/* Areas 8-7 device size selection */
			unsigned short	A7DRA	: 1;	/* Area 7 DRAM selection */
			unsigned short	A8DRA	: 1;	/* Area 8 DRAM selection */
		} bCTL;
		volatile unsigned short	usCTL;
	} rA8_7;

	union {						/* [Areas 6-4 set-up register] rA6_4 */
		volatile struct {
			unsigned short	A5WT	: 3;	/* Areas 5-4 wait control */
			unsigned short	Dummy0	: 1;	/* D3 reserve */
			unsigned short	A5DF	: 2;	/* Areas 5-4 output disable delay time */
			unsigned short	A5SZ	: 1;	/* Areas 5-4 device size selection */
			unsigned short	Dummy1	: 1;	/* D7 reserve */
			unsigned short	A6WT	: 3;	/* Area 6 wait control */
			unsigned short	Dummy2	: 1;	/* DB reserve */
			unsigned short	A6DF	: 2;	/* Area 6 output disable delay time */
		} bCTL;
		volatile unsigned short	usCTL;
	} rA6_4;

	unsigned char	Dummy0;

	volatile unsigned char	rTBRP;			/* TTBR write protection register */

	union {						/* [Bus control register] rBUS */
		volatile struct {
			unsigned short	SWAITE	: 1;	/* #WAIT enable */
			unsigned short	SEPD	: 1;	/* External power-down control */
			unsigned short	SEMAS	: 1;	/* External bus master set-up */
			unsigned short	SBUSST	: 1;	/* External interface method selection */
			unsigned short	S2WE	: 1;	/* DRAM interface method selection */
			unsigned short	RRA	: 2;	/* Refresh RAS pulse width selection */
			unsigned short	RPC0	: 1;	/* Refresh RPC delay set-up */
			unsigned short	RPC1	: 1;	/* Refresh method selection */
			unsigned short	RPC2	: 1;	/* Refresh enable */
			unsigned short	RCA	: 2;	/* Column address size selection */
			unsigned short	REDO	: 1;	/* DRAM page mode selection */
			unsigned short	RBST8	: 1;	/* Burst ROM burst mode selection */
			unsigned short	Dummy	: 1;	/* DE reserve */
			unsigned short	RBCLK	: 1;	/* BCLK output control */
		} bCTL;
		volatile unsigned short	usCTL;
	} rBUS;

	union {						/* [DRAM timing set-up register] rDRAM */
		volatile struct {
			unsigned short	RASC	: 2;	/* DRAM RAS cycle selection */
			unsigned short	Dummy0	: 1;	/* Reserve */
			unsigned short	CASC	: 2;	/* DRAM CAS cycle selection */
			unsigned short	Dummy1	: 1;	/* Reserve */
			unsigned short	RPRC	: 2;	/* DRAM RAS precharge cycle selection */
			unsigned short	CRAS	: 1;	/* Successive RAS mode */
			unsigned short	CEFUNC	: 2;	/* #CE pin function selection */
			unsigned short	A3EEN	: 1;	/* Area 3 emulation */
		} bCTL;
		volatile unsigned short	usCTL;
	} rDRAM;

	union {						/* [Access control register] rACCESS */
		volatile struct {
			unsigned short	A5EC	: 1;	/* Area 5,4 endian control */
			unsigned short	A6EC	: 1;	/* Area 6 endian control */
			unsigned short	A8EC	: 1;	/* Area 8,7 endian control */
			unsigned short	A10EC	: 1;	/* Area 10,9 endian control */
			unsigned short	A12EC	: 1;	/* Area 12,11 endian control */
			unsigned short	A14EC	: 1;	/* Area 14,13 endian control */
			unsigned short	A16EC	: 1;	/* Area 16,15 endian control */
			unsigned short	A18EC	: 1;	/* Area 18,17 endian control */
			unsigned short	A5IO	: 1;	/* Area 5,4 external/internal access */
			unsigned short	A6IO	: 1;	/* Area 6 external/internal access */
			unsigned short	A8IO	: 1;	/* Area 8,7 external/internal access */
			unsigned short	Dummy	: 1;	/* DB reserve */
			unsigned short	A12IO	: 1;	/* Area 12.11 external/internal access */
			unsigned short	A14IO	: 1;	/* Area 14,13 external/internal access */
			unsigned short	A16IO	: 1;	/* Area 16,15 external/internal access */
			unsigned short	A18IO	: 1;	/* Area 18,17 external/internal access */
		} bCTL;
		volatile unsigned short	usCTL;
	} rACCESS;

	volatile unsigned int	rTTBR;			/* Trap table base address register */

	union {						/* [G/A read signal control register] rGA */
		volatile struct {
			unsigned short	A5RD	: 1;	/* Area 5,4 read signal */
			unsigned short	A6RD	: 1;	/* Area 6 read signal */
			unsigned short	A8RD	: 1;	/* Area 8,7 read signal */
			unsigned short	Dummy0	: 1;	/* D3 reserve */
			unsigned short	A12RD	: 1;	/* Area 12,11 read signal */
			unsigned short	A14RD	: 1;	/* Area 14,13 read signal */
			unsigned short	A16RD	: 1;	/* Area 16,15 read signal */
			unsigned short	A18RD	: 1;	/* Area 18,17 read signal */
			unsigned short	A5AS	: 1;	/* Area 5,4 address strobe signal */
			unsigned short	A6AS	: 1;	/* Area 6 address strobe signal */
			unsigned short	A8AS	: 1;	/* Area 8,7 address strobe signal */
			unsigned short	Dummy1	: 1;	/* DB reserve */
			unsigned short	A12AS	: 1;	/* Area 12,11 address strobe signal */
			unsigned short	A14AS	: 1;	/* Area 14,13 address strobe signal */
			unsigned short	A16AS	: 1;	/* Area 16,15 address strobe signal */
			unsigned short	A18AS	: 1;	/* Area 18,17 address strobe signal */
		} bCTL;
		volatile unsigned short	usCTL;
	} rGA;

	volatile unsigned char	rBCLKSEL;		/* BCLK select register */

	unsigned char	Dummy1;
};

/****************************************************************/
/* [16-bit timer] c_T16						*/
/****************************************************************/
struct c_T16tag {
	volatile unsigned short	rCRA;			/* 16-bit timer comparison data A set-up register */
	volatile unsigned short	rCRB;			/* 16-bit timer comparison data B set-up register */
	volatile unsigned short	rTC;			/* 16-bit timer counter data register */

	union {						/* 16-bit timer control register */
		volatile struct {
			unsigned char	PRUN	: 1;	/* 16-bit timer Run/Stop control */
			unsigned char	PRESET	: 1;	/* 16-bit timer Reset */
			unsigned char	PTM	: 1;	/* 16-bit timer clock output control */
			unsigned char	CKSL	: 1;	/* 16-bit timer input clock selection */
			unsigned char	OUTINV	: 1;	/* 16-bit timer output inversion */
			unsigned char	SELCRB	: 1;	/* 16-bit timer comparison buffer */
			unsigned char	SELFM	: 1;	/* 16-bit timer fine mode selection */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rT16CTL;
	unsigned char	Dummy;
};

/****************************************************************/
/* [IDMA control register] c_IDMA				*/
/****************************************************************/
struct c_IDMAtag {
	volatile unsigned int	rDBASE;			/* IDMA base address */

	union {						/* IDMA start register */
		volatile struct {
			unsigned char	DCHN	: 7;	/* IDMA channel number */
			unsigned char	DSTART	: 1;	/* IDMA start */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rSTART;

	union {						/* IDMA enable register */
		volatile struct {
			unsigned char	IDMAEN	: 1;	/* IDMA enable */
		} bCTL;
		volatile unsigned char	ucCTL;
	} rIDMAEN;

	unsigned char	Dummy[2];
};

/****************************************************************/
/* [High-speed DMA control register] c_HSDMA			*/
/****************************************************************/
struct c_HSDMAtag {
	union {						/* High-speed DMA transfer counter/control register */
		/* Block transfer mode */
		volatile struct {
			unsigned int	BLKLEN	: 8;	/* Block length */
			unsigned int	TC	: 16;	/* Transfer counter[15:0] */
			unsigned int	Dummy	: 6;	/* DD-8 reserve */
			unsigned int	DIR	: 1;	/* Transfer direction control (dual address mode) */
			unsigned int	DUALM	: 1;	/* Address mode selection */
		} bBLOCK;

		/* Single/successive transfer mode */
		volatile struct {
			unsigned int	TC	: 24;	/* Transfer counter[23:0] */
			unsigned int	Dummy	: 6;	/* DD-8 reserve */
			unsigned int	DIR	: 1;	/* Transfer direction control (single address mode) */
			unsigned int	DUALM	: 1;	/* Address mode selection */
		} bSINGLE;

		volatile unsigned int	uiCTL;
	} rCNT;

	union {						/* High-speed DMA source address set-up register */
		volatile struct {			/* (D):dual address mode, (S):single address mode */
			unsigned int	SADR	: 28;	/* Source address control(D)/memory address control(S) [27:0] */
			unsigned int	SIN	: 2;	/* Source address control(D)/memory address control(S) */
			unsigned int	DATSIZE	: 1;	/* Transfer data size */
			unsigned int	DINTEN	: 1;	/* Interrupt enable */
		} bCTL;
		volatile unsigned int	uiCTL;
	} rSADR;

	union {						/* High-speed DMA destination set-up register */
		volatile struct {
			unsigned int	DADR	: 28;	/* Destination address(D) address control [27:0] */
			unsigned int	DIN	: 2;	/* Destination address(D) address control */
			unsigned int	DMOD	: 2;	/* Transfer mode */
		} bCTL;
		volatile unsigned int	uiCTL;
	} rDADR;

	union {						/* High-speed DMA enable register */
		volatile struct {
			unsigned short	HS_EN	: 1;	/* High-speed DMA enable */
		} bCTL;
		volatile unsigned short	usCTL;
	} rHS_EN;

	union {						/* High-speed DMA trigger flag register */
		volatile struct {
			unsigned short	HS_TF	: 1;	/* Trigger flag clear(WR)/trigger flag status(RD) */
		} bCTL;
		volatile unsigned short	usCTL;
	} rTF;
};


/****************************************************************/
/* Address map I/O						*/
/****************************************************************/
struct c_IOtag {
	/*** 40000H *********************************************/
	unsigned char		Dummy0a[320];		/* 0x4013f - 0x40000 */

	union c_CLKSELtag	c_CLKSEL_T8_45;		/* 8-bit timer4/5 clock selection register */

	unsigned char		Dummy0b[4];		/* 0x40144 - 0x40141 */

	union c_CLKCTLtag	c_CLKCTL_T8_45;		/* 8-bit timer4/5 clock control register */

	union c_CLKSELtag	c_CLKSEL_T8;		/* 8-bit timer clock selection register */

	union c_CLKCTLtag	c_CLKCTL_T16_0;		/* 16-bit timer0 clock control register */
	union c_CLKCTLtag	c_CLKCTL_T16_1;		/* 16-bit timer1 clock control register */
	union c_CLKCTLtag	c_CLKCTL_T16_2;		/* 16-bit timer2 clock control register */
	union c_CLKCTLtag	c_CLKCTL_T16_3;		/* 16-bit timer3 clock control register */
	union c_CLKCTLtag	c_CLKCTL_T16_4;		/* 16-bit timer4 clock control register */
	union c_CLKCTLtag	c_CLKCTL_T16_5;		/* 16-bit timer5 clock control register */

	union c_CLKCTLtag	c_CLKCTL_T8_01;		/* 8-bit timer0/1 clock control register */
	union c_CLKCTLtag	c_CLKCTL_T8_23;		/* 8-bit timer2/3 clock control register */

	union c_CLKCTLtag	c_CLKCTL_AD;		/* A/D converter clock control register	*/

	unsigned char		Dummy1[1];		/* 0x40150 */

	struct c_TIMERtag	c_TIMER;		/* Clock timer */

	unsigned char		Dummy2[4];		/* 0x4015f - 0x4015c */

	struct c_T8tag		c_T8_0;			/* 8-bit timer 0 */
	struct c_T8tag		c_T8_1;			/* 8-bit timer 1 */
	struct c_T8tag		c_T8_2;			/* 8-bit timer 2 */
	struct c_T8tag		c_T8_3;			/* 8-bit timer 3 */

	struct c_WDTtag		c_WDT;			/* Watchdog timer */

	unsigned char		Dummy3[14];		/* 0x4017f - 0x40172 */

	struct c_PWRCTLtag	c_PWRCTL;		/* Power control */

	unsigned char		Dummy4[65];		/* 0x401df - 0x4019f */

	struct c_SIFtag		c_SIF0;			/* Serial I/F Ch.0 */
	struct c_SIFtag		c_SIF1;			/* Serial I/F Ch.1 */

	unsigned char		Dummy5a[6];		/* 0x401ef - 0x401ea */

	struct c_SIFtag		c_SIF2;			/* Serial I/F Ch.2 */
	struct c_SIFtag		c_SIF3;			/* Serial I/F Ch.3 */

	unsigned char		Dummy5b[70];		/* 0x4023f - 0x401fa */

	struct c_ADtag		c_AD;			/* A/D converter */

	unsigned char		Dummy6[26];		/* 0x4025f - 0x40246 */

	struct c_INTCtag	c_INTC;			/* Interrupt controller */

	unsigned char		Dummy7[32];		/* 0x402bf - 0x402a0 */

	struct c_KPORTtag	c_KPORT;		/* Input port (K port) */

	unsigned char		Dummy8[1];		/* 0x402c5 */

	struct c_PINTtag	c_PINT;			/* Port input interrupt */

	struct c_PCTLtag	c_PCTL_P0;		/* Input/output port (P0 port) */
	struct c_PCTLtag	c_PCTL_P1;		/* Input/output port (P1 port) */
	struct c_PCTLtag	c_PCTL_P2;		/* Input/output port (P2 port) */
	struct c_PCTLtag	c_PCTL_P3;		/* Input/output port (P3 port) */

	unsigned char		Dummy9[32320];		/* 0x4811f - 0x402e0 */

	/*** 48000H *********************************************/

	struct c_BCUAREAtag	c_BCUAREA;		/* BCU Area set-up */

	unsigned char		Dummy10[68];		/* 0x4817f - 0x4813c */

	struct c_T16tag		c_T16_0;		/* 16-bit timer 0 */
	struct c_T16tag		c_T16_1;		/* 16-bit timer 1 */
	struct c_T16tag		c_T16_2;		/* 16-bit timer 2 */
	struct c_T16tag		c_T16_3;		/* 16-bit timer 3 */
	struct c_T16tag		c_T16_4;		/* 16-bit timer 4 */
	struct c_T16tag		c_T16_5;		/* 16-bit timer 5 */

	unsigned char		Dummy11[80];		/* 0x481ff - 0x481b0 */

	struct c_IDMAtag	c_IDMA;			/* IDMA control register */

	unsigned char		Dummy12[24];		/* 0x4821f - 0x48208 */	/* Dummy (2byte) in c_IDMA */

	struct c_HSDMAtag	c_HSDMA0;		/* High-speed DMA Ch.0 */
	struct c_HSDMAtag	c_HSDMA1;		/* High-speed DMA Ch.1 */
	struct c_HSDMAtag	c_HSDMA2;		/* High-speed DMA Ch.2 */
	struct c_HSDMAtag	c_HSDMA3;		/* High-speed DMA Ch.3 */
};


/****************************************************************/
/*	Intelligent DMA(IDMA) control structure			*/
/****************************************************************/
typedef struct {
	union {
		volatile struct {
			unsigned int	BLKLEN	: 8;	/* Block size */
			unsigned int	TC	: 16;	/* Data transfer counter */
			unsigned int	LINKCHN	: 7;	/* IDMA link field */
			unsigned int	LINKEN	: 1;	/* IDMA link enable */
		} bBLK;

		volatile struct {
			unsigned int	TC	: 24;	/* Data transfer counter */
			unsigned int	LINKCHN	: 7;	/* IDMA link field */
			unsigned int	LINKEN	: 1;	/* IDAM link enable */
		} bSIN;

		volatile unsigned int	uiTC;
	} rTC;

	union {
		volatile struct {
			unsigned int	SRADR	: 28;	/* Source address */
			unsigned int	SRINC	: 2;	/* Source address control */
			unsigned int	DATSIZ	: 1;	/* Data size control */
			unsigned int	DINTEN	: 1;	/* Interrupt enable */
		} bSRC;

		volatile unsigned int	uiSRC;
	} rSRC;

	union {
		volatile struct {
			unsigned int	DSADR	: 28;	/* Destination address */
			unsigned int	DSINC	: 2;	/* Destination control */
			unsigned int	DMOD	: 2;	/* Transfer mode */
		} bDST;

		volatile unsigned int	uiDST;
	} rDST;
} IDMA_ST;

struct IDMA_ST_tag {
	IDMA_ST	c_IDMA00;				/* Reserve */
	IDMA_ST	c_IDMA01;				/* Port input interrupt 0 */
	IDMA_ST	c_IDMA02;				/* Port input interrupt 1 */
	IDMA_ST	c_IDMA03;				/* Port input interrupt 2 */
	IDMA_ST	c_IDMA04;				/* Port input interrupt 3 */
	IDMA_ST	c_IDMA05;				/* High-speed DMA Ch.0 transfer end */
	IDMA_ST	c_IDMA06;				/* High-speed DMA Ch.1 transfer end */
	IDMA_ST	c_IDMA07;				/* 16-bit timer Ch.0 compare B (overflow) */
	IDMA_ST	c_IDMA08;				/* 16-bit timer Ch.0 compare A */
	IDMA_ST	c_IDMA09;				/* 16-bit timer Ch.1 compare B (overflow) */
	IDMA_ST	c_IDMA0A;				/* 16-bit timer Ch.1 compare A */
	IDMA_ST	c_IDMA0B;				/* 16-bit timer Ch.2 compare B (overflow) */
	IDMA_ST	c_IDMA0C;				/* 16-bit timer Ch.2 compare A */
	IDMA_ST	c_IDMA0D;				/* 16-bit timer Ch.3 compare B (overflow) */
	IDMA_ST	c_IDMA0E;				/* 16-bit timer Ch.3 compare A */
	IDMA_ST	c_IDMA0F;				/* 16-bit timer Ch.4 compare B (overflow) */
	IDMA_ST	c_IDMA10;				/* 16-bit timer Ch.4 compare A */
	IDMA_ST	c_IDMA11;				/* 16-bit timer Ch.5 compare B (overflow) */
	IDMA_ST	c_IDMA12;				/* 16-bit timer Ch.5 compare A */
	IDMA_ST	c_IDMA13;				/* 8-bit timer Ch.0 (underflow) */
	IDMA_ST	c_IDMA14;				/* 8-bit timer Ch.1 (underflow) */
	IDMA_ST	c_IDMA15;				/* 8-bit timer Ch.2 (underflow) */
	IDMA_ST	c_IDMA16;				/* 8-bit timer Ch.3 (underflow) */
	IDMA_ST	c_IDMA17;				/* Serial IF Ch.0 receive buffer full */
	IDMA_ST	c_IDMA18;				/* Serial IF Ch.0 transmit buffer empty */
	IDMA_ST	c_IDMA19;				/* Serial IF Ch.1 receive buffer full */
	IDMA_ST	c_IDMA1A;				/* Serial IF Ch.1 transmit buffer empty */
	IDMA_ST	c_IDMA1B;				/* A/D conversion end */
	IDMA_ST	c_IDMA1C;				/* Port input interrupt 4 */
	IDMA_ST	c_IDMA1D;				/* Port input interrupt 5 */
	IDMA_ST	c_IDMA1E;				/* Port input interrupt 6 */
	IDMA_ST	c_IDMA1F;				/* Port input interrupt 7 */
	IDMA_ST	c_IDMA20;				/* Software run or link run 1 */
	IDMA_ST	c_IDMA21;				/* Software run or link run 2 */
	IDMA_ST	c_IDMA22;				/* Software run or link run 3 */
	IDMA_ST	c_IDMA23;				/* Software run or link run 4 */
	IDMA_ST	c_IDMA24;				/* Software run or link run 5 */
	IDMA_ST	c_IDMA25;				/* Software run or link run 6 */
	IDMA_ST	c_IDMA26;				/* Software run or link run 7 */
	IDMA_ST	c_IDMA27;				/* Software run or link run 8 */
};

/* IDMA, high-speed DMA define */
#define DIN_FIXED	0x0				/* Memory address control fixed */
#define DIN_DEC		0x1				/* Memory address control decrement (no init.) */
#define DIN_INC_INIT	0x2				/* Memory address control increment (init.) */
#define DIN_INC_NOINIT	0x3				/* Memory address control increment (no init.) */

#define DMOD_SINGLE	0x0				/* DMA transfer mode single */
#define DMOD_SUCCESSIVE	0x1				/* DMA transfer mode successive */
#define DMOD_BLOCK	0x2				/* DMA transfer mode block */


/****************************************************************/
/*	I/O memory register define				*/
/****************************************************************/

////////////////////////////////////////////////////////////////
// 8-bit timer 4/5 clock selection register
#define pCLKSEL_T8_45		plc_IO->c_CLKSEL_T8_45.ucCTL	/* 8-bit timer 4/5 clock select register (0x40140) */
#define fCLKSEL_T8_45		plc_IO->c_CLKSEL_T8_45.bCTL	/* 8-bit timer 4/5 clock select register bit field */
#define bCLKSEL_T8_45_P8TPCK4	fCLKSEL_T8_45.P8TPCK0		/* 8-bit timer 4 clock selection */
#define bCLKSEL_T8_45_P8TPCK5	fCLKSEL_T8_45.P8TPCK1		/* 8-bit timer 5 clock selection */

////////////////////////////////////////////////////////////////
// 8-bit timer 4/5 clock control register
#define pCLKCTL_T8_45		plc_IO->c_CLKCTL_T8_45.ucCTL	/* 8-bit timer 4/5 clock control register (0x40145) */
#define fCLKCTL_T8_45		plc_IO->c_CLKCTL_T8_45.bCTL	/* 8-bit timer 4/5 clock control register bit field */
#define bCLKCTL_T8_45_P8TS4	fCLKCTL_T8_45.TSA		/* 8-bit timer 4 clock division ratio selection */
#define bCLKCTL_T8_45_P8TON4	fCLKCTL_T8_45.TONA		/* 8-bit timer 4 clock control */
#define bCLKCTL_T8_45_P8TS5	fCLKCTL_T8_45.TSB		/* 8-bit timer 5 clock division ratio selection */
#define bCLKCTL_T8_45_P8TON5	fCLKCTL_T8_45.TONB		/* 8-bit timer 5 clock control */

////////////////////////////////////////////////////////////////
// 8-bit timer clock selection register
#define pCLKSEL_T8		plc_IO->c_CLKSEL_T8.ucCTL	/* 8-bit timer clock select register (0x40146) */
#define fCLKSEL_T8		plc_IO->c_CLKSEL_T8.bCTL	/* 8-bit timer clock select register bit field */
#define bCLKSEL_T8_P8TPCK0	fCLKSEL_T8.P8TPCK0		/* 8-bit timer 0 clock selection */
#define bCLKSEL_T8_P8TPCK1	fCLKSEL_T8.P8TPCK1		/* 8-bit timer 1 clock selection */
#define bCLKSEL_T8_P8TPCK2	fCLKSEL_T8.P8TPCK2		/* 8-bit timer 2 clock selection */
#define bCLKSEL_T8_P8TPCK3	fCLKSEL_T8.P8TPCK3		/* 8-bit timer 3 clock selection */

////////////////////////////////////////////////////////////////
// 16-bit timer clock control register
#define pCLKCTL_T16_0		plc_IO->c_CLKCTL_T16_0.ucCTL	/* 16-bit timer 0 clock control register (0x40147) */
#define fCLKCTL_T16_0		plc_IO->c_CLKCTL_T16_0.bCTL	/* 16-bit timer 0 clock control register bit field */
#define bCLKCTL_T16_0_P16TS0	fCLKCTL_T16_0.TSA		/* 16-bit timer 0 clock division ratio selection */
#define bCLKCTL_T16_0_P16TON0	fCLKCTL_T16_0.TONA		/* 16-bit timer 0 clock control */

#define pCLKCTL_T16_1		plc_IO->c_CLKCTL_T16_1.ucCTL	/* 16-bit timer 1 clock control register (0x40148) */
#define fCLKCTL_T16_1		plc_IO->c_CLKCTL_T16_1.bCTL	/* 16-bit timer 1 clock control register bit field */
#define bCLKCTL_T16_1_P16TS1	fCLKCTL_T16_1.TSA		/* 16-bit timer 1 clock division ratio selection */
#define bCLKCTL_T16_1_P16TON1	fCLKCTL_T16_1.TONA		/* 16-bit timer 1 clock control */

#define pCLKCTL_T16_2		plc_IO->c_CLKCTL_T16_2.ucCTL	/* 16-bit timer 2 clock control register (0x40149) */
#define fCLKCTL_T16_2		plc_IO->c_CLKCTL_T16_2.bCTL	/* 16-bit timer 2 clock control register bit field */
#define bCLKCTL_T16_2_P16TS2	fCLKCTL_T16_2.TSA		/* 16-bit timer 2 clock division ratio selection */
#define bCLKCTL_T16_2_P16TON2	fCLKCTL_T16_2.TONA		/* 16-bit timer 2 clock control */

#define pCLKCTL_T16_3		plc_IO->c_CLKCTL_T16_3.ucCTL	/* 16-bit timer 3 clock control register (0x4014a) */
#define fCLKCTL_T16_3		plc_IO->c_CLKCTL_T16_3.bCTL	/* 16-bit timer 3 clock control register bit field */
#define bCLKCTL_T16_3_P16TS3	fCLKCTL_T16_3.TSA		/* 16-bit timer 3 clock division ratio selection */
#define bCLKCTL_T16_3_P16TON3	fCLKCTL_T16_3.TONA		/* 16-bit timer 3 clock control */

#define pCLKCTL_T16_4		plc_IO->c_CLKCTL_T16_4.ucCTL	/* 16-bit timer 4 clock control register (0x4014b) */
#define fCLKCTL_T16_4		plc_IO->c_CLKCTL_T16_4.bCTL	/* 16-bit timer 4 clock control register bit field */
#define bCLKCTL_T16_4_P16TS4	fCLKCTL_T16_4.TSA		/* 16-bit timer 4 clock division ratio selection */
#define bCLKCTL_T16_4_P16TON4	fCLKCTL_T16_4.TONA		/* 16-bit timer 4 clock control */

#define pCLKCTL_T16_5		plc_IO->c_CLKCTL_T16_5.ucCTL	/* 16-bit timer 5 clock control register (0x4014c) */
#define fCLKCTL_T16_5		plc_IO->c_CLKCTL_T16_5.bCTL	/* 16-bit timer 5 clock control register bit field */
#define bCLKCTL_T16_5_P16TS5	fCLKCTL_T16_5.TSA		/* 16-bit timer 5 clock division ratio selection */
#define bCLKCTL_T16_5_P16TON5	fCLKCTL_T16_5.TONA		/* 16-bit timer 5 clock control */

////////////////////////////////////////////////////////////////
// 8-bit timer clock control register
#define pCLKCTL_T8_01		plc_IO->c_CLKCTL_T8_01.ucCTL	/* 8-bit timer 0/1 clock control register (0x4014d) */
#define fCLKCTL_T8_01		plc_IO->c_CLKCTL_T8_01.bCTL	/* 8-bit timer 0/1 clock control register bit field */
#define bCLKCTL_T8_01_P8TS0	fCLKCTL_T8_01.TSA		/* 8-bit timer 0 clock division ratio selection */
#define bCLKCTL_T8_01_P8TON0	fCLKCTL_T8_01.TONA		/* 8-bit timer 0 clock control */
#define bCLKCTL_T8_01_P8TS1	fCLKCTL_T8_01.TSB		/* 8-bit timer 1 clock division ratio selection */
#define bCLKCTL_T8_01_P8TON1	fCLKCTL_T8_01.TONB		/* 8-bit timer 1 clock control */

#define pCLKCTL_T8_23		plc_IO->c_CLKCTL_T8_23.ucCTL	/* 8-bit timer 2/3 clock control register (0x4014e) */
#define fCLKCTL_T8_23		plc_IO->c_CLKCTL_T8_23.bCTL	/* 8-bit timer 2/3 clock control register bit field */
#define bCLKCTL_T8_23_P8TS2	fCLKCTL_T8_23.TSA		/* 8-bit timer 2 clock division ratio selection */
#define bCLKCTL_T8_23_P8TON2	fCLKCTL_T8_23.TONA		/* 8-bit timer 2 clock control */
#define bCLKCTL_T8_23_P8TS3	fCLKCTL_T8_23.TSB		/* 8-bit timer 3 clock division ratio selection */
#define bCLKCTL_T8_23_P8TON3	fCLKCTL_T8_23.TONB		/* 8-bit timer 3 clock control */

////////////////////////////////////////////////////////////////
// A/D converter clock control register
#define pCLKCTL_AD		plc_IO->c_CLKCTL_AD.ucCTL	/* A/D converter clock control register (0x4014f) */
#define fCLKCTL_AD		plc_IO->c_CLKCTL_AD.bCTL	/* A/D converter clock control register bit field */
#define bCLKCTL_AD_PSAD		fCLKCTL_AD.TSA			/* A/D converter clock division ratio selection */
#define bCLKCTL_AD_PSONAD	fCLKCTL_AD.TONA			/* A/D converter clock control */

/* Clock division ratio define */
#define CLKDIV_16T_1	0x00				/* 16-bit timer clock division CLK/1 */
#define CLKDIV_16T_2	0x01				/* 16-bit timer clock division CLK/2 */
#define CLKDIV_16T_4	0x02				/* 16-bit timer clock division CLK/4 */
#define CLKDIV_16T_16	0x03				/* 16-bit timer clock division CLK/16 */
#define CLKDIV_16T_64	0x04				/* 16-bit timer clock division CLK/64 */
#define CLKDIV_16T_256	0x05				/* 16-bit timer clock division CLK/256 */
#define CLKDIV_16T_1024	0x06				/* 16-bit timer clock division CLK/1024 */
#define CLKDIV_16T_4096	0x07				/* 16-bit timer clock division CLK/4096 */

#define CLKDIV_8T03_2	0x00				/* 8-bit timer 0,3 clock division CLK/2 */
#define CLKDIV_8T03_4	0x01				/* 8-bit timer 0,3 clock division CLK/4 */
#define CLKDIV_8T03_8	0x02				/* 8-bit timer 0,3 clock division CLK/8 */
#define CLKDIV_8T03_16	0x03				/* 8-bit timer 0,3 clock division CLK/16 */
#define CLKDIV_8T03_32	0x04				/* 8-bit timer 0,3 clock division CLK/32 */
#define CLKDIV_8T03_64	0x05				/* 8-bit timer 0,3 clock division CLK/64 */
#define CLKDIV_8T03_128	0x06				/* 8-bit timer 0,3 clock division CLK/128 */
#define CLKDIV_8T03_256	0x07				/* 8-bit timer 0,3 clock division CLK/256 */

#define CLKDIV_8T1_32	0x00				/* 8-bit timer 1 clock division CLK/32 */
#define CLKDIV_8T1_64	0x01				/* 8-bit timer 1 clock division CLK/64 */
#define CLKDIV_8T1_128	0x02				/* 8-bit timer 1 clock division CLK/128 */
#define CLKDIV_8T1_256	0x03				/* 8-bit timer 1 clock division CLK/256 */
#define CLKDIV_8T1_512	0x04				/* 8-bit timer 1 clock division CLK/512 */
#define CLKDIV_8T1_1024	0x05				/* 8-bit timer 1 clock division CLK/1024 */
#define CLKDIV_8T1_2048	0x06				/* 8-bit timer 1 clock division CLK/2048 */
#define CLKDIV_8T1_4096	0x07				/* 8-bit timer 1 clock division CLK/4096 */

#define CLKDIV_8T2_2	0x00				/* 8-bit timer 2 clock division CLK/2 */
#define CLKDIV_8T2_4	0x01				/* 8-bit timer 2 clock division CLK/4 */
#define CLKDIV_8T2_8	0x02				/* 8-bit timer 2 clock division CLK/8 */
#define CLKDIV_8T2_16	0x03				/* 8-bit timer 2 clock division CLK/16 */
#define CLKDIV_8T2_32	0x04				/* 8-bit timer 2 clock division CLK/32 */
#define CLKDIV_8T2_64	0x05				/* 8-bit timer 2 clock division CLK/64 */
#define CLKDIV_8T2_2048	0x06				/* 8-bit timer 2 clock division CLK/2048 */
#define CLKDIV_8T2_4096	0x07				/* 8-bit timer 2 clock division CLK/4096 */

#define CLKDIV_AD_2	0x00				/* A/D converter clock division CLK/2 */
#define CLKDIV_AD_4	0x01				/* A/D converter clock division CLK/4 */
#define CLKDIV_AD_8	0x02				/* A/D converter clock division CLK/8 */
#define CLKDIV_AD_16	0x03				/* A/D converter clock division CLK/16 */
#define CLKDIV_AD_32	0x04				/* A/D converter clock division CLK/32 */
#define CLKDIV_AD_64	0x05				/* A/D converter clock division CLK/64 */
#define CLKDIV_AD_128	0x06				/* A/D converter clock division CLK/128 */
#define CLKDIV_AD_256	0x07				/* A/D converter clock division CLK/256 */

////////////////////////////////////////////////////////////////
// Clock timer register
#define pCLK_TCR	plc_IO->c_TIMER.rTCR.ucCTL	/* Clock timer Run/Stop register (0x40151) */
#define fCLK_TCR	plc_IO->c_TIMER.rTCR.bCTL	/* Clock timer Run/Stop register bit field */
#define bCLK_TCR_TCRUN	fCLK_TCR.TCRUN			/* Clock timer Run/Stop control */
#define bCLK_TCR_TCRST	fCLK_TCR.TCRST			/* Clock timer Reset */

#define pCLK_TCI	plc_IO->c_TIMER.rTCI.ucCTL	/* Clock timer interrupt control register (0x40152) */
#define fCLK_TCI	plc_IO->c_TIMER.rTCI.bCTL	/* Clock timer interrupt control register bit field */
#define bCLK_TCI_TCAF	fCLK_TCI.TCAF			/* Alarm factor generation flag */
#define bCLK_TCI_TCIF	fCLK_TCI.TCIF			/* Interrupt factor generation flag */
#define bCLK_TCI_TCASE	fCLK_TCI.TCASE			/* Clock timer alarm factor selection */
#define bCLK_TCI_TCISE	fCLK_TCI.TCISE			/* Clock timer interrupt factor selection */

#define pCLK_TCD	plc_IO->c_TIMER.rTCD		/* Clock timer divider register (0x40153) */

#define pCLK_TCMD	plc_IO->c_TIMER.rTCMD		/* Clock timer second register (0x40154) */

#define pCLK_TCHD	plc_IO->c_TIMER.rTCHD		/* Clock timer minute register (0x40155) */

#define pCLK_TCDD	plc_IO->c_TIMER.rTCDD		/* Clock timer hour register (0x40156) */

#define pCLK_TCNDL	plc_IO->c_TIMER.rTCNDL		/* Clock timer day (low-order) register (0x40157) */

#define pCLK_TCNDH	plc_IO->c_TIMER.rTCNDH		/* Clock timer day (high-order) register (0x40158) */

#define pCLK_TCCH	plc_IO->c_TIMER.rTCCH		/* Clock timer minute comparison register (0x40159) */

#define pCLK_TCCD	plc_IO->c_TIMER.rTCCD		/* Clock timer hour comparison register (0x4015a) */

#define pCLK_TCCN	plc_IO->c_TIMER.rTCCN		/* Clock timer day comparison register (0x4015b) */

/* Clock timer macro */
#define TCRUN_RUN	bCLK_TCR_TCRUN = 1		/* Clock timer Run */
#define TCRUN_STOP	bCLK_TCR_TCRUN = 0		/* Clock timer Stop */
#define TCRST_RESET	bCLK_TCR_TCRST = 1		/* Clock timer Reset */

/* Clock timer define */
#define TCASE_NONE	0x00				/* Clock timer none alarm */
#define TCASE_MINUTE	0x01				/* Clock timer minute alarm */
#define TCASE_HOUR	0x02				/* Clock timer hour alarm */
#define TCASE_DAY	0x04				/* Clock timer day alarm */

#define TCISE_32HZ	0x00				/* Clock timer interrupt 32Hz */
#define TCISE_8HZ	0x01				/* Clock timer interrupt 8Hz */
#define TCISE_2HZ	0x02				/* Clock timer interrupt 2Hz */
#define TCISE_1HZ	0x03				/* Clock timer interrupt 1Hz */
#define TCISE_MINUTE	0x04				/* Clock timer interrupt minute */
#define TCISE_HOUR	0x05				/* Clock timer interrupt hour */
#define TCISE_DAY	0x06				/* Clock timer interrupt day */
#define TCISE_NONE	0x07				/* Clock timer interrupt none */

////////////////////////////////////////////////////////////////
// 8-bit timer control register
#define pT8_CTL0	plc_IO->c_T8_0.rT8CTL.ucCTL	/* 8-bit timer 0 clock control register (0x40160) */
#define fT8_CTL0	plc_IO->c_T8_0.rT8CTL.bCTL	/* 8-bit timer 0 clock control register bit field */
#define bT8_CTL0_PTRUN0	fT8_CTL0.PTRUN			/* 8-bit timer 0 Run/Stop control */
#define bT8_CTL0_PSET0	fT8_CTL0.PSET			/* 8-bit timer 0 preset */
#define bT8_CTL0_PTOUT0	fT8_CTL0.PTOUT			/* 8-bit timer 0 clock output control */

#define pT8_RLD0	plc_IO->c_T8_0.rRLD		/* 8-bit timer 0 reload data register (0x40161) */

#define pT8_PTD0	plc_IO->c_T8_0.rPTD		/* 8-bit timer 0 counter data register (0x40162) */

#define pT8_CTL1	plc_IO->c_T8_1.rT8CTL.ucCTL	/* 8-bit timer 1 clock control register (0x40164) */
#define fT8_CTL1	plc_IO->c_T8_1.rT8CTL.bCTL	/* 8-bit timer 1 clock control register bit field */
#define bT8_CTL1_PTRUN1	fT8_CTL1.PTRUN			/* 8-bit timer 1 Run/Stop control */
#define bT8_CTL1_PSET1	fT8_CTL1.PSET			/* 8-bit timer 1 preset */
#define bT8_CTL1_PTOUT1	fT8_CTL1.PTOUT			/* 8-bit timer 1 clock output control */

#define pT8_RLD1	plc_IO->c_T8_1.rRLD		/* 8-bit timer 1 reload data register (0x40165) */

#define pT8_PTD1	plc_IO->c_T8_1.rPTD		/* 8-bit timer 1 counter data register (0x40166) */

#define pT8_CTL2	plc_IO->c_T8_2.rT8CTL.ucCTL	/* 8-bit timer 2 clock control register (0x40168) */
#define fT8_CTL2	plc_IO->c_T8_2.rT8CTL.bCTL	/* 8-bit timer 2 clock control register bit field */
#define bT8_CTL2_PTRUN2	fT8_CTL2.PTRUN			/* 8-bit timer 2 Run/Stop control */
#define bT8_CTL2_PSET2	fT8_CTL2.PSET			/* 8-bit timer 2 preset */
#define bT8_CTL2_PTOUT2	fT8_CTL2.PTOUT			/* 8-bit timer 2 clock output control */

#define pT8_RLD2	plc_IO->c_T8_2.rRLD		/* 8-bit timer 2 reload data register (0x40169) */

#define pT8_PTD2	plc_IO->c_T8_2.rPTD		/* 8-bit timer 2 counter data register (0x4016a) */

#define pT8_CTL3	plc_IO->c_T8_3.rT8CTL.ucCTL	/* 8-bit timer 3 clock control register (0x4016c) */
#define fT8_CTL3	plc_IO->c_T8_3.rT8CTL.bCTL	/* 8-bit timer 3 clock control register bit field */
#define bT8_CTL3_PTRUN3	fT8_CTL3.PTRUN			/* 8-bit timer 3 Run/Stop control */
#define bT8_CTL3_PSET3	fT8_CTL3.PSET			/* 8-bit timer 3 preset */
#define bT8_CTL3_PTOUT3	fT8_CTL3.PTOUT			/* 8-bit timer 3 clock output control */

#define pT8_RLD3	plc_IO->c_T8_3.rRLD		/* 8-bit timer 3 reload data register (0x4016d) */

#define pT8_PTD3	plc_IO->c_T8_3.rPTD		/* 8-bit timer 3 counter data register (0x4016e) */

/* 8-bit timer macro */
#define PTRUN0_RUN	bT8_CTL0_PTRUN0 = 1		/* 8-bit timer 0 Run */
#define PTRUN0_STOP	bT8_CTL0_PTRUN0 = 0		/* 8-bit timer 0 Stop */
#define PTRUN1_RUN	bT8_CTL1_PTRUN1 = 1		/* 8-bit timer 1 Run */
#define PTRUN1_STOP	bT8_CTL1_PTRUN1 = 0		/* 8-bit timer 1 Stop */
#define PTRUN2_RUN	bT8_CTL2_PTRUN2 = 1		/* 8-bit timer 2 Run */
#define PTRUN2_STOP	bT8_CTL2_PTRUN2 = 0		/* 8-bit timer 2 Stop */
#define PTRUN3_RUN	bT8_CTL3_PTRUN3 = 1		/* 8-bit timer 3 Run */
#define PTRUN3_STOP	bT8_CTL3_PTRUN3 = 0		/* 8-bit timer 3 Stop */
#define PSET0_PRESET	bT8_CTL0_PSET0 = 1		/* 8-bit timer 0 preset */
#define PSET1_PRESET	bT8_CTL1_PSET1 = 1		/* 8-bit timer 1 preset */
#define PSET2_PRESET	bT8_CTL2_PSET2 = 1		/* 8-bit timer 2 preset */
#define PSET3_PRESET	bT8_CTL3_PSET3 = 1		/* 8-bit timer 3 preset */

////////////////////////////////////////////////////////////////
// Watchdog timer register
#define pWD_WP		plc_IO->c_WDT.rWRWD.ucCTL	/* Watchdog timer write-protect register (0x40170) */
#define fWD_WP		plc_IO->c_WDT.rWRWD.bCTL	/* Watchdog timer write-protect register bit field */
#define bWD_WP_WRWD	fWD_WP.WRWD			/* Watchdog timer write protection */

#define pWD_EN		plc_IO->c_WDT.rEWD.ucCTL	/* Watchdog timer enable register (0x40171) */
#define fWD_EN		plc_IO->c_WDT.rEWD.bCTL		/* Watchdog timer enable register bit field */
#define bWD_EN_EWD	fWD_EN.EWD			/* Watchdog timer enable */

////////////////////////////////////////////////////////////////
// Power control register
#define pPWR_CTL	plc_IO->c_PWRCTL.rPWRCTL.ucCTL	/* Power control register (0x40180) */
#define fPWR_CTL	plc_IO->c_PWRCTL.rPWRCTL.bCTL	/* Power control register bit field */
#define bPWR_CTL_SOSC1	fPWR_CTL.SOSC1			/* Low-speed (OSC1) oscillation On/Off */
#define bPWR_CTL_SOSC3	fPWR_CTL.SOSC3			/* High-speed (OSC3) oscillation On/Off */
#define bPWR_CTL_CLKCHG	fPWR_CTL.CLKCHG			/* CPU operating clock switch */
#define bPWR_CTL_PSCON	fPWR_CTL.PSCON			/* Prescaler On/Off control */
#define bPWR_CTL_CLKDT	fPWR_CTL.CLKDT			/* System clock division ratio selection */

#define pPWR_CLKSEL		plc_IO->c_PWRCTL.rCLKSEL.ucCTL	/* Prescaler clock select register (0x40181) */
#define fPWR_CLKSEL		plc_IO->c_PWRCTL.rCLKSEL.bCTL	/* Prescaler clock select register bit field */
#define bPWR_CLKSEL_PSCDT0	fPWR_CLKSEL.PSCDT0		/* Prescaler clock selection */

#define pPWR_CLKOPT		plc_IO->c_PWRCTL.rCLKOPT.ucCTL	/* Clock option register (0x40190) */
#define fPWR_CLKOPT		plc_IO->c_PWRCTL.rCLKOPT.bCTL	/* Clock option register bit field */
#define bPWR_CLKOPT_PF1ON	fPWR_CLKOPT.PF1ON		/* OSC1 external output control */
#define bPWR_CLKOPT_P8T1ON	fPWR_CLKOPT.P8T1ON		/* OSC3-stabilize waiting function */
#define bPWR_CLKOPT_HLT2OP	fPWR_CLKOPT.HLT2OP		/* HALT clock option */

#define pPWR_CLGP	plc_IO->c_PWRCTL.rPWRPRT	/* Power control protect register (0x4019e) */

/* System clock division ratio define */
#define SYS_CLKDIV_1	0x00				/* System clock 1/1 */
#define SYS_CLKDIV_2	0x01				/* System clock 1/2 */
#define SYS_CLKDIV_4	0x02				/* System clock 1/4 */
#define SYS_CLKDIV_8	0x03				/* System clock 1/8 */

////////////////////////////////////////////////////////////////
// Serial I/F register
#define pSIF0_TXD	plc_IO->c_SIF0.rTXD		/* Serial I/F Ch.0 transmit data register (0x401e0) */

#define pSIF0_RXD	plc_IO->c_SIF0.rRXD		/* Serial I/F Ch.0 receive data register (0x401e1) */

#define pSIF0_STATUS		plc_IO->c_SIF0.rSTATUS.ucCTL	/* Serial I/F Ch.0 status register (0x401e2) */
#define fSIF0_STATUS		plc_IO->c_SIF0.rSTATUS.bCTL	/* Serial I/F Ch.0 status register bit field */
#define bSIF0_STATUS_RDBF0	fSIF0_STATUS.RDBF		/* Serial I/F Ch.0 receive data buffer full */
#define bSIF0_STATUS_TDBE0	fSIF0_STATUS.TDBE		/* Serial I/F Ch.0 transmit data buffer empty */
#define bSIF0_STATUS_OER0	fSIF0_STATUS.OER		/* Serial I/F Ch.0 overrun error flag */
#define bSIF0_STATUS_PER0	fSIF0_STATUS.PER		/* Serial I/F Ch.0 parity error flag */
#define bSIF0_STATUS_FER0	fSIF0_STATUS.FER		/* Serial I/F Ch.0 flaming error flag */
#define bSIF0_STATUS_TEND0	fSIF0_STATUS.TEND		/* Serial I/F Ch.0 transmit end flag */

#define pSIF0_CTL		plc_IO->c_SIF0.rCONTROL.ucCTL	/* Serial I/F Ch.0 control register (0x401e3) */
#define fSIF0_CTL		plc_IO->c_SIF0.rCONTROL.bCTL	/* Serial I/F Ch.0 control register bit field */
#define bSIF0_CTL_SMD0		fSIF0_CTL.SMD			/* Serial I/F Ch.0 transmit mode selection */
#define bSIF0_CTL_SSCK0		fSIF0_CTL.SSCK			/* Serial I/F Ch.0 input clock selection */
#define bSIF0_CTL_STPB0		fSIF0_CTL.STPB			/* Serial I/F Ch.0 stop bit selection */
#define bSIF0_CTL_PMD0		fSIF0_CTL.PMD			/* Serial I/F Ch.0 parity mode selection */
#define bSIF0_CTL_EPR0		fSIF0_CTL.EPR			/* Serial I/F Ch.0 parity enable */
#define bSIF0_CTL_RXEN0		fSIF0_CTL.RXEN			/* Serial I/F Ch.0 receive enable */
#define bSIF0_CTL_TXEN0		fSIF0_CTL.TXEN			/* Serial I/F Ch.0 transmit enable */

#define pSIF0_IRDA		plc_IO->c_SIF0.rIRDA.ucCTL	/* Serial I/F Ch.0 IrDA register (0x401e4) */
#define fSIF0_IRDA		plc_IO->c_SIF0.rIRDA.bCTL	/* Serial I/F Ch.0 IrDA register bit field */
#define bSIF0_IRDA_IRMD0	fSIF0_IRDA.IRMD			/* Serial I/F Ch.0 interface mode selection */
#define bSIF0_IRDA_IRRL0	fSIF0_IRDA.IRRL			/* Serial I/F Ch.0 IrDA I/F input logic inversion */
#define bSIF0_IRDA_IRTL0	fSIF0_IRDA.IRTL			/* Serial I/F Ch.0 IrDA I/F output logic inversion */
#define bSIF0_IRDA_DIVMD0	fSIF0_IRDA.DIVMD		/* Serial I/F Ch.0 async. clock division ratio */

#define pSIF1_TXD	plc_IO->c_SIF1.rTXD		/* Serial I/F Ch.1 transmit data register (0x401e5) */

#define pSIF1_RXD	plc_IO->c_SIF1.rRXD		/* Serial I/F Ch.1 receive data register (0x401e6) */

#define pSIF1_STATUS		plc_IO->c_SIF1.rSTATUS.ucCTL	/* Serial I/F Ch.1 status register (0x401e7) */
#define fSIF1_STATUS		plc_IO->c_SIF1.rSTATUS.bCTL	/* Serial I/F Ch.1 status register bit field */
#define bSIF1_STATUS_RDBF1	fSIF1_STATUS.RDBF		/* Serial I/F Ch.1 receive data buffer full */
#define bSIF1_STATUS_TDBE1	fSIF1_STATUS.TDBE		/* Serial I/F Ch.1 transmit data buffer empty */
#define bSIF1_STATUS_OER1	fSIF1_STATUS.OER		/* Serial I/F Ch.1 overrun error flag */
#define bSIF1_STATUS_PER1	fSIF1_STATUS.PER		/* Serial I/F Ch.1 parity error flag */
#define bSIF1_STATUS_FER1	fSIF1_STATUS.FER		/* Serial I/F Ch.1 flaming error flag */
#define bSIF1_STATUS_TEND1	fSIF1_STATUS.TEND		/* Serial I/F Ch.1 transmit end flag */

#define pSIF1_CTL		plc_IO->c_SIF1.rCONTROL.ucCTL	/* Serial I/F Ch.1 control register (0x401e8) */
#define fSIF1_CTL		plc_IO->c_SIF1.rCONTROL.bCTL	/* Serial I/F Ch.1 control register bit field */
#define bSIF1_CTL_SMD1		fSIF1_CTL.SMD			/* Serial I/F Ch.1 transmit mode selection */
#define bSIF1_CTL_SSCK1		fSIF1_CTL.SSCK			/* Serial I/F Ch.1 input clock selection */
#define bSIF1_CTL_STPB1		fSIF1_CTL.STPB			/* Serial I/F Ch.1 stop bit selection */
#define bSIF1_CTL_PMD1		fSIF1_CTL.PMD			/* Serial I/F Ch.1 parity mode selection */
#define bSIF1_CTL_EPR1		fSIF1_CTL.EPR			/* Serial I/F Ch.1 parity enable */
#define bSIF1_CTL_RXEN1		fSIF1_CTL.RXEN			/* Serial I/F Ch.1 receive enable */
#define bSIF1_CTL_TXEN1		fSIF1_CTL.TXEN			/* Serial I/F Ch.1 transmit enable */

#define pSIF1_IRDA		plc_IO->c_SIF1.rIRDA.ucCTL	/* Serial I/F Ch.1 IrDA register (0x401e9) */
#define fSIF1_IRDA		plc_IO->c_SIF1.rIRDA.bCTL	/* Serial I/F Ch.1 IrDA register bit field */
#define bSIF1_IRDA_IRMD1	fSIF1_IRDA.IRMD			/* Serial I/F Ch.1 interface mode selection */
#define bSIF1_IRDA_IRRL1	fSIF1_IRDA.IRRL			/* Serial I/F Ch.1 IrDA I/F input logic inversion */
#define bSIF1_IRDA_IRTL1	fSIF1_IRDA.IRTL			/* Serial I/F Ch.1 IrDA I/F output logic inversion */
#define bSIF1_IRDA_DIVMD1	fSIF1_IRDA.DIVMD		/* Serial I/F Ch.1 async. clock division ratio */

#define pSIF2_TXD	plc_IO->c_SIF2.rTXD		/* Serial I/F Ch.2 transmit data register (0x401f0) */

#define pSIF2_RXD	plc_IO->c_SIF2.rRXD		/* Serial I/F Ch.2 receive data register (0x401f1) */

#define pSIF2_STATUS		plc_IO->c_SIF2.rSTATUS.ucCTL	/* Serial I/F Ch.2 status register (0x401f2) */
#define fSIF2_STATUS		plc_IO->c_SIF2.rSTATUS.bCTL	/* Serial I/F Ch.2 status register bit field */
#define bSIF2_STATUS_RDBF2	fSIF2_STATUS.RDBF		/* Serial I/F Ch.2 receive data buffer full */
#define bSIF2_STATUS_TDBE2	fSIF2_STATUS.TDBE		/* Serial I/F Ch.2 transmit data buffer empty */
#define bSIF2_STATUS_OER2	fSIF2_STATUS.OER		/* Serial I/F Ch.2 overrun error flag */
#define bSIF2_STATUS_PER2	fSIF2_STATUS.PER		/* Serial I/F Ch.2 parity error flag */
#define bSIF2_STATUS_FER2	fSIF2_STATUS.FER		/* Serial I/F Ch.2 flaming error flag */
#define bSIF2_STATUS_TEND2	fSIF2_STATUS.TEND		/* Serial I/F Ch.2 transmit end flag */

#define pSIF2_CTL		plc_IO->c_SIF2.rCONTROL.ucCTL	/* Serial I/F Ch.2 control register (0x401f3) */
#define fSIF2_CTL		plc_IO->c_SIF2.rCONTROL.bCTL	/* Serial I/F Ch.2 control register bit field */
#define bSIF2_CTL_SMD2		fSIF2_CTL.SMD			/* Serial I/F Ch.2 transmit mode selection */
#define bSIF2_CTL_SSCK2		fSIF2_CTL.SSCK			/* Serial I/F Ch.2 input clock selection */
#define bSIF2_CTL_STPB2		fSIF2_CTL.STPB			/* Serial I/F Ch.2 stop bit selection */
#define bSIF2_CTL_PMD2		fSIF2_CTL.PMD			/* Serial I/F Ch.2 parity mode selection */
#define bSIF2_CTL_EPR2		fSIF2_CTL.EPR			/* Serial I/F Ch.2 parity enable */
#define bSIF2_CTL_RXEN2		fSIF2_CTL.RXEN			/* Serial I/F Ch.2 receive enable */
#define bSIF2_CTL_TXEN2		fSIF2_CTL.TXEN			/* Serial I/F Ch.2 transmit enable */

#define pSIF2_IRDA		plc_IO->c_SIF2.rIRDA.ucCTL	/* Serial I/F Ch.2 IrDA register (0x401f4) */
#define fSIF2_IRDA		plc_IO->c_SIF2.rIRDA.bCTL	/* Serial I/F Ch.2 IrDA register bit field */
#define bSIF2_IRDA_IRMD2	fSIF2_IRDA.IRMD			/* Serial I/F Ch.2 interface mode selection */
#define bSIF2_IRDA_IRRL2	fSIF2_IRDA.IRRL			/* Serial I/F Ch.2 IrDA I/F input logic inversion */
#define bSIF2_IRDA_IRTL2	fSIF2_IRDA.IRTL			/* Serial I/F Ch.2 IrDA I/F output logic inversion */
#define bSIF2_IRDA_DIVMD2	fSIF2_IRDA.DIVMD		/* Serial I/F Ch.2 async. clock division ratio */

#define pSIF3_TXD	plc_IO->c_SIF3.rTXD		/* Serial I/F Ch.3 transmit data register (0x401f5) */

#define pSIF3_RXD	plc_IO->c_SIF3.rRXD		/* Serial I/F Ch.3 receive data register (0x401f6) */

#define pSIF3_STATUS		plc_IO->c_SIF3.rSTATUS.ucCTL	/* Serial I/F Ch.3 status register (0x401f7) */
#define fSIF3_STATUS		plc_IO->c_SIF3.rSTATUS.bCTL	/* Serial I/F Ch.3 status register bit field */
#define bSIF3_STATUS_RDBF3	fSIF3_STATUS.RDBF		/* Serial I/F Ch.3 receive data buffer full */
#define bSIF3_STATUS_TDBE3	fSIF3_STATUS.TDBE		/* Serial I/F Ch.3 transmit data buffer empty */
#define bSIF3_STATUS_OER3	fSIF3_STATUS.OER		/* Serial I/F Ch.3 overrun error flag */
#define bSIF3_STATUS_PER3	fSIF3_STATUS.PER		/* Serial I/F Ch.3 parity error flag */
#define bSIF3_STATUS_FER3	fSIF3_STATUS.FER		/* Serial I/F Ch.3 flaming error flag */
#define bSIF3_STATUS_TEND3	fSIF3_STATUS.TEND		/* Serial I/F Ch.3 transmit end flag */

#define pSIF3_CTL		plc_IO->c_SIF3.rCONTROL.ucCTL	/* Serial I/F Ch.3 control register (0x401f8) */
#define fSIF3_CTL		plc_IO->c_SIF3.rCONTROL.bCTL	/* Serial I/F Ch.3 control register bit field */
#define bSIF3_CTL_SMD3		fSIF3_CTL.SMD			/* Serial I/F Ch.3 transmit mode selection */
#define bSIF3_CTL_SSCK3		fSIF3_CTL.SSCK			/* Serial I/F Ch.3 input clock selection */
#define bSIF3_CTL_STPB3		fSIF3_CTL.STPB			/* Serial I/F Ch.3 stop bit selection */
#define bSIF3_CTL_PMD3		fSIF3_CTL.PMD			/* Serial I/F Ch.3 parity mode selection */
#define bSIF3_CTL_EPR3		fSIF3_CTL.EPR			/* Serial I/F Ch.3 parity enable */
#define bSIF3_CTL_RXEN3		fSIF3_CTL.RXEN			/* Serial I/F Ch.3 receive enable */
#define bSIF3_CTL_TXEN3		fSIF3_CTL.TXEN			/* Serial I/F Ch.3 transmit enable */

#define pSIF3_IRDA		plc_IO->c_SIF3.rIRDA.ucCTL	/* Serial I/F Ch.3 IrDA register (0x401f9) */
#define fSIF3_IRDA		plc_IO->c_SIF3.rIRDA.bCTL	/* Serial I/F Ch.3 IrDA register bit field */
#define bSIF3_IRDA_IRMD3	fSIF3_IRDA.IRMD			/* Serial I/F Ch.3 interface mode selection */
#define bSIF3_IRDA_IRRL3	fSIF3_IRDA.IRRL			/* Serial I/F Ch.3 IrDA I/F input logic inversion */
#define bSIF3_IRDA_IRTL3	fSIF3_IRDA.IRTL			/* Serial I/F Ch.3 IrDA I/F output logic inversion */
#define bSIF3_IRDA_DIVMD3	fSIF3_IRDA.DIVMD		/* Serial I/F Ch.3 async. clock division ratio */

/* Serial I/F transfer mode define */
#define SMD_MASTER	0x00				/* Serial I/F mode clock synchronous master */
#define SMD_SLAVE	0x01				/* Serial I/F mode clock synchronous slave */
#define SMD_7BIT	0x02				/* Serial I/F mode asynchronous 7bit */
#define SMD_8BIT	0x03				/* Serial I/F mode asynchronous 8bit */

/* Serial I/F interface mode define */
#define IRMD_ORD	0x00				/* Ordinary interface */
#define IRMD_IRDA	0x02				/* IrDA 1.0 */

////////////////////////////////////////////////////////////////
// A/D converter register
#define pAD_ADD		plc_IO->c_AD.rADD		/* A/D conversion result register (0x40240) */

#define pAD_TRIG	plc_IO->c_AD.rTRIG.ucCTL	/* A/D trigger register (0x40242) */
#define fAD_TRIG	plc_IO->c_AD.rTRIG.bCTL		/* A/D trigger register bit field */
#define bAD_TRIG_CH	fAD_TRIG.CH			/* A/D conversion channel status */
#define bAD_TRIG_TS	fAD_TRIG.TS			/* A/D conversion trigger selection */
#define bAD_TRIG_MS	fAD_TRIG.MS			/* A/D conversion mode selection */

#define pAD_CHNL	plc_IO->c_AD.rCHNL.ucCTL	/* A/D channel register (0x40243) */
#define fAD_CHNL	plc_IO->c_AD.rCHNL.bCTL		/* A/D channel register bit field */
#define bAD_CHNL_CS	fAD_CHNL.CS			/* A/D converter start channel selection */
#define bAD_CHNL_CE	fAD_CHNL.CE			/* A/D converter end channel selection */

#define pAD_EN		plc_IO->c_AD.rENBL.ucCTL	/* A/D enable register (0x40244) */
#define fAD_EN		plc_IO->c_AD.rENBL.bCTL		/* A/D enable register bit field */
#define bAD_EN_OWE	fAD_EN.OWE			/* Overwrite error flag */
#define bAD_EN_ADST	fAD_EN.ADST			/* A/D conversion control/status */
#define bAD_EN_ADE	fAD_EN.ADE			/* A/D enable */
#define bAD_EN_ADF	fAD_EN.ADF			/* Conversion-complete flag */

#define pAD_SMPL	plc_IO->c_AD.rSMPL.ucCTL	/* A/D sampling register (0x40245) */
#define fAD_SMPL	plc_IO->c_AD.rSMPL.bCTL		/* A/D sampling register bit field */
#define bAD_SMPL_ST	fAD_SMPL.ST			/* Input sampling time setup */

/* A/D conversion trigger selection define */
#define TS_SOFT		0x00				/* Software trigger */
#define TS_16T0		0x01				/* 16-bit timer 0 underflow trigger */
#define TS_T8T0		0x02				/* 8-bit timer 0 underflow trigger */
#define TS_ADTRG	0x03				/* #ADTRG pin trigger */

/* A/D sampling clock define */
#define ST_3		0x00				/* A/D sampling 3 clocks */
#define ST_5		0x01				/* A/D sampling 5 clocks */
#define ST_7		0x02				/* A/D sampling 7 clocks */
#define ST_9		0x03				/* A/D sampling 9 clocks */

/* A/D conversion macro */
#define ADST_RUN	bAD_EN_ADST = 1			/* A/D conversion Run */
#define ADST_STOP	bAD_EN_ADST = 0			/* A/D conversion Stop */

////////////////////////////////////////////////////////////////
// Interrupt priority register
#define pINT_PP01L	plc_IO->c_INTC.rPP01L.ucPP01L	/* Port input 0/1 interrupt priority register (0x40260) */
#define fINT_PP01L	plc_IO->c_INTC.rPP01L.bPP01L	/* Port input 0/1 interrupt priority register bit field */
#define bINT_PP01L_PP0L	fINT_PP01L.PP0L			/* Port input 0 interrupt priority */
#define bINT_PP01L_PP1L	fINT_PP01L.PP1L			/* Port input 1 interrupt priority */

#define pINT_PP23L	plc_IO->c_INTC.rPP23L.ucPP23L	/* Port input 2/3 interrupt priority register (0x40261) */
#define fINT_PP23L	plc_IO->c_INTC.rPP23L.bPP23L	/* Port input 2/3 interrupt priority register bit field */
#define bINT_PP23L_PP2L	fINT_PP23L.PP2L			/* Port input 2 interrupt priority */
#define bINT_PP23L_PP3L	fINT_PP23L.PP3L			/* Port input 3 interrupt priority */

#define pINT_PK01L	plc_IO->c_INTC.rPK01L.ucPK01L	/* Key input 0/1 interrupt priority register (0x40262) */
#define fINT_PK01L	plc_IO->c_INTC.rPK01L.bPK01L	/* Key input 0/1 interrupt priority register bit field */
#define bINT_PK01L_PK0L	fINT_PK01L.PK0L			/* Key input 0 interrupt priority */
#define bINT_PK01L_PK1L	fINT_PK01L.PK1L			/* Key input 1 interrupt priority */

#define pINT_PHSD01L		plc_IO->c_INTC.rPHSD01L.ucPHSD01L	/* High-speed DMA Ch.0/1 interrupt priority register (0x40263) */
#define fINT_PHSD01L		plc_IO->c_INTC.rPHSD01L.bPHSD01L	/* High-speed DMA Ch.0/1 interrupt priority register bit field */
#define bINT_PHSD01L_PHSD0L	fINT_PHSD01L.PHSD0L			/* High-speed DMA Ch.0 interrupt priority */
#define bINT_PHSD01L_PHSD1L	fINT_PHSD01L.PHSD1L			/* High-speed DMA Ch.1 interrupt priority */

#define pINT_PHSD23L		plc_IO->c_INTC.rPHSD23L.ucPHSD23L	/* High-speed DMA Ch.2/3 interrupt priority register (0x40264) */
#define fINT_PHSD23L		plc_IO->c_INTC.rPHSD23L.bPHSD23L	/* High-speed DMA Ch.2/3 interrupt priority register bit field */
#define bINT_PHSD23L_PHSD2L	fINT_PHSD23L.PHSD2L			/* High-speed DMA Ch.2 interrupt priority */
#define bINT_PHSD23L_PHSD3L	fINT_PHSD23L.PHSD3L			/* High-speed DMA Ch.3 interrupt priority */

#define pINT_PDM	plc_IO->c_INTC.rPDM		/* IDMA interrupt priority (0x40265) */

#define pINT_P16T01		plc_IO->c_INTC.rP16T01.ucP16T01	/* 16-bit timer 0/1 interrupt priority register (0x40266) */
#define fINT_P16T01		plc_IO->c_INTC.rP16T01.bP16T01	/* 16-bit timer 0/1 interrupt priority register bit field */
#define bINT_P16T01_P16T0	fINT_P16T01.P16T0		/* 16-bit timer 0 interrupt priority */
#define bINT_P16T01_P16T1	fINT_P16T01.P16T1		/* 16-bit timer 1 interrupt priority */

#define pINT_P16T23		plc_IO->c_INTC.rP16T23.ucP16T23	/* 16-bit timer 2/3 interrupt priority register (0x40267) */
#define fINT_P16T23		plc_IO->c_INTC.rP16T23.bP16T23	/* 16-bit timer 2/3 interrupt priority register bit field */
#define bINT_P16T23_P16T2	fINT_P16T23.P16T2		/* 16-bit timer 2 interrupt priority */
#define bINT_P16T23_P16T3	fINT_P16T23.P16T3		/* 16-bit timer 3 interrupt priority */

#define pINT_P16T45		plc_IO->c_INTC.rP16T45.ucP16T45	/* 16-bit timer 4/5 interrupt priority register (0x40268) */
#define fINT_P16T45		plc_IO->c_INTC.rP16T45.bP16T45	/* 16-bit timer 4/5 interrupt priority register bit field */
#define bINT_P16T45_P16T4	fINT_P16T45.P16T4		/* 16-bit timer 4 interrupt priority */
#define bINT_P16T45_P16T5	fINT_P16T45.P16T5		/* 16-bit timer 5 interrupt priority */

#define pINT_P8T_PSIO0		plc_IO->c_INTC.rP8TM_PSIO0.ucP8TM_PSIO0	/* 8-bit timer, serial I/F Ch.0 interrupt priority register (0x40269) */
#define fINT_P8T_PSIO0		plc_IO->c_INTC.rP8TM_PSIO0.bP8TM_PSIO0	/* 8-bit timer, serial I/F Ch.0 interrupt priority register bit field */
#define bINT_P8T_PSIO0_P8TM	fINT_P8T_PSIO0.P8TM			/* 8-bit timer interrupt priority */
#define bINT_P8T_PSIO0_PSIO0	fINT_P8T_PSIO0.PSIO0			/* Serial I/F Ch.0 interrupt priority */

#define pINT_PSIO1_PAD		plc_IO->c_INTC.rPSIO1_PAD.ucPSIO1_PAD	/* Serial I/F Ch.1, A/D interrupt priority register (0x4026a) */
#define fINT_PSIO1_PAD		plc_IO->c_INTC.rPSIO1_PAD.bPSIO1_PAD	/* Serial I/F Ch.1, A/D interrupt priority register bit field */
#define bINT_PSIO1_PAD_PSIO1	fINT_PSIO1_PAD.PSIO1			/* Serial I/F Ch.1 interrupt priority */
#define bINT_PSIO1_PAD_PAD	fINT_PSIO1_PAD.PAD			/* A/D converter interrupt priority */

#define pINT_PCTM	plc_IO->c_INTC.rPCTM		/* Clock timer interrupt priority register (0x4026b) */

#define pINT_PP45L	plc_IO->c_INTC.rPP45L.ucPP45L	/* Port input 4/5 interrupt priority register (0x4026c) */
#define fINT_PP45L	plc_IO->c_INTC.rPP45L.bPP45L	/* Port input 4/5 interrupt priority register bit field */
#define bINT_PP45L_PP4L	fINT_PP45L.PP4L			/* Port input 4 interrupt priority */
#define bINT_PP45L_PP5L	fINT_PP45L.PP5L			/* Port input 5 interrupt priority */

#define pINT_PP67L	plc_IO->c_INTC.rPP67L.ucPP67L	/* Port input 6/7 interrupt priority register (0x4026d) */
#define fINT_PP67L	plc_IO->c_INTC.rPP67L.bPP67L	/* Port input 6/7 interrupt priority register bit field */
#define bINT_PP67L_PP6L	fINT_PP67L.PP6L			/* Port input 6 interrupt priority */
#define bINT_PP67L_PP7L	fINT_PP67L.PP7L			/* Port input 7 interrupt priority */

////////////////////////////////////////////////////////////////
// Key input 0-1, port input 0-3 interrupt enable register
#define pINT_EK01_EP03		plc_IO->c_INTC.rIEN1.ucIEN	/* Key input, port input 0-3 interrupt enable register (0x40270) */
#define fINT_EK01_EP03		plc_IO->c_INTC.rIEN1.bIEN	/* Key input, port input 0-3 interrupt enable register bit field */
#define bINT_EK01_EP03_EP0	fINT_EK01_EP03.EP0		/* Port input 0 */
#define bINT_EK01_EP03_EP1	fINT_EK01_EP03.EP1		/* Port input 1 */
#define bINT_EK01_EP03_EP2	fINT_EK01_EP03.EP2		/* Port input 2 */
#define bINT_EK01_EP03_EP3	fINT_EK01_EP03.EP3		/* Port input 3 */
#define bINT_EK01_EP03_EK0	fINT_EK01_EP03.EK0		/* Key input 0 */
#define bINT_EK01_EP03_EK1	fINT_EK01_EP03.EK1		/* Key input 1 */

////////////////////////////////////////////////////////////////
// DMA interrupt enable register
#define pINT_EDMA	plc_IO->c_INTC.rIEN2.ucIEN	/* DMA interrupt enable register (0x40271) */
#define fINT_EDMA	plc_IO->c_INTC.rIEN2.bIEN	/* DMA interrupt enable register bit field */
#define bINT_EDMA_EHDM0	fINT_EDMA.EHDM0			/* High-speed DMA Ch.0 */
#define bINT_EDMA_EHDM1	fINT_EDMA.EHDM1			/* High-speed DMA Ch.1 */
#define bINT_EDMA_EHDM2	fINT_EDMA.EHDM2			/* High-speed DMA Ch.2 */
#define bINT_EDMA_EHDM3	fINT_EDMA.EHDM3			/* High-speed DMA Ch.3 */
#define bINT_EDMA_EIDMA	fINT_EDMA.EIDMA			/* IDMA */

////////////////////////////////////////////////////////////////
// 16-bit timer interrupt enable register
#define pINT_E16T01		plc_IO->c_INTC.rIEN3.ucIEN	/* 16-bit timer 0/1 interrupt enable register (0x40272) */
#define fINT_E16T01		plc_IO->c_INTC.rIEN3.bIEN	/* 16-bit timer 0/1 interrupt enable register bit field */
#define bINT_E16T01_E16TU0	fINT_E16T01.E16TU0		/* 16-bit timer 0 comparison B */
#define bINT_E16T01_E16TC0	fINT_E16T01.E16TC0		/* 16-bit timer 0 comparison A */
#define bINT_E16T01_E16TU1	fINT_E16T01.E16TU1		/* 16-bit timer 1 comparison B */
#define bINT_E16T01_E16TC1	fINT_E16T01.E16TC1		/* 16-bit timer 1 comparison A */

#define pINT_E16T23		plc_IO->c_INTC.rIEN4.ucIEN	/* 16-bit timer 2/3 interrupt enable register (0x40273) */
#define fINT_E16T23		plc_IO->c_INTC.rIEN4.bIEN	/* 16-bit timer 2/3 interrupt enable register bit field */
#define bINT_E16T23_E16TU2	fINT_E16T23.E16TU2		/* 16-bit timer 2 comparison B */
#define bINT_E16T23_E16TC2	fINT_E16T23.E16TC2		/* 16-bit timer 2 comparison A */
#define bINT_E16T23_E16TU3	fINT_E16T23.E16TU3		/* 16-bit timer 3 comparison B */
#define bINT_E16T23_E16TC3	fINT_E16T23.E16TC3		/* 16-bit timer 3 comparison A */

#define pINT_E16T45		plc_IO->c_INTC.rIEN5.ucIEN	/* 16-bit timer 4/5 interrupt enable register (0x40274) */
#define fINT_E16T45		plc_IO->c_INTC.rIEN5.bIEN	/* 16-bit timer 4/5 interrupt enable register bit field */
#define bINT_E16T45_E16TU4	fINT_E16T45.E16TU4		/* 16-bit timer 4 comparison B */
#define bINT_E16T45_E16TC4	fINT_E16T45.E16TC4		/* 16-bit timer 4 comparison A */
#define bINT_E16T45_E16TU5	fINT_E16T45.E16TU5		/* 16-bit timer 5 comparison B */
#define bINT_E16T45_E16TC5	fINT_E16T45.E16TC5		/* 16-bit timer 5 comparison A */

////////////////////////////////////////////////////////////////
// 8-bit timer interrupt enable register
#define pINT_E8T	plc_IO->c_INTC.rIEN6.ucIEN		/* 8-bit timer interrupt enable register (0x40275) */
#define fINT_E8T	plc_IO->c_INTC.rIEN6.bIEN		/* 8-bit timer interrupt enable register bit field */
#define bINT_E8T_E8TU0	fINT_E8T.E8TU0				/* 8-bit timer 0 underflow */
#define bINT_E8T_E8TU1	fINT_E8T.E8TU1				/* 8-bit timer 1 underflow */
#define bINT_E8T_E8TU2	fINT_E8T.E8TU2				/* 8-bit timer 2 underflow */
#define bINT_E8T_E8TU3	fINT_E8T.E8TU3				/* 8-bit timer 3 underflow */

////////////////////////////////////////////////////////////////
// Serial I/F interrupt enable register
#define pINT_ESIF		plc_IO->c_INTC.rIEN7.ucIEN	/* Serial I/F interrupt enable register (0x40276) */
#define fINT_ESIF		plc_IO->c_INTC.rIEN7.bIEN	/* Serial I/F interrupt enable register bit field */
#define bINT_ESIF_ESERR0	fINT_ESIF.ESERR0		/* Serial I/F Ch.0 receive error */
#define bINT_ESIF_ESRX0		fINT_ESIF.ESRX0			/* Serial I/F Ch.0 receive buffer full */
#define bINT_ESIF_ESTX0		fINT_ESIF.ESTX0			/* Serial I/F Ch.0 transmit buffer empty */
#define bINT_ESIF_ESERR1	fINT_ESIF.ESERR1		/* Serial I/F Ch.1 receive error */
#define bINT_ESIF_ESRX1		fINT_ESIF.ESRX1			/* Serial I/F Ch.1 receive buffer full */
#define bINT_ESIF_ESTX1		fINT_ESIF.ESTX1			/* Serial I/F Ch.1 transmit buffer empty */

////////////////////////////////////////////////////////////////
// Port 4-7, clock timer, A/D interrupt enable register
#define pINT_EP47_ECT_EAD	plc_IO->c_INTC.rIEN8.ucIEN	/* Port input 4-7, clock timer, A/D interrupt enable register (0x40277) */
#define fINT_EP47_ECT_EAD	plc_IO->c_INTC.rIEN8.bIEN	/* Port input 4-7, clock timer, A/D interrupt enable register bit field */
#define bINT_EP47_ECT_EAD_EADE	fINT_EP47_ECT_EAD.EADE		/* A/D converter */
#define bINT_EP47_ECT_EAD_ECTM	fINT_EP47_ECT_EAD.ECTM		/* Clock timer */
#define bINT_EP47_ECT_EAD_EP4	fINT_EP47_ECT_EAD.EP4		/* Port input 4 */
#define bINT_EP47_ECT_EAD_EP5	fINT_EP47_ECT_EAD.EP5		/* Port input 5 */
#define bINT_EP47_ECT_EAD_EP6	fINT_EP47_ECT_EAD.EP6		/* Port input 6 */
#define bINT_EP47_ECT_EAD_EP7	fINT_EP47_ECT_EAD.EP7		/* Port input 7 */

////////////////////////////////////////////////////////////////
// Key input 0-1, port input 0-3 interrupt request flag register
#define pINT_FK01_FP03		plc_IO->c_INTC.rISR1.ucISR	/* Key input, port input 0-3 interrupt factor flag register (0x40280) */
#define fINT_FK01_FP03		plc_IO->c_INTC.rISR1.bISR	/* Key input, port input 0-3 interrupt factor flag register bit field */
#define bINT_FK01_FP03_FP0	fINT_FK01_FP03.FP0		/* Port input 0 */
#define bINT_FK01_FP03_FP1	fINT_FK01_FP03.FP1		/* Port input 1 */
#define bINT_FK01_FP03_FP2	fINT_FK01_FP03.FP2		/* Port input 2 */
#define bINT_FK01_FP03_FP3	fINT_FK01_FP03.FP3		/* Port input 3 */
#define bINT_FK01_FP03_FK0	fINT_FK01_FP03.FK0		/* Key input 0 */
#define bINT_FK01_FP03_FK1	fINT_FK01_FP03.FK1		/* Key input 1 */

////////////////////////////////////////////////////////////////
// DMA interrupt request flag register
#define pINT_FDMA	plc_IO->c_INTC.rISR2.ucISR		/* DMA interrupt factor flag register (0x40281) */
#define fINT_FDMA	plc_IO->c_INTC.rISR2.bISR		/* DMA interrupt factor flag register bit field */
#define bINT_FDMA_FHDM0	fINT_FDMA.FHDM0				/* High-speed DMA Ch.0 */
#define bINT_FDMA_FHDM1	fINT_FDMA.FHDM1				/* High-speed DMA Ch.1 */
#define bINT_FDMA_FHDM2	fINT_FDMA.FHDM2				/* High-speed DMA Ch.2 */
#define bINT_FDMA_FHDM3	fINT_FDMA.FHDM3				/* High-speed DMA Ch.3 */
#define bINT_FDMA_FIDMA	fINT_FDMA.FIDMA				/* IDMA */

////////////////////////////////////////////////////////////////
// 16-bit timer interrupt request flag register
#define pINT_F16T01		plc_IO->c_INTC.rISR3.ucISR	/* 16-bit timer 0/1 interrupt factor flag register (0x40282) */
#define fINT_F16T01		plc_IO->c_INTC.rISR3.bISR	/* 16-bit timer 0/1 interrupt factor flag register bit field */
#define bINT_F16T01_F16TU0	fINT_F16T01.F16TU0		/* 16-bit timer 0 comparison B */
#define bINT_F16T01_F16TC0	fINT_F16T01.F16TC0		/* 16-bit timer 0 comparison A */
#define bINT_F16T01_F16TU1	fINT_F16T01.F16TU1		/* 16-bit timer 1 comparison B */
#define bINT_F16T01_F16TC1	fINT_F16T01.F16TC1		/* 16-bit timer 1 comparison A */

#define pINT_F16T23		plc_IO->c_INTC.rISR4.ucISR	/* 16-bit timer 2/3 interrupt factor flag register (0x40283) */
#define fINT_F16T23		plc_IO->c_INTC.rISR4.bISR	/* 16-bit timer 2/3 interrupt factor flag register bit field */
#define bINT_F16T23_F16TU2	fINT_F16T23.F16TU2		/* 16-bit timer 2 comparison B */
#define bINT_F16T23_F16TC2	fINT_F16T23.F16TC2		/* 16-bit timer 2 comparison A */
#define bINT_F16T23_F16TU3	fINT_F16T23.F16TU3		/* 16-bit timer 3 comparison B */
#define bINT_F16T23_F16TC3	fINT_F16T23.F16TC3		/* 16-bit timer 3 comparison A */

#define pINT_F16T45		plc_IO->c_INTC.rISR5.ucISR	/* 16-bit timer 4/5 interrupt factor flag register (0x40284) */
#define fINT_F16T45		plc_IO->c_INTC.rISR5.bISR	/* 16-bit timer 4/5 interrupt factor flag register bit field */
#define bINT_F16T45_F16TU4	fINT_F16T45.F16TU4		/* 16-bit timer 4 comparison B */
#define bINT_F16T45_F16TC4	fINT_F16T45.F16TC4		/* 16-bit timer 4 comparison A */
#define bINT_F16T45_F16TU5	fINT_F16T45.F16TU5		/* 16-bit timer 5 comparison B */
#define bINT_F16T45_F16TC5	fINT_F16T45.F16TC5		/* 16-bit timer 5 comparison A */

////////////////////////////////////////////////////////////////
// 8-bit timer interrupt request flag register
#define pINT_F8T	plc_IO->c_INTC.rISR6.ucISR		/* 8-bit timer interrupt factor flag register (0x40285) */
#define fINT_F8T	plc_IO->c_INTC.rISR6.bISR		/* 8-bit timer interrupt factor flag register bit field */
#define bINT_F8T_F8TU0	fINT_F8T.F8TU0				/* 8-bit timer 0 underflow */
#define bINT_F8T_F8TU1	fINT_F8T.F8TU1				/* 8-bit timer 1 underflow */
#define bINT_F8T_F8TU2	fINT_F8T.F8TU2				/* 8-bit timer 2 underflow */
#define bINT_F8T_F8TU3	fINT_F8T.F8TU3				/* 8-bit timer 3 underflow */

////////////////////////////////////////////////////////////////
// Serial I/F interrupt request flag register
#define pINT_FSIF		plc_IO->c_INTC.rISR7.ucISR	/* Serial I/F interrupt factor flag register (0x40286) */
#define fINT_FSIF		plc_IO->c_INTC.rISR7.bISR	/* Serial I/F interrupt factor flag register bit field */
#define bINT_FSIF_FSERR0	fINT_FSIF.FSERR0		/* Serial I/F Ch.0 receive error */
#define bINT_FSIF_FSRX0		fINT_FSIF.FSRX0			/* Serial I/F Ch.0 receive buffer full */
#define bINT_FSIF_FSTX0		fINT_FSIF.FSTX0			/* Serial I/F Ch.0 transmit buffer empty */
#define bINT_FSIF_FSERR1	fINT_FSIF.FSERR1		/* Serial I/F Ch.1 receive error */
#define bINT_FSIF_FSRX1		fINT_FSIF.FSRX1			/* Serial I/F Ch.1 receive buffer full */
#define bINT_FSIF_FSTX1		fINT_FSIF.FSTX1			/* Serial I/F Ch.1 transmit buffer empty */

////////////////////////////////////////////////////////////////
// Port 4-7, clock timer, A/D interrupt request flag register
#define pINT_FP47_FCT_FAD	plc_IO->c_INTC.rISR8.ucISR	/* Port input 4-7, clock timer, A/D interrupt factor flag register (0x40287) */
#define fINT_FP47_FCT_FAD	plc_IO->c_INTC.rISR8.bISR	/* Port input 4-7, clock timer, A/D interrupt factor flag register bit field */
#define bINT_FP47_FCT_FAD_FADE	fINT_FP47_FCT_FAD.FADE		/* A/D converter */
#define bINT_FP47_FCT_FAD_FCTM	fINT_FP47_FCT_FAD.FCTM		/* Clock timer */
#define bINT_FP47_FCT_FAD_FP4	fINT_FP47_FCT_FAD.FP4		/* Port input 4 */
#define bINT_FP47_FCT_FAD_FP5	fINT_FP47_FCT_FAD.FP5		/* Port input 5 */
#define bINT_FP47_FCT_FAD_FP6	fINT_FP47_FCT_FAD.FP6		/* Port input 6 */
#define bINT_FP47_FCT_FAD_FP7	fINT_FP47_FCT_FAD.FP7		/* Port input 7 */

////////////////////////////////////////////////////////////////
// Port input 0-3, high-speed DMA, 16-bit timer 0 IDMA request register
#define pIDMAREQ_RP03_RHS_R16T0		plc_IO->c_INTC.rIDMAREQ1.ucIDMAREQ	/* Port input 0-3, high-speed DMA, 16-bit timer 0 IDMA request register (0x40290) */
#define fIDMAREQ_RP03_RHS_R16T0		plc_IO->c_INTC.rIDMAREQ1.bIDMAREQ	/* Port input 0-3, high-speed DMA, 16-bit timer 0 IDMA request register bit field */
#define bIDMAREQ_RP03_RHS_R16T0_RP0	fIDMAREQ_RP03_RHS_R16T0.RP0	/* Port input 0 IDMA request */
#define bIDMAREQ_RP03_RHS_R16T0_RP1	fIDMAREQ_RP03_RHS_R16T0.RP1	/* Port input 1 IDMA request */
#define bIDMAREQ_RP03_RHS_R16T0_RP2	fIDMAREQ_RP03_RHS_R16T0.RP2	/* Port input 2 IDMA request */
#define bIDMAREQ_RP03_RHS_R16T0_RP3	fIDMAREQ_RP03_RHS_R16T0.RP3	/* Port input 3 IDMA request */
#define bIDMAREQ_RP03_RHS_R16T0_RHDM0	fIDMAREQ_RP03_RHS_R16T0.RHDM0	/* High-speed DMA Ch.0 IDMA request */
#define bIDMAREQ_RP03_RHS_R16T0_RHDM1	fIDMAREQ_RP03_RHS_R16T0.RHDM1	/* High-speed DMA Ch.1 IDMA request */
#define bIDMAREQ_RP03_RHS_R16T0_R16TU0	fIDMAREQ_RP03_RHS_R16T0.R16TU0	/* 16-bit timer 0 comparison B IDMA request */
#define bIDMAREQ_RP03_RHS_R16T0_R16TC0	fIDMAREQ_RP03_RHS_R16T0.R16TC0	/* 16-bit timer 0 comparison A IDMA request */

//////////////////////////////////////////////////////////
// 16-bit timer 1-4 IDMA request register
#define pIDMAREQ_R16T14		plc_IO->c_INTC.rIDMAREQ2.ucIDMAREQ	/* 16-bit timer 1-4 IDMA request register (0x40291) */
#define fIDMAREQ_R16T14		plc_IO->c_INTC.rIDMAREQ2.bIDMAREQ	/* 16-bit timer 1-4 IDMA request register bit field */
#define bIDMAREQ_R16T14_R16TU1	fIDMAREQ_R16T14.R16TU1			/* 16-bit timer 1 comparison B IDMA request */
#define bIDMAREQ_R16T14_R16TC1	fIDMAREQ_R16T14.R16TC1			/* 16-bit timer 1 comparison A IDMA request */
#define bIDMAREQ_R16T14_R16TU2	fIDMAREQ_R16T14.R16TU2			/* 16-bit timer 2 comparison B IDMA request */
#define bIDMAREQ_R16T14_R16TC2	fIDMAREQ_R16T14.R16TC2			/* 16-bit timer 2 comparison A IDMA request */
#define bIDMAREQ_R16T14_R16TU3	fIDMAREQ_R16T14.R16TU3			/* 16-bit timer 3 comparison B IDMA request */
#define bIDMAREQ_R16T14_R16TC3	fIDMAREQ_R16T14.R16TC3			/* 16-bit timer 3 comparison A IDMA request */
#define bIDMAREQ_R16T14_R16TU4	fIDMAREQ_R16T14.R16TU4			/* 16-bit timer 4 comparison B IDMA request */
#define bIDMAREQ_R16T14_R16TC4	fIDMAREQ_R16T14.R16TC4			/* 16-bit timer 4 comparison A IDMA request */

////////////////////////////////////////////////////////////////
// 16-bit timer 5, 8-bit timer, serial I/F Ch.0 IDMA request register
#define pIDMAREQ_R16T5_R8T_RSIF0		plc_IO->c_INTC.rIDMAREQ3.ucIDMAREQ	/* 16-bit timer 5, 8-bit timer, serial I/F Ch.0 IDMA request register (0x40292) */
#define fIDMAREQ_R16T5_R8T_RSIF0		plc_IO->c_INTC.rIDMAREQ3.bIDMAREQ	/* 16-bit timer 5, 8-bit timer, serial I/F Ch.0 IDMA request register bit field */
#define bIDMAREQ_R16T5_R8T_RSIF0_R16TU5	fIDMAREQ_R16T5_R8T_RSIF0.R16TU5	/* 16-bit timer 5 comparison B IDMA request */
#define bIDMAREQ_R16T5_R8T_RSIF0_R16TC5	fIDMAREQ_R16T5_R8T_RSIF0.R16TC5	/* 16-bit timer 5 comparison A IDMA request */
#define bIDMAREQ_R16T5_R8T_RSIF0_R8TU0	fIDMAREQ_R16T5_R8T_RSIF0.R8TU0	/* 8-bit timer 0 underflow IDMA request */
#define bIDMAREQ_R16T5_R8T_RSIF0_R8TU1	fIDMAREQ_R16T5_R8T_RSIF0.R8TU1	/* 8-bit timer 1 underflow IDMA request */
#define bIDMAREQ_R16T5_R8T_RSIF0_R8TU2	fIDMAREQ_R16T5_R8T_RSIF0.R8TU2	/* 8-bit timer 2 underflow IDMA request */
#define bIDMAREQ_R16T5_R8T_RSIF0_R8TU3	fIDMAREQ_R16T5_R8T_RSIF0.R8TU3	/* 8-bit timer 3 underflow IDMA request */
#define bIDMAREQ_R16T5_R8T_RSIF0_RSRX0	fIDMAREQ_R16T5_R8T_RSIF0.RSRX0	/* Serial I/F Ch.0 receive buffer full IDMA request */
#define bIDMAREQ_R16T5_R8T_RSIF0_RSTX0	fIDMAREQ_R16T5_R8T_RSIF0.RSTX0	/* Serial I/F Ch.0 transmit buffer empty IDMA request */

////////////////////////////////////////////////////////////
// Serial I/F Ch.1, A/D, port input 4-7 IDMA request registe
#define pIDMAREQ_RSIF1_RAD_RP47		plc_IO->c_INTC.rIDMAREQ4.ucIDMAREQ	/* Serial I/F Ch.1, A/D, port input 4-7 IDMA request register (0x40293) */
#define fIDMAREQ_RSIF1_RAD_RP47		plc_IO->c_INTC.rIDMAREQ4.bIDMAREQ	/* Serial I/F Ch.1, A/D, port input 4-7 IDMA request register bit field */
#define bIDMAREQ_RSIF1_RAD_RP47_RSRX1	fIDMAREQ_RSIF1_RAD_RP47.RSRX1	/* Serial I/F Ch.1 receive buffer full IDMA request */
#define bIDMAREQ_RSIF1_RAD_RP47_RSTX1	fIDMAREQ_RSIF1_RAD_RP47.RSTX1	/* Serial I/F Ch.1 transmit buffer empty IDM request */
#define bIDMAREQ_RSIF1_RAD_RP47_RADE	fIDMAREQ_RSIF1_RAD_RP47.RADE	/* A/D converter IDMA request */
#define bIDMAREQ_RSIF1_RAD_RP47_RP4	fIDMAREQ_RSIF1_RAD_RP47.RP4	/* Port input 4 IDMA request */
#define bIDMAREQ_RSIF1_RAD_RP47_RP5	fIDMAREQ_RSIF1_RAD_RP47.RP5	/* Port input 5 IDMA request */
#define bIDMAREQ_RSIF1_RAD_RP47_RP6	fIDMAREQ_RSIF1_RAD_RP47.RP6	/* Port input 6 IDMA request */
#define bIDMAREQ_RSIF1_RAD_RP47_RP7	fIDMAREQ_RSIF1_RAD_RP47.RP7	/* Port input 7 IDMA request */

////////////////////////////////////////////////////////////////
// Port input 0-3, high-speed DMA, 16-bit timer 0 IDMA enable register
#define pIDMAEN_DEP03_DEHS_DE16T0		plc_IO->c_INTC.rIDMA_EN1.ucIDMA_EN	/* Port input 0-3, high-speed DMA, 16-bit timer 0 IDMA enable register (0x40294) */
#define fIDMAEN_DEP03_DEHS_DE16T0		plc_IO->c_INTC.rIDMA_EN1.bIDMA_EN	/* Port input 0-3, high-speed DMA, 16-bit timer 0 IDMA enable register bit field */
#define bIDMAEN_DEP03_DEHS_DE16T0_DEP0		fIDMAEN_DEP03_DEHS_DE16T0.DEP0		/* Port input 0 IDMA enable */
#define bIDMAEN_DEP03_DEHS_DE16T0_DEP1		fIDMAEN_DEP03_DEHS_DE16T0.DEP1		/* Port input 1 IDMA enable */
#define bIDMAEN_DEP03_DEHS_DE16T0_DEP2		fIDMAEN_DEP03_DEHS_DE16T0.DEP2		/* Port input 2 IDMA enable */
#define bIDMAEN_DEP03_DEHS_DE16T0_DEP3		fIDMAEN_DEP03_DEHS_DE16T0.DEP3		/* Port input 3 IDMA enable */
#define bIDMAEN_DEP03_DEHS_DE16T0_DEHDM0	fIDMAEN_DEP03_DEHS_DE16T0.DEHDM0	/* High-speed DMA Ch.0 IDMA enable */
#define bIDMAEN_DEP03_DEHS_DE16T0_DEHDM1	fIDMAEN_DEP03_DEHS_DE16T0.DEHDM1	/* High-speed DMA Ch.1 IDMA enable */
#define bIDMAEN_DEP03_DEHS_DE16T0_DE16TU0	fIDMAEN_DEP03_DEHS_DE16T0.DE16TU0	/* 16-bit timer 0 comparison B IDMA enable */
#define bIDMAEN_DEP03_DEHS_DE16T0_DE16TC0	fIDMAEN_DEP03_DEHS_DE16T0.DE16TC0	/* 16-bit timer 0 comparison A IDMA enable */

////////////////////////////////////////////////////////////////
// 16-bit timer 1-4 IDMA enable register
#define pIDMAEN_DE16T14		plc_IO->c_INTC.rIDMA_EN2.ucIDMA_EN	/* 16-bit timer 1-4 IDMA enable register (0x40295) */
#define fIDMAEN_DE16T14		plc_IO->c_INTC.rIDMA_EN2.bIDMA_EN	/* 16-bit timer 1-4 IDMA enable register bit field */
#define bIDMAEN_DE16T14_DE16TU1	fIDMAEN_DE16T14.DE16TU1			/* 16-bit timer 1 comparison B IDMA enable */
#define bIDMAEN_DE16T14_DE16TC1	fIDMAEN_DE16T14.DE16TC1			/* 16-bit timer 1 comparison A IDMA enable */
#define bIDMAEN_DE16T14_DE16TU2	fIDMAEN_DE16T14.DE16TU2			/* 16-bit timer 2 comparison B IDMA enable */
#define bIDMAEN_DE16T14_DE16TC2	fIDMAEN_DE16T14.DE16TC2			/* 16-bit timer 2 comparison A IDMA enable */
#define bIDMAEN_DE16T14_DE16TU3	fIDMAEN_DE16T14.DE16TU3			/* 16-bit timer 3 comparison B IDMA enable */
#define bIDMAEN_DE16T14_DE16TC3	fIDMAEN_DE16T14.DE16TC3			/* 16-bit timer 3 comparison A IDMA enable */
#define bIDMAEN_DE16T14_DE16TU4	fIDMAEN_DE16T14.DE16TU4			/* 16-bit timer 4 comparison B IDMA enable */
#define bIDMAEN_DE16T14_DE16TC4	fIDMAEN_DE16T14.DE16TC4			/* 16-bit timer 4 comparison A IDMA enable */

////////////////////////////////////////////////////////////////
// 16-bit timer 5, 8-bit timer, serial I/F Ch.0 IDMA enable register
#define pIDMAEN_DE16T5_DE8T_DESIF0	plc_IO->c_INTC.rIDMA_EN3.ucIDMA_EN		/* 16-bit timer 5, 8-bit timer, serial I/F Ch.0 IDMA enable register (0x40296) */
#define fIDMAEN_DE16T5_DE8T_DESIF0	plc_IO->c_INTC.rIDMA_EN3.bIDMA_EN		/* 16-bit timer 5, 8-bit timer, serial I/F Ch.0 IDMA enable register bit field */
#define bIDMAEN_DE16T5_DE8T_DESIF0_DE16TU5	fIDMAEN_DE16T5_DE8T_DESIF0.DE16TU5	/* 16-bit timer 5 comparison B IDMA enable */
#define bIDMAEN_DE16T5_DE8T_DESIF0_DE16TC5	fIDMAEN_DE16T5_DE8T_DESIF0.DE16TC5	/* 16-bit timer 5 comparison A IDMA enable */
#define bIDMAEN_DE16T5_DE8T_DESIF0_DE8TU0	fIDMAEN_DE16T5_DE8T_DESIF0.DE8TU0	/* 8-bit timer 0 underflow IDMA enable */
#define bIDMAEN_DE16T5_DE8T_DESIF0_DE8TU1	fIDMAEN_DE16T5_DE8T_DESIF0.DE8TU1	/* 8-bit timer 1 underflow IDMA enable */
#define bIDMAEN_DE16T5_DE8T_DESIF0_DE8TU2	fIDMAEN_DE16T5_DE8T_DESIF0.DE8TU2	/* 8-bit timer 2 underflow IDMA enable */
#define bIDMAEN_DE16T5_DE8T_DESIF0_DE8TU3	fIDMAEN_DE16T5_DE8T_DESIF0.DE8TU3	/* 8-bit timer 3 underflow IDMA enable */
#define bIDMAEN_DE16T5_DE8T_DESIF0_DESRX0	fIDMAEN_DE16T5_DE8T_DESIF0.DESRX0	/* Serial I/F Ch.0 receive buffer full IDMA enable */
#define bIDMAEN_DE16T5_DE8T_DESIF0_DESTX0	fIDMAEN_DE16T5_DE8T_DESIF0.DESTX0	/* Serial I/F Ch.0 receive buffer empty IDMA enable */

////////////////////////////////////////////////////////////////
// Serial I/F Ch.1, A/D, port input 4-7 IDMA enable register
#define pIDMAEN_DESIF1_DEAD_DEP47		plc_IO->c_INTC.rIDMA_EN4.ucIDMA_EN	/* Serial I/F Ch.1, A/D, port input 4-7 IDMA enable register (0x40297) */
#define fIDMAEN_DESIF1_DEAD_DEP47		plc_IO->c_INTC.rIDMA_EN4.bIDMA_EN	/* Serial I/F Ch.1, A/D, port input 4-7 IDMA enable register bit field */
#define bIDMAEN_DESIF1_DEAD_DEP47_DESRX1	fIDMAEN_DESIF1_DEAD_DEP47.DESRX1	/* Serial I/F Ch.1 receive buffer full IDMA enable */
#define bIDMAEN_DESIF1_DEAD_DEP47_DESTX1	fIDMAEN_DESIF1_DEAD_DEP47.DESTX1	/* Serial I/F Ch.1 receive buffer empty IDMA enable */
#define bIDMAEN_DESIF1_DEAD_DEP47_DEADE		fIDMAEN_DESIF1_DEAD_DEP47.DEADE		/* A/D converter conversion end IDMA enable */
#define bIDMAEN_DESIF1_DEAD_DEP47_DEP4		fIDMAEN_DESIF1_DEAD_DEP47.DEP4		/* Port input 4 IDMA enable */
#define bIDMAEN_DESIF1_DEAD_DEP47_DEP5		fIDMAEN_DESIF1_DEAD_DEP47.DEP5		/* Port input 5 IDMA enable */
#define bIDMAEN_DESIF1_DEAD_DEP47_DEP6		fIDMAEN_DESIF1_DEAD_DEP47.DEP6		/* Port input 6 IDMA enable */
#define bIDMAEN_DESIF1_DEAD_DEP47_DEP7		fIDMAEN_DESIF1_DEAD_DEP47.DEP7		/* Port input 7 IDMA enable */

////////////////////////////////////////////////////////////////
// High-speed DMA trigger set-up register
#define pHSDMA_HTGR1		plc_IO->c_INTC.rHTGR1.ucHTGR		/* High-speed DMA Ch.0/1 trigger set-up register (0x40298) */
#define fHSDMA_HTGR1		plc_IO->c_INTC.rHTGR1.bHTGR		/* High-speed DMA Ch.0/1 trigger set-up register bit field */
#define bHSDMA_HTGR1_HSD0S	plc_IO->c_INTC.rHTGR1.bHTGR.HSD0S	/* High-speed DMA Ch.0 trigger set-up */
#define bHSDMA_HTGR1_HSD1S	plc_IO->c_INTC.rHTGR1.bHTGR.HSD1S	/* High-speed DMA Ch.1 trigger set-up */

#define pHSDMA_HTGR2		plc_IO->c_INTC.rHTGR2.ucHTGR		/* High-speed DMA Ch.2/3 trigger set-up register (0x40299) */
#define fHSDMA_HTGR2		plc_IO->c_INTC.rHTGR2.bHTGR		/* High-speed DMA Ch.2/3 trigger set-up register bit field */
#define bHSDMA_HTGR2_HSD2S	plc_IO->c_INTC.rHTGR2.bHTGR.HSD2S	/* High-speed DMA Ch.2 trigger set-up */
#define bHSDMA_HTGR2_HSD3S	plc_IO->c_INTC.rHTGR2.bHTGR.HSD3S	/* High-speed DMA Ch.3 trigger set-up */

/* High-speed DMA Ch.0 trigger define */
#define HS0_SOFT	0x00					/* Software trigger */
#define HS0_K50F	0x01					/* K50 input (falling edge) */
#define HS0_K50R	0x02					/* K50 input (rising edge) */
#define HS0_P0		0x03					/* P0 input */
#define HS0_P4		0x04					/* P4 input */
#define HS0_8T0		0x05					/* 8-bit timer Ch.0 underflow */
#define HS0_16T0B	0x06					/* 16-bit timer Ch.0 compare B */
#define HS0_16T0A	0x07					/* 16-bit timer Ch.0 compare A */
#define HS0_16T4B	0x08					/* 16-bit timer Ch.4 compare B */
#define HS0_16T4A	0x09					/* 16-bit timer Ch.4 compare A */
#define HS0_SIF0RXBF	0x0a					/* Serial I/F Ch.0 Rx buffer full */
#define HS0_SIF0TXBE	0x0b					/* Serial I/F Ch.0 Tx buffer empty */
#define HS0_AD		0x0c					/* A/D conversion completion */

/* High-speed DMA Ch.1 trigger define */
#define HS1_SOFT	0x00					/* Software trigger */
#define HS1_K51F	0x01					/* K51 input (falling edge) */
#define HS1_K51R	0x02					/* K51 input (rising edge) */
#define HS1_P1		0x03					/* P1 input */
#define HS1_P5		0x04					/* P5 input */
#define HS1_8T1		0x05					/* 8-bit timer Ch.1 underflow */
#define HS1_16T1B	0x06					/* 16-bit timer Ch.1 compare B */
#define HS1_16T1A	0x07					/* 16-bit timer Ch.1 compare A */
#define HS1_16T5B	0x08					/* 16-bit timer Ch.5 compare B */
#define HS1_16T5A	0x09					/* 16-bit timer Ch.5 compare A */
#define HS1_SIF1RXBF	0x0a					/* Serial I/F Ch.1 Rx buffer full */
#define HS1_SIF1TXBE	0x0b					/* Serial I/F Ch.1 Tx buffer empty */
#define HS1_AD		0x0c					/* A/D conversion completion */

/* High-speed DMA Ch.2 trigger define */
#define HS2_SOFT	0x00					/* Software trigger */
#define HS2_K53F	0x01					/* K53 input (falling edge) */
#define HS2_K53R	0x02					/* K53 input (rising edge) */
#define HS2_P2		0x03					/* P2 input */
#define HS2_P6		0x04					/* P6 input */
#define HS2_8T2		0x05					/* 8-bit timer Ch.2 underflow */
#define HS2_16T2B	0x06					/* 16-bit timer Ch.2 compare B */
#define HS2_16T2A	0x07					/* 16-bit timer Ch.2 compare A */
#define HS2_16T4B	0x08					/* 16-bit timer Ch.4 compare B */
#define HS2_16T4A	0x09					/* 16-bit timer Ch.4 compare A */
#define HS2_SIF0RXBF	0x0a					/* Serial I/F Ch.0 Rx buffer full */
#define HS2_SIF0TXBE	0x0b					/* Serial I/F Ch.0 Tx buffer empty */
#define HS2_AD		0x0c					/* A/D conversion completion */

/* High-speed DMA Ch.3 trigger define */
#define HS3_SOFT	0x00					/* Software trigger */
#define HS3_K54F	0x01					/* K54 input (falling edge) */
#define HS3_K54R	0x02					/* K54 input (rising edge) */
#define HS3_P3		0x03					/* P3 input */
#define HS3_P7		0x04					/* P7 input */
#define HS3_8T3		0x05					/* 8-bit timer Ch.3 underflow */
#define HS3_16T3B	0x06					/* 16-bit timer Ch.3 compare B */
#define HS3_16T3A	0x07					/* 16-bit timer Ch.3 compare A */
#define HS3_16T5B	0x08					/* 16-bit timer Ch.5 compare B */
#define HS3_16T5A	0x09					/* 16-bit timer Ch.5 compare A */
#define HS3_SIF1RXBF	0x0a					/* Serial I/F Ch.1 Rx buffer full */
#define HS3_SIF1TXBE	0x0b					/* Serial I/F Ch.1 Tx buffer empty */
#define HS3_AD		0x0c					/* A/D conversion completion */

////////////////////////////////////////////////////////////////
// High-speed DMA software trigger register
#define pHSDMA_HSOFTTGR		plc_IO->c_INTC.rHSOFTTGR.ucHSOFTTGR	/* High-speed DMA software trigger register (0x4029a) */
#define fHSDMA_HSOFTTGR		plc_IO->c_INTC.rHSOFTTGR.bHSOFTTGR	/* High-speed DMA software trigger register bit field */
#define bHSDMA_HSOFTTGR_HST0	fHSDMA_HSOFTTGR.HST0			/* High-speed DMA Ch.0 software trigger */
#define bHSDMA_HSOFTTGR_HST1	fHSDMA_HSOFTTGR.HST1			/* High-speed DMA Ch.1 software trigger */
#define bHSDMA_HSOFTTGR_HST2	fHSDMA_HSOFTTGR.HST2			/* High-speed DMA Ch.2 software trigger */
#define bHSDMA_HSOFTTGR_HST3	fHSDMA_HSOFTTGR.HST3			/* High-speed DMA Ch.3 software trigger */

////////////////////////////////////////////////////////////////
// Flag set/reset method select register
#define pRST_RESET		plc_IO->c_INTC.rRESET.ucRESET	/* Flag set/reset method select register (0x4029f) */
#define fRST_RESET		plc_IO->c_INTC.rRESET.bRESET	/* Flag set/reset method select register bit field */
#define bRST_RESET_RSTONLY	fRST_RESET.RSTONLY		/* Interrupt factor flag reset method selection */
#define bRST_RESET_IDMAONLY	fRST_RESET.IDMAONLY		/* IDMA factor register set method selection */
#define bRST_RESET_DENONLY	fRST_RESET.DENONLY		/* IDMA enable register set method selection */

////////////////////////////////////////////////////////////////
// K5X port register
#define pK5_CFK5	plc_IO->c_KPORT.rCFK5.ucCFK5		/* K5 function select register (0x402c0) */
#define fK5_CFK5	plc_IO->c_KPORT.rCFK5.bCFK5		/* K5 function select register bit field */
#define bK5_CFK5_CFK50	fK5_CFK5.CFK50				/* K50 function selection */
#define bK5_CFK5_CFK51	fK5_CFK5.CFK51				/* K51 function selection */
#define bK5_CFK5_CFK52	fK5_CFK5.CFK52				/* K52 function selection */
#define bK5_CFK5_CFK53	fK5_CFK5.CFK53				/* K53 function selection */
#define bK5_CFK5_CFK54	fK5_CFK5.CFK54				/* K54 function selection */

#define pK5_K5D		plc_IO->c_KPORT.rK5D.ucK5D		/* K5 input port data register (0x402c1) */
#define fK5_K5D		plc_IO->c_KPORT.rK5D.bK5D		/* K5 input port data register bit field */
#define bK5_K5D_K50D	fK5_K5D.K50D				/* K50 input port data */
#define bK5_K5D_K51D	fK5_K5D.K51D				/* K51 input port data */
#define bK5_K5D_K52D	fK5_K5D.K52D				/* K52 input port data */
#define bK5_K5D_K53D	fK5_K5D.K53D				/* K53 input port data */
#define bK5_K5D_K54D	fK5_K5D.K54D				/* K54 input port data */

////////////////////////////////////////////////////////////////
// K6X port register
#define pK6_CFK6	plc_IO->c_KPORT.rCFK6.ucCFK6		/* K6 function select register (0x402c3) */
#define fK6_CFK6	plc_IO->c_KPORT.rCFK6.bCFK6		/* K6 function select register bit field */
#define bK6_CFK6_CFK60	fK6_CFK6.CFK60				/* K60 function selection */
#define bK6_CFK6_CFK61	fK6_CFK6.CFK61				/* K61 function selection */
#define bK6_CFK6_CFK62	fK6_CFK6.CFK62				/* K62 function selection */
#define bK6_CFK6_CFK63	fK6_CFK6.CFK63				/* K63 function selection */
#define bK6_CFK6_CFK64	fK6_CFK6.CFK64				/* K64 function selection */
#define bK6_CFK6_CFK65	fK6_CFK6.CFK65				/* K65 function selection */
#define bK6_CFK6_CFK66	fK6_CFK6.CFK66				/* K66 function selection */
#define bK6_CFK6_CFK67	fK6_CFK6.CFK67				/* K67 function selection */

#define pK6_K6D		plc_IO->c_KPORT.rK6D.ucK6D		/* K6 input port data register (0x402c4) */
#define fK6_K6D		plc_IO->c_KPORT.rK6D.bK6D		/* K6 input port data register bit field */
#define bK6_K6D_K60D	fK6_K6D.K60D				/* K60 input port data */
#define bK6_K6D_K61D	fK6_K6D.K61D				/* K61 input port data */
#define bK6_K6D_K62D	fK6_K6D.K62D				/* K62 input port data */
#define bK6_K6D_K63D	fK6_K6D.K63D				/* K63 input port data */
#define bK6_K6D_K64D	fK6_K6D.K64D				/* K64 input port data */
#define bK6_K6D_K65D	fK6_K6D.K65D				/* K65 input port data */
#define bK6_K6D_K66D	fK6_K6D.K66D				/* K66 input port data */
#define bK6_K6D_K67D	fK6_K6D.K67D				/* K67 input port data */

////////////////////////////////////////////////////////////////
// Port input interrupt select register
#define pPINTSEL_SPT03		plc_IO->c_PINT.rSPT1.ucSPT03	/* Port input interrupt select register1 (0x402c6) */
#define fPINTSEL_SPT03		plc_IO->c_PINT.rSPT1.bSPT03	/* Port input interrupt select register1 bit field */
#define bPINTSEL_SPT03_SPT0	fPINTSEL_SPT03.SPT0		/* FPT0 interrupt input port selection */
#define bPINTSEL_SPT03_SPT1	fPINTSEL_SPT03.SPT1		/* FPT1 interrupt input port selection */
#define bPINTSEL_SPT03_SPT2	fPINTSEL_SPT03.SPT2		/* FPT2 interrupt input port selection */
#define bPINTSEL_SPT03_SPT3	fPINTSEL_SPT03.SPT3		/* FPT3 interrupt input port selection */

#define pPINTSEL_SPT47		plc_IO->c_PINT.rSPT2.ucSPT47	/* Port input interrupt select register2 (0x402c7) */
#define fPINTSEL_SPT47		plc_IO->c_PINT.rSPT2.bSPT47	/* Port input interrupt select register2 bit field */
#define bPINTSEL_SPT47_SPT4	fPINTSEL_SPT47.SPT4		/* FPT4 interrupt input port selection */
#define bPINTSEL_SPT47_SPT5	fPINTSEL_SPT47.SPT5		/* FPT5 interrupt input port selection */
#define bPINTSEL_SPT47_SPT6	fPINTSEL_SPT47.SPT6		/* FPT6 interrupt input port selection */
#define bPINTSEL_SPT47_SPT7	fPINTSEL_SPT47.SPT7		/* FPT7 interrupt input port selection */

////////////////////////////////////////////////////////////////
// Port input interrupt input polarity select register
#define pPINTPOL_SPPT		plc_IO->c_PINT.rSPPT.ucSPPT	/* Port input interrupt input polarity select register (0x402c8) */
#define fPINTPOL_SPPT		plc_IO->c_PINT.rSPPT.bSPPT	/* Port input interrupt input polarity select register bit field */
#define bPINTPOL_SPPT_SPP0	fPINTPOL_SPPT.SPPT0		/* FPT0 input polarity selection */
#define bPINTPOL_SPPT_SPP1	fPINTPOL_SPPT.SPPT1		/* FPT1 input polarity selection */
#define bPINTPOL_SPPT_SPP2	fPINTPOL_SPPT.SPPT2		/* FPT2 input polarity selection */
#define bPINTPOL_SPPT_SPP3	fPINTPOL_SPPT.SPPT3		/* FPT3 input polarity selection */
#define bPINTPOL_SPPT_SPP4	fPINTPOL_SPPT.SPPT4		/* FPT4 input polarity selection */
#define bPINTPOL_SPPT_SPP5	fPINTPOL_SPPT.SPPT5		/* FPT5 input polarity selection */
#define bPINTPOL_SPPT_SPP6	fPINTPOL_SPPT.SPPT6		/* FPT6 input polarity selection */
#define bPINTPOL_SPPT_SPP7	fPINTPOL_SPPT.SPPT7		/* FPT7 input polarity selection */

////////////////////////////////////////////////////////////////
// Port input interrupt edge/level select register
#define pPINTEL_SEPT		plc_IO->c_PINT.rSEPT.ucSEPT	/* Port input interrupt edge/level select register (0x402c9) */
#define fPINTEL_SEPT		plc_IO->c_PINT.rSEPT.bSEPT	/* Port input interrupt edge/level select register bit field */
#define bPINTEL_SEPT_SEPT0	fPINTEL_SEPT.SEPT0		/* FPT0 edge/level selection */
#define bPINTEL_SEPT_SEPT1	fPINTEL_SEPT.SEPT1		/* FPT1 edge/level selection */
#define bPINTEL_SEPT_SEPT2	fPINTEL_SEPT.SEPT2		/* FPT2 edge/level selection */
#define bPINTEL_SEPT_SEPT3	fPINTEL_SEPT.SEPT3		/* FPT3 edge/level selection */
#define bPINTEL_SEPT_SEPT4	fPINTEL_SEPT.SEPT4		/* FPT4 edge/level selection */
#define bPINTEL_SEPT_SEPT5	fPINTEL_SEPT.SEPT5		/* FPT5 edge/level selection */
#define bPINTEL_SEPT_SEPT6	fPINTEL_SEPT.SEPT6		/* FPT6 edge/level selection */
#define bPINTEL_SEPT_SEPT7	fPINTEL_SEPT.SEPT7		/* FPT7 edge/level selection */

////////////////////////////////////////////////////////////////
// Key input interrupt select register
#define pKINTSEL_SPPK		plc_IO->c_PINT.rSPPK.ucSPPK	/* Key input interrupt select register (0x402ca) */
#define fKINTSEL_SPPK		plc_IO->c_PINT.rSPPK.bSPPK	/* Key input interrupt select register bit field */
#define bKINTSEL_SPPK_SPPK0	fKINTSEL_SPPK.SPPK0		/* FPK0 interrupt input port selection */
#define bKINTSEL_SPPK_SPPK1	fKINTSEL_SPPK.SPPK1		/* FPK1 interrupt input port selection */

////////////////////////////////////////////////////////////////
// Key input interrupt input comparison register
#define pKINTCOMP_SCPK0		plc_IO->c_PINT.rSCPK0.ucSCPK0	/* Key input interrupt (FPK0) input comparison register (0x402cc) */
#define fKINTCOMP_SCPK0		plc_IO->c_PINT.rSCPK0.bSCPK0	/* Key input interrupt (FPK0) input comparison register bit field */
#define bKINTCOMP_SCPK0_SCPK00	fKINTCOMP_SCPK0.SCPK00		/* FPK00 input comparison */
#define bKINTCOMP_SCPK0_SCPK01	fKINTCOMP_SCPK0.SCPK01		/* FPK01 input comparison */
#define bKINTCOMP_SCPK0_SCPK02	fKINTCOMP_SCPK0.SCPK02		/* FPK02 input comparison */
#define bKINTCOMP_SCPK0_SCPK03	fKINTCOMP_SCPK0.SCPK03		/* FPK03 input comparison */
#define bKINTCOMP_SCPK0_SCPK04	fKINTCOMP_SCPK0.SCPK04		/* FPK04 input comparison */

#define pKINTCOMP_SCPK1		plc_IO->c_PINT.rSCPK1.ucSCPK1	/* Key input interrupt (FPK1) input comparison register (0x402cd) */
#define fKINTCOMP_SCPK1		plc_IO->c_PINT.rSCPK1.bSCPK1	/* Key input interrupt (FPK1) input comparison register bit field */
#define bKINTCOMP_SCPK1_SCPK10	fKINTCOMP_SCPK1.SCPK10		/* FPK10 input comparison */
#define bKINTCOMP_SCPK1_SCPK11	fKINTCOMP_SCPK1.SCPK11		/* FPK11 input comparison */
#define bKINTCOMP_SCPK1_SCPK12	fKINTCOMP_SCPK1.SCPK12		/* FPK12 input comparison */
#define bKINTCOMP_SCPK1_SCPK13	fKINTCOMP_SCPK1.SCPK13		/* FPK13 input comparison */

////////////////////////////////////////////////////////////////
// Key input interrupt input mask register
#define pKINTMASK_SMPK0		plc_IO->c_PINT.rSMPK0.ucSMPK0	/* Key input interrupt (FPK0) input mask register (0x402ce) */
#define fKINTMASK_SMPK0		plc_IO->c_PINT.rSMPK0.bSMPK0	/* Key input interrupt (FPK0) input mask register bit field */
#define bKINTMASK_SMPK0_SMPK00	fKINTMASK_SMPK0.SMPK00		/* FPK00 input mask */
#define bKINTMASK_SMPK0_SMPK01	fKINTMASK_SMPK0.SMPK01		/* FPK01 input mask */
#define bKINTMASK_SMPK0_SMPK02	fKINTMASK_SMPK0.SMPK02		/* FPK02 input mask */
#define bKINTMASK_SMPK0_SMPK03	fKINTMASK_SMPK0.SMPK03		/* FPK03 input mask */
#define bKINTMASK_SMPK0_SMPK04	fKINTMASK_SMPK0.SMPK04		/* FPK04 input mask */

#define pKINTMASK_SMPK1		plc_IO->c_PINT.rSMPK1.ucSMPK1	/* Key input interrupt (FPK1) input mask register (0x402cf) */
#define fKINTMASK_SMPK1		plc_IO->c_PINT.rSMPK1.bSMPK1	/* Key input interrupt (FPK1) input mask register bit field */
#define bKINTMASK_SMPK1_SMPK10	fKINTMASK_SMPK1.SMPK10		/* FPK10 input mask */
#define bKINTMASK_SMPK1_SMPK11	fKINTMASK_SMPK1.SMPK11		/* FPK11 input mask */
#define bKINTMASK_SMPK1_SMPK12	fKINTMASK_SMPK1.SMPK12		/* FPK12 input mask */
#define bKINTMASK_SMPK1_SMPK13	fKINTMASK_SMPK1.SMPK13		/* FPK13 input mask */

////////////////////////////////////////////////////////////////
// P0X port register
#define pP0_CFP0	plc_IO->c_PCTL_P0.rCFP.ucCFP	/* P0 function select register (0x402d0) */
#define fP0_CFP0	plc_IO->c_PCTL_P0.rCFP.bCFP	/* P0 function select register bit field */
#define bP0_CFP0_CFP00	fP0_CFP0.CFP0			/* P00 function selection SIN0/P00 */
#define bP0_CFP0_CFP01	fP0_CFP0.CFP1			/* P01 function selection SOUT0/P01 */
#define bP0_CFP0_CFP02	fP0_CFP0.CFP2			/* P02 function selection #SCLK0/P02 */
#define bP0_CFP0_CFP03	fP0_CFP0.CFP3			/* P03 function selection #SRDY0/P03 */
#define bP0_CFP0_CFP04	fP0_CFP0.CFP4			/* P04 function selection SIN1/P04 */
#define bP0_CFP0_CFP05	fP0_CFP0.CFP5			/* P05 function selection SOUT1/P05 */
#define bP0_CFP0_CFP06	fP0_CFP0.CFP6			/* P06 function selection #SCLK1/P06 */
#define bP0_CFP0_CFP07	fP0_CFP0.CFP7			/* P07 function selection #SRDY1/P07 */

#define pP0_P0D		plc_IO->c_PCTL_P0.rPD.ucPD	/* P0 I/O port data register (0x402d1) */
#define fP0_P0D		plc_IO->c_PCTL_P0.rPD.bPD	/* P0 I/O port data register bit field */
#define bP0_P0D_P00D	fP0_P0D.P0D			/* P00 port */
#define bP0_P0D_P01D	fP0_P0D.P1D			/* P01 port */
#define bP0_P0D_P02D	fP0_P0D.P2D			/* P02 port */
#define bP0_P0D_P03D	fP0_P0D.P3D			/* P03 port */
#define bP0_P0D_P04D	fP0_P0D.P4D			/* P04 port */
#define bP0_P0D_P05D	fP0_P0D.P5D			/* P05 port */
#define bP0_P0D_P06D	fP0_P0D.P6D			/* P06 port */
#define bP0_P0D_P07D	fP0_P0D.P7D			/* P07 port */

#define pP0_IOC0	plc_IO->c_PCTL_P0.rIOC.ucIOC	/* P0 I/O control register (0x402d2) */
#define fP0_IOC0	plc_IO->c_PCTL_P0.rIOC.bIOC	/* P0 I/O control register bit field */
#define bP0_IOC0_IOC00	fP0_IOC0.IOC0			/* P00 I/O control */
#define bP0_IOC0_IOC01	fP0_IOC0.IOC1			/* P01 I/O control */
#define bP0_IOC0_IOC02	fP0_IOC0.IOC2			/* P02 I/O control */
#define bP0_IOC0_IOC03	fP0_IOC0.IOC3			/* P03 I/O control */
#define bP0_IOC0_IOC04	fP0_IOC0.IOC4			/* P04 I/O control */
#define bP0_IOC0_IOC05	fP0_IOC0.IOC5			/* P05 I/O control */
#define bP0_IOC0_IOC06	fP0_IOC0.IOC6			/* P06 I/O control */
#define bP0_IOC0_IOC07	fP0_IOC0.IOC7			/* P07 I/O control */

////////////////////////////////////////////////////////////////
// P1X port register
#define pP1_CFP1	plc_IO->c_PCTL_P1.rCFP.ucCFP	/* P1 function select register (0x402d4) */
#define fP1_CFP1	plc_IO->c_PCTL_P1.rCFP.bCFP	/* P1 function select register bit field */
#define bP1_CFP1_CFP10	fP1_CFP1.CFP0			/* P10 function selection EXCL0, T8UF0/P10 */
#define bP1_CFP1_CFP11	fP1_CFP1.CFP1			/* P11 function selection EXCL1, T8UF1/P11 */
#define bP1_CFP1_CFP12	fP1_CFP1.CFP2			/* P12 function selection EXCL2, T8UF2/P12 */
#define bP1_CFP1_CFP13	fP1_CFP1.CFP3			/* P13 function selection EXCL3, T8UF3/P13 */
#define bP1_CFP1_CFP14	fP1_CFP1.CFP4			/* P14 function selection FOSC1/P14 */
#define bP1_CFP1_CFP15	fP1_CFP1.CFP5			/* P15 function selection EXCL4, #DMAEND0/P15 */
#define bP1_CFP1_CFP16	fP1_CFP1.CFP6			/* P16 function selection EXCL5, #DMAEND1/P16 */

#define pP1_P1D		plc_IO->c_PCTL_P1.rPD.ucPD	/* P1 I/O port data register (0x402d5) */
#define fP1_P1D		plc_IO->c_PCTL_P1.rPD.bPD	/* P1 I/O port data register bit field */
#define bP1_P1D_P10D	fP1_P1D.P0D			/* P10 port */
#define bP1_P1D_P11D	fP1_P1D.P1D			/* P11 port */
#define bP1_P1D_P12D	fP1_P1D.P2D			/* P12 port */
#define bP1_P1D_P13D	fP1_P1D.P3D			/* P13 port */
#define bP1_P1D_P14D	fP1_P1D.P4D			/* P14 port */
#define bP1_P1D_P15D	fP1_P1D.P5D			/* P15 port */
#define bP1_P1D_P16D	fP1_P1D.P6D			/* P16 port */

#define pP1_IOC1	plc_IO->c_PCTL_P1.rIOC.ucIOC	/* P1 I/O control register (0x402d6) */
#define fP1_IOC1	plc_IO->c_PCTL_P1.rIOC.bIOC	/* P1 I/O control register bit field */
#define bP1_IOC1_IOC10	fP1_IOC1.IOC0			/* P10 I/O control */
#define bP1_IOC1_IOC11	fP1_IOC1.IOC1			/* P11 I/O control */
#define bP1_IOC1_IOC12	fP1_IOC1.IOC2			/* P12 I/O control */
#define bP1_IOC1_IOC13	fP1_IOC1.IOC3			/* P13 I/O control */
#define bP1_IOC1_IOC14	fP1_IOC1.IOC4			/* P14 I/O control */
#define bP1_IOC1_IOC15	fP1_IOC1.IOC5			/* P15 I/O control */
#define bP1_IOC1_IOC16	fP1_IOC1.IOC6			/* P16 I/O control */

////////////////////////////////////////////////////////////////
// Port SIO function extension register
#define pPSIO3_CFEX		plc_IO->c_PCTL_P1.rCFEX.ucCFEX	/* Port SIO function extension register (0x402d7) */
#define fPSIO3_CFEX		plc_IO->c_PCTL_P1.rCFEX.bCFEX	/* Port SIO function extension register bit field */
#define bPSIO3_CFEX_SSIN3	fPSIO3_CFEX.CFEX0		/* SSIN3  P33 function selection  SIN3 /      #DMAACK1・P33 */
#define bPSIO3_CFEX_SSOUT3	fPSIO3_CFEX.CFEX1		/* SSOUT3 P16 function selection  SOUT3/EXCL5・#DMAEND1・P16 */
#define bPSIO3_CFEX_SSCLK3	fPSIO3_CFEX.CFEX2		/* SSCLK3 P15 function selection #SCLK3/EXCL4・#DMAEND0・P15 */
#define bPSIO3_CFEX_SSRDY3	fPSIO3_CFEX.CFEX3		/* SSRDY3 P32 function selection #SRDY3/      #DMAACK0・P32 */

////////////////////////////////////////////////////////////////
// P2X port register
#define pP2_CFP2	plc_IO->c_PCTL_P2.rCFP.ucCFP	/* P2 function select register (0x402d8) */
#define fP2_CFP2	plc_IO->c_PCTL_P2.rCFP.bCFP	/* P2 function select register bit field */
#define bP2_CFP2_CFP20	fP2_CFP2.CFP0			/* P20 function selection #DRD/P20 */
#define bP2_CFP2_CFP21	fP2_CFP2.CFP1			/* P21 function selection #DWE/P21 */
#define bP2_CFP2_CFP22	fP2_CFP2.CFP2			/* P22 function selection TM0/P22 */
#define bP2_CFP2_CFP23	fP2_CFP2.CFP3			/* P23 function selection TM1/P23 */
#define bP2_CFP2_CFP24	fP2_CFP2.CFP4			/* P24 function selection TM2/P24 */
#define bP2_CFP2_CFP25	fP2_CFP2.CFP5			/* P25 function selection TM3/P25 */
#define bP2_CFP2_CFP26	fP2_CFP2.CFP6			/* P26 function selection TM4/P26 */
#define bP2_CFP2_CFP27	fP2_CFP2.CFP7			/* P27 function selection TM5/P27 */

#define pP2_P2D		plc_IO->c_PCTL_P2.rPD.ucPD	/* P2 I/O port data register (0x402d9) */
#define fP2_P2D		plc_IO->c_PCTL_P2.rPD.bPD	/* P2 I/O port data register bit field */
#define bP2_P2D_P20D	fP2_P2D.P0D			/* P20 port */
#define bP2_P2D_P21D	fP2_P2D.P1D			/* P21 port */
#define bP2_P2D_P22D	fP2_P2D.P2D			/* P22 port */
#define bP2_P2D_P23D	fP2_P2D.P3D			/* P23 port */
#define bP2_P2D_P24D	fP2_P2D.P4D			/* P24 port */
#define bP2_P2D_P25D	fP2_P2D.P5D			/* P25 port */
#define bP2_P2D_P26D	fP2_P2D.P6D			/* P26 port */
#define bP2_P2D_P27D	fP2_P2D.P7D			/* P27 port */

#define pP2_IOC2	plc_IO->c_PCTL_P2.rIOC.ucIOC	/* P2 I/O control register (0x402da) */
#define fP2_IOC2	plc_IO->c_PCTL_P2.rIOC.bIOC	/* P2 I/O control register bit field */
#define bP2_IOC2_IOC20	fP2_IOC2.IOC0			/* P20 I/O control */
#define bP2_IOC2_IOC21	fP2_IOC2.IOC1			/* P21 I/O control */
#define bP2_IOC2_IOC22	fP2_IOC2.IOC2			/* P22 I/O control */
#define bP2_IOC2_IOC23	fP2_IOC2.IOC3			/* P23 I/O control */
#define bP2_IOC2_IOC24	fP2_IOC2.IOC4			/* P24 I/O control */
#define bP2_IOC2_IOC25	fP2_IOC2.IOC5			/* P25 I/O control */
#define bP2_IOC2_IOC26	fP2_IOC2.IOC6			/* P26 I/O control */
#define bP2_IOC2_IOC27	fP2_IOC2.IOC7			/* P27 I/O control */

////////////////////////////////////////////////////////////////
// Port SIO function extension register
#define pPSIO2_CFEX		plc_IO->c_PCTL_P2.rCFEX.ucCFEX	/* Port SIO function extension register (0x402db) */
#define fPSIO2_CFEX		plc_IO->c_PCTL_P2.rCFEX.bCFEX	/* Port SIO function extension register bit field */
#define bPSIO2_CFEX_SSIN2	fPSIO2_CFEX.CFEX0		/* SSIN2  P27 function selection  SIN2 /TM5・P27 */
#define bPSIO2_CFEX_SSOUT2	fPSIO2_CFEX.CFEX1		/* SSOUT2 P26 function selection  SOUT2/TM4・P26 */
#define bPSIO2_CFEX_SSCLK2	fPSIO2_CFEX.CFEX2		/* SSCLK2 P25 function selection #SCLK2/TM3・P25 */
#define bPSIO2_CFEX_SSRDY2	fPSIO2_CFEX.CFEX3		/* SSRDY2 P24 function selection #SRDY2/TM2・P24 */

////////////////////////////////////////////////////////////////
// P3X port register
#define pP3_CFP3	plc_IO->c_PCTL_P3.rCFP.ucCFP	/* P3 function select register (0x402dc) */
#define fP3_CFP3	plc_IO->c_PCTL_P3.rCFP.bCFP	/* P3 function select register bit field */
#define bP3_CFP3_CFP30	fP3_CFP3.CFP0			/* P30 function selection #WAIT, #CE4, #CE5/P30 */
#define bP3_CFP3_CFP31	fP3_CFP3.CFP1			/* P31 function selection #BUSGET/P31 */
#define bP3_CFP3_CFP32	fP3_CFP3.CFP2			/* P32 function selection #DMAACK0/P32 */
#define bP3_CFP3_CFP33	fP3_CFP3.CFP3			/* P33 function selection #DMAACK1/P33 */
#define bP3_CFP3_CFP34	fP3_CFP3.CFP4			/* P34 function selection #BUSREQ, #CE6/P34 */
#define bP3_CFP3_CFP35	fP3_CFP3.CFP5			/* P35 function selection #BUSACK/P35 */

#define pP3_P3D		plc_IO->c_PCTL_P3.rPD.ucPD	/* P3 I/O port data register (0x402dd) */
#define fP3_P3D		plc_IO->c_PCTL_P3.rPD.bPD	/* P3 I/O port data register bit field */
#define bP3_P3D_P30D	fP3_P3D.P0D			/* P30 port */
#define bP3_P3D_P31D	fP3_P3D.P1D			/* P31 port */
#define bP3_P3D_P32D	fP3_P3D.P2D			/* P32 port */
#define bP3_P3D_P33D	fP3_P3D.P3D			/* P33 port */
#define bP3_P3D_P34D	fP3_P3D.P4D			/* P34 port */
#define bP3_P3D_P35D	fP3_P3D.P5D			/* P35 port */

#define pP3_IOC3	plc_IO->c_PCTL_P3.rIOC.ucIOC	/* P3 I/O control register (0x402de) */
#define fP3_IOC3	plc_IO->c_PCTL_P3.rIOC.bIOC	/* P3 I/O control register bit field */
#define bP3_IOC3_IOC30	fP3_IOC3.IOC0			/* P30 I/O control */
#define bP3_IOC3_IOC31	fP3_IOC3.IOC1			/* P31 I/O control */
#define bP3_IOC3_IOC32	fP3_IOC3.IOC2			/* P32 I/O control */
#define bP3_IOC3_IOC33	fP3_IOC3.IOC3			/* P33 I/O control */
#define bP3_IOC3_IOC34	fP3_IOC3.IOC4			/* P34 I/O control */
#define bP3_IOC3_IOC35	fP3_IOC3.IOC5			/* P35 I/O control */

////////////////////////////////////////////////////////////////
// Port function extension register
#define pPF_CFEX	plc_IO->c_PCTL_P3.rCFEX.ucCFEX	/* Port function extension register (0x402df) */
#define fPF_CFEX	plc_IO->c_PCTL_P3.rCFEX.bCFEX	/* Port function extension register bit field */
#define bPF_CFEX_CFEX0	fPF_CFEX.CFEX0			/* CFEX0 P12, P14 port extended function */
#define bPF_CFEX_CFEX1	fPF_CFEX.CFEX1			/* CREX1 P10, P11, P13 port extended function */
#define bPF_CFEX_CFEX2	fPF_CFEX.CFEX2			/* CFEX2 P21 port extended function */
#define bPF_CFEX_CFEX3	fPF_CFEX.CFEX3			/* CFEX3 P32 port extended function */
#define bPF_CFEX_CFEX4	fPF_CFEX.CFEX4			/* CFEX4 P04 port extended function */
#define bPF_CFEX_CFEX5	fPF_CFEX.CFEX5			/* CFEX5 P05 port extended function */
#define bPF_CFEX_CFEX6	fPF_CFEX.CFEX6			/* CFEX6 P06 port extended function */
#define bPF_CFEX_CFEX7	fPF_CFEX.CFEX7			/* CFEX7 P07 port extended function */

////////////////////////////////////////////////////////////////
// Areas 18-15 set-up register
#define pA18_15		plc_IO->c_BCUAREA.rA18_15.usCTL	/* Areas 18-15 set-up register (0x48120) */
#define fA18_15		plc_IO->c_BCUAREA.rA18_15.bCTL	/* Areas 18-15 set-up register bit field */
#define bA18_15_A16WT	fA18_15.A16WT			/* Areas 16-15 wait control */
#define bA18_15_A16DF	fA18_15.A16DF			/* Areas 16-15 output disable delay time */
#define bA18_15_A16SZ	fA18_15.A16SZ			/* Areas 16-15 device size selection */
#define bA18_15_A18WT	fA18_15.A18WT			/* Areas 18-17 wait control */
#define bA18_15_A18DF	fA18_15.A18DF			/* Areas 18-17 output disable delay time */
#define bA18_15_A18SZ	fA18_15.A18SZ			/* Areas 18-17 device size selection */

////////////////////////////////////////////////////////////////
// Areas 14-13 set-up register
#define pA14_13		plc_IO->c_BCUAREA.rA14_13.usCTL	/* Areas 14-13 set-up register (0x48122) */
#define fA14_13		plc_IO->c_BCUAREA.rA14_13.bCTL	/* Areas 14-13 set-up register bit field */
#define bA14_13_A14WT	fA14_13.A14WT			/* Areas 14-13 wait control */
#define bA14_13_A14DF	fA14_13.A14DF			/* Areas 14-13 output disable delay time */
#define bA14_13_A14SZ	fA14_13.A14SZ			/* Areas 14-13 device size selection */
#define bA14_13_A13DRA	fA14_13.A13DRA			/* Area 13 DRAM selection */
#define bA14_13_A14DRA	fA14_13.A14DRA			/* Area 14 DRAM selection */

////////////////////////////////////////////////////////////////
// Areas 12-11 set-up register
#define pA12_11		plc_IO->c_BCUAREA.rA12_11.usCTL	/* Areas 12-11 set-up register (0x48124) */
#define fA12_11		plc_IO->c_BCUAREA.rA12_11.bCTL	/* Areas 12-11 set-up register bit field */
#define bA12_11_A12WT	fA12_11.A12WT			/* Areas 12-11 wait control */
#define bA12_11_A12DF	fA12_11.A12DF			/* Areas 12-11 output disable delay time */
#define bA12_11_A12SZ	fA12_11.A12SZ			/* Areas 12-11 device size selection */

////////////////////////////////////////////////////////////////
// Areas 10-9 set-up register
#define pA10_9		plc_IO->c_BCUAREA.rA10_9.usCTL	/* Areas 10-9 set-up register (0x48126) */
#define fA10_9		plc_IO->c_BCUAREA.rA10_9.bCTL	/* Areas 10-9 set-up register bit field */
#define bA10_9_A10WT	fA10_9.A10WT			/* Areas 10-9 wait control */
#define bA10_9_A10DF	fA10_9.A10DF			/* Areas 10-9 output disable delay time */
#define bA10_9_A10SZ	fA10_9.A10SZ			/* Areas 10-9 device size selection */
#define bA10_9_A9DRA	fA10_9.A9DRA			/* Area 9 burst ROM selection */
#define bA10_9_A10DRA	fA10_9.A10DRA			/* Area 10 burst ROM selection */
#define bA10_9_A10BW	fA10_9.A10BW			/* Areas 10-9 burst ROM burst read cycle wait control */
#define bA10_9_A10IR	fA10_9.A10IR			/* Area 10 internal ROM capacity selection */

////////////////////////////////////////////////////////////////
// Areas 8-7 set-up register
#define pA8_7		plc_IO->c_BCUAREA.rA8_7.usCTL	/* Areas 8-7 set-up register (0x48128) */
#define fA8_7		plc_IO->c_BCUAREA.rA8_7.bCTL	/* Areas 8-7 set-up register bit field */
#define bA8_7_A8WT	fA8_7.A8WT			/* Areas 8-7 wait control */
#define bA8_7_A8DF	fA8_7.A8DF			/* Areas 8-7 output disable delay time */
#define bA8_7_A8SZ	fA8_7.A8SZ			/* Areas 8-7 device size selection */
#define bA8_7_A7DRA	fA8_7.A7DRA			/* Area 7 DRAM selection */
#define bA8_7_A8DRA	fA8_7.A8DRA			/* Area 8 DRAM selection */

////////////////////////////////////////////////////////////////
// Areas 6-4 set-up register
#define pA6_4		plc_IO->c_BCUAREA.rA6_4.usCTL	/* Areas 6-4 set-up register (0x4812a) */
#define fA6_4		plc_IO->c_BCUAREA.rA6_4.bCTL	/* Areas 6-4 set-up register bit field */
#define bA6_4_A5WT	fA6_4.A5WT			/* Areas 5-4 wait control */
#define bA6_4_A5DF	fA6_4.A5DF			/* Areas 5-4 output disable delay time */
#define bA6_4_A5SZ	fA6_4.A5SZ			/* Areas 5-4 device size selection */
#define bA6_4_A6WT	fA6_4.A6WT			/* Area 6 wait control */
#define bA6_4_A6DF	fA6_4.A6DF			/* Area 6 output disable delay time */

////////////////////////////////////////////////////////////////
// TTBR write protect register
#define pTTBR_WP	plc_IO->c_BCUAREA.rTBRP		/* TTBR write protect register (0x4812d) */

////////////////////////////////////////////////////////////////
// Bus control register
#define pBUS_CTL		plc_IO->c_BCUAREA.rBUS.usCTL	/* Bus control register (0x4812e) */
#define fBUS_CTL		plc_IO->c_BCUAREA.rBUS.bCTL	/* Bus control register bit field */
#define bBUS_CTL_SWAITE		fBUS_CTL.SWAITE		/* #WAIT enable */
#define bBUS_CTL_SEPD		fBUS_CTL.SEPD		/* External power-down control */
#define bBUS_CTL_SEMAS		fBUS_CTL.SEMAS		/* External bus master set-up */
#define bBUS_CTL_SEBUSST	fBUS_CTL.SBUSST		/* External interface method selection */
#define bBUS_CTL_S2WE		fBUS_CTL.S2WE		/* DRAM interface method selection */
#define bBUS_CTL_RRA		fBUS_CTL.RRA		/* Refresh RAS pulse width selection */
#define bBUS_CTL_RPC0		fBUS_CTL.RPC0		/* Refresh RPC delay set-up */
#define bBUS_CTL_RPC1		fBUS_CTL.RPC1		/* Refresh method selection */
#define bBUS_CTL_RPC2		fBUS_CTL.RPC2		/* Refresh enable */
#define bBUS_CTL_RCA		fBUS_CTL.RCA		/* Column address size selection */
#define bBUS_CTL_RED0		fBUS_CTL.REDO		/* DRAM page mode selection */
#define bBUS_CTL_RBST8		fBUS_CTL.RBST8		/* Burst ROM burst mode selection */
#define bBUS_CTL_RBCLK		fBUS_CTL.RBCLK		/* BCLK output control */

////////////////////////////////////////////////////////////////
// DRAM timing set-up register
#define pDRAM_SETUP		plc_IO->c_BCUAREA.rDRAM.usCTL	/* DRAM timing set-up register (0x48130) */
#define fDRAM_SETUP		plc_IO->c_BCUAREA.rDRAM.bCTL	/* DRAM timing set-up register bit field */
#define bDRAM_SETUP_RASC	fDRAM_SETUP.RASC		/* DRAM RAS cycle selection */
#define bDRAM_SETUP_CASC	fDRAM_SETUP.CASC		/* DRAM CAS cycle selection */
#define bDRAM_SETUP_RPRC	fDRAM_SETUP.RPRC		/* DRAM RAS precharge cycle selection */
#define bDRAM_SETUP_CRAS	fDRAM_SETUP.CRAS		/* Successive RAS mode */
#define bDRAM_SETUP_CEFUNC	fDRAM_SETUP.CEFUNC		/* #CE pin function selection */
#define bDRAM_SETUP_A3EEN	fDRAM_SETUP.A3EEN		/* Area 3 emulation */

////////////////////////////////////////////////////////////////
// Access control register
#define pACCESS_CTL		plc_IO->c_BCUAREA.rACCESS.usCTL	/* Access control register (0x48132) */
#define fACCESS_CTL		plc_IO->c_BCUAREA.rACCESS.bCTL	/* Access control register bit field */
#define bACCESS_CTL_A5EC	fACCESS_CTL.A5EC		/* Area 5,4 endian control */
#define bACCESS_CTL_A6EC	fACCESS_CTL.A6EC		/* Area 6 endian control */
#define bACCESS_CTL_A8EC	fACCESS_CTL.A8EC		/* Area 8,7 endian control */
#define bACCESS_CTL_A10EC	fACCESS_CTL.A10EC		/* Area 10,9 endian control */
#define bACCESS_CTL_A12EC	fACCESS_CTL.A12EC		/* Area 12,11 endian control */
#define bACCESS_CTL_A14EC	fACCESS_CTL.A14EC		/* Area 14,13 endian control */
#define bACCESS_CTL_A16EC	fACCESS_CTL.A16EC		/* Area 16,15 endian control */
#define bACCESS_CTL_A18EC	fACCESS_CTL.A18EC		/* Area 18,17 endian control */
#define bACCESS_CTL_A5IO	fACCESS_CTL.A5IO		/* Area 5,4 external/internal access */
#define bACCESS_CTL_A6IO	fACCESS_CTL.A6IO		/* Area 6 external/internal access */
#define bACCESS_CTL_A8IO	fACCESS_CTL.A8IO		/* Area 8,7 external/internal access */
#define bACCESS_CTL_A12IO	fACCESS_CTL.A12IO		/* Area 12.11 external/internal access */
#define bACCESS_CTL_A14IO	fACCESS_CTL.A14IO		/* Area 14,13 external/internal access */
#define bACCESS_CTL_A16IO	fACCESS_CTL.A16IO		/* Area 16,15 external/internal access */
#define bACCESS_CTL_A18IO	fACCESS_CTL.A18IO		/* Area 18,17 external/internal access */

////////////////////////////////////////////////////////////////
// TTBR register
#define pTTBR_REG	plc_IO->c_BCUAREA.rTTBR			/* TTBR register (0x48134) */

////////////////////////////////////////////////////////////////
// G/A read signal control register
#define pGA_RSIGCTL		plc_IO->c_BCUAREA.rGA.usCTL	/* G/A read signal control register (0x48138) */
#define fGA_RSIGCTL		plc_IO->c_BCUAREA.rGA.bCTL	/* G/A read signal control register bit field */
#define bGA_RSIGCTL_A5RD	fGA_RSIGCTL.A5RD		/* Area 5,4 read signal */
#define bGA_RSIGCTL_A6RD	fGA_RSIGCTL.A6RD		/* Area 6 read signal */
#define bGA_RSIGCTL_A8RD	fGA_RSIGCTL.A8RD		/* Area 8,7 read signal */
#define bGA_RSIGCTL_A12RD	fGA_RSIGCTL.A12RD		/* Area 12,11 read signal */
#define bGA_RSIGCTL_A14RD	fGA_RSIGCTL.A14RD		/* Area 14,13 read signal */
#define bGA_RSIGCTL_A16RD	fGA_RSIGCTL.A16RD		/* Area 16,15 read signal */
#define bGA_RSIGCTL_A18RD	fGA_RSIGCTL.A18RD		/* Area 18,17 read signal */
#define bGA_RSIGCTL_A5AS	fGA_RSIGCTL.A5AS		/* Area 5,4 address strobe signal */
#define bGA_RSIGCTL_A6AS	fGA_RSIGCTL.A6AS		/* Area 6 address strobe signal */
#define bGA_RSIGCTL_A8AS	fGA_RSIGCTL.A8AS		/* Area 8,7 address strobe signal */
#define bGA_RSIGCTL_A12AS	fGA_RSIGCTL.A12AS		/* Area 12,11 address strobe signal */
#define bGA_RSIGCTL_A14AS	fGA_RSIGCTL.A14AS		/* Area 14,13 address strobe signal */
#define bGA_RSIGCTL_A16AS	fGA_RSIGCTL.A16AS		/* Area 16,15 address strobe signal */
#define bGA_RSIGCTL_A18AS	fGA_RSIGCTL.A18AS		/* Area 18,17 address strobe signal */

////////////////////////////////////////////////////////////////
// BCLK select register
#define pBCLK_SEL	plc_IO->c_BCUAREA.rBCLKSEL		/* BCLK select register (0x4813a) */

/* BCLK selection define */
#define BCLK_CPU	0x00
#define BCLK_BCU	0x01
#define BCLK_OSC3	0x02
#define BCLK_PLL	0x03

////////////////////////////////////////////////////////////////
// 16-bit timer control register
#define pT16_CR0A		plc_IO->c_T16_0.rCRA		/* 16-bit timer 0 comparison data A set-up register (0x48180) */

#define pT16_CR0B		plc_IO->c_T16_0.rCRB		/* 16-bit timer 0 comparison data B set-up register (0x48182) */

#define pT16_TC0		plc_IO->c_T16_0.rTC		/* 16-bit timer 0 counter data register (0x48184) */

#define pT16_CTL0		plc_IO->c_T16_0.rT16CTL.ucCTL	/* 16-bit timer 0 control register (0x48186) */
#define fT16_CTL0		plc_IO->c_T16_0.rT16CTL.bCTL	/* 16-bit timer 0 control register bit field */
#define bT16_CTL0_PRUN0		fT16_CTL0.PRUN			/* 16-bit timer 0 Run/Stop control */
#define bT16_CTL0_PRESET0	fT16_CTL0.PRESET		/* 16-bit timer 0 Reset */
#define bT16_CTL0_PTM0		fT16_CTL0.PTM			/* 16-bit timer 0 clock output control */
#define bT16_CTL0_CKSL0		fT16_CTL0.CKSL			/* 16-bit timer 0 input clock selection */
#define bT16_CTL0_OUTINV0	fT16_CTL0.OUTINV		/* 16-bit timer 0 output inversion */
#define bT16_CTL0_SELCRB0	fT16_CTL0.SELCRB		/* 16-bit timer 0 comparison buffer */
#define bT16_CTL0_SELFM0	fT16_CTL0.SELFM			/* 16-bit timer 0 fine mode selection */

#define pT16_CR1A		plc_IO->c_T16_1.rCRA		/* 16-bit timer 1 comparison data A set-up register (0x48188) */

#define pT16_CR1B		plc_IO->c_T16_1.rCRB		/* 16-bit timer 1 comparison data B set-up register (0x4818a) */

#define pT16_TC1		plc_IO->c_T16_1.rTC		/* 16-bit timer 1 counter data register (0x4818c) */

#define pT16_CTL1		plc_IO->c_T16_1.rT16CTL.ucCTL	/* 16-bit timer 1 control register (0x4818e) */
#define fT16_CTL1		plc_IO->c_T16_1.rT16CTL.bCTL	/* 16-bit timer 1 control register bit field */
#define bT16_CTL1_PRUN1		fT16_CTL1.PRUN			/* 16-bit timer 1 Run/Stop control */
#define bT16_CTL1_PRESET1	fT16_CTL1.PRESET		/* 16-bit timer 1 Reset */
#define bT16_CTL1_PTM1		fT16_CTL1.PTM			/* 16-bit timer 1 clock output control */
#define bT16_CTL1_CKSL1		fT16_CTL1.CKSL			/* 16-bit timer 1 input clock selection */
#define bT16_CTL1_OUTINV1	fT16_CTL1.OUTINV		/* 16-bit timer 1 output inversion */
#define bT16_CTL1_SELCRB1	fT16_CTL1.SELCRB		/* 16-bit timer 1 comparison buffer */
#define bT16_CTL1_SELFM1	fT16_CTL1.SELFM			/* 16-bit timer 1 fine mode selection */

#define pT16_CR2A		plc_IO->c_T16_2.rCRA		/* 16-bit timer 2 comparison data A set-up register (0x48190) */

#define pT16_CR2B		plc_IO->c_T16_2.rCRB		/* 16-bit timer 2 comparison data B set-up register (0x48192) */

#define pT16_TC2		plc_IO->c_T16_2.rTC		/* 16-bit timer 2 counter data register (0x48194) */

#define pT16_CTL2		plc_IO->c_T16_2.rT16CTL.ucCTL	/* 16-bit timer 2 control register (0x48196) */
#define fT16_CTL2		plc_IO->c_T16_2.rT16CTL.bCTL	/* 16-bit timer 2 control register bit field */
#define bT16_CTL2_PRUN2		fT16_CTL2.PRUN			/* 16-bit timer 2 Run/Stop control */
#define bT16_CTL2_PRESET2	fT16_CTL2.PRESET		/* 16-bit timer 2 Reset */
#define bT16_CTL2_PTM2		fT16_CTL2.PTM			/* 16-bit timer 2 clock output control */
#define bT16_CTL2_CKSL2		fT16_CTL2.CKSL			/* 16-bit timer 2 input clock selection */
#define bT16_CTL2_OUTINV2	fT16_CTL2.OUTINV		/* 16-bit timer 2 output inversion */
#define bT16_CTL2_SELCRB2	fT16_CTL2.SELCRB		/* 16-bit timer 2 comparison buffer */
#define bT16_CTL2_SELFM2	fT16_CTL2.SELFM			/* 16-bit timer 2 fine mode selection */

#define pT16_CR3A		plc_IO->c_T16_3.rCRA		/* 16-bit timer 3 comparison data A set-up register (0x48198) */

#define pT16_CR3B		plc_IO->c_T16_3.rCRB		/* 16-bit timer 3 comparison data B set-up register (0x4819a) */

#define pT16_TC3		plc_IO->c_T16_3.rTC		/* 16-bit timer 3 counter data register (0x4819c) */

#define pT16_CTL3		plc_IO->c_T16_3.rT16CTL.ucCTL	/* 16-bit timer 3 control register (0x4819e) */
#define fT16_CTL3		plc_IO->c_T16_3.rT16CTL.bCTL	/* 16-bit timer 3 control register bit field */
#define bT16_CTL3_PRUN3		fT16_CTL3.PRUN			/* 16-bit timer 3 Run/Stop control */
#define bT16_CTL3_PRESET3	fT16_CTL3.PRESET		/* 16-bit timer 3 Reset */
#define bT16_CTL3_PTM3		fT16_CTL3.PTM			/* 16-bit timer 3 clock output control */
#define bT16_CTL3_CKSL3		fT16_CTL3.CKSL			/* 16-bit timer 3 input clock selection */
#define bT16_CTL3_OUTINV3	fT16_CTL3.OUTINV		/* 16-bit timer 3 output inversion */
#define bT16_CTL3_SELCRB3	fT16_CTL3.SELCRB		/* 16-bit timer 3 comparison buffer */
#define bT16_CTL3_SELFM3	fT16_CTL3.SELFM			/* 16-bit timer 3 fine mode selection */

#define pT16_CR4A		plc_IO->c_T16_4.rCRA		/* 16-bit timer 4 comparison data A set-up register (0x481a0) */

#define pT16_CR4B		plc_IO->c_T16_4.rCRB		/* 16-bit timer 4 comparison data B set-up register (0x481a2) */

#define pT16_TC4		plc_IO->c_T16_4.rTC		/* 16-bit timer 4 counter data register (0x481a4) */

#define pT16_CTL4		plc_IO->c_T16_4.rT16CTL.ucCTL	/* 16-bit timer 4 control register (0x481a6) */
#define fT16_CTL4		plc_IO->c_T16_4.rT16CTL.bCTL	/* 16-bit timer 4 control register bit field */
#define bT16_CTL4_PRUN4		fT16_CTL4.PRUN			/* 16-bit timer 4 Run/Stop control */
#define bT16_CTL4_PRESET4	fT16_CTL4.PRESET		/* 16-bit timer 4 Reset */
#define bT16_CTL4_PTM4		fT16_CTL4.PTM			/* 16-bit timer 4 clock output control */
#define bT16_CTL4_CKSL4		fT16_CTL4.CKSL			/* 16-bit timer 4 input clock selection */
#define bT16_CTL4_OUTINV4	fT16_CTL4.OUTINV		/* 16-bit timer 4 output inversion */
#define bT16_CTL4_SELCRB4	fT16_CTL4.SELCRB		/* 16-bit timer 4 comparison buffer */
#define bT16_CTL4_SELFM4	fT16_CTL4.SELFM			/* 16-bit timer 4 fine mode selection */

#define pT16_CR5A		plc_IO->c_T16_5.rCRA		/* 16-bit timer 5 comparison data A set-up register (0x481a8) */

#define pT16_CR5B		plc_IO->c_T16_5.rCRB		/* 16-bit timer 5 comparison data B set-up register (0x481aa) */

#define pT16_TC5		plc_IO->c_T16_5.rTC		/* 16-bit timer 5 counter data register (0x481ac) */

#define pT16_CTL5		plc_IO->c_T16_5.rT16CTL.ucCTL	/* 16-bit timer 5 control register (0x481ae) */
#define fT16_CTL5		plc_IO->c_T16_5.rT16CTL.bCTL	/* 16-bit timer 5 control register bit field */
#define bT16_CTL5_PRUN5		fT16_CTL5.PRUN			/* 16-bit timer 5 Run/Stop control */
#define bT16_CTL5_PRESET5	fT16_CTL5.PRESET		/* 16-bit timer 5 Reset */
#define bT16_CTL5_PTM5		fT16_CTL5.PTM			/* 16-bit timer 5 clock output control */
#define bT16_CTL5_CKSL5		fT16_CTL5.CKSL			/* 16-bit timer 5 input clock selection */
#define bT16_CTL5_OUTINV5	fT16_CTL5.OUTINV		/* 16-bit timer 5 output inversion */
#define bT16_CTL5_SELCRB5	fT16_CTL5.SELCRB		/* 16-bit timer 5 comparison buffer */
#define bT16_CTL5_SELFM5	fT16_CTL5.SELFM			/* 16-bit timer 5 fine mode selection */

/* 16-bit timer macro */
#define PRUN0_RUN	bT16_CTL0_PRUN0 = 1			/* 16-bit timer 0 Run */
#define PRUN0_STOP	bT16_CTL0_PRUN0 = 0			/* 16-bit timer 0 Stop */
#define PRUN1_RUN	bT16_CTL1_PRUN1 = 1			/* 16-bit timer 1 Run */
#define PRUN1_STOP	bT16_CTL1_PRUN1 = 0			/* 16-bit timer 1 Stop */
#define PRUN2_RUN	bT16_CTL2_PRUN2 = 1			/* 16-bit timer 2 Run */
#define PRUN2_STOP	bT16_CTL2_PRUN2 = 0			/* 16-bit timer 2 Stop */
#define PRUN3_RUN	bT16_CTL3_PRUN3 = 1			/* 16-bit timer 3 Run */
#define PRUN3_STOP	bT16_CTL3_PRUN3 = 0			/* 16-bit timer 3 Stop */
#define PRUN4_RUN	bT16_CTL4_PRUN4 = 1			/* 16-bit timer 4 Run */
#define PRUN4_STOP	bT16_CTL4_PRUN4 = 0			/* 16-bit timer 4 Stop */
#define PRUN5_RUN	bT16_CTL5_PRUN5 = 1			/* 16-bit timer 5 Run */
#define PRUN5_STOP	bT16_CTL5_PRUN5 = 0			/* 16-bit timer 5 Stop */
#define PRESET0_RESET	bT16_CTL0_PRESET0 = 1			/* 16-bit timer 0 reset */
#define PRESET1_RESET	bT16_CTL1_PRESET1 = 1			/* 16-bit timer 1 reset */
#define PRESET2_RESET	bT16_CTL2_PRESET2 = 1			/* 16-bit timer 2 reset */
#define PRESET3_RESET	bT16_CTL3_PRESET3 = 1			/* 16-bit timer 3 reset */
#define PRESET4_RESET	bT16_CTL4_PRESET4 = 1			/* 16-bit timer 4 reset */
#define PRESET5_RESET	bT16_CTL5_PRESET5 = 1			/* 16-bit timer 5 reset */

////////////////////////////////////////////////////////////////
// IDMA control register
#define pIDMA_DBASE		plc_IO->c_IDMA.rDBASE		/* IDMA base address (0x48200) */

#define pIDMA_START		plc_IO->c_IDMA.rSTART.ucCTL	/* IDMA start register (0x48204) */
#define fIDMA_START		plc_IO->c_IDMA.rSTART.bCTL	/* IDMA start register bit field */
#define bIDMA_START_DCHN	fIDMA_START.DCHN		/* IDMA channel number */
#define bIDMA_START_DSTART	fIDMA_START.DSTART		/* IDMA start */

#define pIDMA_EN		plc_IO->c_IDMA.rIDMAEN.ucCTL	/* IDMA enable register (0x48205) */
#define fIDMA_EN		plc_IO->c_IDMA.rIDMAEN.bCTL	/* IDMA enable register bit field */
#define bIDMA_EN_IDMAEN		fIDMA_EN.IDMAEN			/* IDMA enable */

/* IDMA control macro */
#define IDMA_START	bIDMA_START_DSTART = 1
#define IDMA_STOP	bIDMA_START_DSTART = 0
#define IDMAEN_ENABLE	bIDMA_EN_IDMAEN = 1
#define IDMAEN_DISABLE	bIDMA_EN_IDMAEN = 0

////////////////////////////////////////////////////////////////
// High-speed DMA control register
#define pHS0_CNT		plc_IO->c_HSDMA0.rCNT.uiCTL	/* High-speed DMA Ch.0 transfer counter and control register (0x48220) */
#define fHS0_CNT_BLK		plc_IO->c_HSDMA0.rCNT.bBLOCK	/* High-speed DMA Ch.0 transfer counter and control register block transfer mode bit field */
#define fHS0_CNT_SIG		plc_IO->c_HSDMA0.rCNT.bSINGLE	/* High-speed DMA Ch.0 transfer counter and control register single transfer mode bit field */
#define bHS0_CNT_BLK_BLKLEN0	fHS0_CNT_BLK.BLKLEN		/* High-speed DMA Ch.0 block length */
#define bHS0_CNT_BLK_TC0	fHS0_CNT_BLK.TC			/* High-speed DMA Ch.0 transfer counter[15:0] */
#define bHS0_CNT_BLK_D0DIR	fHS0_CNT_BLK.DIR		/* High-speed DMA Ch.0 transfer direction control (dual address mode) */
#define bHS0_CNT_BLK_DUALM0	fHS0_CNT_BLK.DUALM		/* High-speed DMA Ch.0 address mode selection */
#define bHS0_CNT_SIG_TC0	fHS0_CNT_SIG.TC			/* High-speed DMA Ch.0 transfer counter[23:0] */
#define bHS0_CNT_SIG_D0DIR	fHS0_CNT_SIG.DIR		/* High-speed DMA Ch.0 transfer direction control (single address mode) */
#define bHS0_CNT_SIG_DUALM0	fHS0_CNT_SIG.DUALM		/* High-speed DMA Ch.0 address mode selection */

#define pHS0_SADR		plc_IO->c_HSDMA0.rSADR.uiCTL	/* High-speed DMA Ch.0 source address set-up register (0x48224) */
#define fHS0_SADR		plc_IO->c_HSDMA0.rSADR.bCTL	/* High-speed DMA Ch.0 source address set-up register bit field */
#define bHS0_SADR_S0ADR		fHS0_SADR.SADR			/* High-speed DMA Ch.0 source address control(D)/memory address control(S) [27:0] */
#define bHS0_SADR_S0IN		fHS0_SADR.SIN			/* High-speed DMA Ch.0 source address control(D)/memory address control(S) */
#define bHS0_SADR_DATSIZE0	fHS0_SADR.DATSIZE		/* High-speed DMA Ch.0 transfer data size */
#define bHS0_SADR_DINTEN0	fHS0_SADR.DINTEN		/* High-speed DMA Ch.0 interrupt enable */

#define pHS0_DADR		plc_IO->c_HSDMA0.rDADR.uiCTL	/* High-speed DMA Ch.0 destination address set-up register (0x48228) */
#define fHS0_DADR		plc_IO->c_HSDMA0.rDADR.bCTL	/* High-speed DMA Ch.0 destination address set-up register bit field */
#define bHS0_DADR_D0ADR		fHS0_DADR.DADR			/* High-speed DMA Ch.0 destination address(D) address control [27:0] */
#define bHS0_DADR_D0IN		fHS0_DADR.DIN			/* High-speed DMA Ch.0 destination address(D) address control */
#define bHS0_DADR_D0MOD		fHS0_DADR.DMOD			/* High-speed DMA Ch.0 transfer mode */

#define pHS0_EN			plc_IO->c_HSDMA0.rHS_EN.usCTL	/* High-speed DMA Ch.0 enable register (0x4822c) */
#define fHS0_EN			plc_IO->c_HSDMA0.rHS_EN.bCTL	/* High-speed DMA Ch.0 enable register bit field */
#define bHS0_EN_HS0EN		fHS0_EN.HS_EN			/* High-speed DMA Ch.0 high-speed DMA enable */

#define pHS0_TF			plc_IO->c_HSDMA0.rTF.usCTL	/* High-speed DMA Ch.0 trigger flag register (0x4822e) */
#define fHS0_TF			plc_IO->c_HSDMA0.rTF.bCTL	/* High-speed DMA Ch.0 trigger flag register bit field) */
#define bHS0_TF_HS0TF		fHS0_TF.HS_TF			/* High-speed DMA Ch.0 trigger flag clear(WR)/trigger flag status(RD) */

#define pHS1_CNT		plc_IO->c_HSDMA1.rCNT.uiCTL	/* High-speed DMA Ch.1 transfer counter and control register (0x48230) */
#define fHS1_CNT_BLK		plc_IO->c_HSDMA1.rCNT.bBLOCK	/* High-speed DMA Ch.1 transfer counter and control register block transfer mode bit field */
#define fHS1_CNT_SIG		plc_IO->c_HSDMA1.rCNT.bSINGLE	/* High-speed DMA Ch.1 transfer counter and control register single transfer mode bit field */
#define bHS1_CNT_BLK_BLKLEN1	fHS1_CNT_BLK.BLKLEN		/* High-speed DMA Ch.1 block length */
#define bHS1_CNT_BLK_TC1	fHS1_CNT_BLK.TC			/* High-speed DMA Ch.1 transfer counter[15:0] */
#define bHS1_CNT_BLK_D1DIR	fHS1_CNT_BLK.DIR		/* High-speed DMA Ch.1 transfer direction control (dual address mode) */
#define bHS1_CNT_BLK_DUALM1	fHS1_CNT_BLK.DUALM		/* High-speed DMA Ch.1 address mode selection */
#define bHS1_CNT_SIG_TC1	fHS1_CNT_SIG.TC			/* High-speed DMA Ch.1 transfer counter[23:0] */
#define bHS1_CNT_SIG_D1DIR	fHS1_CNT_SIG.DIR		/* High-speed DMA Ch.1 transfer direction control (single address mode) */
#define bHS1_CNT_SIG_DUALM1	fHS1_CNT_SIG.DUALM		/* High-speed DMA Ch.1 address mode selection */

#define pHS1_SADR		plc_IO->c_HSDMA1.rSADR.uiCTL	/* High-speed DMA Ch.1 source address set-up register (0x48234) */
#define fHS1_SADR		plc_IO->c_HSDMA1.rSADR.bCTL	/* High-speed DMA Ch.1 source address set-up register bit field */
#define bHS1_SADR_S1ADR		fHS1_SADR.SADR			/* High-speed DMA Ch.1 source address control(D)/memory address control(S) [27:0] */
#define bHS1_SADR_S1IN		fHS1_SADR.SIN			/* High-speed DMA Ch.1 source address control(D)/memory address control(S) */
#define bHS1_SADR_DATSIZE1	fHS1_SADR.DATSIZE		/* High-speed DMA Ch.1 transfer data size */
#define bHS1_SADR_DINTEN1	fHS1_SADR.DINTEN		/* High-speed DMA Ch.1 interrupt enable */

#define pHS1_DADR		plc_IO->c_HSDMA1.rDADR.uiCTL	/* High-speed DMA Ch.1 destination address set-up register (0x48238) */
#define fHS1_DADR		plc_IO->c_HSDMA1.rDADR.bCTL	/* High-speed DMA Ch.1 destination address set-up register bit field */
#define bHS1_DADR_D1ADR		fHS1_DADR.DADR			/* High-speed DMA Ch.1 destination address(D) address control [27:0] */
#define bHS1_DADR_D1IN		fHS1_DADR.DIN			/* High-speed DMA Ch.1 destination address(D) address control */
#define bHS1_DADR_D1MOD		fHS1_DADR.DMOD			/* High-speed DMA Ch.1 transfer mode */

#define pHS1_EN			plc_IO->c_HSDMA1.rHS_EN.usCTL	/* High-speed DMA Ch.1 enable register (0x4823c) */
#define fHS1_EN			plc_IO->c_HSDMA1.rHS_EN.bCTL	/* High-speed DMA Ch.1 enable register bit field */
#define bHS1_EN_HS1EN		fHS1_EN.HS_EN			/* High-speed DMA Ch.1 high-speed DMA enable */

#define pHS1_TF			plc_IO->c_HSDMA1.rTF.usCTL	/* High-speed DMA Ch.1 trigger flag register (0x4823e) */
#define fHS1_TF			plc_IO->c_HSDMA1.rTF.bCTL	/* High-speed DMA Ch.1 trigger flag register bit field) */
#define bHS1_TF_HS1TF		fHS1_TF.HS_TF			/* High-speed DMA Ch.1 trigger flag clear(WR)/trigger flag status(RD) */

#define pHS2_CNT		plc_IO->c_HSDMA2.rCNT.uiCTL	/* High-speed DMA Ch.2 transfer counter and control register (0x48240) */
#define fHS2_CNT_BLK		plc_IO->c_HSDMA2.rCNT.bBLOCK	/* High-speed DMA Ch.2 transfer counter and control register block transfer mode bit field */
#define fHS2_CNT_SIG		plc_IO->c_HSDMA2.rCNT.bSINGLE	/* High-speed DMA Ch.2 transfer counter and control register single transfer mode bit field */
#define bHS2_CNT_BLK_BLKLEN2	fHS2_CNT_BLK.BLKLEN		/* High-speed DMA Ch.2 block length */
#define bHS2_CNT_BLK_TC2	fHS2_CNT_BLK.TC			/* High-speed DMA Ch.2 transfer counter[15:0] */
#define bHS2_CNT_BLK_D2DIR	fHS2_CNT_BLK.DIR		/* High-speed DMA Ch.2 transfer direction control (dual address mode) */
#define bHS2_CNT_BLK_DUALM2	fHS2_CNT_BLK.DUALM		/* High-speed DMA Ch.2 address mode selection */
#define bHS2_CNT_SIG_TC2	fHS2_CNT_SIG.TC			/* High-speed DMA Ch.2 transfer counter[23:0] */
#define bHS2_CNT_SIG_D2DIR	fHS2_CNT_SIG.DIR		/* High-speed DMA Ch.2 transfer direction control (single address mode) */
#define bHS2_CNT_SIG_DUALM2	fHS2_CNT_SIG.DUALM		/* High-speed DMA Ch.2 address mode selection */

#define pHS2_SADR		plc_IO->c_HSDMA2.rSADR.uiCTL	/* High-speed DMA Ch.2 source address set-up register (0x48244) */
#define fHS2_SADR		plc_IO->c_HSDMA2.rSADR.bCTL	/* High-speed DMA Ch.2 source address set-up register bit field */
#define bHS2_SADR_S2ADR		fHS2_SADR.SADR			/* High-speed DMA Ch.2 source address control(D)/memory address control(S) [27:0] */
#define bHS2_SADR_S2IN		fHS2_SADR.SIN			/* High-speed DMA Ch.2 source address control(D)/memory address control(S) */
#define bHS2_SADR_DATSIZE2	fHS2_SADR.DATSIZE		/* High-speed DMA Ch.2 transfer data size */
#define bHS2_SADR_DINTEN2	fHS2_SADR.DINTEN		/* High-speed DMA Ch.2 interrupt enable */

#define pHS2_DADR		plc_IO->c_HSDMA2.rDADR.uiCTL	/* High-speed DMA Ch.2 destination address set-up register (0x48248) */
#define fHS2_DADR		plc_IO->c_HSDMA2.rDADR.bCTL	/* High-speed DMA Ch.2 destination address set-up register bit field */
#define bHS2_DADR_D2ADR		fHS2_DADR.DADR			/* High-speed DMA Ch.2 destination address(D) address control [27:0] */
#define bHS2_DADR_D2IN		fHS2_DADR.DIN			/* High-speed DMA Ch.2 destination address(D) address control */
#define bHS2_DADR_D2MOD		fHS2_DADR.DMOD			/* High-speed DMA Ch.2 transfer mode */

#define pHS2_EN			plc_IO->c_HSDMA2.rHS_EN.usCTL	/* High-speed DMA Ch.2 enable register (0x4824c) */
#define fHS2_EN			plc_IO->c_HSDMA2.rHS_EN.bCTL	/* High-speed DMA Ch.2 enable register bit field */
#define bHS2_EN_HS2EN		fHS2_EN.HS_EN			/* High-speed DMA Ch.2 high-speed DMA enable */

#define pHS2_TF			plc_IO->c_HSDMA2.rTF.usCTL	/* High-speed DMA Ch.2 trigger flag register (0x4824e) */
#define fHS2_TF			plc_IO->c_HSDMA2.rTF.bCTL	/* High-speed DMA Ch.2 trigger flag register bit field) */
#define bHS2_TF_HS2TF		fHS2_TF.HS_TF			/* High-speed DMA Ch.2 trigger flag clear(WR)/trigger flag status(RD) */

#define pHS3_CNT		plc_IO->c_HSDMA3.rCNT.uiCTL	/* High-speed DMA Ch.3 transfer counter and control register (0x48250) */
#define fHS3_CNT_BLK		plc_IO->c_HSDMA3.rCNT.bBLOCK	/* High-speed DMA Ch.3 transfer counter and control register block transfer mode bit field */
#define fHS3_CNT_SIG		plc_IO->c_HSDMA3.rCNT.bSINGLE	/* High-speed DMA Ch.3 transfer counter and control register single transfer mode bit field */
#define bHS3_CNT_BLK_BLKLEN3	fHS3_CNT_BLK.BLKLEN		/* High-speed DMA Ch.3 block length */
#define bHS3_CNT_BLK_TC3	fHS3_CNT_BLK.TC			/* High-speed DMA Ch.3 transfer counter[15:0] */
#define bHS3_CNT_BLK_D3DIR	fHS3_CNT_BLK.DIR		/* High-speed DMA Ch.3 transfer direction control (dual address mode) */
#define bHS3_CNT_BLK_DUALM3	fHS3_CNT_BLK.DUALM		/* High-speed DMA Ch.3 address mode selection */
#define bHS3_CNT_SIG_TC3	fHS3_CNT_SIG.TC			/* High-speed DMA Ch.3 transfer counter[23:0] */
#define bHS3_CNT_SIG_D3DIR	fHS3_CNT_SIG.DIR		/* High-speed DMA Ch.3 transfer direction control (single address mode) */
#define bHS3_CNT_SIG_DUALM3	fHS3_CNT_SIG.DUALM		/* High-speed DMA Ch.3 address mode selection */

#define pHS3_SADR		plc_IO->c_HSDMA3.rSADR.uiCTL	/* High-speed DMA Ch.3 source address set-up register (0x48254) */
#define fHS3_SADR		plc_IO->c_HSDMA3.rSADR.bCTL	/* High-speed DMA Ch.3 source address set-up register bit field */
#define bHS3_SADR_S3ADR		fHS3_SADR.SADR			/* High-speed DMA Ch.3 source address control(D)/memory address control(S) [27:0] */
#define bHS3_SADR_S3IN		fHS3_SADR.SIN			/* High-speed DMA Ch.3 source address control(D)/memory address control(S) */
#define bHS3_SADR_DATSIZE3	fHS3_SADR.DATSIZE		/* High-speed DMA Ch.3 transfer data size */
#define bHS3_SADR_DINTEN3	fHS3_SADR.DINTEN		/* High-speed DMA Ch.3 interrupt enable */

#define pHS3_DADR		plc_IO->c_HSDMA3.rDADR.uiCTL	/* High-speed DMA Ch.3 destination address set-up register (0x48258) */
#define fHS3_DADR		plc_IO->c_HSDMA3.rDADR.bCTL	/* High-speed DMA Ch.3 destination address set-up register bit field */
#define bHS3_DADR_D3ADR		fHS3_DADR.DADR			/* High-speed DMA Ch.3 destination address(D) address control [27:0] */
#define bHS3_DADR_D3IN		fHS3_DADR.DIN			/* High-speed DMA Ch.3 destination address(D) address control */
#define bHS3_DADR_D3MOD		fHS3_DADR.DMOD			/* High-speed DMA Ch.3 transfer mode */

#define pHS3_EN			plc_IO->c_HSDMA3.rHS_EN.usCTL	/* High-speed DMA Ch.3 enable register (0x4825c) */
#define fHS3_EN			plc_IO->c_HSDMA3.rHS_EN.bCTL	/* High-speed DMA Ch.3 enable register bit field */
#define bHS3_EN_HS3EN		fHS3_EN.HS_EN			/* High-speed DMA Ch.3 high-speed DMA enable */

#define pHS3_TF			plc_IO->c_HSDMA3.rTF.usCTL	/* High-speed DMA Ch.3 trigger flag register (0x4825e) */
#define fHS3_TF			plc_IO->c_HSDMA3.rTF.bCTL	/* High-speed DMA Ch.3 trigger flag register bit field) */
#define bHS3_TF_HS3TF		fHS3_TF.HS_TF			/* High-speed DMA Ch.3 trigger flag clear(WR)/trigger flag status(RD) */

/* High-speed DMA control macro */
#define HS0_ENABLE	bHS0_EN_HS0EN = 1
#define HS0_DISABLE	bHS0_EN_HS0EN = 0
#define HS1_ENABLE	bHS1_EN_HS1EN = 1
#define HS1_DISABLE	bHS1_EN_HS1EN = 0
#define HS2_ENABLE	bHS2_EN_HS2EN = 1
#define HS2_DISABLE	bHS2_EN_HS2EN = 0
#define HS3_ENABLE	bHS3_EN_HS3EN = 1
#define HS3_DISABLE	bHS3_EN_HS3EN = 0

/****************************************************************/
/*	割り込みベクタ番号					*/
/****************************************************************/

/*  0〜15：例外要因等の内部割り込み
 *         シンボルの名称は、/usr/PIECE/docs/datasheet/EPSON/cc33eva.zip の中の SAMPLE/drv33A104/ad/vector.s に従いました。
 *         割り込みの説明は、/usr/PIECE/docs/datasheet/EPSON/cc33eva.zip の中の SAMPLE/inclink/prog1/table.s を使いました。
 * 16〜71：マスク可能な外部割り込み
 *         シンボルの名称は、/usr/PIECE/docs/datasheet/EPSON/cc33eva.zip の中の SAMPLE/hdr33208/c33208.h に従いました。
 *         割り込みの説明は、/usr/PIECE/docs/datasheet/EPSON/cc33eva.zip の中の SAMPLE/hdr33208/c33208.h を使いました。
 */

/* 例外要因等の内部割り込み */
#define TRAP_RESET	0	/* Reset */
				/* 1-3: reserved */
#define TRAP_ZERODIV	4	/* Zero Div. */
				/* 5: reserved */
#define TRAP_ADDRERR	6	/* Address Error */
#define TRAP_NMI	7	/* NMI */
				/* 8-11: reserved */
#define TRAP_SOFTINT0	12	/* Software Exception 0 */
#define TRAP_SOFTINT1	13	/* Software Exception 1 */
#define TRAP_SOFTINT2	14	/* Software Exception 2 */
#define TRAP_SOFTINT3	15	/* Software Exception 3 */
/* マスク可能な外部割り込み */
#define TRAP_P0		16	/* Port input 0 */
#define TRAP_P1		17	/* Port input 1 */
#define TRAP_P2		18	/* Port input 2 */
#define TRAP_P3		19	/* Port input 3 */
#define TRAP_K0		20	/* Key input 0 */
#define TRAP_K1		21	/* Key input 1 */
#define TRAP_HDM0	22	/* High-speed DMA Ch.0 */
#define TRAP_HDM1	23	/* High-speed DMA Ch.1 */
#define TRAP_HDM2	24	/* High-speed DMA Ch.2 */
#define TRAP_HDM3	25	/* High-speed DMA Ch.3 */
#define TRAP_IDMA	26	/* IDMA */
				/* 27-29: reserved */
#define TRAP_16TU0	30	/* 16-bit timer 0 comparison B */
#define TRAP_16TC0	31	/* 16-bit timer 0 comparison A */
				/* 32-33: reserved */
#define TRAP_16TU1	34	/* 16-bit timer 1 comparison B */
#define TRAP_16TC1	35	/* 16-bit timer 1 comparison A */
				/* 36-37: reserved */
#define TRAP_16TU2	38	/* 16-bit timer 2 comparison B */
#define TRAP_16TC2	39	/* 16-bit timer 2 comparison A */
				/* 40-41: reserved */
#define TRAP_16TU3	42	/* 16-bit timer 3 comparison B */
#define TRAP_16TC3	43	/* 16-bit timer 3 comparison A */
				/* 44-45: reserved */
#define TRAP_16TU4	46	/* 16-bit timer 4 comparison B */
#define TRAP_16TC4	47	/* 16-bit timer 4 comparison A */
				/* 48-49: reserved */
#define TRAP_16TU5	50	/* 16-bit timer 5 comparison B */
#define TRAP_16TC5	51	/* 16-bit timer 5 comparison A */
#define TRAP_8TU0	52	/* 8-bit timer 0 underflow */
#define TRAP_8TU1	53	/* 8-bit timer 1 underflow */
#define TRAP_8TU2	54	/* 8-bit timer 2 underflow */
#define TRAP_8TU3	55	/* 8-bit timer 3 underflow */
#define TRAP_SERR0	56	/* Serial I/F Ch.0 receive error */
#define TRAP_SRX0	57	/* Serial I/F Ch.0 receive buffer full */
#define TRAP_STX0	58	/* Serial I/F Ch.0 transmit buffer empty */
				/* 59: reserved */
#define TRAP_SERR1	60	/* Serial I/F Ch.1 receive error */
#define TRAP_SRX1	61	/* Serial I/F Ch.1 receive buffer full */
#define TRAP_STX1	62	/* Serial I/F Ch.1 transmit buffer empty */
				/* 63: reserved */
#define TRAP_ADE	64	/* A/D converter */
#define TRAP_CTM	65	/* Clock timer */
				/* 66-67: reserved */
#define TRAP_P4		68	/* Port input 4 */
#define TRAP_P5		69	/* Port input 5 */
#define TRAP_P6		70	/* Port input 6 */
#define TRAP_P7		71	/* Port input 7 */

#endif /*C33209_H*/

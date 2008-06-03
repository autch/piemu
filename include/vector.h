
/////////////////////////////////////////////////////////////////////////////
//
//             /
//      -  P  /  E  C  E  -
//           /                 mobile equipment
//
//              System Programs
//
//
// PIECE KERNEL : Ver 1.00
//
// Copyright (C)2001 AUQAPLUS Co., Ltd. / OeRSTED, Inc. all rights reserved.
//
// Coded by MIO.H (OeRSTED)
//
// Comments:
//
//  ベクター番号の定義です。
//
//  v1.00 2001.11.09 MIO.H
//  v1.05 2001.11.21 MIO.H  TPMAX を 72個に
//



// 0x400 番地からトラップテーブルを置きます。
// 72 個 終わりは 0x51f
//
// 0x20 番地からPieceカーネルサービステーブルを置きます。
// 248 個 終わりは 0x3ff
//

#define TPVECTORTOP		0x400	// トラップテーブルの先頭アドレス(1KB境界)
#define TPMAX   	  	72		// システムが使用するベクタの最大数

#define KSVECTORTOP		0x20	// カーネルサービステーブルの先頭アドレス
#define KSMAX     		248		// カーネルサービスが使用するベクタの最大数


#define TPNO_INT0 12
#define TPNO_INT1 13
#define TPNO_INT2 14
#define TPNO_INT3 15


#define KSNO_VectorSetTrap      0
#define KSNO_VectorSetKs        1
#define KSNO_SystemGetInfo      2
#define KSNO_DebugSetMon        3

#define KSNO_AppSetProcPeriod   4
#define KSNO_AppReqExit         5
#define KSNO_AppExecFile        6
#define KSNO_AppActiveResponse  7

#define KSNO_USBDisconnect      8
#define KSNO_USBReconnect       9
#define KSNO_USBSetupMode      10

#define KSNO_PadGetDirect      12
#define KSNO_PadGetProc        13
#define KSNO_PadGet            14
#define KSNO_PadSetTrigMode    15

#define KSNO_LCDDispStart      16
#define KSNO_LCDDispStop       17
#define KSNO_LCDTrans          18
#define KSNO_LCDSetBuffer      19
#define KSNO_LCDSetOrientation 20
#define KSNO_LCDSetBright      21
#define KSNO_LCDTransDirect    22
#define KSNO_LCDTransRange     23

#define KSNO_FontGetAdrs       24
#define KSNO_FontPut           25
#define KSNO_FontSetType       26
#define KSNO_FontSetPos        27
#define KSNO_FontPutStr        28
#define KSNO_FontPrintf        29
#define KSNO_FontSetTxColor    30
#define KSNO_FontSetBkColor    31

#define KSNO_sprintf           32
#define KSNO_vsprintf          33
#define KSNO_CRC32             34

#define KSNO_CPUSetSpeed       36

#define KSNO_WaveCheckBuffs    40
#define KSNO_WaveDataOut       41
#define KSNO_WaveSetChAtt      42
#define KSNO_WaveSetMasterAtt  43
#define KSNO_WaveStop          44
#define KSNO_WaveAbort         45

#define KSNO_TimeSet           48
#define KSNO_TimeGet           49
#define KSNO_TimeSetAlarm      50
#define KSNO_TimeGetAlarm      51

#define KSNO_FlashErase        64
#define KSNO_FlashWrite        65

#define KSNO_TimerSetContextSwitcher   68
#define KSNO_TimerAdjustPrecisionCount 69
#define KSNO_TimerSetCallback  70
#define KSNO_TimerGetCount     71

#define KSNO_FileFindOpen      72
#define KSNO_FileFindNext      73
#define KSNO_FileFindClose     74
#define KSNO_FileLoad          75
#define KSNO_FileOpen          76
#define KSNO_FileReadSct       77
#define KSNO_FileWriteSct      78
#define KSNO_FileClose         79

#define KSNO_PowerSetReport    80
#define KSNO_PowerGetStatus    81
#define KSNO_PowerForceBatt    82
#define KSNO_PowerEnterStandby 83

#define KSNO_IRStartRx         88
#define KSNO_IRStartTx         89
#define KSNO_IRGetStat         90
#define KSNO_IRStop            91
#define KSNO_IRStartRxEx       92
#define KSNO_IRStartTxEx       93
#define KSNO_IRStartRxPulse    94
#define KSNO_IRStartTxPulse    95

#define KSNO_LCDPoint          96
#define KSNO_LCDLine           97
#define KSNO_LCDPaint          98
#define KSNO_LCDSetObject      99
#define KSNO_LCDDrawObject     100

#define KSNO_USBCOMSetup       104
#define KSNO_USBCOMStartRx     105
#define KSNO_USBCOMStartTx     106
#define KSNO_USBCOMStop        107
#define KSNO_USBCOMGetStat     108

#define KSNO_HeapAlloc         112
#define KSNO_HeapFree          113
#define KSNO_HeapRealloc       114
#define KSNO_HeapGetMaxFreeSize 115

#define KSNO_FileCreate        120
#define KSNO_FileDelete        121
#define KSNO_FileSetBufferMode 122


// 192〜223 は同期用特殊サービス

#define KSNO2_SyncEnterCriticalSection 192
#define KSNO2_SyncLeaveCriticalSection 193
#define KSNO2_SyncSafetyAddition       194
#define KSNO2_SyncSafetySetIL          195
#define KSNO2_TimerGetPrecisionCount   196

// 224〜247 はユーザ拡張用


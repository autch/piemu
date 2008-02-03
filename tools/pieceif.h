
/////////////////////////////////////////////////////////////////////////////
//
//             /
//      -  P  /  E  C  E  -
//           /                 mobile equipment
//
//              System Programs
//
//
// PIECE TOOLS : pieceif.dll : Ver 1.00
//
// Copyright (C)2001 AUQAPLUS Co., Ltd. / OeRSTED, Inc. all rights reserved.
//
// Coded by MIO.H (OeRSTED)
//
// Comments:
//
// USB 転送コアの DLL
//
//  v1.00 2001.11.09 MIO.H
//  v1.01 2001.11.10 MIO.H ismRTCSet 追加
//  v1.06 2001.12.21 MIO.H エラーコード追加
//  v1.07 2002.01.06 MIO.H 複数台接続
//



#ifndef _PIECEIF_H
#define _PIECEIF_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef INDLL
#define DLLAPI __declspec( dllexport ) WINAPI
#define DLLVAR __declspec( dllexport )
#else
#define DLLAPI __declspec( dllimport ) WINAPI
#define DLLVAR extern __declspec( dllimport )
#endif //#ifdef INDLL

typedef struct tagUSBCOMS {
	unsigned char rxstat; // 受信状況
	unsigned char txstat; // 送信状況
	unsigned char pistat; // PIECE のオープンフラグ
	unsigned char mystat; // PC のオープンフラグ
	unsigned long rxlen;  // 受信要求サイズ
	unsigned long txlen;  // 送信要求サイズ
	char signature[16];   // 照合文字列
} USBCOMS;

#define USBCOM_STAT_RXWAIT 1
#define USBCOM_STAT_TXWAIT 1

#define PIECE_INVALID_VERSION (-1)

#define PIECE_DEF_WAITN (-1)

#define PIECE_MAX_DEVICES 22



// low-level interface functions
//
int DLLAPI ismInit( void );
int DLLAPI ismExit( void );
int DLLAPI ismInitEx( int devno, int waitn );
int DLLAPI ismExitEx( int devno );
int DLLAPI ismSelect( int devno );
int DLLAPI ismCmdW( const void *ptr1, unsigned len1, const void *ptr2, unsigned len2 );
int DLLAPI ismCmdR( const void *ptr1, unsigned len1, void *ptr2, unsigned len2 );

// basic-level interface functions
//
int DLLAPI ismGetVersion( void *ptr, int renew );
int DLLAPI ismWriteMem( const unsigned char *pBuff, unsigned long adrs, unsigned len );
int DLLAPI ismReadMem( unsigned char *pBuff, unsigned long adrs, unsigned len );
int DLLAPI ismExec( unsigned long adrs );

// USBCOM-level interface functions
//
int DLLAPI ismUCOpen( USBCOMS *pucs );
int DLLAPI ismUCClose( void );
int DLLAPI ismUCGetStat( USBCOMS *pucs );
int DLLAPI ismUCWrite( const void *ptr, int len );
int DLLAPI ismUCRead( void *ptr, int len );

// isd-level interface functions -> isdsub.c

int DLLAPI ismAppStop( void );
int DLLAPI ismAppStart( void );
int DLLAPI ismWriteSrfFile( const char *infname, int run );
int DLLAPI ismPFFSDir( char *buff, unsigned len, int flag );
int DLLAPI ismPFFSWrite( const char *fname, const char *infname );
int DLLAPI ismPFFSRead( const char *fname,  const char *outfname );
int DLLAPI ismPFFSDelete( const char *fname );
int DLLAPI ismPFFSInit( void );
int DLLAPI ismRTCSet( int year, int mon, int day, int hour, int min, int sec );
int DLLAPI ismLCDCaptureScreen( unsigned char *buff, int len );
int DLLAPI ismHeapGetAdrs( unsigned long *padr );

#ifdef __cplusplus
};
#endif

// エラー番号の定義

#define ERR_PIECEIF_TIMEOUT            1001  // タイムアウト
#define ERR_PIECEIF_ILL_VER            1002  // BIOSのバージョンが違う
#define ERR_PIECEIF_PFFS_EMPTY         1005  // PFFS の容量が一杯でファイルを書き込めない
#define ERR_PIECEIF_PFFS_DIR_EMPTY     1006  // PFFS のディレクトリが一杯でファイルを書き込めない
#define ERR_PIECEIF_PFFS_BAD_FAILENAME 1007  // PFFS のファイル名が不正

#define ERR_PIECEIF_ALREADY_RUNNING 1010  // pieceif.dll 二重起動
//#define ERR_PIECEIF_NO_DRIVERS      1011  // PIECEのデバイスドライバが無い
#define ERR_PIECEIF_OVER_DEVICES      1012  // デバイス数が多すぎる

#endif //_PIECEIF_H

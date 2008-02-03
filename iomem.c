/*
 *  iomem.c
 *
 *  P/EMU - P/ECE Emulator
 *  Copyright (C) 2003 Naoyuki Sawa
 *
 *  * Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *  - 作成開始。
 *  * Wed Apr 22 00:00:00 JST 2003 Naoyuki Sawa
 *  - サウンド対応。（途中）
 *  * Wed Apr 22 05:37:00 JST 2003 Naoyuki Sawa
 *  - muslibのノイズ回避。(サウンド割り込みのレベルを下げた)
 */
#include "app.h"

#include <math.h>

/****************************************************************************
 *  グローバル変数
 ****************************************************************************/

//IOMEM iomem;

/****************************************************************************
 *  I/Oマップ定義
 ****************************************************************************/

//#define IOMAP_TBL
//#include "iomap.h"
//#undef IOMAP_TBL

// private
DECLSPEC void SDLCALL SDLAudioCallback(void* userdata, Uint8* stream, int len);

/****************************************************************************
 *  グローバル関数
 ****************************************************************************/

void
iomem_init(PIEMU_CONTEXT* context)
{
  int i;
  IOMAP* p;
  IOMAP iomap;
  SDL_AudioSpec desired;
  WAVEBUFFER* buffer;

  memset(&context->iomem, 0, sizeof context->iomem);

  context->iomem.iomap_tbl = (IOMAP*)calloc(IOMAP_SIZE, sizeof(IOMAP));
  p = context->iomem.iomap_tbl;

/* PAD */
// IOR_(pK5_K5D)
  iomap.addr = PIOR_(pK5_K5D); iomap.read = IOR_(pK5_K5D); iomap.write = NULL;
  *p++ = iomap;
// IOR_(pK6_K6D)
  iomap.addr = PIOR_(pK6_K6D); iomap.read = IOR_(pK6_K6D); iomap.write = NULL;
  *p++ = iomap;
/* LCDC */
// IO_W(pSIF3_TXD)
  iomap.addr = PIO_W(pSIF3_TXD); iomap.read = NULL; iomap.write = IO_W(pSIF3_TXD);
  *p++ = iomap;
/* SOUND */
// IO_W(pHS1_EN)
  iomap.addr = PIO_W(pHS1_EN); iomap.read = NULL; iomap.write = IO_W(pHS1_EN);
  *p++ = iomap;

  /* LCDC */
  bSIF3_STATUS_TDBE3 = 1; /* 常に転送データバッファエンプティ */

  /* SOUND */

  desired.freq = 32000;
  desired.channels = 1;
  desired.format = AUDIO_S16LSB;
  desired.samples = WAVEBUFFER_SAMPLES; // 変えちゃだめ
  desired.callback = SDLAudioCallback;
  desired.userdata = (void*)context;

  for(i = 0, buffer = context->iomem.buffer; i < BLKN; i++, buffer++)
  {
    buffer->dwBufferLength = desired.samples * sizeof(short);
    buffer->pData = malloc(buffer->dwBufferLength);
    buffer->dwBytesRecorded = 0;
    buffer->nReady = WAVEBUFFER_NOT_READY;
    buffer->next = NULL;
  }
  context->iomem.head = context->iomem.tail = NULL;
  context->iomem.nQueuedBuffers = 0;

  if(SDL_OpenAudio(&desired, NULL) < 0)
  {
#ifdef PSP
    //pspDebugScreenPrintf("can't open audio\n");
#endif
  }
}

static IOMAP*
iomap_sel(PIEMU_CONTEXT* context, unsigned ofs)
{
  IOMAP* iomap;
  /* 呼び出し側で「ofs &= IOMEM_SIZE - 1;」しておくこと。 */
  for(iomap = context->iomem.iomap_tbl; iomap->addr; iomap++) {
    if(ofs == IOOFS(iomap->addr)) return iomap;
  }
  return NULL;
}

int
iomem_read(PIEMU_CONTEXT* context, unsigned ofs, int size)
{
  IOMAP* iomap;

  ofs &= IOMEM_SIZE - 1; /* 必要 */

  /* ハンドラ検索。 */
  iomap = iomap_sel(context, ofs);
  if(iomap && iomap->read) {
    return iomap->read(context, ofs, size);
  }

  /* ハンドラがなければ、iomem.mem[]から単純に読み出す。 */
  // dbg("未対応I/O読み出し: %07x", ofs);
  return iomem_read_default(context, ofs, size);
}

/* iomem.mem[]からの読み出し。各I/Oハンドラからも利用可能です。 */
int
iomem_read_default(PIEMU_CONTEXT* context, unsigned ofs, int size)
{
  if(ofs & ~(IOMEM_SIZE - 1)) DIE();
  switch(size) {
  case 1: return READ_MEM_B(context->iomem.mem + ofs);
  case 2: return READ_MEM_H(context->iomem.mem + ofs);
  case 4: return READ_MEM_W(context->iomem.mem + ofs);
  }
  DIE();
  return -1; /* 警告抑制 */
}

void
iomem_write(PIEMU_CONTEXT* context, unsigned ofs, int data, int size)
{
  IOMAP* iomap;

  ofs &= IOMEM_SIZE - 1; /* 必要 */

  /* ハンドラ検索。 */
  iomap = iomap_sel(context, ofs);
  if(iomap && iomap->write) {
    iomap->write(context, ofs, data, size);
    return;
  }

  /* ハンドラがなければ、iomem.mem[]に単純に書き込む。 */
  // dbg("未対応I/O書き込み: %07x", ofs);
  iomem_write_default(context, ofs, data, size);
}

/* iomem.mem[]への書き込み。各I/Oハンドラからも利用可能です。 */
void
iomem_write_default(PIEMU_CONTEXT* context, unsigned ofs, int data, int size)
{
  if(ofs & ~(IOMEM_SIZE - 1)) DIE();
  switch(size) {
  case 1: *(char *)&context->iomem.mem[ofs] = data; return;
  case 2: *(short*)&context->iomem.mem[ofs] = data; return;
  case 4: *(int  *)&context->iomem.mem[ofs] = data; return;
  }
  DIE();
}

/****************************************************************************
 *  I/Oハンドラ
 ****************************************************************************/

int IOR_(pK5_K5D)(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size)
{
  int data = -1;
  if(context->keystate[KEY_SELECT]) data &= ~(1 << 3); /* SELECT */
  if(context->keystate[KEY_START ]) data &= ~(1 << 4); /* START  */
  return data;
}

int IOR_(pK6_K6D)(struct tagPIEMU_CONTEXT* context, unsigned ofs, int size)
{
  int data = -1;
  if(context->keystate[KEY_RIGHT]) data &= ~(1 << 0);
  if(context->keystate[KEY_LEFT ]) data &= ~(1 << 1);
  if(context->keystate[KEY_DOWN ]) data &= ~(1 << 2);
  if(context->keystate[KEY_UP   ]) data &= ~(1 << 3);
  if(context->keystate[KEY_B    ]) data &= ~(1 << 4); /* B */
  if(context->keystate[KEY_A    ]) data &= ~(1 << 5); /* A */
  return data;
}

/*
 1f5  pSIF3_TXD
 1f8  pSIF3_CTL default
 2d9  pP2_P2D   default
8220  pHS0_CNT  default
8224  pHS0_SADR default
8228  pHS0_DADR default
822c  pHS0_EN   default
822e  pHS0_TF   default
*/

void IO_W(pSIF3_TXD)(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size)
{
  for(;;) {
    lcdc_write(context, (unsigned char)data);   /* シリアルCh.3データ→LCDCへ */
    if(!bHS0_EN_HS0EN) break;     /* HSDMA Ch.0停止なら抜ける */
    data = mem_read_nowait(context, bHS0_SADR_S0ADR, 1);  /* 次のデータを取得 */
    bHS0_SADR_S0ADR++;        /* 転送元アドレスを進める */
    if(!--bHS0_CNT_SIG_TC0) bHS0_EN_HS0EN = 0;  /* 転送カウンタを減らし、0になったらDMA停止 */
  }
}

void IO_W(pHS1_EN)(struct tagPIEMU_CONTEXT* context, unsigned ofs, int data, int size)
{
  int i, en_save, buflen;
  short v, *p;
  WAVEBUFFER* buffer;

  /* HSDMA1イネーブルビットの書き込み。 */
  en_save = bHS1_EN_HS1EN;
  iomem_write_default(context, ofs, data, size);
  if(bHS1_EN_HS1EN == en_save) return; /* DISABLE→DISABLE or ENABLE→ENABLE */
  if(!bHS1_EN_HS1EN) /* ENABLE→DISABLE */
  {
    // 発音停止→バッファを破棄
    SDL_PauseAudio(1);
    SDL_LockAudio();
    {
      for(i = 0, buffer = context->iomem.buffer; i < BLKN; i++, buffer++)
        buffer->nReady = WAVEBUFFER_NOT_READY;
      context->iomem.head = context->iomem.tail = NULL;
      context->iomem.nQueuedBuffers = 0;
    }
    SDL_UnlockAudio();
    return;
  }
  /* 以下、DISABLE→ENABLE */

  SDL_LockAudio();
  {
    /* 空きバッファを探します。 */
    for(i = 0, buffer = context->iomem.buffer; i < BLKN; i++, buffer++)
    {
      if(buffer->nReady == WAVEBUFFER_NOT_READY) break;
    }
    if(i == BLKN) DIE(); // no buffers available

    /* 必要に応じてバッファメモリを拡張。 */
    buflen = bHS1_CNT_SIG_TC1 * sizeof(short);
    if(buffer->dwBufferLength < (unsigned)buflen)
    {
      buffer->pData = realloc(buffer->pData, buflen);
      if(!buffer->pData) DIE();
      buffer->dwBufferLength = buflen;
    }
    buffer->dwBytesRecorded = buflen;

    /* ウェーブデータ変換。 */
    p = (short*)buffer->pData;
    do {
      v = (short)mem_read_nowait(context, bHS1_SADR_S1ADR, 2);  /*      0～750～ 1500 */
      v = v - 750;          /*   -750～  0～  750 */
      v = v * 32767 / 750;        /* -32767～  0～32767 */
      *p++ = v;
      bHS1_SADR_S1ADR += 2;
    } while(--bHS1_CNT_SIG_TC1);

    buffer->nReady = WAVEBUFFER_READY;
    buffer->next = NULL;  // こいつが末尾のノード
    if(context->iomem.head) // 先客がいる
      context->iomem.tail->next = buffer; // 現在の末尾の次に割り当てる
    else
      context->iomem.head = buffer; // 先頭に割り当てる
    context->iomem.tail = buffer;   // 末尾を更新する
    context->iomem.nQueuedBuffers++;
  }
  SDL_UnlockAudio();
  if(context->iomem.nQueuedBuffers > WAVEBUFFER_MIN_FILLED_BUFFERS)
    SDL_PauseAudio(0);
}

// オーディオコールバックが別スレッドから呼ばれるのは
// core module にとってきりのいいタイミングでないと困る
void iomem_work(PIEMU_CONTEXT* context)
{
  int i;
  WAVEBUFFER* buffer;

  SDL_LockAudio();
  for(i = 0, buffer = context->iomem.buffer; i < BLKN; i++, buffer++)
  {
    // 使用済みバッファを片付ける
    if(buffer->nReady == WAVEBUFFER_DONE)
    {
      buffer->nReady = WAVEBUFFER_NOT_READY;
      context->iomem.nQueuedBuffers--;
    }
    /* HSDMA1動作中で、空きバッファがあれば、HSDMA1転送完了と見なして割り込み要因発生。 */
    if(bHS1_EN_HS1EN && buffer->nReady == WAVEBUFFER_NOT_READY)
    {
      bHS1_EN_HS1EN = 0;
      bINT_FDMA_FHDM1 = 1;
    }
  }
  SDL_UnlockAudio();
  /* HSDMA1割り込みが許可されていて、割り込み要因が発生していたら、割り込み発行。 */
  if(bINT_EDMA_EHDM1 && bINT_FDMA_FHDM1)
  {
    //core_trap(context, TRAP_HDM1, bINT_PHSD01L_PHSD1L);
    //↑本当はこうですが...
    //　P/ECEカーネルのサウンドドライバは多重割り込みを許可しているのですが、
    //　サウンド処理中(IL=5で多重割り込み受付)にサウンドの割り込み(IL=7なので割り込み可能)が発生すると、
    //　ノイズになるようです。
    //　通常のWave再生なら軽いので大丈夫みたいですけれど、muslibによるBGM再生でノイズが乗りまくります。
    //↓muslibのノイズを防ぐために、超法規的にサウンドの割り込みレベルを下げてみました。
    //　本物のカーネルでも、サウンド処理中のサウンド割り込み受け付けはなくした方がよさそうな気が…(^^;
    core_trap(context, TRAP_HDM1, 1/*5以下ならなんでもいいけど、とりあえず割り込み可能な最低レベルにしてみた*/);
  }
}

DECLSPEC void SDLCALL SDLAudioCallback(void* userdata, Uint8* stream, int len)
{
  int nBytesToRender = len;
  PIEMU_CONTEXT* context = (PIEMU_CONTEXT*)userdata;
  WAVEBUFFER* node;
  Uint8* p = stream;

  node = context->iomem.head;
  while(node && nBytesToRender)
  {
    if(node->nReady == WAVEBUFFER_READY)
    {
      SDL_MixAudio(p, node->pData, node->dwBytesRecorded, SDL_MIX_MAXVOLUME);
      p += node->dwBytesRecorded;
      nBytesToRender -= node->dwBytesRecorded;
      node->nReady = WAVEBUFFER_DONE;
    }
    node = node->next;
    context->iomem.head = node;
  }
}

IOHANDLER_R(iomem, B)
{
  return iomem_read(context, ofs, 1);
}
IOHANDLER_W(iomem, B)
{
  iomem_write(context, ofs, data, 1);
}
IOHANDLER_R(iomem, H)
{
  return iomem_read(context, ofs, 2);
}
IOHANDLER_W(iomem, H)
{
  iomem_write(context, ofs, data, 2);
}
IOHANDLER_R(iomem, W)
{
  return iomem_read(context, ofs, 4);
}
IOHANDLER_W(iomem, W)
{
  iomem_write(context, ofs, data, 4);
}

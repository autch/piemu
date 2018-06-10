/*
 * p/emu -- P/ECE emulator
 * originally coded by Naoyuki Sawa
 * rewritten by Autch (Yui N.)
 */

#if !defined(PIEMU_H)
#define PIEMU_H

// ここにすべての状態変数とコールバックを持つ
typedef struct tagPIEMU_CONTEXT
{
  EMU emu;
  CORE core;
  FLASH flash;
  USBC usbc;
  SRAM sram;
  FRAM fram;
  LCDC lcdc;
  IOMEM iomem;
  int o_oc;   /* オーバークロックレート(〜100[%]〜) */
  int o_fps;    /* 画面更新レート（[frame/sec]） */
  int o_nowait;   /* 実時間との同期なし（0:あり/1:なし） */
  int o_dbg;    /* デバッグメッセージの出力有り？ */
  //
  uint8_t* keystate;    /* キー状態 */
  uint8_t vbuff[DISP_Y][DISP_X];  /* 仮想VRAM */
  uint32_t texture_pixels[DISP_Y * DISP_X];  // pixels in ARGB8888

  // SDL
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  SDL_Joystick* pad; // only for PSP

  int bEndFlag;

  void* pUser;
  // zero if failure, others success.
  int (*pfnSetEmuParameters)(struct tagPIEMU_CONTEXT* context, EMU* pEmuInfo, void* pUser);
  int (*pfnLoadFlashImage)(struct tagPIEMU_CONTEXT* context, FLASH* pFlashInfo, void* pUser);
  int (*pfnUpdateScreen)(struct tagPIEMU_CONTEXT* context, void* pUser);
}PIEMU_CONTEXT;


int SetEmuParameters(struct tagPIEMU_CONTEXT* context, EMU* pEmuInfo, void* pUser);
int LoadFlashImage(struct tagPIEMU_CONTEXT* context, FLASH* pFlashInfo, void* pUser);
int UpdateScreen(struct tagPIEMU_CONTEXT* context, void* pUser);

#endif // !PIEMU_H

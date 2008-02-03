/*
 *  main.c
 *
 *  P/EMU - P/ECE Emulator
 *  Copyright (C) 2003 Naoyuki Sawa
 *
 *  * Mon Apr 14 00:00:00 JST 2003 Naoyuki Sawa
 *  - 作成開始。
 */
#include "app.h"
//#ifndef PSP
#include "SDL_thread.h"
//#endif
#define SC_CONFIG 100
#define SC_ABOUT  101

#define OC_MIN     20
#define OC_MAX    300
#define OC_DEFAULT  100

#define FPS_MIN    10
#define FPS_MAX   150
#define FPS_DEFAULT  60

#define SEC_CONFIG  "config"
#define KEY_OC    "oc"
#define KEY_FPS   "fps"
#define KEY_NOWAIT  "nowait"
//#define KEY_DBG "dbg" //デバッグメッセージ状態は保存しない

/****************************************************************************
 *  グローバル変数
 ****************************************************************************/

/****************************************************************************
 *  ローカル変数
 ****************************************************************************/

int fullscreen = 0; /* 0:ウインドウモード/1:フルスクリーンモード */
unsigned char fsbuff[DISP_Y * 5][DISP_X * 5]; /* フルスクリーン展開用 */

#ifdef PSP
int nCpuFreq, nBusFreq;
#endif

/****************************************************************************
 *
 ****************************************************************************/

// /* デバッグメッセージの出力 */
// void
// dbg(const char* fmt, ...)
// {
//  if(o_dbg) {
//    va_list ap;
//    va_start(ap, fmt);
//    vfprintf(stderr, fmt, ap);
//    va_end(ap);
//    fprintf(stderr, "\n");
//  }
//  /* 継続 */
// }
//
// /* 異常終了 */
// void
// die(const char* fmt, ...)
// {
//  va_list ap;
//  va_start(ap, fmt);
//  vfprintf(stderr, fmt, ap);
//  va_end(ap);
//  fprintf(stderr, "\n");
//  abort();
// }

/****************************************************************************
 *
 ****************************************************************************/

void ui_init(PIEMU_CONTEXT* context)
{

  SDL_WM_SetCaption("P/EMU/SDL", "P/EMU/SDL");

  context->screen = SDL_SetVideoMode(128, 88, 32, SDL_HWSURFACE /*| SDL_HWPALETTE*/);
//  context->buffer = SDL_CreateRGBSurface(SDL_HWPALETTE, 128, 88, 8, 0xff, 0xff << 8, 0xff << 16, 0xff << 24);

  SDL_EnableKeyRepeat(0, 0);
}

#ifdef LINUX
int main(int argc, char *argv[])
#else
int SDL_main(int argc, char *argv[])
#endif
{
  PIEMU_CONTEXT context;
  SDL_Event event;
  SDL_Thread* thEmuThread;

#ifdef PSP
  nCpuFreq = scePowerGetCpuClockFrequency();
  nBusFreq = scePowerGetBusClockFrequency();
#endif

  SDL_Init(SDL_INIT_EVERYTHING);

  //setvbuf(stdout, NULL, _IONBF, 0);
  //setvbuf(stderr, NULL, _IONBF, 0);

  context.pfnSetEmuParameters = SetEmuParameters;
  context.pfnLoadFlashImage = LoadFlashImage;
  context.pfnUpdateScreen = UpdateScreen;

//  printf("=================================\n");
//  printf("P/EMU - P/ECE Emulator (%s)\n", VERSION);
//  printf("Copyright (C) 2003 Naoyuki Sawa\n");
//  printf("=================================\n");

  /* 設定復元。 */
  context.o_oc = OC_DEFAULT;
  context.o_fps = FPS_DEFAULT;
  context.o_nowait = 0;
  context.o_nowait = context.o_nowait != 0;

  memset(context.vbuff, 0, sizeof context.vbuff);
  memset(context.keystate, 0, sizeof context.keystate);

  /* UI初期化。 */
  ui_init(&context);

  /* エミュレータ初期化。 */
  emu_init(&context);

#ifdef PSP
  SDL_JoystickEventState(SDL_ENABLE);
  if(SDL_NumJoysticks() > 0)
    context.pad = SDL_JoystickOpen(0);

  SDL_ShowCursor(SDL_DISABLE);
#endif

  context.bEndFlag = 0;
  thEmuThread = SDL_CreateThread(emu_work, &context);

  while(1)
  {
    while(SDL_WaitEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT:
          goto L_EXIT;
        case SDL_KEYDOWN:
          context.keystate[event.key.keysym.sym] = 1;
          break;
        case SDL_KEYUP:
          context.keystate[event.key.keysym.sym] = 0;
          break;
#ifdef PSP
        case SDL_JOYBUTTONDOWN:
        {
          switch(event.jbutton.button)
          {
            case JOY_CIRCLE: context.keystate[KEY_A] = 1; break;
            case JOY_CROSS:  context.keystate[KEY_B] = 1; break;
            case JOY_START:  context.keystate[KEY_START] = 1; break;
            case JOY_SELECT: context.keystate[KEY_SELECT] = 1; break;
            case JOY_LEFT:   context.keystate[KEY_LEFT] = 1; break;
            case JOY_RIGHT:  context.keystate[KEY_RIGHT] = 1; break;
            case JOY_UP:     context.keystate[KEY_UP] = 1; break;
            case JOY_DOWN:   context.keystate[KEY_DOWN] = 1; break;
          }
          break;
        }
        case SDL_JOYBUTTONUP:
        {
          switch(event.jbutton.button)
          {
            case JOY_CIRCLE: context.keystate[KEY_A] = 0; break;
            case JOY_CROSS:  context.keystate[KEY_B] = 0; break;
            case JOY_START:  context.keystate[KEY_START] = 0; break;
            case JOY_SELECT: context.keystate[KEY_SELECT] = 0; break;
            case JOY_LEFT:   context.keystate[KEY_LEFT] = 0; break;
            case JOY_RIGHT:  context.keystate[KEY_RIGHT] = 0; break;
            case JOY_UP:     context.keystate[KEY_UP] = 0; break;
            case JOY_DOWN:   context.keystate[KEY_DOWN] = 0; break;
          }
          break;
        }
#endif
      }
    }
  }
L_EXIT:

  context.bEndFlag = 1;
  SDL_WaitThread(thEmuThread, NULL);

  if(SDL_JoystickOpened(0))
    SDL_JoystickClose(context.pad);
  SDL_CloseAudio();

#ifdef PSP
  scePowerSetCpuClockFrequency(nCpuFreq);
  scePowerSetBusClockFrequency(nBusFreq);
#endif

  SDL_Quit();

  return 0;
}

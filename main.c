/** @file main.c
 * Entry point of p/emu
 */

#include <SDL.h>
#include <SDL_thread.h>

int main_event_loop(void);
#ifdef __linux__
int main(int argc, char** argv)
#else
int SDL_main(int argc, char** argv)
#endif
{
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_SetVideoMode(128, 88, 32, SDL_HWSURFACE);

  main_event_loop();

  SDL_Quit();

  return 0;
}

int main_event_loop(void)
{
  SDL_Event event;

  while(1)
  {
    while(SDL_WaitEvent(&event))
    {
      switch(event.type)
      {
      case SDL_QUIT:
	return 0;
      }
    }
  }
  return 0;
}

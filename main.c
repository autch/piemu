/** @file main.c
 * Entry point of p/emu
 */

#include <SDL.h>
#include <SDL_thread.h>

int main_event_loop(void);

int main(int argc, char** argv)
{
  SDL_Init(SDL_INIT_EVERYTHING);

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

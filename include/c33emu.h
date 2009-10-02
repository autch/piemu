
#ifndef c33emu_h
#define c33emu_h

#include "types.h"
#include "c33emu.h"

class c33emu
{
private:
	c33core core;

	SYSTEMINFO info;
	// keystate[]
	// vram[]
	
	// some sdl variables

	// overall control variables

	// host-specific implementation
	// - loding flash
	// - sounds
	// - rendering screen
	// ...

};


#endif // !c33emu_h

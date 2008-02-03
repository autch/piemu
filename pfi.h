/*
 * p/emu/sdl -- sdl port of p/emu, p/ece emulator.
 * p/emu by Naoyuki Sawa
 * p/emu/sdl patched by Yui N.
 */

/*
 * pfi.h -- p/emu/sdl flash image format
 */

#if !defined(PFI_H)
#define PFI_H

typedef struct tagPFIHEADER
{
	unsigned long dwSignature; // 'PFI1', '1' indicates its version, initially 1.
	unsigned long dwOffsetToFlash; // offset, in bytes, to the flash image.
	SYSTEMINFO siSysInfo;          // machine information.
	// ... and flash image follows.
}PFIHEADER;


#endif // !PFI_H

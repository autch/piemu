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
	uint32_t dwSignature; // 'PFI1', '1' indicates its version, initially 1.
	uint32_t dwOffsetToFlash; // offset, in bytes, to the flash image.
	SYSTEMINFO siSysInfo;          // machine information.
	// ... and flash image follows.
}PFIHEADER __attribute__((packed));


#endif // !PFI_H

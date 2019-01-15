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

#ifdef _MSC_VER
#pragma pack(push)
#define ATTR_PACKED /* */
#else
#define ATTR_PACKED	__attribute__((packed))
#endif

typedef struct tagPFIHEADER
{
	uint8_t dwSignature[4]; // 'PFI1', '1' indicates its version, initially 1.
	uint32_t dwOffsetToFlash; // offset, in bytes, to the flash image.
	SYSTEMINFO siSysInfo;          // machine information.
	// ... and flash image follows.
}PFIHEADER ATTR_PACKED;

#ifdef _MSC_VER
#pragma pack(pop)
#endif

#endif // !PFI_H


#include "app.h"
#include "pfi.h"

int SetEmuParameters(struct tagPIEMU_CONTEXT *context, EMU *pEmuInfo, void *pUser) {
    FILE *fp;
    PFIHEADER pfi;

    fp = fopen("piece.pfi", "rb");
    if (fp == NULL) DIE("Cannot open piece.pfi");
    if (!fread(&pfi, sizeof(PFIHEADER), 1, fp)) DIE("Cannot read header from piece.pfi");
    if (memcmp(&pfi.dwSignature, "1IFP", 4) != 0) DIE("Signature mismatch in piece.pfi (possibly newer image version)");
    fclose(fp);

    if (pfi.siSysInfo.size != sizeof(SYSTEMINFO)) DIE("System info header size mismatch in piece.pfi");

    pEmuInfo->sysinfo = pfi.siSysInfo;

    return 1;
}

#define FLASH_TOP 0x0c00000
#define READ_UNIT 0x1000        // 4096

int LoadFlashImage(struct tagPIEMU_CONTEXT *context, FLASH *pFlashInfo, void *pUser) {
    FILE *fp;
    PFIHEADER pfi;
    unsigned int nOffset = 0;

    fp = fopen("piece.pfi", "rb");
    if (!fp) DIE("Cannot open piece.pfi");
    if (!fread(&pfi, sizeof(PFIHEADER), 1, fp)) DIE("Cannot read header from piece.pfi");
    if (memcmp(&pfi.dwSignature, "1IFP", 4) != 0) DIE("Signature mismatch in piece.pfi (possibly newer image version)");
    if (!pfi.dwOffsetToFlash) DIE("Bad image offset in piece.pfi");
    if (pfi.siSysInfo.size != sizeof(SYSTEMINFO)) DIE("System info header size mismatch in piece.pfi");

    fseek(fp, pfi.dwOffsetToFlash, SEEK_SET);

    for (nOffset = 0; nOffset < (unsigned) pFlashInfo->mem_size; nOffset += READ_UNIT) {
        fread(pFlashInfo->mem + nOffset, 1, READ_UNIT, fp);
    }

    fclose(fp);
    return 1;
}

// ARGB8888
static const uint32_t palette[] = {
    0xffffffff,
    0xffaaaaaa,
    0xff555555,
    0xff000000
};

int UpdateScreen(PIEMU_CONTEXT *context, void *pUser) {
    uint32_t *pixels;
    uint8_t *pp;

    pixels = context->texture_pixels;
    pp = *context->vbuff;
    for (int row = 0; row < DISP_Y; row++) {
        for (int col = 0; col < DISP_X; col++) {
            pixels[row * DISP_X + col] = palette[*pp++ & 0x03u];
        }
    }
    SDL_UpdateTexture(context->texture, NULL, pixels, DISP_X * sizeof(uint32_t));

    SDL_RenderClear(context->renderer);
    SDL_RenderCopy(context->renderer, context->texture, NULL, NULL);
    SDL_RenderPresent(context->renderer);

    return 1;
}

/*	
 *	lcdc.c
 *
 *	P/EMU - P/ECE Emulator
 *	Copyright (C) 2003 Naoyuki Sawa
 *
 *	* Mon Apr 16 05:06:00 JST 2003 Naoyuki Sawa
 *	- 作成開始。
 */
#include "app.h"

/****************************************************************************
 *	グローバル変数
 ****************************************************************************/

//LCDC lcdc;

/****************************************************************************
 *	グローバル関数
 ****************************************************************************/

void
lcdc_init(PIEMU_CONTEXT* context)
{
	memset(&context->lcdc, 0, sizeof context->lcdc);
}

void
lcdc_write(PIEMU_CONTEXT* context, unsigned char data)
{
	data = (data & 0x01) << 7 |
	       (data & 0x02) << 5 |
	       (data & 0x04) << 3 |
	       (data & 0x08) << 1 |
	       (data & 0x10) >> 1 |
	       (data & 0x20) >> 3 |
	       (data & 0x40) >> 5 |
	       (data & 0x80) >> 7;

	if(!bP2_P2D_P21D) { /* コマンド */
		/* 以下の処理では、他のコマンドやパラメータも間違って処理してしまう可能性がありますが、
		 * 現在のところデータ転送の直前にページ・列アドレス設定コマンドが発行されているので、
		 * とりあえず問題ありません。（※いずれ他のコマンドもちゃんと実装する予定です）
		 */
		switch(data & 0xf0) {
		case 0x00: /* 列アドレス(下位桁)設定 */
			context->lcdc.col2 = (context->lcdc.col2 & 0xe0) | (data & 0x0f) << 1;
			break;
		case 0x10: /* 列アドレス(上位桁)設定 */
			context->lcdc.col2 = (context->lcdc.col2 & 0x1e) | (data & 0x07) << 5;
			break;
		case 0xb0: /* ページアドレス設定 */
			context->lcdc.page = data & 0x0f;
			break;
		}
	} else { /* データ */
		context->lcdc.vram[context->lcdc.page][context->lcdc.col2] = data;
		context->lcdc.col2 = (context->lcdc.col2 + 1) & 0xff;
	}
}

void
lcdc_conv(PIEMU_CONTEXT* context, unsigned char bits[DISP_Y][DISP_X])
{
	int page, col, x, y, i;
	unsigned char c, c1, c2;

	for(page = 0; page < 16; page++) {
		x = (DISP_X - 1) - ((page - 10) & 0x0f) * 8;
		for(col = 0; col < DISP_Y; col++) {
			y = col;
			c1 = context->lcdc.vram[page][col * 2 + 0];
			c2 = context->lcdc.vram[page][col * 2 + 1];
			for(i = 0; i < 8; i++) {
				c = (c1 & 1) << 1 | (c2 & 1);
				bits[y][x - i] = c;
				c1 >>= 1;
				c2 >>= 1;
			}
		}
	}
}

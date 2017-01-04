#include <stdio.h>
#include <string.h>
#include <math.h>
#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include <FreeSans_bin.h>
#include "res.h"
#include "menu.h"

int textSize;
sf2d_texture* topBG, *bottomBG;
sftd_font* font;
const char* someText = "This is text!";

void initMenu()
{
	sf2d_init();
	sftd_init();
	sf2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
	sf2d_set_vblank_wait(0);
	textSize = 12;

	font = sftd_load_font_mem(FreeSans_bin, FreeSans_bin_len);
	bottomBG = sf2d_create_texture_mem_RGBA8(BGBottom.pixel_data, BGBottom.width, BGBottom.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	topBG = sf2d_create_texture_mem_RGBA8(BGTop.pixel_data, BGTop.width, BGTop.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
}

void exitMenu()
{
	sftd_free_font(font);
	sftd_fini();
	sf2d_free_texture(topBG);
	sf2d_free_texture(bottomBG);
	sf2d_fini();
}

void doMenuLoop()
{
	while (aptMainLoop()) {

		hidScanInput();

		if (hidKeysDown() & KEY_START)
			break;

		//draw top
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sf2d_draw_texture(topBG, 0, 0);
		sftd_draw_text_wrap(font, 10, 40, RGBA8(255, 255, 255, 255), textSize, 300, someText);
		sf2d_end_frame();

		//draw bottom
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_texture(bottomBG, 0, 0);
		sf2d_end_frame();

		sf2d_swapbuffers();
	}
}
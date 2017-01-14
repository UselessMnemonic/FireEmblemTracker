#pragma once
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sf2d.h>
#include <sftd.h>

#include <FreeSans_bin.h>
#include "bgBttm.h"
#include "bgTop.h"

sftd_font *FEfont;
sf2d_texture *bottomBG, *topBG;
int textSize;

void doMenuLoop()
{
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
	sf2d_draw_texture(topBG, 0, 0);
	sftd_draw_text(FEfont, 5, 5, RGBA8(255, 255, 255, 255), textSize, "Uhm?");
	sf2d_end_frame();

	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
	sf2d_draw_texture(bottomBG, 0, 0);
	sf2d_end_frame();

	sf2d_swapbuffers();
}

void initMenu()
{
	sf2d_init();
	sf2d_set_3D(false);
	sf2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	sf2d_set_vblank_wait(0);
	sftd_init();

	textSize = 12;
	FEfont = sftd_load_font_mem(FreeSans_bin, FreeSans_bin_len);
	bottomBG = sf2d_create_texture_mem_RGBA8(BGBottom.pixel_data, BGBottom.width, BGBottom.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	topBG = sf2d_create_texture_mem_RGBA8(BGTop.pixel_data, BGTop.width, BGTop.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
}

void exitMenu()
{
	sftd_free_font(FEfont);
	sf2d_free_texture(topBG);
	sf2d_free_texture(bottomBG);

	sftd_fini();
	sf2d_fini();
}
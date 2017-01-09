#include <stdio.h>
#include <string.h>
#include <math.h>
#include <3ds.h>
#include <FreeSans_bin.h>
#include "res.h"
#include "menu.h"

Menu::Menu()
{
	sf2d_init();
	sftd_init();
	sf2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	sf2d_set_vblank_wait(0);
	textSize = 12;
	FEfont = sftd_load_font_mem(FreeSans_bin, FreeSans_bin_len);
	bottomBG = sf2d_create_texture_mem_RGBA8(BGBottom.pixel_data, BGBottom.width, BGBottom.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	topBG = sf2d_create_texture_mem_RGBA8(BGTop.pixel_data, BGTop.width, BGTop.height, TEXFMT_RGBA8, SF2D_PLACE_RAM);
	BGMPlayer = new WavPlayer();
}

Menu::~Menu()
{
	sftd_free_font(FEfont);
	sftd_fini();
	sf2d_free_texture(topBG);
	sf2d_free_texture(bottomBG);
	sf2d_fini();

}

void Menu::doMenuLoop()
{
	BGMPlayer->doBGMLoop();
}

void Menu::addMenuItem(const char* content)
{

}

void Menu::drawActiveMenu()
{

}

void Menu::setBGM(const char* filename, long loopingSample = 0)
{
	if(loopingSample)
		BGMPlayer->loadBGM(filename, loopingSample);
	else
		BGMPlayer->loadBGM(filename);
}
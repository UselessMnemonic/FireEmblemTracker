/*#include "menu.h"
#include "WavPlayer.h"
#include "ResourceManager.h"


int main()
{
	initMenu();
	initWavPlayer(2, (16*1024));
		loadBGM("bgm.wav", 120000);
		startBGM();

	while (aptMainLoop())
	{
		hidScanInput();

		if (hidKeysHeld() & KEY_START) break;

		doMenuLoop();
		doBGMLoop();
	}

	stopBGM();

	exitWavPlayer();
	exitMenu();

	return 0;
}*/

#include <3ds.h>
#include <stdio.h>

#include <sf2d.h>
#include <sftd.h>
#include <FEFont_bin.h>

int main(int argc, char* argv[])
{
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
	sftd_init();
	sftd_font *FEfont = sftd_load_font_mem(FEFont_bin, FEFont_bin_len);

	romfsInit();
	FILE* file = fopen("romfs:/dummy.txt", "r");
	char content[6] = "abcde";
	fread(content, 1, 5, file);

	while (aptMainLoop())
	{
		hidScanInput();

		if (hidKeysHeld() & KEY_START) break;

		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		sftd_draw_text(FEfont, 0, 0, RGBA8(0xFF, 0xFF, 0xFF, 0xFF), 12, content);
		sf2d_end_frame();

		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		sf2d_draw_line(0, 0, 50, 50, 1, RGBA8(0xFF, 0xFF, 0xFF, 0xFF));
		sf2d_end_frame();

		sf2d_swapbuffers();
	}

	sftd_fini();
	sf2d_fini();
	romfsExit();

}

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <3ds.h>
#include <sf2d.h>
#include <sftd.h>
#include "FreeSans_otf.h"

int main()
{
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));
	sf2d_set_vblank_wait(0);

	// Font loading
	sftd_init();
	sftd_font *font = sftd_load_font_mem(FreeSans_otf, FreeSans_otf_size);

	const char *someText = "Font drawing on the top screen! Text wraps after 300 pixels... Lorem ipsum dolor sit amet, consetetur sadipscing elit.";
	int textWidth = 0;
	int textHeight = 0;

	while (aptMainLoop()) {

		hidScanInput();
		if (hidKeysDown() & KEY_START) break;

		sf2d_start_frame(GFX_TOP, GFX_LEFT);

		sftd_draw_textf(font, 10, 10, RGBA8(0, 255, 0, 255), 20, "FPS %f", sf2d_get_fps());
		sftd_calc_bounding_box(&textWidth, &textHeight, font, 20, 300, someText);
		sf2d_draw_rectangle(10, 40, textWidth, textHeight, RGBA8(0, 100, 0, 255));
		sftd_draw_text_wrap(font, 10, 40, RGBA8(255, 255, 255, 255), 20, 300, someText);

		sf2d_end_frame();

		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);

		sftd_draw_text(font, 10, 10, RGBA8(0, 255, 0, 255), 20, "Font drawing on the bot. screen!");
		sftd_draw_text(font, 10, 30, RGBA8(255, 0, 0, 255), 10, "Font drawing on the bot. screen!");
		sftd_draw_text(font, 10, 50, RGBA8(0, 255, 0, 255), 15, "Font drawing on the bot. screen!");
		sftd_draw_text(font, 10, 70, RGBA8(0, 0, 255, 255), 5, "Font drawing on the bot. screen!");
		sftd_draw_text(font, 10, 90, RGBA8(255, 255, 0, 255), 25, "Font drawing on the bot. screen!");
		sftd_draw_text(font, 10, 120, RGBA8(255, 0, 255, 255), 20, "Font drawing on the bot. screen!");
		sftd_draw_text(font, 10, 140, RGBA8(0, 255, 255, 255), 12, "Font drawing on the bot. screen!");
		sftd_draw_text(font, 10, 150, RGBA8(255, 0, 0, 255), 28, "Font drawing on the bot. screen!");

		sf2d_end_frame();

		sf2d_swapbuffers();
	}

	sftd_free_font(font);
	sftd_fini();

	sf2d_fini();
	return 0;
}
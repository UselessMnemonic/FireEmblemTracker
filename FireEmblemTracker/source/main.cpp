#include "menu.h"
#include "WavPlayer.h"


int main()
{
	sdmcInit();
	romfsInit();
	initMenu();
	initWavPlayer(2, (16*1024));
		loadBGM("romfs:/bgm.wav", 120000);
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
	sdmcExit();
	romfsExit();

	return 0;
}

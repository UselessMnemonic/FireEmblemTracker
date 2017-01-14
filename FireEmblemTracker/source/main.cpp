#include "menu.h"
#include "WavPlayer.h"

int main()
{
	initMenu();
	initWavPlayer(2, (16*1024));
		loadBGM("bgm.wav", 120000/*120171*/);
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
}

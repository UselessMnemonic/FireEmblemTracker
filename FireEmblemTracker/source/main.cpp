#include "menu.h"
#include "WavPlayer.h"

int main()
{
	Menu menu;
	WavPlayer BGMPlayer(2, (16*1024));
	BGMPlayer.loadBGM("bgm.wav", 120171);
	BGMPlayer.playBGM();

	while (aptMainLoop())
	{
		hidScanInput();

		if (hidKeysHeld() & KEY_START) break;

		menu.doMenuLoop();
		BGMPlayer.doBGMLoop();
	}

	BGMPlayer.stopBGM();

	return 0;
}

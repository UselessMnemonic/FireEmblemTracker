#include "menu.h"
#include "WavPlayer.h"

int main()
{
	Menu menu;
	WavPlayer BGMPlayer;
	BGMPlayer.loadBGM("bgm.wav", 120171);
	BGMPlayer.playBGM();

	menu.addWavPlayer(&BGMPlayer);

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

#include "menu.h"
#include <3ds.h>

int main()
{
	Menu mainMenu;
	mainMenu.setBGM("bgm.wav", 120171);

	while (aptMainLoop())
	{
		hidScanInput();
		if (hidKeysDown() & KEY_START)
			break;
		mainMenu.doMenuLoop();
	}
	return 0;
}

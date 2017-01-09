#pragma once
#include <sf2d.h>
#include <sftd.h>
#include "WavPlayer.h"

class Menu
{
public:
	Menu();
	~Menu();
	void doMenuLoop();
	void addMenuItem(const char* content);
	void setBGM(const char* filename, long loopingSample);
private:
	void drawActiveMenu();
	int textSize;
	sf2d_texture* topBG, *bottomBG;
	sftd_font* FEfont;
	WavPlayer* BGMPlayer;
};

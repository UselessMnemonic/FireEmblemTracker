#pragma once
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sf2d.h>
#include <sftd.h>

#include <FreeSans_bin.h>
#include "res.h"

#include "WavPlayer.h"

class Menu
{
public:
	Menu();
	~Menu();
	void doMenuLoop();
	void addWavPlayer(WavPlayer*);

private:
	int textSize;
	sf2d_texture* topBG, *bottomBG;
	sftd_font* FEfont;
	WavPlayer* BGMForDebug;
};

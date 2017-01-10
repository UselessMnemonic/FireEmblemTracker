#pragma once
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WavFile.h"

class WavPlayer
{
public:
	WavPlayer();
	~WavPlayer();

	void loadBGM(const char* filename, long loopSample = 0);
	void playBGM();

	//void loadSoundEffect(const char* filename);

	//void playSoundEffect();

	//void unloadSoundEffect();

	void doBGMLoop();
	void stopBGM();

	WavFile* getWavFile();

private:
	WavFile* currentBGM;
	bool BGMIsPlaying;
	ndspWaveBuf wavBuffer[2];
	s16 *buffer1 = NULL, *buffer2 = NULL;
	unsigned long loopSample;
};
#pragma once
#include "WavFile.h"


class WavPlayer
{
public:
	WavPlayer();
	~WavPlayer();

	void loadBGM(const char* filename, long loopSample = 0);
	void playBGM();

	void loadSoundEffect();

	void playSoundEffect();

	void unloadSoundEffect();

	void doBGMLoop();
	void stopBGM();
private:
	WavFile* currentBGM;
	bool BGMIsPlaying;
	ndspWaveBuf wavBuffer[2];
	s16 *buffer1 = NULL, *buffer2 = NULL;
	long loopSample;
};
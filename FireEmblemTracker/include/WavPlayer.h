#pragma once
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WavFile.h"

	//void loadSoundEffect(const char* filename);

	//void playSoundEffect();

	//void unloadSoundEffect();

#define BGM_CHANNEL 0x08
#define SFX1_CHANNEL 0x09
#define SFX2_CHANNEL 0x0A
#define BUFFER_SIZE bufferSize

WavFile currentBGM;
bool BGMIsPlaying;
ndspWaveBuf* wavBuffer;
s16** buffers;
long bufferSize;
int numOfBuffers;

void initWavPlayer(int numBuffers, long bytesPerBuffer)
{
	BGMIsPlaying = false;
	ndspInit();
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);

	numOfBuffers = numBuffers;
	bufferSize = bytesPerBuffer;
	wavBuffer = new ndspWaveBuf[numBuffers];
	buffers = new s16*[numBuffers];

	memset(&currentBGM, 0, sizeof(currentBGM));
	currentBGM.file = NULL;
}

void exitWavPlayer()
{
	ndspExit();
	delete[] wavBuffer;
	delete[] buffers;
}

void loadBGM(const char* filename, long loopingSample = 0)
{
	if (!BGMIsPlaying)
	{
		closeWavFile(&currentBGM);
		openWavFile(filename, &currentBGM, loopingSample);
	}
}

void startBGM()
{
	if (!BGMIsPlaying) //if the BGM isn't already playing...
	{
		unsigned long nsamples = (BUFFER_SIZE / currentBGM.blockAlign);

		ndspChnReset(BGM_CHANNEL);
		ndspChnWaveBufClear(BGM_CHANNEL);
		ndspChnSetInterp(BGM_CHANNEL, NDSP_INTERP_POLYPHASE);
		ndspChnSetRate(BGM_CHANNEL, currentBGM.sampleRate);
		ndspChnSetFormat(BGM_CHANNEL, NDSP_FORMAT_STEREO_PCM16);

		u32 samplesRead;

		for (int i = 0; i < numOfBuffers; i++)
		{
			wavBuffer[i].adpcm_data = 0;
			wavBuffer[i].data_adpcm = 0;
			wavBuffer[i].data_pcm16 = 0;
			wavBuffer[i].data_pcm8 = 0;
			wavBuffer[i].data_vaddr = 0;
			wavBuffer[i].looping = 0;
			wavBuffer[i].next = 0;
			wavBuffer[i].nsamples = 0;
			wavBuffer[i].offset = 0;
			wavBuffer[i].sequence_id = 0;
			wavBuffer[i].status = 0;

			buffers[i] = (s16*)linearAlloc(BUFFER_SIZE);
			samplesRead = readRawAudioStream(&currentBGM, buffers[i], nsamples);
			wavBuffer[i].nsamples = samplesRead;
			wavBuffer[i].data_vaddr = &buffers[i][0];
			ndspChnWaveBufAdd(BGM_CHANNEL, &wavBuffer[i]);
		}

		while (ndspChnIsPlaying(BGM_CHANNEL) == false);

		BGMIsPlaying = true;
	}
}

void stopBGM()
{
	if (BGMIsPlaying)
	{
		ndspChnWaveBufClear(BGM_CHANNEL);
		for (int i = 0; i < numOfBuffers; i++)
			linearFree(buffers[i]);

		currentBGM.sampleSeek = 0;
	}

	BGMIsPlaying = false;
}

void doBGMLoop()
{
	unsigned long nsamples = BUFFER_SIZE / currentBGM.blockAlign;
	unsigned long samplesRead = 0;

	if (BGMIsPlaying)
	{
		for (int i = 0; i < numOfBuffers; i++)
		{
			if (wavBuffer[i].status == NDSP_WBUF_DONE)
			{

				samplesRead = readRawAudioStream(&currentBGM, buffers[i], nsamples);

				wavBuffer[i].nsamples = samplesRead;

				ndspChnWaveBufAdd(BGM_CHANNEL, &wavBuffer[i]);
			}
		}

		for (int i = 0; i < numOfBuffers; i++)
		{
			DSP_FlushDataCache(buffers[i], BUFFER_SIZE);
		}
	}
}

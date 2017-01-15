#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ResourceManager.h"

struct WavFile
{
	FILE* file;
	u16 audioFormat;
	u16 numChannels;
	u32 sampleRate;
	u32 byteRate;
	u16 blockAlign;
	u16 bitsPerSample;
	u32 audioStreamSize;
	long numSamples;
	long sampleSeek;
	long loopingSample;
};

#define AUDIO_FMT_OFFSET 20 //little, 2
#define NUM_CHANNEL_OFFSET 22 //little, 2
#define SAMP_RATE_OFFSET 24 //little, 4
#define BYTE_RATE_OFFSET 28 //little, 4
#define BLK_ALIGN_OFFSET 32 //little, 2
#define BPS_OFFSET 34 //little, 2
#define AUD_STRM_SIZE_OFFSET 40 //little, 4
#define AUD_STRM_OFFSET 44 //little

u8 getu8AtX(long x, FILE* file)
{
	u8 result = 0;
	if (file != NULL)
	{
		fseek(file, x, SEEK_SET);

		if (fread(&result, 1, 1, file))
			return result;
		else
			return 0;
	}
	return result;
}

void openWavFile(const char* filename, WavFile* wf, long loopStartSample)
{
	wf->file = rm_open(filename, "rb");

	if (wf->file != NULL)
	{
		wf->audioFormat = (getu8AtX(AUDIO_FMT_OFFSET + 1, wf->file) << 8) + (getu8AtX(AUDIO_FMT_OFFSET, wf->file));
		wf->numChannels = (getu8AtX(NUM_CHANNEL_OFFSET + 1, wf->file) << 8) + (getu8AtX(NUM_CHANNEL_OFFSET, wf->file));
		wf->sampleRate = (getu8AtX(SAMP_RATE_OFFSET + 3, wf->file) << 24) + (getu8AtX(SAMP_RATE_OFFSET + 2, wf->file) << 16) + (getu8AtX(SAMP_RATE_OFFSET + 1, wf->file) << 8) + (getu8AtX(SAMP_RATE_OFFSET, wf->file));
		wf->byteRate = (getu8AtX(BYTE_RATE_OFFSET + 3, wf->file) << 24) + (getu8AtX(BYTE_RATE_OFFSET + 2, wf->file) << 16) + (getu8AtX(BYTE_RATE_OFFSET + 1, wf->file) << 8) + (getu8AtX(BYTE_RATE_OFFSET, wf->file));
		wf->blockAlign = (getu8AtX(BLK_ALIGN_OFFSET + 1, wf->file) << 8) + (getu8AtX(BLK_ALIGN_OFFSET, wf->file));
		wf->bitsPerSample = (getu8AtX(BPS_OFFSET + 1, wf->file) << 8) + (getu8AtX(BPS_OFFSET, wf->file));
		wf->audioStreamSize = (getu8AtX(AUD_STRM_SIZE_OFFSET + 3, wf->file) << 24) + (getu8AtX(AUD_STRM_SIZE_OFFSET + 2, wf->file) << 16) + (getu8AtX(AUD_STRM_SIZE_OFFSET + 1, wf->file) << 8) + (getu8AtX(AUD_STRM_SIZE_OFFSET, wf->file));
	}
	else
	{
		wf->audioFormat = 0;
		wf->numChannels = 0;
		wf->sampleRate = 0;
		wf->byteRate = 0;
		wf->blockAlign = 0;
		wf->bitsPerSample = 0;
		wf->audioStreamSize = 0;
	}

	fseek(wf->file, 0, SEEK_END);
	long fileSize = ftell(wf->file);
	fseek(wf->file, 0, SEEK_SET);

	fileSize -= 44;

	wf->numSamples = fileSize / wf->blockAlign;

	wf->loopingSample = loopStartSample;
}

void closeWavFile(WavFile* wf)
{
	if (wf->file != NULL)
		fclose(wf->file);
}

long readRawAudioStream(WavFile* wf, void* buffer, long numSamplesToRead)//returns #samples read
{
	long startpos = AUD_STRM_OFFSET + (wf->blockAlign * wf->sampleSeek); //find the start position in bytes


	fseek(wf->file, startpos, SEEK_SET); //seek to that position
	long readResult = fread(buffer, 1, (wf->blockAlign*numSamplesToRead), wf->file); //read how ever many bytes to read
	readResult /= wf->blockAlign; //get the result of bytes read as samples read

	wf->sampleSeek += readResult; //set the next seek position to the next sample offset

	if (numSamplesToRead > readResult) //if we read more than available, we've reached EOF; next read starts at loop sample
		wf->sampleSeek = wf->loopingSample;

	return readResult; //return the number of samples read
}

bool isFileValid(WavFile* wf)
{
	return (wf->file != NULL);
}
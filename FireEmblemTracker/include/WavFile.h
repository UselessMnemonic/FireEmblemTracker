#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class WavFile {

public:
	WavFile(const char* filename);
	~WavFile();
	u16 getAudioFormat();
	u16 getNumChannels();
	u32 getSampleRate();
	u32 getByteRate();
	u16 getBlockAlign();
	u16 getBitsPerSample();
	u32 getAudioStreamSize();
	u8 getu8AtX(long position);
	bool isFileValid();
	size_t readRawFileStream(void* buffer, size_t _size, size_t _n, long seek = -1); //functions exactly as fread, seek is independent of other fread calls, last long is for seek purposes
	long readRawAudioStream(void* buffer, long numSamplesToRead, long startPositionInSamples = -1);
	long getNumSamples();
	long getSampleSeek();

private:
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
};
#include "WavFile.h"

#define AUDIO_FMT_OFFSET 20 //little, 2
#define NUM_CHANNEL_OFFSET 22 //little, 2
#define SAMP_RATE_OFFSET 24 //little, 4
#define BYTE_RATE_OFFSET 28 //little, 4
#define BLK_ALIGN_OFFSET 32 //little, 2
#define BPS_OFFSET 34 //little, 2
#define AUD_STRM_SIZE_OFFSET 40 //little, 4
#define AUD_STRM_OFFSET 44 //little

u8 WavFile::getu8AtX(long x)
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

WavFile::WavFile(const char* filename)
{
	file = fopen(filename, "rb");

	if (file != NULL)
	{
		audioFormat = (getu8AtX(AUDIO_FMT_OFFSET + 1) << 8) + (getu8AtX(AUDIO_FMT_OFFSET));
		numChannels = (getu8AtX(NUM_CHANNEL_OFFSET + 1) << 8) + (getu8AtX(NUM_CHANNEL_OFFSET));
		sampleRate = (getu8AtX(SAMP_RATE_OFFSET + 3) << 24) + (getu8AtX(SAMP_RATE_OFFSET + 2) << 16) + (getu8AtX(SAMP_RATE_OFFSET + 1) << 8) + (getu8AtX(SAMP_RATE_OFFSET));
		byteRate = (getu8AtX(BYTE_RATE_OFFSET + 3) << 24) + (getu8AtX(BYTE_RATE_OFFSET + 2) << 16) + (getu8AtX(BYTE_RATE_OFFSET + 1) << 8) + (getu8AtX(BYTE_RATE_OFFSET));
		blockAlign = (getu8AtX(BLK_ALIGN_OFFSET + 1) << 8) + (getu8AtX(BLK_ALIGN_OFFSET));
		bitsPerSample = (getu8AtX(BPS_OFFSET + 1) << 8) + (getu8AtX(BPS_OFFSET));
		audioStreamSize = (getu8AtX(AUD_STRM_SIZE_OFFSET + 3) << 24) + (getu8AtX(AUD_STRM_SIZE_OFFSET + 2) << 16) + (getu8AtX(AUD_STRM_SIZE_OFFSET + 1) << 8) + (getu8AtX(AUD_STRM_SIZE_OFFSET));
	}
	else
	{
		audioFormat = 0;
		numChannels = 0;
		sampleRate = 0;
		byteRate = 0;
		blockAlign = 0;
		bitsPerSample = 0;
		audioStreamSize = 0;
	}

	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	fileSize -= 44;

	numSamples = fileSize / blockAlign;
	
}

WavFile::~WavFile()
{
	if(file != NULL)
		fclose(file);
}

u16 WavFile::getAudioFormat()
{
	return audioFormat;
}

u16 WavFile::getNumChannels()
{
	return numChannels;
}

u32 WavFile::getSampleRate()
{
	return sampleRate;
}

u32 WavFile::getByteRate()
{
	return byteRate;
}

u16 WavFile::getBlockAlign()
{
	return blockAlign;
}

u16 WavFile::getBitsPerSample() //16 bps = 2 Bps, but I bet you already knew that.
{
	return bitsPerSample;
}

u32 WavFile::getAudioStreamSize()
{
	return audioStreamSize;
}

size_t WavFile::readRawFileStream(void* buffer, size_t _size, size_t _n, long setSeek)
{
	static long seaking = 0;

	if (setSeek != -1)
		seaking = setSeek;

	fseek(file, seaking, SEEK_SET);
	size_t res = fread(buffer, _size, _n, file);
	seaking = ftell(file);

	return res;
}

long WavFile::readRawAudioStream(void* buffer, long numSamplesToRead, long startPositionInSamples)//returns #samples read
{
	if (sampleSeek == numSamples - 1)
		return 0;

	if (startPositionInSamples != -1) //if a start position was specified...
		sampleSeek = startPositionInSamples; //use that start position

		long startpos = AUD_STRM_OFFSET + (blockAlign * sampleSeek); //find the start position in bytes


		fseek(file, startpos, SEEK_SET); //seek to that position
		long readResult = fread(buffer, 1, (blockAlign*numSamplesToRead), file); //read how ever many bytes to read
		readResult /= blockAlign; //get the result of bytes read as samples read

		sampleSeek += readResult; //set the next seek position to the next sample offset

		return readResult; //return the number of samples read
}

bool WavFile::isFileValid()
{
	return (file != NULL);
}

long WavFile::getNumSamples()
{
	return numSamples;
}

long WavFile::getSampleSeek()
{
	return sampleSeek;
}
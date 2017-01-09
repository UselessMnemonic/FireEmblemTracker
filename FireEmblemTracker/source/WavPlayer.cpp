#include <WavPlayer.h>
#include <3ds.h>

#define BUFFER_SIZE (16 * 1024)
#define BGM_CHANNEL 0x08

WavPlayer::WavPlayer()
{
	currentBGM = NULL;
	BGMIsPlaying = false;
	loopSample = 0;
	ndspInit();
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);
}

WavPlayer::~WavPlayer()
{
	ndspExit();
}

void WavPlayer::loadBGM(const char* filename, long loopingSample)
{
	if (!BGMIsPlaying)
	{
		if (currentBGM == NULL)
		{
			currentBGM = new WavFile(filename);

			if (currentBGM->isFileValid())
				return;
			delete currentBGM;
			currentBGM = NULL;
		}
		else
		{
			delete currentBGM;
			currentBGM = NULL;
			loadBGM(filename, loopSample);
		}
		loopSample = loopingSample;
	}
}

void WavPlayer::playBGM()
{
	if (!BGMIsPlaying) //if the BGM isn't already playing...
	{
		u32 nsamples = (BUFFER_SIZE / 4);

		ndspChnReset(BGM_CHANNEL);
		ndspChnWaveBufClear(BGM_CHANNEL);
		ndspChnSetInterp(BGM_CHANNEL, NDSP_INTERP_POLYPHASE);
		ndspChnSetRate(BGM_CHANNEL, currentBGM->getSampleRate());
		ndspChnSetFormat(BGM_CHANNEL, NDSP_FORMAT_STEREO_PCM16);
		memset(wavBuffer, 0, sizeof(wavBuffer));

		buffer1 = (s16*)linearAlloc(BUFFER_SIZE);
		buffer2 = (s16*)linearAlloc(BUFFER_SIZE);

		currentBGM->readRawAudioStream(buffer1, nsamples);
		wavBuffer[0].nsamples = nsamples;
		wavBuffer[0].data_vaddr = &buffer1[0];
		ndspChnWaveBufAdd(BGM_CHANNEL, &wavBuffer[0]);

		currentBGM->readRawAudioStream(buffer2, nsamples);
		wavBuffer[1].nsamples = nsamples;
		wavBuffer[1].data_vaddr = &buffer2[0];
		ndspChnWaveBufAdd(BGM_CHANNEL, &wavBuffer[1]);
		while (ndspChnIsPlaying(BGM_CHANNEL) == false);
	}
}

void WavPlayer::stopBGM()
{
	if (BGMIsPlaying)
	{
		ndspChnWaveBufClear(BGM_CHANNEL);
		linearFree(buffer1);
		linearFree(buffer2);
	}

	BGMIsPlaying = false;
}

void WavPlayer::doBGMLoop()
{
	u32 nsamples = BUFFER_SIZE / 4;

	static bool lastbuf = false;
	static size_t samplesRead = 0;
	
	if (BGMIsPlaying)
	{
		if (BGMIsPlaying == false)
			return;

		if (wavBuffer[0].status == NDSP_WBUF_DONE)
		{
			samplesRead = currentBGM->readRawAudioStream(buffer1, nsamples);

			if (samplesRead == 0)
			{
				samplesRead = currentBGM->readRawAudioStream(buffer1, nsamples, loopSample);
			}
			
			wavBuffer[0].nsamples = samplesRead;

			ndspChnWaveBufAdd(BGM_CHANNEL, &wavBuffer[0]);
		}

		if (wavBuffer[1].status == NDSP_WBUF_DONE)
		{
			samplesRead = currentBGM->readRawAudioStream(buffer2, nsamples);

			if (samplesRead == 0)
			{
				samplesRead = currentBGM->readRawAudioStream(buffer2, nsamples, loopSample);
			}

			wavBuffer[1].nsamples = samplesRead;

			ndspChnWaveBufAdd(BGM_CHANNEL, &wavBuffer[1]);
		}

		DSP_FlushDataCache(buffer1, BUFFER_SIZE);
		DSP_FlushDataCache(buffer2, BUFFER_SIZE);
	}
}
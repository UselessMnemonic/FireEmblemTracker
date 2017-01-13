#include <WavPlayer.h>

#define BGM_CHANNEL 0x08
#define SFX1_CHANNEL 0x09
#define SFX2_CHANNEL 0x0A
#define BUFFER_SIZE (const long)bufferSize

WavPlayer::WavPlayer(int numBuffers, long bytesPerBuffer)
{
	currentBGM = NULL;
	BGMIsPlaying = false;
	loopSample = 0;
	ndspInit();
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);
	
	numOfBuffers = numBuffers;
	bufferSize = bytesPerBuffer;
	wavBuffer = new ndspWaveBuf[numBuffers];
	buffers = new s16*[numBuffers];
}

WavPlayer::~WavPlayer()
{
	ndspExit();
	delete[] wavBuffer;
	delete[] buffers;
}

void WavPlayer::loadBGM(const char* filename, long loopingSample)
{
	if (!BGMIsPlaying)
	{
		if (currentBGM == NULL)
		{
			currentBGM = new WavFile(filename);
			if (currentBGM->isFileValid())
			{
				loopSample = loopingSample;
				return;
			}
			delete currentBGM;
			currentBGM = NULL;
			loopSample = 0;
		}
		else
		{
			delete currentBGM;
			currentBGM = NULL;
			loadBGM(filename, loopSample);
		}
	}
}

void WavPlayer::playBGM()
{
	if (!BGMIsPlaying) //if the BGM isn't already playing...
	{
		unsigned long nsamples = (BUFFER_SIZE / currentBGM->getBlockAlign());

		ndspChnReset(BGM_CHANNEL);
		ndspChnWaveBufClear(BGM_CHANNEL);
		ndspChnSetInterp(BGM_CHANNEL, NDSP_INTERP_POLYPHASE);
		ndspChnSetRate(BGM_CHANNEL, currentBGM->getSampleRate());
		ndspChnSetFormat(BGM_CHANNEL, NDSP_FORMAT_STEREO_PCM16);

		memset(wavBuffer, 0, sizeof(wavBuffer)*numOfBuffers);

		u32 samplesRead;

		for (int i = 0; i < numOfBuffers; i++)
		{
			/*wavBuffer[i].adpcm_data = 0;
			wavBuffer[i].data_adpcm = 0;
			wavBuffer[i].data_pcm16 = 0;
			wavBuffer[i].data_pcm8 = 0;
			wavBuffer[i].data_vaddr = 0;
			wavBuffer[i].looping = 0;
			wavBuffer[i].next = 0;
			wavBuffer[i].nsamples = 0;
			wavBuffer[i].offset = 0;
			wavBuffer[i].sequence_id = 0;
			wavBuffer[i].status = 0;*/

			buffers[i] = (s16*)linearAlloc(BUFFER_SIZE);
			samplesRead = currentBGM->readRawAudioStream(buffers[i], nsamples);
			wavBuffer[i].nsamples = samplesRead;
			wavBuffer[i].data_vaddr = &buffers[i];
			ndspChnWaveBufAdd(BGM_CHANNEL, &wavBuffer[i]);
		}

		while (ndspChnIsPlaying(BGM_CHANNEL) == false);

		BGMIsPlaying = true;
	}
}

void WavPlayer::stopBGM()
{
	if (BGMIsPlaying)
	{
		ndspChnWaveBufClear(BGM_CHANNEL);
		for(int i = 0; i < numOfBuffers; i++)
			linearFree(buffers[i]);
	}

	BGMIsPlaying = false;
}

void WavPlayer::doBGMLoop()
{
	unsigned long nsamples = BUFFER_SIZE / currentBGM->getBlockAlign();
	unsigned long samplesRead = 0;
	
	if (BGMIsPlaying)
	{
		for (int i = 0; i < numOfBuffers; i++)
		{
			if (wavBuffer[i].status == NDSP_WBUF_DONE)
			{

				samplesRead = currentBGM->readRawAudioStream(buffers[i], nsamples);

				if (samplesRead == 0)
				{
					samplesRead = currentBGM->readRawAudioStream(buffers[i], nsamples, loopSample);
				}

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

WavFile* WavPlayer::getWavFile()
{
	return currentBGM;
}
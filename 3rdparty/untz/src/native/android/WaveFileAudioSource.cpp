#include "WaveFileAudioSource.h"
#include "SystemData.h"
#include <math.h>
#if defined(__ANDROID__)
	#include <android/log.h>
#endif

WaveFileAudioSource::WaveFileAudioSource()
{
}

WaveFileAudioSource::~WaveFileAudioSource()
{
	mWaveFile.close();
}

double WaveFileAudioSource::getSampleRate()
{
    return (double)mWaveFile.getHeader().samplesPerSecond;
}

double WaveFileAudioSource::getLength()
{
    // seconds
    return (double)mWaveFile.getNumberOfFrames() / getSampleRate();
}

UInt32 WaveFileAudioSource::getNumChannels()
{
    return mWaveFile.getHeader().numChannels;
}

bool WaveFileAudioSource::init(const RString& path, bool loadIntoMemory)
{
	mPath = path;

	if(mWaveFile.open(path.c_str()) != 0)
		return false;

	// Check that we support the sample format
	if(mWaveFile.getHeader().formatTag != R_WAVEFORMAT_PCM &&
		mWaveFile.getHeader().formatTag != R_WAVEFORMAT_IEEE)
	{
		printf("WAVE: unsupported format (%d)\n", mWaveFile.getHeader().formatTag);
		return false; // error
	}

	// Check that we support the sample rate
	if(mWaveFile.getHeader().samplesPerSecond != UNTZ::System::get()->getSampleRate())
	{
		printf("WAVE: unsupported sample rate(%d)\n", mWaveFile.getHeader().samplesPerSecond);
		return false; // error
	}

	mRawBuffer.resize(mWaveFile.chunkSize());

    return BufferedAudioSource::init(path, loadIntoMemory);
}

void WaveFileAudioSource::close()
{
	mWaveFile.close();
}

Int64 WaveFileAudioSource::decodeData(float* buffer, UInt32 numFrames)
{
#if defined(__ANDROID__)
    char str[512];
#endif

	UInt32 readBytes = 0;
	UInt32 readFrames = 0;
	
	// IEEE sample format
	if(mWaveFile.getHeader().formatTag == R_WAVEFORMAT_IEEE)
	{
		readBytes = mWaveFile.readData((void*)buffer, numFrames * getNumChannels() * sizeof(float));
	}
	// PCM sample format
	else if(mWaveFile.getHeader().formatTag == R_WAVEFORMAT_PCM)
	{
		UInt32 totalBytes = numFrames * mWaveFile.getHeader().bytesPerFrame;
		if(totalBytes > mRawBuffer.capacity())
			mRawBuffer.resize(totalBytes);

		readBytes = mWaveFile.readData((void*)&mRawBuffer[0], totalBytes);
		readFrames = readBytes / mWaveFile.getHeader().bytesPerFrame;

		// 8-bit PCM
		if(mWaveFile.getHeader().bitsPerSample == 8)
		{
			UInt32 totalSamples = readFrames * getNumChannels();
			Int8* byteSamples = (Int8*)&mRawBuffer[0];
			float *pTemp = buffer;
			for(int i = 0; i < totalSamples; ++i)
			{
				*pTemp++ = *(byteSamples++) / 127.0f;
			}
		}
		// 16-bit PCM
		if(mWaveFile.getHeader().bitsPerSample == 16)
		{
			UInt32 totalSamples = readFrames * getNumChannels();
			Int16* shortSamples = (Int16*)&mRawBuffer[0];
			float *pTemp = buffer;
			for(int i = 0; i < totalSamples; ++i)
			{
				*pTemp++ = *(shortSamples++) / 32767.0f;
			}
		}
		// 24-bit PCM
		else if(mWaveFile.getHeader().bitsPerSample == 24)
		{
			int byteCount = 0;
			Int8 byte = 0;
			Int32 sample = 0;
			float scale = pow(2.0f, 23.0f) - 1;
			for(int i = 0; i < readFrames; ++i)
			{
				sample = mRawBuffer[byteCount] << 16 | mRawBuffer[byteCount+1] << 8 | mRawBuffer[byteCount+2];
				*buffer++ = sample / scale;
				byteCount += 3;
			}
		}
	}

	readFrames = readBytes / mWaveFile.getHeader().bytesPerFrame;

#if defined(__ANDROID__)
	//sprintf(str, "read %ld frames", readFrames);
    //__android_log_write(ANDROID_LOG_ERROR, "UntzJNI", str);
#else
	printf("read %d frames\n", readFrames);
#endif

	if(readFrames == 0)
	{
#if defined(__ANDROID__)
		//__android_log_write(ANDROID_LOG_ERROR, "UntzJNI", "EOF reached");
#else
		printf("EOF reached\n", readFrames);
#endif
		mEOF = true;
	}

	return readFrames;
}

void WaveFileAudioSource::setDecoderPosition(Int64 startFrame)
{
	mWaveFile.setPosition(startFrame * mWaveFile.getHeader().bytesPerFrame);
	if(startFrame < getLength() * getSampleRate())
		mEOF = false;
}

#include "OggAudioSource.h"

#define fmin(a,b) (a < b) ? a : b
#define fmax(a,b) (b < a) ? a : b

#define BUFFER_SIZE     32768       // 32 KB buffers


OggAudioSource::OggAudioSource()
{
	mCurrentPosition = 0;
    mpOggInfo = NULL;
	mInFile = 0;
}

OggAudioSource::~OggAudioSource()
{
	ov_clear(&mOggFile);

	if(mInFile)
		fclose(mInFile);
}

bool OggAudioSource::load(const RString& path)
{
	mPath = path;
	mInFile = fopen(mPath.c_str(), "rb");

	if(mInFile == NULL)
	{
		std::cerr << "Cannot open " << mPath.c_str() << " for reading..." << std::endl;
		return false;
	}

	// Try opening the given file
	if(ov_open(mInFile, &mOggFile, NULL, 0) != 0)
	{
		std::cerr << "Error opening " << mPath.c_str() << " for decoding..." << std::endl;
		return false;
	}

	// Get some information about the OGG file
	mpOggInfo = ov_info(&mOggFile, -1);

	// Start the decoder thread
	mLoadMoreDataEvent.post();
	start();

	return true;
}

void OggAudioSource::start()
{
	RThread::start();
}

void OggAudioSource::stop()
{
	RThread::stop();
}

UInt32 OggAudioSource::readFrames(float* data, UInt32 numChannels, UInt32 numFrames)
{
	if(!mpSound->isPlaying())
		return 0;

	mCriticalSection.lock();

	UInt32 framesRead = numFrames;
	if(mBuffer.size() > 0)
	{
		if(mBuffer.size() / mpOggInfo->channels < framesRead)
		{
			framesRead = mBuffer.size() / mpOggInfo->channels;
		}

		for(UInt32 j = 0; j < numChannels; ++j)
		{
			float *in = NULL;
			if(mpOggInfo->channels == 1)
				in = &mBuffer[0];
			else
				in = &mBuffer[j];

			for(UInt32 i = 0; i < framesRead; ++i)
			{
				*(data++) = *in;
				in += mpOggInfo->channels;
			}
		}

		mBuffer.erase(mBuffer.begin(), mBuffer.begin()+(framesRead * mpOggInfo->channels));
	}
	else
		printf("not writing data.\n");

/*
	mCriticalSection.lock();

	UInt32 framesRead = numFrames;
	if(mBuffer.size() > 0)
	{
		if(mBuffer.size() < numFrames * numChannels)
		{
			framesRead = mBuffer.size() / numChannels;
		}

		for(UInt32 j = 0; j < numChannels; ++j)
		{
			float *in = &mBuffer[j];
			for(UInt32 i = 0; i < framesRead; ++i)
			{
				*(buffer++) = *in;
				in += numChannels;
			}
		}

		mBuffer.erase(mBuffer.begin(), mBuffer.begin()+(framesRead * numChannels));
	}
*/
	mCurrentPosition += framesRead;
	printf("current position = %d\n", mCurrentPosition);

	// Check if we need to decode more data
	if(mBuffer.size() < numFrames * mpOggInfo->channels * 2)
	{
		if(mEOF && getLooping())
		{
			setDecoderPosition(0);
		}
	}	

	mCriticalSection.unlock();

	if(mEOF && framesRead < numFrames)
	{
		mpSound->stop();
	}

	return framesRead;
}

void OggAudioSource::setDecoderPosition(double position)
{
	mEOF = false;
	mCriticalSection.lock();
	ov_time_seek(&mOggFile, position);
	mCriticalSection.unlock();
	mLoadMoreDataEvent.post();
}

void OggAudioSource::setPosition(double position)
{
	setDecoderPosition(position);
	mCriticalSection.lock();
	mBuffer.clear();
	mCurrentPosition = (UInt32)(position * getSampleRate());
	mLoadMoreDataEvent.post();
	mCriticalSection.unlock();
}

double OggAudioSource::getPosition()
{
	return (double)mCurrentPosition / getSampleRate();
}

double OggAudioSource::getLength() 
{ 
	return ov_time_total(&mOggFile, -1);
}

UInt32 OggAudioSource::getSampleRate() 
{
	if(mpOggInfo)
		return mpOggInfo->rate;
	return 0; 
}

void OggAudioSource::run()
{
	int endian = 0; // 0 for Little-Endian, 1 for Big-Endian
	int bitStream;
	long bytes;
	char data[BUFFER_SIZE]; // Local fixed size array

	do
	{
		mLoadMoreDataEvent.wait();
		// See if need to stop thread
		if (shouldThreadExit())
			break;

		mCriticalSection.lock();
		// Keep reading until all have 2 secs data
		do
		{
			// Read up to a buffer's worth of decoded sound data
			bytes = ov_read(&mOggFile, data, BUFFER_SIZE, endian, 2, 1, &bitStream);

			if (bytes > 0)
			{

				// Read samples and convert from short to float
				Int16* p = (Int16*)&data;
				UInt32 samples = bytes / 2;
				for(UInt32 i = 0; i < samples; ++i)
				{
					float temp = *(p++) / 32767.0f;
					mBuffer.push_back(temp);
				}

				// Stop decoding once we have 2 seconds of data
				if(mBuffer.size() > UNTZ::System::get()->getSampleRate()*2)
				{
					mLoadMoreDataEvent.reset();
					break;
				}
			}
			else
			{
				mEOF = true;
				break;
			}

		} while (true);
		
		mCriticalSection.unlock();

	}
	while (true);

	printf("thread exited\n");
}

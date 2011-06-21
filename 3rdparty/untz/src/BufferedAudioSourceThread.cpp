
#include "BufferedAudioSourceThread.h"
#include "BufferedAudioSource.h"


BufferedAudioSourceThread* BufferedAudioSourceThread::mInstance = NULL;

BufferedAudioSourceThread::BufferedAudioSourceThread()
{
}

BufferedAudioSourceThread::~BufferedAudioSourceThread()
{
}

BufferedAudioSourceThread* BufferedAudioSourceThread::getInstance()
{
	if(!mInstance)
	{
		mInstance = new BufferedAudioSourceThread();
	}
	return mInstance;
}

void BufferedAudioSourceThread::deleteInstance()
{
	if(mInstance)
	{
		mInstance->stop();
		mInstance->mReadMore.signal();
		mInstance->wait();

		delete mInstance;
		mInstance = NULL;
	}
}

bool BufferedAudioSourceThread::addSource(BufferedAudioSource *source)
{
	RScopedLock l(&mLock);

	mSources.push_back(source);
	if(mSources.size() == 1)
	{
		if(!isRunning())
			start();

		mReadMore.signal();
	}
	
	return true;
}

bool BufferedAudioSourceThread::removeSource(BufferedAudioSource *source)
{
	RScopedLock l(&mLock);

	int index = -1;
	for(UInt32 i = 0; i < mSources.size(); ++i)
	{
		if(source == mSources[i])
		{
			index = i;	
			break;
		}
	}

	if(index >= 0)
	{
		mSources.erase(mSources.begin() + index);	
	}
	
	return true;
}

void BufferedAudioSourceThread::readMore()
{
	mReadMore.signal();
}

void BufferedAudioSourceThread::run()
{
    std::vector<float> tempBuffer;
    
	while(1)
	{
		mReadMore.wait();

		if(shouldThreadExit())
			break;

		mLock.lock();

		for(UInt32 i = 0; i < mSources.size(); ++i)
		{
			BufferedAudioSource *pSource = mSources[i];

//			RScopedLock l(&pSource->mLock);
            
            pSource->mLock.lock();

			Int64 totalFrames = pSource->getSampleRate() * SECONDS_TO_BUFFER;
			Int64 availableFrames = pSource->mBuffer.size() / pSource->getNumChannels();

            pSource->mLock.unlock();
            
			if(availableFrames < totalFrames)
			{
				Int64 numFrames = totalFrames - availableFrames;                
				Int64 framesToRead = numFrames;
				Int64 frames = 0;
                tempBuffer.resize(numFrames * pSource->getNumChannels());
                float *pBuffer = &tempBuffer[0];
				do
				{
					frames = pSource->decodeData(pBuffer, framesToRead);
					framesToRead -= frames;
					pBuffer += frames * pSource->getNumChannels();
				}
				while(framesToRead > 0 && !pSource->isEOF());

                // Now write the data back to the buffer
                pSource->mLock.lock();
                
                int fillPosition = pSource->mBuffer.size();
                Int64 totalSamples = pSource->mBuffer.size() + (numFrames - framesToRead) * pSource->getNumChannels();
                pSource->mBuffer.resize(totalSamples);
                memcpy(&pSource->mBuffer[fillPosition], &tempBuffer[0], sizeof(float) * (numFrames - framesToRead) * pSource->getNumChannels());

                pSource->mLock.unlock();
                
                
                // Check if we've reached the end
				if(pSource->isEOF())
                {
                    if(pSource->isLooping())
                        // Reset the buffering position and keep on going...
                        pSource->setDecoderPosition(0);
//                    else
//                        // We didn't fill the whole buffer, so resize appropriately
//                        pSource->mBuffer.resize(availableFrames + numFrames - framesToRead);
                }
			}

			mLock.unlock();
		}
	}
}


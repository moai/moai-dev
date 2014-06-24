//
//  BufferedAudioSourceThread.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

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

	for(UInt32 i = 0; i < mSources.size(); ++i)
	{
		if(source == mSources[i])
		{
            return false; // Already added
		}
	}
    
	mSources.push_back(source);
	if(mSources.size() == 1)
	{
		if(!isRunning())
			start();
	}

	mReadMore.signal();
	
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

		
		{ // create a local scope for our locking object
			RScopedLock l(&mLock);
		
			for(UInt32 i = 0; i < mSources.size(); ++i)
			{
				BufferedAudioSource *pSource = mSources[i];
				
				pSource->mLock.lock();

				Int64 totalFrames = pSource->getSampleRate() * SECONDS_TO_BUFFER;
				Int64 framesThreshold = totalFrames / 2; // 
				Int64 availableFrames = pSource->mBuffer.size() / pSource->getNumChannels();

				pSource->mLock.unlock();
				
				if(availableFrames <= framesThreshold)
				{
					Int64 numFrames = totalFrames - availableFrames;                
					Int64 framesToRead = numFrames;
					Int64 frames = 0;
					tempBuffer.resize(numFrames * pSource->getNumChannels());
					float *pBuffer = &tempBuffer[0];
                    
					// keep track of last known version when starting to buffer
					// to discard any previously buffered data in case source
					// changed position. This avoids having to keep a lock
					// while buffering data.
					int lastVersion = pSource->getPositionVersion();
					
					while(framesToRead > 0 && !pSource->isEOF())
					{
                        int currentVersion = 0;
						frames = pSource->decodeData(pBuffer, framesToRead, currentVersion);
						
						// check if version changed since we started buffering
						// and discard everything we've buffered before if so
                        if (currentVersion != lastVersion) {
                            tempBuffer.erase(tempBuffer.begin(), tempBuffer.begin() + ((numFrames - framesToRead) * pSource->getNumChannels()));
                            tempBuffer.resize(numFrames * pSource->getNumChannels());
                            pBuffer = &tempBuffer[0];
                            framesToRead = numFrames;
                        }
						framesToRead -= frames;
						pBuffer += frames * pSource->getNumChannels();
                        lastVersion = currentVersion;
					}

					// Now write the data back to the buffer
					Int64 framesRead = numFrames - framesToRead;
					if(framesRead > 0)
					{
						pSource->mLock.lock();
						pSource->mBuffer.putData(&tempBuffer[0], framesRead * pSource->getNumChannels());
						pSource->mLock.unlock();
					}
				}
			}
		}
		
	}
}


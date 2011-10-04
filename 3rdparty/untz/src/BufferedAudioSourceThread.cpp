//
//  BufferedAudioSourceThread.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright 2011 Retronyms. All rights reserved.
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
				Int64 framesRead = numFrames - framesToRead;
				if(framesRead > 0)
				{
	                pSource->mLock.lock();
                
//		            int fillPosition = pSource->mBuffer.size();
//			        Int64 totalSamples = pSource->mBuffer.size() + framesRead * pSource->getNumChannels();
//				    pSource->mBuffer.resize(totalSamples);
	//				memcpy(&pSource->mBuffer[fillPosition], &tempBuffer[0], sizeof(float) * framesRead * pSource->getNumChannels());
//					pSource->mBuffer.putData(&tempBuffer[0], framesRead * pSource->getNumChannels(), startFrame);

					pSource->mBuffer.putData(&tempBuffer[0], framesRead * pSource->getNumChannels());
					pSource->mLock.unlock();
				}
                
                // Check if we've reached the end
/*
				double loopStart;
				double loopEnd;
				pSource->getLoopPoints(loopStart, loopEnd);
				Int64 loopEndFrame = pSource->convertSecondsToSamples(pSource->mLoopEnd);

                if(pSource->isLooping() && pSource->mCurrentFrame >= loopEndFrame && loopEndFrame > 0)
					pSource->setPosition(loopStart);


				if(pSource->isEOF() || (pSource->mCurrentFrame >= loopEndFrame && loopEndFrame > 0))
                {
                    if(pSource->isLooping())
                        // Reset the buffering position and keep on going...
                        pSource->setPosition(loopStart);
//                    else
//                        // We didn't fill the whole buffer, so resize appropriately
//                        pSource->mBuffer.resize(availableFrames + numFrames - framesToRead);
                }
*/
			}

		}

        mLock.unlock();
	}
}


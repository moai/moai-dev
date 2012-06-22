

//
//  BufferedAudioSource.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "BufferedAudioSource.h"
#include "BufferedAudioSourceThread.h"

BufferedAudioSource::BufferedAudioSource()
{
	mLoadedInMemory = false;
    mEOF = false;
}

BufferedAudioSource::~BufferedAudioSource()
{
}

/*
bool BufferedAudioSource::init(float* interleavedData, Int64 numSamples)
{
    return true;
}
*/
bool BufferedAudioSource::init(const RString& path, bool loadIntoMemory) 
{ 
	if(loadIntoMemory)
	{
		RPRINT("loading sound into memory...\n");
        int channels = getNumChannels();
        double length = getLength();
		double srate = getSampleRate();
		UInt32 totalFrames = getSampleRate() * getLength();
		mBuffer.resize(getNumChannels(), totalFrames);//getSampleRate() * getLength());
		float *pWritePos = mBuffer.getData();
		UInt32 numFrames = (UInt32)(getSampleRate());
		UInt32 framesRead = 0;
		UInt32 remainingFrames = totalFrames;
		UInt32 tf = 0;
		do
		{
			framesRead = decodeData(pWritePos, remainingFrames >= numFrames ? numFrames : remainingFrames);
			pWritePos += framesRead * getNumChannels();
			tf += framesRead;
			remainingFrames -= framesRead;
		}
		while(remainingFrames > 0);
		mEOF = true;
		mLoadedInMemory = loadIntoMemory; 
		doneDecoding();
	}
	else
	{
        RScopedLock l(&mLock);
		mBuffer = RAudioBuffer(getNumChannels(), getSampleRate() * SECONDS_TO_BUFFER);
		BufferedAudioSourceThread::getInstance()->addSource(this);
	}

	return true;
}

void BufferedAudioSource::close()
{
	if(!isLoadedInMemory())
	{
		BufferedAudioSourceThread::getInstance()->removeSource(this);        
    }
}

void BufferedAudioSource::setPosition(double seconds)
{
	seconds = seconds < 0 ? 0.0f : seconds;
	seconds = seconds > getLength() ? getLength() : seconds;

	RScopedLock l(&mLock);

	Int64 frames = (Int64)(seconds * getSampleRate()); 
    if(!isLoadedInMemory())
    {
        mBuffer.clear();
        setDecoderPosition(frames);
		BufferedAudioSourceThread::getInstance()->readMore();
    }
}

Int64 BufferedAudioSource::readFrames(float* buffer, UInt32 numChannels, UInt32 numFrames, AudioSourceState& state)
{
	mLock.lock();

	Int64 framesRead = numFrames;
	int framesAvailable = mBuffer.size() / getNumChannels() - state.mCurrentFrame;
    
	// For disk-streaming sources we calculate available frames using the whole buffer
    if(!isLoadedInMemory())
        framesAvailable = mBuffer.size() / getNumChannels();
    
	mLock.unlock();

	Int64 loopEndFrame = convertSecondsToSamples(state.mLoopEnd);
	Int64 totalFrames = convertSecondsToSamples(getLength());
	bool needToLoop = state.mLooping && ((state.mCurrentFrame >= loopEndFrame && loopEndFrame > 0) || (framesAvailable == 0 && mEOF));
	
	if(framesAvailable > 0 && !needToLoop)
	{
		RScopedLock l(&mLock);

		if(framesAvailable < numFrames)
			framesRead = framesAvailable;

		int sourceChannels = getNumChannels();
        int frameOffset = state.mCurrentFrame;
        
        // For disk-streaming sources we always start at the beginning of the buffer
        if(!isLoadedInMemory())
            frameOffset = 0;
        
		for(UInt32 j = 0; j < numChannels; ++j)
		{
			float *in = NULL;
			if(sourceChannels == 1)
				in = mBuffer.getData(0, frameOffset);
			else
				in = mBuffer.getData(j, frameOffset);

			for(UInt32 i = 0; i < framesRead; ++i)
			{
				*(buffer++) = *in;
				in += sourceChannels;
			}
		}

        state.mCurrentFrame += framesRead;
		
        if(!isLoadedInMemory())
		{
			mBuffer.erase(0, framesRead);
			framesAvailable = mBuffer.size() / getNumChannels();
			UInt32 minimumFrames = getSampleRate() * SECONDS_TO_BUFFER / 2;
			if(framesAvailable <= minimumFrames)
			{
				BufferedAudioSourceThread::getInstance()->readMore();
			}
		}
	}
    else
    {
        framesRead = ERR_BUFFERING;
        
		if(needToLoop)
		{
			setPosition(state.mLoopStart);
			state.mCurrentFrame = convertSecondsToSamples(state.mLoopStart);
		}

		Int64 totalFrames = convertSecondsToSamples(getLength());
        if(state.mCurrentFrame >= totalFrames)
        {
            return 0; // signal that we are done
        }
		else if(!isLoadedInMemory())
		{
			BufferedAudioSourceThread::getInstance()->readMore();
		}
    }
    
	return framesRead;
}
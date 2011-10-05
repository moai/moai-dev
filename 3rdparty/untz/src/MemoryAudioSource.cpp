//
//  MemoryAudioSource.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright 2011 Retronyms. All rights reserved.
//

#include "MemoryAudioSource.h"
#include <math.h>

using namespace UNTZ;


MemoryAudioSource::MemoryAudioSource(UNTZ::SoundInfo& info, float* interleavedData, bool ownsData)
{
	mInfo = info;
	UInt32 frames = info.mLength * info.mSampleRate;
	mBuffer = RAudioBuffer(info.mChannels, info.mTotalFrames, interleavedData, ownsData);
	mLoadedInMemory = true;
	mEOF = true;
}
/*
MemoryAudioSource::MemoryAudioSource(UInt32 sampleRate, UInt32 numChannels, UInt32 numSamples, Int16* buffer)
{
	mInfo.mChannels = numChannels;
	mInfo.mSampleRate = sampleRate;
	mInfo.mLength = (double)numSamples * sampleRate; 
	mInfo.mBitsPerSample = 4;
    
    std::vector<float> data(numSamples, 0);    
    for(UInt32 i = 0; i < numSamples; i++)
    {
        data[i] = *(buffer++)/32767.0;
    }

	mBuffer = RAudioBuffer(numChannels, numSamples / numChannels, &data[0], true);
	mLoadedInMemory = true;
	mEOF = true;
}
*/

double MemoryAudioSource::getSampleRate()
{
    return mInfo.mSampleRate;
}

double MemoryAudioSource::getLength()
{
    return mInfo.mLength;
}

UInt32 MemoryAudioSource::getNumChannels()
{
    return mInfo.mChannels;
}

UInt32 MemoryAudioSource::getBitsPerSample()
{
	return mInfo.mBitsPerSample;
}
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


MemoryAudioSource::MemoryAudioSource(UInt32 sampleRate, UInt32 numChannels, UInt32 numSamples, Int16* buffer)
{
	mSampleRate = (double)sampleRate;
	mNumChannels = numChannels;
    mNumFrames = numSamples / numChannels;
    
    std::vector<float> data(numSamples, 0);    
    for(UInt32 i = 0; i < numSamples; i++)
    {
        data[i] = *(buffer++)/32767.0;
    }

    init(&data[0], numSamples);
}

bool MemoryAudioSource::init(const RString& path, bool loadIntoMemory)
{
    return false;
}

bool MemoryAudioSource::init(float* interleavedData, Int64 numSamples)
{
    return BufferedAudioSource::init(interleavedData, numSamples);
}

Int64 MemoryAudioSource::decodeData(float* buffer, UInt32 numFrames)
{
    return 0;
}

void MemoryAudioSource::setDecoderPosition(Int64 startFrame)
{
}

double MemoryAudioSource::getSampleRate()
{
    return mSampleRate;
}

double MemoryAudioSource::getLength()
{
    return (double)mNumFrames / getSampleRate();
}

UInt32 MemoryAudioSource::getNumChannels()
{
    return mNumChannels;
}

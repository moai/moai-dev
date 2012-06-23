//
//  MemoryAudioSource.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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
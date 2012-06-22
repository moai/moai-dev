//
//  UserAudioSource.cpp
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "UserAudioSource.h"

using namespace UNTZ;


UserAudioSource::UserAudioSource(UInt32 sampleRate, UInt32 numChannels, StreamCallback* callback, void* userdata)
{
	mSampleRate = sampleRate;
	mNumChannels = numChannels;
	mCallback = callback;
	mUserdata = userdata;
}

Int64 UserAudioSource::readFrames(float* buffers, UInt32 numChannels, UInt32 numSamples, AudioSourceState& state)
{
	return mCallback(buffers, mNumChannels, numSamples, mUserdata);
}
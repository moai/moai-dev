//
//  UserAudioSource.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef USERAUDIOSOURCE_H_
#define USERAUDIOSOURCE_H_

#include "Untz.h"
#include "AudioSource.h"

class UserAudioSource : public AudioSource
{
public:
	UserAudioSource(UInt32 sampleRate, UInt32 numChannels, UNTZ::StreamCallback* callback, void* userdata);

	// AudioSource
	virtual Int64 readFrames(float* buffer, UInt32 numChannels, UInt32 numSamples, AudioSourceState& state);

private:
	UNTZ::StreamCallback* mCallback;
	void* mUserdata;
	UInt32 mNumChannels;
	UInt32 mSampleRate;
};

#endif
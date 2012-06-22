//
//  AudioMixer.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef AUDIOMIXER_H_
#define AUDIOMIXER_H_

#include "Untz.h"
#include <threading/Threading.h>
#include <vector>

class AudioMixer
{
	friend class UNTZ::SystemData;
public:
	AudioMixer();
	virtual ~AudioMixer();	

	void init();
	void addSound(UNTZ::Sound* sound);
	void removeSound(UNTZ::Sound* sound);
	void removeSound(int index);
	void setVolume(float volume);
	float getVolume() const;
	int process(UInt32 numInputChannels, float* inputChannelData, 
		UInt32 numOutputChannels, float* outputChannelData, UInt32 numFrames);
	
protected:
	int mNumChannels;
	float mVolume;
	RCriticalSection mLock;
	std::vector<UNTZ::Sound*> mSounds;
	std::vector<float> mBuffer;
};

#endif
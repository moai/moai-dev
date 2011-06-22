#pragma once

#include "Untz.h"
#include <Threading/Threading.h>
#include <vector>

class AudioMixer
{
public:
	AudioMixer();
	virtual ~AudioMixer();	

	void init();
	void addSound(UNTZ::Sound* sound);
	void removeSound(UNTZ::Sound* sound);
	void removeSound(int index);
	int process(UInt32 numInputChannels, float* inputChannelData, 
		UInt32 numOutputChannels, float* outputChannelData, UInt32 numFrames);
private:
	int mNumChannels;
	RCriticalSection mLock;
	std::vector<UNTZ::Sound*> mSounds;
	std::vector<float> mBuffer;
};


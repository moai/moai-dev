//
//  AudioMixer.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#include "AudioMixer.h"
#include "SystemData.h"
#include "SoundData.h"

using namespace UNTZ;


AudioMixer::AudioMixer(void)
{
    mNumChannels = 0;
	mVolume = 1.0f;
}

AudioMixer::~AudioMixer(void)
{
}

void AudioMixer::init()
{
	int numSamples = UNTZ::System::get()->getData()->getNumFrames() *
		UNTZ::System::get()->getData()->getNumOutputChannels();
	mBuffer.resize(numSamples, 0);
}

void AudioMixer::addSound(UNTZ::Sound* sound)
{
	RScopedLock l(&mLock);

	for(UInt32 i = 0; i < mSounds.size(); ++i)
	{
		if(mSounds[i] == sound)
			return;
	}
	mSounds.push_back(sound);
}

void AudioMixer::removeSound(UNTZ::Sound *sound)
{
	RScopedLock l(&mLock);

	int index = -1;
	for(UInt32 i = 0; i < mSounds.size(); ++i)
	{
		if(sound == mSounds[i])
        {
			index = i;	
            break;
        }
	}
    if(index >= 0)
       mSounds.erase(mSounds.begin() + index);
}

void AudioMixer::removeSound(int index)
{
	RScopedLock l(&mLock);
	mSounds.erase(mSounds.begin() + index);
}

void AudioMixer::setVolume(float volume)
{
	volume = volume < 0.0f ? 0.0f : volume;
	volume = volume > 1.0f ? 1.0f : volume;
	mVolume = volume;
}

float AudioMixer::getVolume() const
{
	return mVolume;
}

int AudioMixer::process(UInt32 numInputChannels, float* inputBuffer, UInt32 numOutputChannels, float *outputBuffer, UInt32 numFrames)
{
    mLock.lock();

	int z = 0;
    memset(outputBuffer, 0, sizeof(float) * numFrames * numOutputChannels);  
	for(UInt32 i = 0; i < mSounds.size(); ++i)
	{		
		UNTZ::Sound *s = mSounds[i];
		if(s->getData()->getState() == kPlayStatePlaying)
		{
			++z;
			Int64 totalFramesRead = 0;
			Int64 framesRead = 0;
			do
			{
				framesRead = s->getData()->getSource()->readFrames((float*)&mBuffer[0], numOutputChannels, numFrames - totalFramesRead, s->getData()->mState);
				if(framesRead > 0)
                {
                    for(UInt32 k = 0; k < numOutputChannels; ++k)
                    {
                        float *out = &outputBuffer[k*numFrames + totalFramesRead];
                        float *in = &mBuffer[k*framesRead + totalFramesRead]; 
                        for(UInt32 j = 0; j < framesRead; j++)
                            *(out++) += *(in++) * s->getData()->mVolume;
                    }
                    totalFramesRead += framesRead;
                }
			}
			while(framesRead > 0 && totalFramesRead < numFrames);
            
			if(framesRead == 0)
            {
				s->stop();
            }
		}
	}
	
	RPRINT("processed %d sources\n", z);

    mLock.unlock();

	// volume & clipping
	
	// commented out in HBS contrib
    //for(UInt32 k = 0; k < numOutputChannels * numFrames; ++k)
    //{
	//	float val = *outputBuffer * mVolume;;
	//	val = val > 1.0 ? 1.0 : val;
	//	val = val < -1.0 ? -1.0 : val;
	//	*(outputBuffer)++ = val;
    //}

	return 0;
}


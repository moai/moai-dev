#include "AudioMixer.h"
#include "SystemData.h"
#include "SoundData.h"

using namespace UNTZ;


AudioMixer::AudioMixer(void)
{
    mNumChannels = 0;
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
	mCriticalSection.lock();
	for(UInt32 i = 0; i < mSounds.size(); ++i)
	{
		if(mSounds[i] == sound)
			return;
	}
	mSounds.push_back(sound);
	mCriticalSection.unlock();
}

void AudioMixer::removeSound(UNTZ::Sound *sound)
{
	mCriticalSection.lock();
	int index = 0;
	for(UInt32 i = 0; i < mSounds.size(); ++i)
	{
		if(sound == mSounds[i])
			index = i;	
	}
	mSounds.erase(mSounds.begin() + index);
	mCriticalSection.unlock();
}

void AudioMixer::removeSound(int index)
{
	mCriticalSection.lock();
	mSounds.erase(mSounds.begin() + index);
	mCriticalSection.unlock();
}

int AudioMixer::process(UInt32 numInputChannels, float* inputBuffer, UInt32 numOutputChannels, float *outputBuffer, UInt32 numFrames)
{
	mCriticalSection.lock();

    memset(outputBuffer, 0, sizeof(float) * numFrames * numOutputChannels);  
	for(UInt32 i = 0; i < mSounds.size(); ++i)
	{		
		UNTZ::Sound *s = mSounds[i];
		if(s->getData()->getState() == kPlayStatePlaying)
		{
			UInt32 totalFramesRead = 0;
			UInt32 framesRead = 0;
			do
			{
				framesRead = s->getData()->getSource()->readFrames((float*)&mBuffer[0], numOutputChannels, numFrames - totalFramesRead);
				for(UInt32 k = 0; k < numOutputChannels; ++k)
				{
					float *out = &outputBuffer[k*numFrames + totalFramesRead];
					float *in = &mBuffer[k*numFrames + totalFramesRead]; 
					for(UInt32 j = 0; j < framesRead; j++)
						*(out++) += *(in++) * s->getData()->mVolume;
				}
				totalFramesRead += framesRead;
			}
			while(totalFramesRead > 0 && 
				totalFramesRead < numFrames && 
				s->getData()->getState() != kPlayStateStopped);
		}
	}

	mCriticalSection.unlock();

	return 0;
}


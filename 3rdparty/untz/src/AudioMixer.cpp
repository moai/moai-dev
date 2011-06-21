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

int AudioMixer::process(UInt32 numInputChannels, float* inputBuffer, UInt32 numOutputChannels, float *outputBuffer, UInt32 numFrames)
{
	RScopedLock l(&mLock);

    memset(outputBuffer, 0, sizeof(float) * numFrames * numOutputChannels);  
	for(UInt32 i = 0; i < mSounds.size(); ++i)
	{		
		UNTZ::Sound *s = mSounds[i];
		if(s->getData()->getState() == kPlayStatePlaying)
		{
			Int64 totalFramesRead = 0;
			Int64 framesRead = 0;
			do
			{
				framesRead = s->getData()->getSource()->readFrames((float*)&mBuffer[0], numOutputChannels, numFrames - totalFramesRead);
				if(framesRead > 0)
                {
                    for(UInt32 k = 0; k < numOutputChannels; ++k)
                    {
                        float *out = &outputBuffer[k*numFrames + totalFramesRead];
                        float *in = &mBuffer[k*numFrames + totalFramesRead]; 
                        for(UInt32 j = 0; j < framesRead; j++)
                            *(out++) += *(in++) * s->getData()->mVolume;
                    }
                    totalFramesRead += framesRead;
                }
			}
			while(totalFramesRead > 0 && 
				totalFramesRead < numFrames && 
				framesRead >= 0);
            
			if(framesRead < 0)
				s->stop();
		}
	}

	return 0;
}


#pragma once;

#include "Untz.h"
#include "AudioSource.h"

class MemoryAudioSource : public AudioSource
{
public:
	MemoryAudioSource(UInt32 sampleRate, UInt32 numChannels, UInt32 numSamples, Int16 *interleavedData);

	// AudioSource
	virtual Int64 readFrames(float *buffers, UInt32 numChannels, UInt32 numSamples);
private:
    std::vector < std::vector< float > > mData;
    UInt32 mCurrentPos;
	UInt32 mNumChannels;
	UInt32 mSampleRate;
};
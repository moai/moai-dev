#include "MemoryAudioSource.h"
#include <math.h>

using namespace UNTZ;

#define fmin(a,b) (a < b) ? a : b
#define fmax(a,b) (b < a) ? a : b

MemoryAudioSource::MemoryAudioSource(UInt32 sampleRate, UInt32 numChannels, UInt32 numSamples, Int16* buffers)
{
	mSampleRate = sampleRate;
	mNumChannels = numChannels;

    // Allocate non-interleaved storage for the audio.
    mData.resize(numChannels);
    for(UInt32 i = 0; i < mData.size(); i++)
        mData[i].resize(numSamples);
    
    //Copy into non-interleaved storage and convert to float.
    for(UInt32 i = 0; i < numSamples; i++)
        for(UInt32 j = 0; j < numChannels; j++)
        {
            mData[j][i] = *(buffers++)/32767.0;
        }
    
    mCurrentPos = 0;
}

Int64 MemoryAudioSource::readFrames(float *buffers, UInt32 numChannels, UInt32 numSamples)
{
    UInt32 readCount = (UInt32)fmin(mData[0].size() - mCurrentPos, numSamples);

    for(UInt32 channel = 0; channel < mData.size(); channel++)
    {
        memcpy(buffers+channel*numSamples, &mData[channel][0]+mCurrentPos, readCount*sizeof(float));
    }
    mCurrentPos+=readCount;
    
    if(mCurrentPos >= mData[0].size())
    {
        mCurrentPos = 0;
        if(!mLooping)
			return -1;
    }
    
	return readCount;
}
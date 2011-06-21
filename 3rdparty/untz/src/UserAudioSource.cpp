#include "UserAudioSource.h"

using namespace UNTZ;

UserAudioSource::UserAudioSource(UInt32 sampleRate, UInt32 numChannels, StreamCallback* callback, void* userdata)
{
	mSampleRate = sampleRate;
	mNumChannels = numChannels;
	mCallback = callback;
	mUserdata = userdata;
}

Int64 UserAudioSource::readFrames(float* buffers, UInt32 numChannels, UInt32 numSamples)
{
	return mCallback(buffers, mNumChannels, numSamples, mUserdata);
}
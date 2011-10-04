//
//  RAudioBuffer.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright 2011 Retronyms. All rights reserved.
//

#ifndef RAUDIOBUFFER_H_
#define RAUDIOBUFFER_H_

#include <vector>

class RAudioBuffer
{
public:
	RAudioBuffer() : mChannels(0), mFrames(0) {};
	RAudioBuffer(int channels, int frames) : mChannels(channels), mFrames(frames)
	{
		resize(mChannels, mFrames);
	}
	~RAudioBuffer() {};

	void resize(UInt32 channels, UInt32 frames, bool allocate = true)
	{
		mChannels = channels;
		mFrames = frames;
		if(allocate)
			mBuffer.resize(mChannels * mFrames, 0);
		else
			mBuffer.reserve(mChannels * mFrames);
	}

	void resize(int frames)
	{
		resize(1, frames);
	}

	void clear()
	{
		 mBuffer.clear();
	}

	UInt32 size() const
	{
		return mBuffer.size();
	}

	void erase(UInt32 startFrame, UInt32 numFrames)
	{
		mBuffer.erase(mBuffer.begin() + (startFrame * mChannels), mBuffer.begin() + ((startFrame + numFrames) * mChannels));
	}

	void putSample(float sample)
	{
		mBuffer.push_back(sample);
	}

	void putData(float *data, UInt32 numSamples)
	{
		UInt32 pos = mBuffer.size();
		mBuffer.resize(mBuffer.size() + numSamples);
		memcpy(&mBuffer[pos], data, sizeof(float) * numSamples);
	}

	float* getData(UInt32 channel, UInt32 startFrame)
	{
		return &mBuffer[mChannels * startFrame + channel];
	}

	float* getData(UInt32 startFrame = 0)
	{
		return &mBuffer[mChannels * startFrame];
	}

private:
	UInt32 mChannels;
	UInt32 mFrames;
	std::vector<float> mBuffer;
};

#endif // RAUDIOBUFFER_H_
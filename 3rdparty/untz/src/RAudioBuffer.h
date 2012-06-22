//
//  RAudioBuffer.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef RAUDIOBUFFER_H_
#define RAUDIOBUFFER_H_

#include <vector>

class RAudioBuffer
{
public:
	RAudioBuffer() : mChannels(0), mFrames(0), mOwnsBuffer(true) {};
	RAudioBuffer(UInt32 channels, UInt32 frames)
	{
		mChannels = channels;
		mFrames = frames;
		mOwnsBuffer = true;
		mBuffer.reserve(mChannels * mFrames);
	}
	RAudioBuffer(UInt32 channels, UInt32 frames, float* buffer, bool ownsBuffer)
	{
		mChannels = channels;
		mFrames = frames;
		mOwnsBuffer = ownsBuffer;
		if(mOwnsBuffer)
		{
			resize(mChannels, mFrames);
			memcpy(mBufferPtr, buffer, sizeof(float) * mBuffer.size());
		}
		else		
			mBufferPtr = buffer;
	}
	~RAudioBuffer() {};

	void resize(UInt32 channels, UInt32 frames)
	{
		if(mOwnsBuffer)
		{
			mChannels = channels;
			mFrames = frames;
			mBuffer.resize(mChannels * mFrames, 0);
			mBufferPtr = &mBuffer[0];
		}
	}

	void clear()
	{
		if(mOwnsBuffer)
			 mBuffer.clear();
	}

	UInt32 size() const
	{
		if(mOwnsBuffer)
			return mBuffer.size();
		return mChannels * mFrames;
	}

	void erase(UInt32 startFrame, UInt32 numFrames)
	{
		if(mOwnsBuffer)
			mBuffer.erase(mBuffer.begin() + (startFrame * mChannels), mBuffer.begin() + ((startFrame + numFrames) * mChannels));
	}

	void putSample(float sample)
	{
		if(mOwnsBuffer)
		{
			mBuffer.push_back(sample);
			mBufferPtr = &mBuffer[0];
		}
	}

	void putData(float *data, UInt32 numSamples)
	{
		if(mOwnsBuffer)
		{
			UInt32 pos = mBuffer.size();
			mBuffer.resize(mBuffer.size() + numSamples);
			memcpy(&mBuffer[pos], data, sizeof(float) * numSamples);
			mBufferPtr = &mBuffer[0];
		}
	}

	float* getData(UInt32 channel, UInt32 startFrame)
	{
		return &mBufferPtr[mChannels * startFrame + channel];
	}

	float* getData(UInt32 startFrame = 0)
	{
		return &mBufferPtr[mChannels * startFrame];
	}

	UInt32 getDataSize()
	{
		return size() * sizeof(float);
	}

	void copyInto(float* buffer)
	{
		memcpy(buffer, getData(), getDataSize());
	}

private:
	UInt32 mChannels;
	UInt32 mFrames;
	std::vector<float> mBuffer;
	float* mBufferPtr;
	bool mOwnsBuffer;
};

#endif // RAUDIOBUFFER_H_
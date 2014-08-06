//
//  AudioSource.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef AUDIOSOURCE_H_
#define AUDIOSOURCE_H_

#include "Untz.h"
#include "RAudioBuffer.h"
#define NO_MEMBER_TEMPLATES
#include "counted_ptr.h"

#define ERR_BUFFERING (-1)

struct AudioSourceState
{
	Int64 mCurrentFrame;
	double mLoopStart;
	double mLoopEnd;
	bool mLooping;
};

class AudioSource
{
public:
	AudioSource() : mEOF(false), mLoadedInMemory(false) {};
	virtual ~AudioSource() {};
	virtual Int64 readFrames(float* buffer, UInt32 numChannels, UInt32 numFrames, AudioSourceState& state) = 0;
	virtual void setPosition(double position) {};
	virtual UInt32 getBitsPerSample() { return 0; }
	virtual double getSampleRate() { return 0; }
	virtual UInt32 getNumChannels() { return 0; }
	virtual double getLength() { return 0; }
	virtual void clear() {};

	double convertSamplesToSeconds(Int64 samples) 
	{
		double time = (double)samples;
		return time / getSampleRate();
	}

	Int64 convertSecondsToSamples(float seconds)
	{
		return (Int64)(seconds * getSampleRate());
	}

	Int64 convertSecondsToBytes(float seconds)
	{
		return (Int64)(seconds * getSampleRate() * getBitsPerSample() / 8);
	}

	bool isEOF() const { return mEOF; }
    bool isLoadedInMemory() { return mLoadedInMemory; }    

	RAudioBuffer* getBuffer() { return &mBuffer; }

protected:
	bool mEOF;
	bool mLoadedInMemory;
	RAudioBuffer mBuffer;
};

typedef counted_ptr<AudioSource> AudioSourcePtr;

#endif
//
//  BufferedAudioSource.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef BUFFEREDAUDIOSOURCE_H_
#define BUFFEREDAUDIOSOURCE_H_

#include "AudioSource.h"
#include <threading/Threading.h>

#define SECONDS_TO_BUFFER (2)

class BufferedAudioSourceThread;

class BufferedAudioSource : public AudioSource
{
	friend class BufferedAudioSourceThread;
public:
	BufferedAudioSource();
	virtual ~BufferedAudioSource();

	// AudioSource
	virtual Int64 readFrames(float* buffer, UInt32 numChannels, UInt32 numFrames, AudioSourceState& state);
	virtual void setPosition(double seconds);

	// BufferedAudioSource
	virtual bool init(const RString& path, bool loadIntoMemory);
//	virtual bool init(float* interleavedData, Int64 numSamples);
    virtual void close();
	virtual Int64 decodeData(float* buffer, UInt32 numFrames) { return 0; }
	virtual void setDecoderPosition(Int64 startFrame) {};
	virtual void doneDecoding() {};

protected:
	RCriticalSection mLock;
};

#endif
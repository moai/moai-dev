//
//  OggAudioSource.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef OPUSAUDIOSOURCE_H_
#define OPUSAUDIOSOURCE_H_

#include "BufferedAudioSource.h"
#include <threading/Threading.h>
#include <opusfile.h>
#include <vector>
#include <cstdio>
#include <iostream>

class OpusAudioSource : public BufferedAudioSource
{
public:
	OpusAudioSource();
	~OpusAudioSource();

	// AudioSource
	double getSampleRate();
	double getLength();
	UInt32 getNumChannels();
	UInt32 getBitsPerSample();

	// BufferedAudioSource
	virtual bool init(const RString& path, bool loadIntoMemory);
    virtual void close();
	virtual Int64 decodeData(float* buffer, UInt32 size);
	virtual Int64 setDecoderPosition(Int64 startFrame);
	
private:
	void setDecoderPosition(double position);

	RCriticalSection mDecodeLock;
	RString mPath;
	FILE* mInFile;
	OggOpusFile* opusFile;

	int64_t total_samples;
	unsigned int channels;
	float *localBuffer;
	unsigned int localBufferSize;
};

#endif
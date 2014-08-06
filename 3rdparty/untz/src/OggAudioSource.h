//
//  OggAudioSource.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef OGGAUDIOSOURCE_H_
#define OGGAUDIOSOURCE_H_

#include "BufferedAudioSource.h"
#include <threading/Threading.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <vector>
#include <cstdio>
#include <iostream>

class OggAudioSource : public BufferedAudioSource
{
public:
	OggAudioSource();
	~OggAudioSource();

	// AudioSource
	double getSampleRate();
	double getLength();
	UInt32 getNumChannels();
	UInt32 getBitsPerSample();

	// BufferedAudioSource
	virtual bool init(const RString& path, bool loadIntoMemory);
    virtual void close();
	virtual Int64 decodeData(float* buffer, UInt32 size);
	virtual void setDecoderPosition(Int64 startFrame);
	
private:
	void setDecoderPosition(double position);

	RCriticalSection mDecodeLock;
	RString mPath;
	FILE* mInFile;
	vorbis_info* mpOggInfo;
	OggVorbis_File mOggFile;
};

#endif
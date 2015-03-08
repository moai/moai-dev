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
	double getSampleRate() const;
	double getLength() const;
	UInt32 getNumChannels() const;
	UInt32 getBitsPerSample() const;

	// BufferedAudioSource
	virtual bool init(const RString& path, bool loadIntoMemory);
    virtual void close();
	virtual Int64 decodeData(float* buffer, UInt32 size);
	virtual void setDecoderPosition(Int64 startFrame);
	
private:
	void setDecoderPosition(double position);
	const char* getErrorDescription(const int status) const;

#ifdef _DEBUG  //TODO: MOAI C++11 :/
	template<typename... Arguments>
	static inline void printError(const char* const message, Arguments... argv) 	{
	   printf(message, argv...);
	}
#else // NDEBUG
	template<typename... Arguments> 
	static inline void printError(const char* const message, Arguments... argv) 
	{ }
#endif 

	RCriticalSection mDecodeLock;
	RString mPath;
	FILE* mInFile;
	vorbis_info* mpOggInfo;
	OggVorbis_File mOggFile;
};

#endif

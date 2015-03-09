//
//  OggAudioSource.h
//  Part of UNTZ
//
//  Created by Robert Dalton Jr. (bob@retronyms.com) on 06/01/2011.
//  Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
//

#ifndef OGGAUDIOSOURCE_H_
#define OGGAUDIOSOURCE_H_

#include <threading/Threading.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <cstdio>

#include "BufferedAudioSource.h"

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
	const char* getErrorDescription(const int status) const;

/*
 * TODO: MOAI C++11, maybe to 2020 :)
#ifdef _DEBUG
	template<typename... Arguments>
		static inline void printError(const char* const message, Arguments... argv) {
			printf(message, argv...);
		}
#else
	template<typename... Arguments>
		static inline void printError(const char* const message, Arguments... argv) { }
#endif 
*/

#if _DEBUG
	static inline void printError(const char* const message, ...) {
		va_list vl;
		va_start(vl, message);
		printf(message, vl);
		va_end(vl);
	}
#else
	static inline void printError(const char* const message, ...) { }
#endif



	RCriticalSection mDecodeLock;
	vorbis_info* mpOggInfo;
	OggVorbis_File mOggFile;
};

#endif

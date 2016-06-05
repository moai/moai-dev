// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLSAMPLE_H
#define ZLSAMPLE_H

#include <zl-util/ZLAccessors.h>

class ZLStream;

//================================================================//
// ZLSample
//================================================================//
class ZLSample {
public:

	static const u32 SAMPLE_BUFFER_SIZE = 4;

	enum {
		SAMPLE_S8,
		SAMPLE_U8,
		SAMPLE_S16,
		SAMPLE_U16,
		SAMPLE_S32,
		SAMPLE_U32,
		SAMPLE_FLOAT,
		SAMPLE_UNKNOWN,
	};

	//----------------------------------------------------------------//
	static size_t		Convert					( u32 sourceType, const void* source, u32 destType, void* dest );
	static size_t		GetSize					( u32 type );
	static size_t		ReadSample				( ZLStream& stream, u32 streamType, void* buffer, u32 bufferType );
	static size_t		WriteSample				( ZLStream& stream, u32 streamType, const void* buffer, u32 bufferType );
};

#endif

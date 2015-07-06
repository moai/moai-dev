// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLREVBUFFERSTREAM_H
#define ZLREVBUFFERSTREAM_H

#include <zl-util/ZLRevBuffer.h>
#include <zl-util/ZLStream.h>

//================================================================//
// ZLRevBufferStream
//================================================================//
class ZLRevBufferStream :
	public virtual ZLStream,
	public ZLRevBuffer {
private:

	size_t			mCursor;
	size_t			mLength;

public:

	//----------------------------------------------------------------//
	size_t			GetCapacity				();
	u32				GetCaps					();
	size_t			GetCursor				();
	size_t			GetLength				();
	size_t			ReadBytes				( void* buffer, size_t size );
	int				SetCursor				( long offset );
	size_t			SetLength				( size_t length );
	size_t			WriteBytes				( const void* buffer, size_t size );
					ZLRevBufferStream		();
					~ZLRevBufferStream		();
};

#endif

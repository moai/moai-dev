// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLBYTESTREAM_H
#define ZLBYTESTREAM_H

#include <zl-util/ZLStream.h>

//================================================================//
// ZLByteStream
//================================================================//
class ZLByteStream :
	public ZLStream {
private:

	void*		mBuffer;
	size_t		mCursor;
	size_t		mLength;
	size_t		mCapacity;

	//----------------------------------------------------------------//
	int			SetCursor		( long offset );

public:

	//----------------------------------------------------------------//
	size_t		GetCapacity		();
	u32			GetCaps			();
	size_t		GetCursor		();
	void*		GetBuffer		();
	size_t		GetLength		();
	size_t		ReadBytes		( void* buffer, size_t size );
	void		SetBuffer		( void* buffer, size_t size, size_t length = 0 );
	void		SetLength		( size_t size );
	size_t		WriteBytes		( const void* buffer, size_t size );
				ZLByteStream	();
				~ZLByteStream	();
};

#endif

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USBYTESTREAM_H
#define USBYTESTREAM_H

#include <uslscore/USStream.h>

//================================================================//
// USByteStream
//================================================================//
class USByteStream :
	public USStream {
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
				USByteStream	();
				~USByteStream	();
	size_t		WriteBytes		( const void* buffer, size_t size );
};

#endif

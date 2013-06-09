// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLMEMSTREAM_H
#define ZLMEMSTREAM_H

#include <zl-util/STLString.h>
#include <zl-util/ZLStream.h>

//================================================================//
// ZLMemStream
//================================================================//
class ZLMemStream :
	public ZLStream {
private:

	void*		mGuestBuffer;
	size_t		mGuestBufferSize;

	size_t		mChunkSize;
	size_t		mTotalChunks;
	void**		mChunks;

	size_t		mCursor;
	size_t		mLength;

	//----------------------------------------------------------------//
	void			ClearChunks			();
	int				SetCursor			( long offset );

public:

	static const size_t DEFAULT_CHUNK_SIZE = 2048;

	//----------------------------------------------------------------//
	void			Clear				();
	u32				GetCaps				();
	size_t			GetCursor			();
	size_t			GetLength			();
	size_t			ReadBytes			( void* buffer, size_t size );
	void			Reserve				( size_t length );
	void			SetChunkSize		( size_t chunkSize );
	void			SetGuestBuffer		( void* guestBuffer, size_t guestBufferSize );
	size_t			WriteBytes			( const void* buffer, size_t size );
					ZLMemStream			();
					~ZLMemStream		();
};

#endif

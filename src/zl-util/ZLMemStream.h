// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef ZLMEMSTREAM_H
#define ZLMEMSTREAM_H

#include <zl-util/STLString.h>
#include <zl-util/ZLLeanPool.h>
#include <zl-util/ZLStream.h>

//================================================================//
// ZLMemStream
//================================================================//
class ZLMemStream :
	public virtual ZLStream {
private:

	// guest buffer is an optimization; handle case where we want to use some locally
	// allocated memory to read data of unknown size. if a write operation overflows the
	// guest buffer then its contents are copied into the main buffer and the guest buffer
	// is forgotten. usage would be to allocate a buffer on the stack, set it as the guest
	// buffer then write whatever is needed.
	void*		mGuestBuffer;
	size_t		mGuestBufferSize;

	size_t		mChunkSize;
	size_t		mTotalChunks;

	ZLLeanArray < void* > mChunks;

	size_t		mBase;			// offset into base chunk (in the event of DiscardFront); never larged than chunk size
	size_t		mCursor;		// current position in stream
	size_t		mLength;		// current length of stream

	//----------------------------------------------------------------//
	void			ClearChunks			();
	int				SetCursor			( long offset );

public:

	static const size_t DEFAULT_CHUNK_SIZE = 2048;

	//----------------------------------------------------------------//
	void			Clear				();
	void			DiscardAll			();
	void			DiscardBack			( size_t size );
	void			DiscardFront		( size_t size );
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

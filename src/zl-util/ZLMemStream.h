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
	// allocated memory to read or write data of unknown size. if a write operation overflows the
	// guest buffer then its contents are copied into the main buffer and the guest buffer
	// is forgotten. usage would be to allocate a buffer on the stack, set it as the guest
	// buffer then write whatever is needed.
	void*		mGuestBuffer;
	size_t		mGuestBufferSize;

	size_t		mChunkSize;

	ZLLeanArray < ZLLeanArray < u8 > > mChunks;

	size_t		mBase;			// offset into first chunk (in the event of DiscardFront); never larger than chunk size
	size_t		mCursor;		// current position in stream
	size_t		mLength;		// current length of stream

	//----------------------------------------------------------------//

public:

	static const size_t DEFAULT_CHUNK_SIZE = 2048;

	//----------------------------------------------------------------//
	void			Clear				();
	void			Compact				();
	void			DiscardAll			();
	void			DiscardBack			();
	void			DiscardBack			( size_t size );
	void			DiscardFront		();
	void			DiscardFront		( size_t size );
	u32				GetCaps				();
	size_t			GetCursor			();
	size_t			GetLength			();
	ZLSizeResult	ReadBytes			( void* buffer, size_t size );
	ZLResultCode	Reserve				( size_t length );
	void			SetChunkSize		( size_t chunkSize );
	ZLResultCode	SetCursor			( size_t offset );
	void			SetGuestBuffer		( void* guestBuffer, size_t guestBufferSize );
	ZLSizeResult	SetLength			( size_t length );
	ZLSizeResult	WriteBytes			( const void* buffer, size_t size );
					ZLMemStream			();
					~ZLMemStream		();
};

#endif

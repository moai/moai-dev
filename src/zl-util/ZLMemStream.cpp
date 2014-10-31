// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLFileSys.h>
#include <zl-util/ZLDirectoryItr.h>
#include <zl-util/ZLMemStream.h>

#include <math.h>

//================================================================//
// ZLMemStream
//================================================================//

//----------------------------------------------------------------//
void ZLMemStream::Clear () {

	this->ClearChunks ();

	this->mGuestBuffer = 0;
	this->mGuestBufferSize = 0;

	this->mCursor = 0;
	this->mLength = 0;
}

//----------------------------------------------------------------//
void ZLMemStream::DiscardAll () {

	this->DiscardFront ( this->mLength );
}

//----------------------------------------------------------------//
void ZLMemStream::DiscardBack ( size_t size ) {

	if ( !size ) return;

	if ( size >= this->mLength ) {
		this->mBase = 0;
		this->mCursor = 0;
		this->mLength = 0;
	}
	else {
		this->mLength -= size;
		this->mCursor = ( this->mCursor <= this->mLength ) ? this->mCursor : this->mLength;
	}
}

//----------------------------------------------------------------//
void ZLMemStream::DiscardFront ( size_t size ) {

	if ( !size ) return;

	if ( size >= this->mLength ) {
		this->mBase = 0;
		this->mCursor = 0;
		this->mLength = 0;
		return;
	}
	
	if ( !this->mGuestBuffer ) {
	
		// it's safe to discard any portion of the guest buffer
		this->mBase += size;
	}
	else {

		// no guest buffer; kill any chunks we don't need and set the base to whetever's left
		size_t chunks = ( size_t )( size / this->mChunkSize );
		this->mChunks.RotateLeft ( chunks );
		this->mBase = size - ( chunks * this->mChunkSize );
	}
	
	this->mCursor = ( size <= this->mCursor ) ? this->mCursor - size : 0;
	this->mLength -= size;
}

//----------------------------------------------------------------//
void ZLMemStream::ClearChunks () {

	if ( this->mChunks ) {
		for ( size_t i = 0; i < this->mTotalChunks; ++i ) {
			free ( this->mChunks [ i ]);
		}
		this->mChunks.Clear ();
		this->mTotalChunks = 0;
	}
}

//----------------------------------------------------------------//
u32 ZLMemStream::GetCaps () {

	return CAN_READ | CAN_WRITE | CAN_SEEK;
}

//----------------------------------------------------------------//
size_t ZLMemStream::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
size_t ZLMemStream::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
size_t ZLMemStream::ReadBytes ( void* buffer, size_t size ) {

	size_t cursor0 = this->mBase + this->mCursor;
	size_t cursor1 = cursor0 + size;
	size_t top = this->mBase + this->mLength;

	if ( cursor1 > top ) {
		size = top - cursor0;
		cursor1 = top;
	}

	if ( size == 0 ) return 0;

	// if there's a guest buffer, use it
	if ( this->mGuestBuffer ) {
		memcpy ( buffer, &(( u8* )this->mGuestBuffer )[ cursor0 ], size );
		this->mCursor += size;
		return size;
	}

	assert ( this->mChunks );

	size_t chunk0 = ( size_t )( cursor0 / this->mChunkSize );
	size_t chunk1 = ( size_t )( cursor1 / this->mChunkSize );

	size_t offset0 = cursor0 - ( chunk0 * this->mChunkSize );
	size_t offset1 = cursor1 - ( chunk1 * this->mChunkSize );

	void* src = ( void* )(( size_t )this->mChunks [ chunk0 ] + offset0 );
	void* dest = buffer;

	if ( chunk0 == chunk1 ) {
		
		memcpy ( dest, src, offset1 - offset0 );
	}
	else {
		
		memcpy ( dest, src, this->mChunkSize - offset0 );
		dest = ( void* )(( size_t )dest + this->mChunkSize - offset0 );
		
		for ( size_t i = ( chunk0 + 1 ); i < chunk1; ++i ) {
			memcpy ( dest, this->mChunks [ i ], this->mChunkSize );
			dest = ( void* )(( size_t )dest + this->mChunkSize );
		}
		memcpy ( dest, this->mChunks [ chunk1 ], offset1 );
	}

	this->mCursor = cursor1 - this->mBase;
	return size;
}

//----------------------------------------------------------------//
void ZLMemStream::Reserve ( size_t length ) {

	// already have some buffer large enough to accomodate length, so bail
	if ( length <= this->mLength ) return;
	
	if ( this->mGuestBuffer ) {
	
		// guest buffer can accomodate new length even if there's an offset, so bail
		if ( length <= ( this->mGuestBufferSize - this->mBase )) return;
		
		if ( length <= this->mGuestBufferSize ) {
			// guest buffer can accomodate new length but only if there's no offset, so shift the contents of the guest buffer and bail	
			memmove ( this->mGuestBuffer, ( void* )(( size_t )this->mGuestBuffer + this->mBase ), this->mLength );
			this->mBase = 0;
			return;
		}
		else {
			// pop the guest buffer
			this->SetGuestBuffer ( 0, 0 );
		}
	}

	size_t top = this->mBase + length;

	size_t totalChunks = ( top / this->mChunkSize ) + 1;
	if ( totalChunks <= this->mTotalChunks ) return;
	
	this->mChunks.Grow ( totalChunks );
	
	for ( size_t i = this->mTotalChunks; i < totalChunks; ++i ) {
		this->mChunks [ i ] = malloc ( this->mChunkSize );
	}
	
	this->mTotalChunks = totalChunks;
}

//----------------------------------------------------------------//
void ZLMemStream::SetChunkSize ( size_t chunkSize ) {

	assert ( chunkSize );
	this->Clear ();
	this->mChunkSize = chunkSize;
}

//----------------------------------------------------------------//
int ZLMemStream::SetCursor ( long offset ) {

	this->mCursor = offset > 0 ? offset : 0;
	return 0;
}

//----------------------------------------------------------------//
void ZLMemStream::SetGuestBuffer ( void* guestBuffer, size_t guestBufferSize ) {

	// if guest buffer will not be large enough to accomodate contents of stream...
	if ( guestBufferSize < this->mLength ) {
	
		// clear these out
		this->mGuestBuffer = 0;
		this->mGuestBufferSize = 0;
	
		// copy contents of the old guest buffer to chunks
		if ( this->mGuestBuffer ) {
			void* buffer = this->mGuestBuffer;
			this->WriteBytes ( buffer, this->mLength );
		}
	}
	else {
	
		// this will also copy from the existing guest buffer, if any
		this->ReadBytes ( guestBuffer, this->mLength );
		
		// don't need chunks (if any)
		this->ClearChunks ();
		
		this->mGuestBuffer = guestBuffer;
		this->mGuestBufferSize = guestBufferSize;
	}
}

//----------------------------------------------------------------//
size_t ZLMemStream::WriteBytes ( const void* buffer, size_t size ) {

	if ( !size ) return 0;

	this->Reserve ( this->mCursor + size );

	size_t cursor0 = this->mBase + this->mCursor;
	size_t cursor1 = cursor0 + size;

	if( this->mGuestBuffer ) {
		memcpy ( &(( u8* )this->mGuestBuffer )[ cursor0 ], buffer, size );
		this->mCursor += size;
		if ( this->mLength < this->mCursor ) {
			this->mLength = this->mCursor;
		}
		return size;
	}

	size_t chunk0 = ( size_t )( cursor0 / this->mChunkSize );
	size_t chunk1 = ( size_t )( cursor1 / this->mChunkSize );

	size_t offset0 = cursor0 - ( chunk0 * this->mChunkSize );
	size_t offset1 = cursor1 - ( chunk1 * this->mChunkSize );

	void* dest = ( void* )(( size_t )this->mChunks [ chunk0 ] + offset0 );
	const void* src = buffer;

	if ( chunk0 == chunk1 ) {
	
		memcpy ( dest, src, offset1 - offset0 );
	}
	else {
		
		memcpy ( dest, src, this->mChunkSize - offset0 );
		src = ( void* )(( size_t )src + this->mChunkSize - offset0 );
		
		for ( size_t i = ( chunk0 + 1 ); i < chunk1; ++i ) {
			memcpy ( this->mChunks [ i ], src, this->mChunkSize );
			src = ( void* )(( size_t )src + this->mChunkSize );
		}
		memcpy ( this->mChunks [ chunk1 ], src, offset1 );
	}

	this->mCursor = cursor1 - this->mBase;
	if ( this->mLength < this->mCursor ) {
		this->mLength = this->mCursor;
	}
	return size;
}

//----------------------------------------------------------------//
ZLMemStream::ZLMemStream () :
	mGuestBuffer ( 0 ),
	mGuestBufferSize ( 0 ),
	mChunkSize ( DEFAULT_CHUNK_SIZE ),
	mTotalChunks ( 0 ),
	mBase ( 0 ),
	mCursor ( 0 ),
	mLength ( 0 ) {
}

//----------------------------------------------------------------//
ZLMemStream::~ZLMemStream () {
	this->Clear ();
}

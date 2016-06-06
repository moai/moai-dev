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

	this->mChunks.Clear ();

	this->mGuestBuffer = 0;
	this->mGuestBufferSize = 0;

	this->mCursor = 0;
	this->mLength = 0;
}

//----------------------------------------------------------------//
void ZLMemStream::Compact () {

	size_t totalChunks = this->mChunks.Size ();

	size_t chunks = ( size_t )( this->mLength / this->mChunkSize ) + 1;
	if ( chunks < totalChunks ) {
		
		ZLLeanArray < ZLLeanArray < u8 > > temp;
		temp.Init ( chunks );
		
		for ( size_t i = 0; i < chunks; ++i ) {
			temp [ i ].Take ( this->mChunks [ i ]);
		}
		this->mChunks.Take ( temp );
	}
}

//----------------------------------------------------------------//
void ZLMemStream::DiscardAll () {

	this->DiscardFront ( this->mLength );
}

//----------------------------------------------------------------//
void ZLMemStream::DiscardBack () {

	this->DiscardBack ( this->mLength - this->mCursor );
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
void ZLMemStream::DiscardFront () {

	this->DiscardFront ( this->mCursor );
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
	
	if ( this->mGuestBuffer ) {
	
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
u32 ZLMemStream::GetCaps () {

	return CAN_READ | CAN_WRITE | CAN_SEEK | CAN_TRUNC;
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
ZLSizeResult ZLMemStream::ReadBytes ( void* buffer, size_t size ) {

	if ( size == 0 ) ZL_RETURN_SIZE_RESULT ( 0, ZL_OK );

	size_t cursor0 = this->mBase + this->mCursor;
	size_t cursor1 = cursor0 + size;
	size_t top = this->mBase + this->mLength;

	if ( cursor1 > top ) {
		size = top - cursor0;
		cursor1 = top;
	}

	if ( cursor0 == cursor1 ) ZL_RETURN_SIZE_RESULT ( 0, ZL_OK );

	// if there's a guest buffer, use it
	// if the guest buffer exists, it is guaranteed to be a large enough size
	if ( this->mGuestBuffer ) {
		memcpy ( buffer, &(( u8* )this->mGuestBuffer )[ cursor0 ], size );
		this->mCursor += size;
	}
	else {
	
		assert ( this->mChunks );

		size_t chunk0 = ( size_t )( cursor0 / this->mChunkSize );
		size_t chunk1 = ( size_t )( cursor1 / this->mChunkSize );

		size_t offset0 = cursor0 - ( chunk0 * this->mChunkSize );
		size_t offset1 = cursor1 - ( chunk1 * this->mChunkSize );

		void* src = ( void* )(( size_t )this->mChunks [ chunk0 ].Data () + offset0 );
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
	}
	
	ZL_RETURN_SIZE_RESULT ( size, ZL_OK );
}

//----------------------------------------------------------------//
ZLResultCode ZLMemStream::Reserve ( size_t length ) {

	size_t totalChunks = this->mChunks.Size ();

	// already have some buffer large enough to accomodate length, so bail
	if ( length <= this->mLength ) return ZL_OK;
	
	if ( this->mGuestBuffer ) {
	
		// guest buffer can accomodate new length even if there's an offset, so bail
		if ( length <= ( this->mGuestBufferSize - this->mBase )) return ZL_OK;
		
		if ( length <= this->mGuestBufferSize ) {
			// guest buffer can accomodate new length but only if there's no offset, so shift the contents of the guest buffer and bail	
			memmove ( this->mGuestBuffer, ( void* )(( size_t )this->mGuestBuffer + this->mBase ), this->mLength );
			this->mBase = 0;
			return ZL_OK;
		}
	}

	size_t top = this->mBase + length;

	size_t neededChunks = ( top / this->mChunkSize ) + 1;
	if ( neededChunks <= totalChunks ) return ZL_OK;
	
	ZLLeanArray < ZLLeanArray < u8 > > temp;
	if ( temp.Init ( neededChunks ) != ZL_OK ) return ZL_ALLOCATION_ERROR;
	
	for ( size_t i = totalChunks; i < neededChunks; ++i ) {
		if ( temp [ i ].Init ( this->mChunkSize )) return ZL_ALLOCATION_ERROR;
	}
	
	// we made it this far, so we're through the woods: no more allocations
	// it is now OK to mutate internal state
	
	for ( size_t i = 0; i < totalChunks; ++i ) {
		temp [ i ].Take ( this->mChunks [ i ]);
	}
	
	this->SetGuestBuffer ( 0, 0 ); // won't need the guest buffer any more
	
	this->mChunks.Take ( temp );
	return ZL_OK;
}

//----------------------------------------------------------------//
void ZLMemStream::SetChunkSize ( size_t chunkSize ) {

	assert ( chunkSize );
	this->Clear ();
	this->mChunkSize = chunkSize;
}

//----------------------------------------------------------------//
ZLResultCode ZLMemStream::SetCursor ( size_t offset ) {

	if (( offset < 0 ) || ( this->mLength < offset )) return ZL_ERROR;

	this->mCursor = offset;
	return ZL_OK;
}

//----------------------------------------------------------------//
void ZLMemStream::SetGuestBuffer ( void* guestBuffer, size_t guestBufferSize ) {

	// if guest buffer will not be large enough to accomodate contents of stream...
	if ( guestBufferSize < this->mLength ) {
	
		// copy contents of the old guest buffer to chunks
		if ( this->mGuestBuffer ) {
		
			void* buffer = this->mGuestBuffer;
			
			// clear these out
			this->mGuestBuffer = 0;
			this->mGuestBufferSize = 0;
			
			this->WriteBytes ( buffer, this->mLength );
		}
	}
	else {
	
		if ( guestBuffer ) {
	
			// this will also copy from the existing guest buffer, if any
			this->ReadBytes ( guestBuffer, this->mLength );
			
			// don't need chunks (if any)
			this->mChunks.Clear ();
		}
		this->mGuestBuffer = guestBuffer;
		this->mGuestBufferSize = guestBufferSize;
	}
}

//----------------------------------------------------------------//
ZLSizeResult ZLMemStream::SetLength ( size_t length ) {

	if ( length < this->mLength ) {
		this->DiscardBack ( this->mLength - length );
	}
	else {
		if ( this->Reserve ( length ) != ZL_OK ) ZL_RETURN_SIZE_RESULT  ( 0, ZL_ALLOCATION_ERROR );
		this->mLength = length;
	}
	ZL_RETURN_SIZE_RESULT  ( length, ZL_OK );
}

//----------------------------------------------------------------//
ZLSizeResult ZLMemStream::WriteBytes ( const void* buffer, size_t size ) {

	if ( !size ) ZL_RETURN_SIZE_RESULT ( 0, ZL_OK );

	if ( this->Reserve ( this->mCursor + size ) != ZL_OK ) ZL_RETURN_SIZE_RESULT ( 0, ZL_ALLOCATION_ERROR );

	size_t cursor0 = this->mBase + this->mCursor;
	size_t cursor1 = cursor0 + size;

	if( this->mGuestBuffer ) {
		memcpy ( &(( u8* )this->mGuestBuffer )[ cursor0 ], buffer, size );
		this->mCursor += size;
		if ( this->mLength < this->mCursor ) {
			this->mLength = this->mCursor;
		}
	}
	else {

		size_t chunk0 = ( size_t )( cursor0 / this->mChunkSize );
		size_t chunk1 = ( size_t )( cursor1 / this->mChunkSize );

		size_t offset0 = cursor0 - ( chunk0 * this->mChunkSize );
		size_t offset1 = cursor1 - ( chunk1 * this->mChunkSize );

		void* dest = ( void* )(( size_t )this->mChunks [ chunk0 ].Data () + offset0 );
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
	}
	
	ZL_RETURN_SIZE_RESULT ( size, ZL_OK );
}

//----------------------------------------------------------------//
ZLMemStream::ZLMemStream () :
	mGuestBuffer ( 0 ),
	mGuestBufferSize ( 0 ),
	mChunkSize ( DEFAULT_CHUNK_SIZE ),
	mBase ( 0 ),
	mCursor ( 0 ),
	mLength ( 0 ) {
}

//----------------------------------------------------------------//
ZLMemStream::~ZLMemStream () {
	this->Clear ();
}

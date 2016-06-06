// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLCopyOnWrite.h>

//================================================================//
// ZLCopyOnWriteBuffer
//================================================================//

//----------------------------------------------------------------//
ZLCopyOnWriteBuffer::ZLCopyOnWriteBuffer () :
	mBuffer ( 0 ),
	mSize ( 0 ),
	mLength ( 0 ) {
}

//----------------------------------------------------------------//
ZLCopyOnWriteBuffer::~ZLCopyOnWriteBuffer () {

	if ( this->mBuffer ) {
		free ( this->mBuffer );
	}
}

//================================================================//
// ZLCopyOnWrite
//================================================================//

//----------------------------------------------------------------//
void* ZLCopyOnWrite::Alloc ( size_t size ) {

	this->Free ();
	if ( !size ) return 0;
	
	if ( !this->mInternal ) {
	
		this->mInternal = new ZLCopyOnWriteBuffer ();
		this->mInternal->Retain ();
	}
	
	void* buffer = malloc ( size );
	
	if ( buffer ) {
		this->mInternal->mBuffer = buffer;
		this->mInternal->mSize = size;
		this->mInternal->mLength = size;
	}
	return buffer;
}

//----------------------------------------------------------------//
void* ZLCopyOnWrite::Alloc ( size_t size, u8 fill ) {

	void* buffer = this->Alloc ( size );
	if ( buffer ) {
		memset ( buffer, fill, size );
	}
	return buffer;
}

//----------------------------------------------------------------//
void* ZLCopyOnWrite::Alloc ( size_t size, const void* fill ) {

	void* buffer = this->Alloc ( size );
	if ( buffer ) {
		memcpy ( buffer, fill, size );
	}
	return buffer;
}

//----------------------------------------------------------------//
void ZLCopyOnWrite::Assign ( const ZLCopyOnWrite& assign ) {
	
	if ( this->mInternal != assign.mInternal ) {
	
		this->Free ();
		
		this->mInternal = assign.mInternal;
		
		if ( this->mInternal ) {
			this->mInternal->Retain ();
		}
	}
	this->mCursor = assign.mCursor;
}

//----------------------------------------------------------------//
void ZLCopyOnWrite::Free () {

	this->mCursor = 0;

	if ( this->mInternal ) {
		this->mInternal->Release ();
		this->mInternal = 0;
	}
}

//----------------------------------------------------------------//
const void* ZLCopyOnWrite::GetBuffer () const {

	return this->mInternal ? this->mInternal->mBuffer : 0;
}

//----------------------------------------------------------------//
u32 ZLCopyOnWrite::GetCaps () {

	return CAN_READ | CAN_WRITE | CAN_SEEK;
}

//----------------------------------------------------------------//
size_t ZLCopyOnWrite::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
size_t ZLCopyOnWrite::GetLength () {

	return this->mInternal ? this->mInternal->mLength : 0;
}

//----------------------------------------------------------------//
size_t ZLCopyOnWrite::GetSize () const {

	return this->mInternal ? this->mInternal->mSize : 0;
}

//----------------------------------------------------------------//
void* ZLCopyOnWrite::Invalidate () {
	
	// if the internal buffer has been referenced by more than one client,
	// we're going to abandon it and create a new internal buffer
	
	if (( this->mInternal ) && ( this->mInternal->GetRefCount () > 1 )) {
	
		// original buffer
		ZLCopyOnWriteBuffer* original = this->mInternal;
		
		// new buffer to hold locally
		this->mInternal = new ( ZLCopyOnWriteBuffer );
		this->mInternal->Retain ();
	
		// grab the original buffer
		this->mInternal->mBuffer	= malloc ( original->mSize );
		this->mInternal->mSize		= original->mSize;
		this->mInternal->mLength	= original->mLength;
		
		// just copy the length (as we don't need the entire buffer)
		assert ( this->mInternal->mBuffer );
		memcpy ( this->mInternal->mBuffer, original->mBuffer, original->mLength );
		
		// one less client for the original
		original->Release ();
	}
	return this->mInternal ? this->mInternal->mBuffer : 0;
}

//----------------------------------------------------------------//
ZLSizeResult ZLCopyOnWrite::ReadBytes ( void* buffer, size_t size ) {

	ZLCopyOnWriteBuffer* internal = this->mInternal;
	size_t readSize = 0;
	
	if ( internal ) {

		readSize = (( this->mCursor + size ) > internal->mLength ) ? ( internal->mLength - this->mCursor ) : size;

		if ( readSize ) {
			memcpy ( buffer, ( const void* )(( size_t )internal->mBuffer + this->mCursor ), readSize );
			this->mCursor += readSize;
		}
	}
	ZL_RETURN_SIZE_RESULT ( readSize, ZL_OK );
}

//----------------------------------------------------------------//
void* ZLCopyOnWrite::Reserve ( size_t size ) {

	void* buffer = this->Alloc ( size );
	this->SetLength ( size );
	return buffer;
}

//----------------------------------------------------------------//
ZLResultCode ZLCopyOnWrite::SetCursor ( size_t offset ) {

	size_t length = this->GetLength ();

	if (( offset < 0 ) || ( length < ( size_t )offset )) return ZL_ERROR;
	
	this->mCursor = offset;
	return 0;
}

//----------------------------------------------------------------//
ZLSizeResult ZLCopyOnWrite::SetLength ( size_t length ) {

	ZLCopyOnWriteBuffer* internal = this->mInternal;
	
	if ( internal ) {

		length = length > internal->mLength ? internal->mLength : length;
		
		if ( length< internal->mLength ) {
			this->Invalidate ();
		}
		internal->mLength = length;
		
		if ( this->mCursor > length ) {
			this->mCursor = length;
		}
		ZL_RETURN_SIZE_RESULT ( length, ZL_OK );
	}
	ZL_RETURN_SIZE_RESULT ( 0, ZL_ERROR );
}

//----------------------------------------------------------------//
ZLSizeResult ZLCopyOnWrite::WriteBytes ( const void* buffer, size_t size ) {

	ZLCopyOnWriteBuffer* internal = this->mInternal;
	size_t writeSize = 0;
	
	if ( internal ) {

		if ( this->mCursor < internal->mLength ) {
			this->Invalidate ();
		}

		writeSize = (( this->mCursor + size ) > internal->mSize ) ? ( internal->mSize - this->mCursor ) : size;

		if ( writeSize ) {
			memcpy (( void* )(( size_t )internal->mBuffer + this->mCursor ), buffer, writeSize );
			this->mCursor += writeSize;
			if ( internal->mLength < this->mCursor ) {
				internal->mLength = this->mCursor;
			}
		}
	}
	ZL_RETURN_SIZE_RESULT ( writeSize, ZL_OK );
}

//----------------------------------------------------------------//
ZLCopyOnWrite::ZLCopyOnWrite () :
	mInternal ( 0 ),
	mCursor ( 0 ) {
}

//----------------------------------------------------------------//
ZLCopyOnWrite::ZLCopyOnWrite ( size_t size ) :
	mInternal ( 0 ),
	mCursor ( 0 ) {
	
	this->Alloc ( size );
}

//----------------------------------------------------------------//
ZLCopyOnWrite::ZLCopyOnWrite ( size_t size, u8 fill ) :
	mInternal ( 0 ),
	mCursor ( 0 ) {
	
	this->Alloc ( size, fill );
}

//----------------------------------------------------------------//
ZLCopyOnWrite::ZLCopyOnWrite ( size_t size, const void* fill ) :
	mInternal ( 0 ),
	mCursor ( 0 ) {
	
	this->Alloc ( size, fill );
}

//----------------------------------------------------------------//
ZLCopyOnWrite::ZLCopyOnWrite ( const ZLCopyOnWrite& copy ) :
	mInternal ( 0 ),
	mCursor ( 0 ) {
	
	this->Assign ( copy );
}

//----------------------------------------------------------------//
ZLCopyOnWrite::~ZLCopyOnWrite () {

	this->Free ();
}

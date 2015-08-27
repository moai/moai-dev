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
void ZLCopyOnWrite::AffirmInternal () {

	if ( !this->mInternal ) {
		this->mInternal = new ZLCopyOnWriteBuffer ();
		this->mInternal->Retain ();
	}
}

//----------------------------------------------------------------//
void* ZLCopyOnWrite::Alloc ( size_t size ) {

	this->Free ();
	if ( !size ) return 0;
	
	this->AffirmInternal ();
	
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

	this->Free ();
	if ( !size ) return 0;
	
	this->AffirmInternal ();
	
	void* buffer = fill == 0 ? calloc ( 1, size ) : malloc ( size );
	
	if ( buffer ) {
	
		if ( fill != 0 ) {
			memset ( buffer, fill, size );
		}
	
		this->mInternal->mBuffer = buffer;
		this->mInternal->mSize = size;
		this->mInternal->mLength = size;
	}
	return buffer;
}

//----------------------------------------------------------------//
void* ZLCopyOnWrite::Alloc ( size_t size, const void* fill ) {

	this->Free ();
	if ( !size ) return 0;
	
	this->AffirmInternal ();
	
	void* buffer = fill == 0 ? calloc ( 1, size ) : malloc ( size );
	
	if ( buffer ) {
	
		if ( fill != 0 ) {
			memcpy ( buffer, fill, size );
		}
	
		this->mInternal->mBuffer = buffer;
		this->mInternal->mSize = size;
		this->mInternal->mLength = size;
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

	ZLCopyOnWriteBuffer* internal = this->mInternal;
	
	if (( internal ) && ( internal->GetRefCount () > 1 )) {
	
		this->mInternal = new ZLCopyOnWriteBuffer ();
		this->mInternal->Retain ();
	
		this->mInternal->mBuffer	= internal->mBuffer;
		this->mInternal->mSize		= internal->mSize;
		this->mInternal->mLength	= internal->mLength;
	
		internal->Release ();
		
		internal->mSize = internal->mLength;
		internal->mBuffer = malloc ( internal->mLength );
		
		assert ( internal->mBuffer );
	
		memcpy ( internal->mBuffer, this->mInternal->mBuffer, internal->mLength );
	}
	return this->mInternal ? this->mInternal->mBuffer : 0;
}

//----------------------------------------------------------------//
size_t ZLCopyOnWrite::ReadBytes ( void* buffer, size_t size ) {

	ZLCopyOnWriteBuffer* internal = this->mInternal;
	
	if ( internal ) {

		if (( this->mCursor + size ) > internal->mLength ) {
			size = internal->mLength - this->mCursor;
		}

		if ( size ) {
			memcpy ( buffer, ( const void* )(( size_t )internal->mBuffer + this->mCursor ), size );
			this->mCursor += size;
		}
		return size;
	}
	return 0;
}

//----------------------------------------------------------------//
void* ZLCopyOnWrite::Reserve ( size_t size ) {

	void* buffer = this->Alloc ( size );
	this->SetLength ( size );
	return buffer;
}

//----------------------------------------------------------------//
int ZLCopyOnWrite::SetCursor ( long offset ) {

	this->mCursor = offset > 0 ? offset : 0;
	
	size_t length = this->GetLength ();
	if ( this->mCursor > length ) {
		this->mCursor = length;
	}
	return 0;
}

//----------------------------------------------------------------//
size_t ZLCopyOnWrite::SetLength ( size_t length ) {

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
		return length;
	}
	return 0;
}

//----------------------------------------------------------------//
size_t ZLCopyOnWrite::WriteBytes ( const void* buffer, size_t size ) {

	ZLCopyOnWriteBuffer* internal = this->mInternal;
	
	if ( internal ) {

		if ( this->mCursor < internal->mLength ) {
			this->Invalidate ();
		}

		if (( this->mCursor + size ) > internal->mSize ) {
			size = internal->mSize - this->mCursor;
		}

		if ( size ) {
			memcpy (( void* )(( size_t )internal->mBuffer + this->mCursor ), buffer, size );
			this->mCursor += size;
			if ( internal->mLength < this->mCursor ) {
				internal->mLength = this->mCursor;
			}
			return size;
		}
	}
	return 0;
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

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLCowBuffer.h>

//================================================================//
// ZLCowBufferInternal
//================================================================//

//----------------------------------------------------------------//
ZLCowBufferInternal::ZLCowBufferInternal () :
	mBuffer ( 0 ),
	mSize ( 0 ) {
}

//----------------------------------------------------------------//
ZLCowBufferInternal::~ZLCowBufferInternal () {

	if ( this->mBuffer ) {
		free ( this->mBuffer );
	}
}

//================================================================//
// ZLCowBuffer
//================================================================//

//----------------------------------------------------------------//
void ZLCowBuffer::AffirmInternal () {

	if ( !this->mInternal ) {
		this->mInternal = new ZLCowBufferInternal ();
		this->mInternal->Retain ();
	}
}

//----------------------------------------------------------------//
void* ZLCowBuffer::Alloc ( size_t size ) {

	this->Free ();
	if ( !size ) return 0;
	
	this->AffirmInternal ();
	
	void* buffer = malloc ( size );
	
	if ( buffer ) {
		this->mInternal->mBuffer = buffer;
		this->mInternal->mSize = size;
	}
	return buffer;
}

//----------------------------------------------------------------//
void* ZLCowBuffer::Alloc ( size_t size, u8 fill ) {

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
	}
	return buffer;
}

//----------------------------------------------------------------//
void* ZLCowBuffer::Alloc ( size_t size, const void* fill ) {

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
	}
	return buffer;
}

//----------------------------------------------------------------//
void ZLCowBuffer::Assign ( const ZLCowBuffer& assign ) {

	if ( this->mInternal == assign.mInternal ) return;

	this->Free ();
	this->mInternal = assign.mInternal;
	if ( this->mInternal ) {
		this->mInternal->Retain ();
	}
}

//----------------------------------------------------------------//
void ZLCowBuffer::Free () {

	if ( this->mInternal ) {
		this->mInternal->Release ();
		this->mInternal = 0;
	}
}

//----------------------------------------------------------------//
const void* ZLCowBuffer::GetBuffer () const {

	return this->mInternal ? this->mInternal->mBuffer : 0;
}

//----------------------------------------------------------------//
void* ZLCowBuffer::GetBufferMutable () {

	ZLCowBufferInternal* internal = this->mInternal;

	if ( internal ) {
		if ( internal->GetRefCount () > 1 ) {
			this->Free ();
			this->Alloc ( internal->mSize, internal->mBuffer );
		}
	}
	return this->mInternal ? this->mInternal->mBuffer : 0;
}

//----------------------------------------------------------------//
size_t ZLCowBuffer::GetSize () const {

	return this->mInternal ? this->mInternal->mSize : 0;
}

//----------------------------------------------------------------//
ZLCowBuffer::ZLCowBuffer () :
	mInternal ( 0 ) {
}

//----------------------------------------------------------------//
ZLCowBuffer::ZLCowBuffer ( size_t size ) :
	mInternal ( 0 ) {
	
	this->Alloc ( size );
}

//----------------------------------------------------------------//
ZLCowBuffer::ZLCowBuffer ( size_t size, u8 fill ) :
	mInternal ( 0 ) {
	
	this->Alloc ( size, fill );
}

//----------------------------------------------------------------//
ZLCowBuffer::ZLCowBuffer ( size_t size, const void* fill ) :
	mInternal ( 0 ) {
	
	this->Alloc ( size, fill );
}

//----------------------------------------------------------------//
ZLCowBuffer::ZLCowBuffer ( const ZLCowBuffer& copy ) :
	mInternal ( 0 ) {
	
	this->Assign ( copy );
}

//----------------------------------------------------------------//
ZLCowBuffer::~ZLCowBuffer () {

	this->Free ();
}

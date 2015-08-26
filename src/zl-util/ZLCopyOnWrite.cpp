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
	mSize ( 0 ) {
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
	}
	return buffer;
}

//----------------------------------------------------------------//
void ZLCopyOnWrite::Assign ( const ZLCopyOnWrite& assign ) {

	if ( this->mInternal == assign.mInternal ) return;

	this->Free ();
	this->mInternal = assign.mInternal;
	if ( this->mInternal ) {
		this->mInternal->Retain ();
	}
}

//----------------------------------------------------------------//
void ZLCopyOnWrite::Free () {

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
void* ZLCopyOnWrite::GetBufferMutable () {

	ZLCopyOnWriteBuffer* internal = this->mInternal;

	if ( internal ) {
		if ( internal->GetRefCount () > 1 ) {
			this->Free ();
			this->Alloc ( internal->mSize, internal->mBuffer );
		}
	}
	return this->mInternal ? this->mInternal->mBuffer : 0;
}

//----------------------------------------------------------------//
size_t ZLCopyOnWrite::GetSize () const {

	return this->mInternal ? this->mInternal->mSize : 0;
}

//----------------------------------------------------------------//
ZLCopyOnWrite::ZLCopyOnWrite () :
	mInternal ( 0 ) {
}

//----------------------------------------------------------------//
ZLCopyOnWrite::ZLCopyOnWrite ( size_t size ) :
	mInternal ( 0 ) {
	
	this->Alloc ( size );
}

//----------------------------------------------------------------//
ZLCopyOnWrite::ZLCopyOnWrite ( size_t size, u8 fill ) :
	mInternal ( 0 ) {
	
	this->Alloc ( size, fill );
}

//----------------------------------------------------------------//
ZLCopyOnWrite::ZLCopyOnWrite ( size_t size, const void* fill ) :
	mInternal ( 0 ) {
	
	this->Alloc ( size, fill );
}

//----------------------------------------------------------------//
ZLCopyOnWrite::ZLCopyOnWrite ( const ZLCopyOnWrite& copy ) :
	mInternal ( 0 ) {
	
	this->Assign ( copy );
}

//----------------------------------------------------------------//
ZLCopyOnWrite::~ZLCopyOnWrite () {

	this->Free ();
}

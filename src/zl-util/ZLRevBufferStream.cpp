// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLRevBufferStream.h>

//================================================================//
// ZLRevBufferEdition
//================================================================//

//----------------------------------------------------------------//
ZLRevBufferEdition::ZLRevBufferEdition () :
	mData ( 0 ),
	mSize ( 0 ),
	mAllocator ( 0 ) {
}

//----------------------------------------------------------------//
ZLRevBufferEdition::~ZLRevBufferEdition () {

	if ( this->mData ) {
		free ( this->mData );
	}
}

//================================================================//
// ZLRevBufferStream
//================================================================//

//----------------------------------------------------------------//
void ZLRevBufferStream::Clear () {

	if ( this->mEdition ) {
		this->mEdition->Release ();
	}
	this->mEdition = 0;
}

//----------------------------------------------------------------//
size_t ZLRevBufferStream::GetCapacity () {

	return this->GetSize ();
}

//----------------------------------------------------------------//
u32 ZLRevBufferStream::GetCaps () {

	return CAN_READ | CAN_WRITE | CAN_SEEK | CAN_TRUNC;
}

//----------------------------------------------------------------//
size_t ZLRevBufferStream::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
void* ZLRevBufferStream::GetData () {

	return this->mEdition ? this->mEdition->mData : 0;
}

//----------------------------------------------------------------//
ZLRevBufferEdition* ZLRevBufferStream::GetEdition ( ZLRevBufferAllocator* allocator ) {

	return this->mEdition;
}

//----------------------------------------------------------------//
size_t ZLRevBufferStream::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
void ZLRevBufferStream::MakeDirty () {

	if ( this->mEdition->GetRefCount () > 1 ) {
	
		ZLRevBufferEdition* prevEdition = this->mEdition;
		prevEdition->Release ();
		
		this->mEdition = new ZLRevBufferEdition ();
		this->mEdition->Retain ();
	
		this->mEdition->mSize = this->mSize;
		this->mEdition->mData = prevEdition->mData;
		
		prevEdition->mSize = this->mEditionLength;
		prevEdition->mData = malloc ( prevEdition->mSize );
		memcpy ( prevEdition->mData, this->mEdition->mData, prevEdition->mSize );
		
		this->mEditionLength = this->mLength;
	}
}

//----------------------------------------------------------------//
size_t ZLRevBufferStream::ReadBytes ( void* buffer, size_t size ) {

	void* data = this->GetData ();

	if (( this->mCursor + size ) > this->mLength ) {
		size = this->mLength - this->mCursor;
	}

	if ( size ) {
		memcpy ( buffer, ( const void* )(( size_t )data + this->mCursor ), size );
		this->mCursor += size;
	}
	
	return size;
}

//----------------------------------------------------------------//
void ZLRevBufferStream::Reserve ( size_t size ) {

	this->Clear ();
	this->mSize = size;
	
	this->mEdition = new ZLRevBufferEdition ();
	this->mEdition->Retain ();
	
	this->mEdition->mSize = size;
	this->mEdition->mData = malloc ( size );
}

//----------------------------------------------------------------//
void ZLRevBufferStream::Reset () {

	this->mCursor = 0;
	this->mLength = 0;
}

//----------------------------------------------------------------//
int ZLRevBufferStream::SetCursor ( long offset ) {

	this->mCursor = offset > 0 ? offset : 0;
	return 0;
}

//----------------------------------------------------------------//
size_t ZLRevBufferStream::SetLength ( size_t length ) {

	length = length > this->mSize ? this->mSize : length;
	this->mLength = length;
	
	if ( length < this->mEditionLength ) {
		this->MakeDirty ();
	}
	this->mEditionLength = length;
	return length;
}

//----------------------------------------------------------------//
size_t ZLRevBufferStream::WriteBytes ( const void* buffer, size_t size ) {

	if ( this->mCursor < this->mEditionLength ) {
		this->MakeDirty ();
	}

	void* data = this->GetData ();

	if (( this->mCursor + size ) > this->mSize ) {
		size = this->mSize - this->mCursor;
	}

	if ( size ) {
		memcpy (( void* )(( size_t )data + this->mCursor ), buffer, size );
		this->mCursor += size;
		if ( this->mLength < this->mCursor ) {
			this->mLength = this->mCursor;
		}
		this->mEditionLength = this->mLength;
		return size;
	}
	return 0;
}

//----------------------------------------------------------------//
ZLRevBufferStream::ZLRevBufferStream () :
	mSize ( 0 ),
	mLength ( 0 ),
	mEditionLength ( 0 ),
	mEdition ( 0 ),
	mCursor ( 0 ) {
}

//----------------------------------------------------------------//
ZLRevBufferStream::~ZLRevBufferStream () {

	this->Clear ();
	
	if ( this->mEdition ) {
		this->mEdition->Release ();
		this->mEdition = 0;
	}
}

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLRevBufferStream.h>

//================================================================//
// ZLRevBufferStream
//================================================================//

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
size_t ZLRevBufferStream::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
size_t ZLRevBufferStream::ReadBytes ( void* buffer, size_t size ) {

	const void* immutableData = this->GetImmutableData ();

	if (( this->mCursor + size ) > this->mLength ) {
		size = this->mLength - this->mCursor;
	}

	if ( size ) {
		memcpy ( buffer, ( const void* )(( size_t )immutableData + this->mCursor ), size );
		this->mCursor += size;
	}
	
	return size;
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
	return length;
}

//----------------------------------------------------------------//
size_t ZLRevBufferStream::WriteBytes ( const void* buffer, size_t size ) {

	void* mutableData = this->GetMutableData ();

	if (( this->mCursor + size ) > this->mSize ) {
		size = this->mSize - this->mCursor;
	}

	if ( size ) {
		memcpy (( void* )(( size_t )mutableData + this->mCursor ), buffer, size );
		this->mCursor += size;
		if ( this->mLength < this->mCursor ) {
			this->mLength = this->mCursor;
		}
		return size;
	}
	return 0;
}

//----------------------------------------------------------------//
ZLRevBufferStream::ZLRevBufferStream () :
	mCursor ( 0 ),
	mLength ( 0 ) {
}

//----------------------------------------------------------------//
ZLRevBufferStream::~ZLRevBufferStream () {
}

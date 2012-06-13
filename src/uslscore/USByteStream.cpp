// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USFileSys.h>
#include <uslscore/USDirectoryItr.h>
#include <uslscore/USByteStream.h>
#include <stdio.h>

//================================================================//
// USByteStream
//================================================================//

//----------------------------------------------------------------//
void* USByteStream::GetBuffer () {

	return this->mBuffer;
}

//----------------------------------------------------------------//
size_t USByteStream::GetCapacity () {

	return this->mCapacity;
}

//----------------------------------------------------------------//
u32 USByteStream::GetCaps () {

	return this->mBuffer ? CAN_READ | CAN_WRITE | CAN_SEEK : 0;
}

//----------------------------------------------------------------//
size_t USByteStream::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
size_t USByteStream::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
size_t USByteStream::ReadBytes ( void* buffer, size_t size ) {

	if (( this->mCursor + size ) > this->mLength ) {
		size = this->mLength - this->mCursor;
	}

	if ( size ) {
		memcpy ( buffer, &(( u8* )this->mBuffer )[ this->mCursor ], size );
		this->mCursor += size;
	}
	
	return size;
}

//----------------------------------------------------------------//
void USByteStream::SetBuffer ( void* buffer, size_t size, size_t length ) {

	this->mCursor = 0;
	this->mLength = length;
	this->mCapacity = size;
	this->mBuffer = buffer;
}

//----------------------------------------------------------------//
int USByteStream::SetCursor ( long offset ) {

	this->mCursor = offset;
	return 0;
}

//----------------------------------------------------------------//
void USByteStream::SetLength ( size_t size ) {

	this->mLength = size;
}

//----------------------------------------------------------------//
USByteStream::USByteStream () :
	mBuffer ( 0 ),
	mCursor ( 0 ),
	mLength ( 0 ),
	mCapacity ( 0 ) {
}

//----------------------------------------------------------------//
USByteStream::~USByteStream () {
}

//----------------------------------------------------------------//
size_t USByteStream::WriteBytes ( const void* buffer, size_t size ) {

	if (( this->mCursor + size ) > this->mCapacity ) {
		size = this->mCapacity - this->mCursor;
	}

	if ( size ) {
		memcpy ( &(( u8* )this->mBuffer )[ this->mCursor ], buffer, size );
		this->mCursor += size;
		this->mLength += size;
		return size;
	}
	
	return 0;
}

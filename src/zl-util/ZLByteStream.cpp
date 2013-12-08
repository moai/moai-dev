// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLFileSys.h>
#include <zl-util/ZLDirectoryItr.h>
#include <zl-util/ZLByteStream.h>
#include <stdio.h>

//================================================================//
// ZLByteStream
//================================================================//

//----------------------------------------------------------------//
void* ZLByteStream::GetBuffer () {

	return this->mBuffer;
}

//----------------------------------------------------------------//
size_t ZLByteStream::GetCapacity () {

	return this->mCapacity;
}

//----------------------------------------------------------------//
u32 ZLByteStream::GetCaps () {

	return this->mBuffer ? CAN_READ | CAN_WRITE | CAN_SEEK : 0;
}

//----------------------------------------------------------------//
size_t ZLByteStream::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
size_t ZLByteStream::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
size_t ZLByteStream::ReadBytes ( void* buffer, size_t size ) {

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
void ZLByteStream::SetBuffer ( void* buffer, size_t size, size_t length ) {

	this->mCursor = 0;
	this->mLength = length;
	this->mCapacity = size;
	this->mBuffer = buffer;
}

//----------------------------------------------------------------//
int ZLByteStream::SetCursor ( long offset ) {

	this->mCursor = offset;
	return 0;
}

//----------------------------------------------------------------//
void ZLByteStream::SetLength ( size_t size ) {

	this->mLength = size;
}

//----------------------------------------------------------------//
size_t ZLByteStream::WriteBytes ( const void* buffer, size_t size ) {

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

//----------------------------------------------------------------//
ZLByteStream::ZLByteStream () :
	mBuffer ( 0 ),
	mCursor ( 0 ),
	mLength ( 0 ),
	mCapacity ( 0 ) {
}

//----------------------------------------------------------------//
ZLByteStream::~ZLByteStream () {
}

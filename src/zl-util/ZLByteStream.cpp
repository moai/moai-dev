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
void ZLByteStream::Clear () {

	this->mCursor = 0;
	this->mLength = 0;
	this->mCapacity = 0;
	this->mReadBuffer = 0;
	this->mWriteBuffer = 0;
}

//----------------------------------------------------------------//
size_t ZLByteStream::GetCapacity () {

	return this->mCapacity;
}

//----------------------------------------------------------------//
u32 ZLByteStream::GetCaps () {

	if ( this->mWriteBuffer ) return CAN_READ | CAN_WRITE | CAN_SEEK;
	if ( this->mReadBuffer ) return CAN_READ | CAN_SEEK;
	return 0;
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
const void* ZLByteStream::GetReadBuffer () {

	return this->mReadBuffer;
}

//----------------------------------------------------------------//
void* ZLByteStream::GetWriteBuffer () {

	return this->mWriteBuffer;
}

//----------------------------------------------------------------//
size_t ZLByteStream::ReadBytes ( void* buffer, size_t size ) {

	if (( this->mCursor + size ) > this->mLength ) {
		size = this->mLength - this->mCursor;
	}

	if ( size ) {
		memcpy ( buffer, &(( u8* )this->mReadBuffer )[ this->mCursor ], size );
		this->mCursor += size;
	}
	
	return size;
}

//----------------------------------------------------------------//
void ZLByteStream::SetBuffer ( void* buffer, size_t size, size_t length ) {

	this->SetBuffer (( const void* )buffer, size, length );
	this->mWriteBuffer = buffer;
}

//----------------------------------------------------------------//
void ZLByteStream::SetBuffer ( const void* buffer, size_t size, size_t length ) {

	this->mCursor = 0;
	this->mLength = length;
	this->mCapacity = size;
	this->mReadBuffer = buffer;
	this->mWriteBuffer = 0;
}

//----------------------------------------------------------------//
int ZLByteStream::SetCursor ( long offset ) {

	this->mCursor = offset > 0 ? offset : 0;
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
		memcpy ( &(( u8* )this->mWriteBuffer )[ this->mCursor ], buffer, size );
		this->mCursor += size;
		if ( this->mLength < this->mCursor ) {
			this->mLength = this->mCursor;
		}
		return size;
	}
	return 0;
}

//----------------------------------------------------------------//
ZLByteStream::ZLByteStream () :
	mReadBuffer ( 0 ),
	mWriteBuffer ( 0 ),
	mCursor ( 0 ),
	mLength ( 0 ),
	mCapacity ( 0 ) {
}

//----------------------------------------------------------------//
ZLByteStream::~ZLByteStream () {
}

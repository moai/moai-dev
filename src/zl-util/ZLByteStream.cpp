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

	if ( this->mWriteBuffer ) return CAN_READ | CAN_WRITE | CAN_SEEK | CAN_TRUNC;
	if ( this->mReadBuffer ) return CAN_READ | CAN_SEEK | CAN_TRUNC;
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
ZLSizeResult ZLByteStream::ReadBytes ( void* buffer, size_t size ) {

	size_t readSize = (( this->mCursor + size ) > this->mLength ) ? ( this->mLength - this->mCursor ) : size;

	if ( readSize ) {
		memcpy ( buffer, &(( u8* )this->mReadBuffer )[ this->mCursor ], readSize );
		this->mCursor += readSize;
	}
	ZL_RETURN_SIZE_RESULT ( readSize, ZL_OK );
}

//----------------------------------------------------------------//
void ZLByteStream::SetBuffer ( void* buffer, size_t size, size_t length ) {

	this->SetBuffer (( const void* )buffer, size, length );
	this->mWriteBuffer = buffer;
}

//----------------------------------------------------------------//
void ZLByteStream::SetBuffer ( const void* buffer, size_t size, size_t length ) {

	length = length > size ? size : length;

	this->mCursor = 0;
	this->mLength = length;
	this->mCapacity = size;
	this->mReadBuffer = buffer;
	this->mWriteBuffer = 0;
}

//----------------------------------------------------------------//
ZLResultCode ZLByteStream::SetCursor ( size_t offset ) {

	if ( this->mLength < offset ) return ZL_ERROR;

	this->mCursor = offset;
	return ZL_OK;
}

//----------------------------------------------------------------//
ZLSizeResult ZLByteStream::SetLength ( size_t length ) {

	this->mLength = length > this->mCapacity ? this->mCapacity : length;
	ZL_RETURN_SIZE_RESULT ( length, length <= this->mCapacity ? ZL_OK : ZL_ERROR );
}

//----------------------------------------------------------------//
ZLSizeResult ZLByteStream::WriteBytes ( const void* buffer, size_t size ) {

	size_t writeSize = (( this->mCursor + size ) > this->mCapacity ) ? ( this->mCapacity - this->mCursor ) : size;

	if ( writeSize ) {
		memcpy ( &(( u8* )this->mWriteBuffer )[ this->mCursor ], buffer, writeSize );
		this->mCursor += writeSize;
		if ( this->mLength < this->mCursor ) {
			this->mLength = this->mCursor;
		}
	}
	ZL_RETURN_SIZE_RESULT ( writeSize, ZL_OK );
}

//----------------------------------------------------------------//
void ZLByteStream::WriteBytesUnsafe ( const void* buffer, size_t size ) {
	
	memcpy ( &(( u8* )this->mWriteBuffer )[ this->mCursor ], buffer, size );
	this->mCursor += size;
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

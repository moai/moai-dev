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
u32 USByteStream::GetCapacity () {

	return this->mCapacity;
}

//----------------------------------------------------------------//
u32 USByteStream::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
u32 USByteStream::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
u32 USByteStream::ReadBytes ( void* buffer, u32 size ) {

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
void USByteStream::Seek ( long offset, int origin ) {

	switch ( origin ) {
		case SEEK_CUR: {
			this->mCursor = this->mCursor + offset;
			break;
		}
		case SEEK_END: {
			this->mCursor = this->mLength + offset;
			break;
		}
		case SEEK_SET: {
			this->mCursor = offset;
			break;
		}
	}
	
	if ( this->mCursor > this->mLength ) {
		this->mCursor = this->mLength;
	}
}

//----------------------------------------------------------------//
void USByteStream::SetBuffer ( void* buffer, u32 size ) {

	this->mCursor = 0;
	this->mLength = 0;
	this->mCapacity = size;
	this->mBuffer = buffer;
}

//----------------------------------------------------------------//
void USByteStream::SetLength ( u32 size ) {

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
u32 USByteStream::WriteBytes ( const void* buffer, u32 size ) {

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

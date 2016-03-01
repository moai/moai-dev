// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLLexStream.h>

//================================================================//
// ZLLexStream
//================================================================//

//----------------------------------------------------------------//
u32 ZLLexStream::GetCaps () {

	return this->mStream ? CAN_READ | CAN_SEEK : 0; 
}

//----------------------------------------------------------------//
size_t ZLLexStream::GetCursor () {

	assert ( this->mStream );
	return this->mStream->GetCursor ();
}

//----------------------------------------------------------------//
size_t ZLLexStream::GetLength () {

	assert ( this->mStream );
	return this->mStream->GetLength ();
}

//----------------------------------------------------------------//
ZLSizeResult ZLLexStream::ReadBytes ( void* buffer, size_t size ) {

	size_t cursor = this->GetCursor ();
	size_t length = this->GetLength ();
	
	size_t readSize = (( cursor + size ) > length ) ? ( length - cursor ) : size;

	u8* bytes = ( u8* )buffer;
	for ( u32 i = 0; i < readSize; ++i ) {
		bytes [ i ] = this->ReadByte ();
	}
	ZL_RETURN_SIZE_RESULT ( readSize, ZL_OK );
}

//----------------------------------------------------------------//
u8 ZLLexStream::ReadByte () {

	assert ( this->mStream );
	u8 value = this->mStream->Read < u8 >( 0 );
	
	if ( value == '\n' ) {
		this->mLine++;
	}
	return value;
}

//----------------------------------------------------------------//
int ZLLexStream::SetCursor ( long offset ) {

	offset = offset - (long) this->GetCursor ();
	
	if ( offset > 0 ) {
		for ( long i = 0; i < offset; ++i ) {
			this->ReadByte ();
		}
	}
	else {
		offset = -offset;
		for ( long i = 0; i < offset; ++i ) {
			this->UnreadByte ();
		}
	}
	return 0;
}

//----------------------------------------------------------------//
void ZLLexStream::SetStream ( ZLStream* stream ) {

	this->mStream = stream;
}

//----------------------------------------------------------------//
u8 ZLLexStream::UnreadByte () {

	assert ( this->mStream );

	this->mStream->Seek ( -1, SEEK_CUR );
	u8 value = this->mStream->Read < u8 >( 0 );
	this->mStream->Seek ( -1, SEEK_CUR );

	if ( value == '\n' ) {
		this->mLine--;
	}
	return value;
}

//----------------------------------------------------------------//
ZLLexStream::ZLLexStream () :
	mLine ( DEFAULT_LINE_NO ) {
}

//----------------------------------------------------------------//
ZLLexStream::~ZLLexStream () {
}

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USLexStream.h>

//================================================================//
// USLexStream
//================================================================//

//----------------------------------------------------------------//
u32 USLexStream::GetCaps () {

	return this->mStream ? CAN_READ | CAN_SEEK : 0; 
}

//----------------------------------------------------------------//
size_t USLexStream::GetCursor () {

	assert ( this->mStream );
	return this->mStream->GetCursor ();
}

//----------------------------------------------------------------//
size_t USLexStream::GetLength () {

	assert ( this->mStream );
	return this->mStream->GetLength ();
}

//----------------------------------------------------------------//
size_t USLexStream::ReadBytes ( void* buffer, size_t size ) {

	size_t cursor = this->GetCursor ();

	size_t length = this->GetLength ();
	if (( cursor + size ) > length ) {
		size = length - cursor;
	}

	u8* bytes = ( u8* )buffer;
	for ( u32 i = 0; i < size; ++i ) {
		bytes [ i ] = this->ReadByte ();
	}
	return size;
}

//----------------------------------------------------------------//
u8 USLexStream::ReadByte () {

	assert ( this->mStream );
	u8 value = this->mStream->Read < u8 >( 0 );
	
	if ( value == '\n' ) {
		this->mLine++;
	}
	return value;
}

//----------------------------------------------------------------//
int USLexStream::SetCursor ( long offset ) {

	offset = offset - this->GetCursor ();
	
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
void USLexStream::SetStream ( USStream* stream ) {

	this->mStream = stream;
}

//----------------------------------------------------------------//
u8 USLexStream::UnreadByte () {

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
USLexStream::USLexStream () :
	mLine ( DEFAULT_LINE_NO ) {
}

//----------------------------------------------------------------//
USLexStream::~USLexStream () {
}

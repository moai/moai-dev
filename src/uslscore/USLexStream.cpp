// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USLexStream.h>

//================================================================//
// USLexStream
//================================================================//

//----------------------------------------------------------------//
u32 USLexStream::GetCursor () {

	assert ( this->mStream );
	return this->mStream->GetCursor ();
}

//----------------------------------------------------------------//
u32 USLexStream::GetLength () {

	assert ( this->mStream );
	return this->mStream->GetLength ();
}

//----------------------------------------------------------------//
u32 USLexStream::ReadBytes ( void* buffer, u32 size ) {

	u32 cursor = this->GetCursor ();

	u32 length = this->GetLength ();
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
	u8 value = this->mStream->Read < u8 >();
	
	if ( value == '\n' ) {
		this->mLine++;
	}
	return value;
}

//----------------------------------------------------------------//
void USLexStream::Seek ( long offset, int origin ) {

	size_t cursor = this->GetCursor ();
	
	switch ( origin ) {
		case SEEK_CUR: {
			offset = ( cursor + offset ) - cursor;
			break;
		}
		case SEEK_END: {
			offset = ( this->GetLength () + offset ) - cursor;
			break;
		}
		case SEEK_SET: {
			offset = offset - cursor;
			break;
		}
	}
	
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
}

//----------------------------------------------------------------//
void USLexStream::SetStream ( USStream* stream ) {

	this->mStream = stream;
}

//----------------------------------------------------------------//
u8 USLexStream::UnreadByte () {

	assert ( this->mStream );

	this->mStream->Seek ( -1, SEEK_CUR );
	u8 value = this->mStream->Read < u8 >();
	this->mStream->Seek ( -1, SEEK_CUR );

	if ( value == '\n' ) {
		this->mLine--;
	}
	return value;
}

//----------------------------------------------------------------//
USLexStream::USLexStream () :
	mLine ( DEFAULT_LINE_NO ),
	mLinePad ( DEFAULT_LINE_PAD ) {
}

//----------------------------------------------------------------//
USLexStream::~USLexStream () {
}

//----------------------------------------------------------------//
u32 USLexStream::WriteBytes ( const void* buffer, u32 size ) {
	UNUSED ( buffer );
	UNUSED ( size );

	assert ( false );
	return 0;
}
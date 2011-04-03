// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USLexStream.h>

//================================================================//
// USLexStream
//================================================================//

//----------------------------------------------------------------//
u32 USLexStream::ReadBytes ( void* buffer, u32 size ) {

	u32 cursor = this->GetCursor ();

	if (( cursor + size ) > this->mLength ) {
		size = this->mLength - cursor;
	}

	u8* bytes = ( u8* )buffer;
	for ( u32 i = 0; i < size; ++i ) {
		bytes [ i ] = this->ReadByte ();
	}
	
	return size;
}

//----------------------------------------------------------------//
u8 USLexStream::ReadByte () {

	u8 value;
	fread ( &value, sizeof ( u8 ), 1, this->mFile );
	
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
			offset = ( this->mLength - offset ) - cursor;
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
void USLexStream::SetFormatting ( u32 linePad ) {

	this->mLinePad = linePad;
}

//----------------------------------------------------------------//
u8 USLexStream::UnreadByte () {

	u8 value;
	fseek ( this->mFile, -1, SEEK_CUR );
	fread ( &value, sizeof ( u8 ), 1, this->mFile );
	fseek ( this->mFile, -1, SEEK_CUR );

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


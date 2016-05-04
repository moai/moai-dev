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
ZLCharResult ZLLexStream::ReadByte () {

	if ( !this->mStream ) ZL_RETURN_CHAR_RESULT ( 0, ZL_ERROR );
	
	u8 value = this->mStream->Read < u8 >( 0 );
	
	if ( value == '\n' ) {
		this->mLine++;
	}
	ZL_RETURN_CHAR_RESULT ( value, ZL_OK );
}

//----------------------------------------------------------------//
ZLResultCode ZLLexStream::SetCursor ( long offset ) {

	if ( !( this->mStream && ( this->GetCaps () & ( CAN_READ | CAN_WRITE )))) return ZL_ERROR;

	size_t restoreCursor = this->GetCursor ();
	size_t restoreLine = this->mLine;

	if (( offset < 0 ) || ( this->GetLength () < ( size_t )offset )) return ZL_ERROR;

	long localOffset = offset - ( long )restoreCursor;
	
	if ( localOffset > 0 ) {
		for ( long i = 0; i < localOffset; ++i ) {
			this->ReadByte ();
		}
	}
	else {
		localOffset = -localOffset;
		for ( long i = 0; i < localOffset; ++i ) {
			this->UnreadByte ();
		}
	}
	
	// instead of checking each byte, we'll check the final cursor position
	// if it isn't where we expected it to be, reset and return an error
	if ( this->mStream->GetCursor () != ( size_t )offset ) {
	
		this->mStream->SetCursor ( restoreCursor );
		this->mLine = restoreLine;
		return ZL_ERROR;
	}
	return ZL_OK;
}

//----------------------------------------------------------------//
void ZLLexStream::SetStream ( ZLStream* stream ) {

	this->mStream = stream;
}

//----------------------------------------------------------------//
ZLCharResult ZLLexStream::UnreadByte () {

	if ( !this->mStream ) ZL_RETURN_CHAR_RESULT ( 0, ZL_ERROR );

	this->mStream->Seek ( -1, SEEK_CUR );
	u8 value = this->mStream->Read < u8 >( 0 );
	this->mStream->Seek ( -1, SEEK_CUR );

	if ( value == '\n' ) {
		this->mLine--;
	}
	ZL_RETURN_CHAR_RESULT ( value, ZL_OK );
}

//----------------------------------------------------------------//
ZLLexStream::ZLLexStream () :
	mLine ( DEFAULT_LINE_NO ) {
}

//----------------------------------------------------------------//
ZLLexStream::~ZLLexStream () {
}

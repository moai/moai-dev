// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USStreamFormatter.h>
#include <uslscore/USStream.h>

//----------------------------------------------------------------//
u32 USStreamFormatter::AppendInput ( const void* buffer, u32 size ) {

	if ( !( buffer && size )) return 0;

	u32 available = INPUT_CHUNKSIZE - this->mInputCursor;
	if ( size > available ) {
		size = available;
	}
	
	if ( size ) {
		u8* input = ( u8* )(( size_t )this->mInputChunk + ( size_t )this->mInputCursor );
		memcpy ( input, buffer, size );
		this->mInputCursor += size;
	}
	return size;
}

//----------------------------------------------------------------//
void USStreamFormatter::Flush () {

	if ( this->mOutStream ) {
		this->WriteBytes ( 0, 0, false );
	}
}

//----------------------------------------------------------------//
u32 USStreamFormatter::GetCursor () {

	if ( this->mOutStream ) {
		return this->mOutStream->GetCursor ();
	}
	return 0;
}

//----------------------------------------------------------------//
u32 USStreamFormatter::GetLength () {

	if ( this->mOutStream ) {
		return this->mOutStream->GetLength ();
	}
	return 0;
}

//----------------------------------------------------------------//
u32 USStreamFormatter::ReadBytes ( void* buffer, u32 size ) {

	if ( this->mOutStream ) {
		return this->mOutStream->ReadBytes ( buffer, size );
	}
	return 0;
}

//----------------------------------------------------------------//
void USStreamFormatter::Seek ( long offset, int origin ) {

	if ( this->mOutStream ) {
		return this->mOutStream->Seek ( offset, origin );
	}
}

//----------------------------------------------------------------//
USStreamFormatter::USStreamFormatter () :
	mState ( IDLE ),
	mInputCursor ( 0 ),
	mOutStream ( 0 ),
	mBlockCount ( 0 ) {
}

//----------------------------------------------------------------//
USStreamFormatter::~USStreamFormatter () {
}

//----------------------------------------------------------------//
u32 USStreamFormatter::WriteBytes ( const void* buffer, u32 size ) {

	return this->WriteBytes ( buffer, size, true );
}

//----------------------------------------------------------------//
u32 USStreamFormatter::WriteBytes ( const void* buffer, u32 size, bool more ) {

	u32 total = 0;
	this->AffirmInit ();
	
	if (( this->mState == ERROR ) || ( this->mState == DONE )) return 0;
	
	u32 cursor = this->mOutStream->GetCursor ();
	this->mOutStream->Seek ( 0, SEEK_END );
	
	do {
		
		u32 append = this->AppendInput ( buffer, size );
		buffer = ( void* )(( size_t )buffer + append );
		size -= append;
		
		if (( this->mInputCursor == INPUT_CHUNKSIZE ) || ( more == false )) {
			total += this->Format ( more );
		}
	}
	while ( size );
	
	this->mOutStream->Seek ( cursor, SEEK_SET );
	
	if ( more == false ) {
		this->Finish ( DONE );
	}
	return total;
}

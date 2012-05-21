// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USStreamFormatter.h>
#include <uslscore/USStream.h>

//----------------------------------------------------------------//
size_t USStreamFormatter::AppendInput ( const void* buffer, size_t size ) {

	if ( !( buffer && size )) return 0;

	size_t available = INPUT_CHUNKSIZE - this->mInputCursor;
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
u32 USStreamFormatter::GetCaps () {

	return this->mOutStream ? CAN_WRITE : 0;
}

//----------------------------------------------------------------//
size_t USStreamFormatter::GetCursor () {

	if ( this->mOutStream ) {
		return this->mOutStream->GetCursor ();
	}
	return 0;
}

//----------------------------------------------------------------//
size_t USStreamFormatter::GetLength () {

	if ( this->mOutStream ) {
		return this->mOutStream->GetLength ();
	}
	return 0;
}

//----------------------------------------------------------------//
size_t USStreamFormatter::ReadBytes ( void* buffer, size_t size ) {

	if ( this->mOutStream ) {
		return this->mOutStream->ReadBytes ( buffer, size );
	}
	return 0;
}

//----------------------------------------------------------------//
int USStreamFormatter::SetCursor ( long offset ) {

	if ( this->mOutStream ) {
		return this->mOutStream->Seek ( offset, SEEK_SET );
	}
	return -1;
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
size_t USStreamFormatter::WriteBytes ( const void* buffer, size_t size ) {

	return this->WriteBytes ( buffer, size, true );
}

//----------------------------------------------------------------//
size_t USStreamFormatter::WriteBytes ( const void* buffer, size_t size, bool more ) {

	size_t total = 0;
	this->AffirmInit ();
	
	if (( this->mState == ERROR ) || ( this->mState == DONE )) return 0;
	
	size_t cursor = this->mOutStream->GetCursor ();
	this->mOutStream->Seek ( 0, SEEK_END );
	
	do {
		
		size_t append = this->AppendInput ( buffer, size );
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

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USFileSys.h>
#include <uslscore/USFileStream.h>

//----------------------------------------------------------------//
void USFileStream::Close () {

	if ( this->mFile ) {
		zl_fclose ( this->mFile );
	}
	
	this->mLength = 0;
	this->mFile = 0;
}

//----------------------------------------------------------------//
void USFileStream::Flush () {

	zl_fflush ( this->mFile );
}

//----------------------------------------------------------------//
u32 USFileStream::GetCaps () {

	return this->mFile ? this->mCaps : 0;
}

//----------------------------------------------------------------//
size_t USFileStream::GetCursor () {

	return ( size_t )zl_ftell ( this->mFile );
}

//----------------------------------------------------------------//
ZLFILE* USFileStream::GetFile () {

	return this->mFile;
}

//----------------------------------------------------------------//
size_t USFileStream::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
bool USFileStream::Open ( cc8* filename, u32 mode ) {

	this->Close ();
	
	if ( !filename ) return false;
	if ( !filename [ 0 ]) return false;
	
	zl_stat fileStat;
	bool exists = USFileSys::GetFileStat ( filename, fileStat );
	
	bool affirmPath = false;
	cc8* modeStr = 0;

	switch ( mode ) {
	
		case APPEND:
			
			modeStr = "a+";
			this->mCaps = CAN_WRITE | CAN_READ | CAN_SEEK;
			break;
			
		case READ:
			
			modeStr = "rb";
			this->mCaps = CAN_READ | CAN_SEEK;
			break;
		
		case READ_WRITE:
		
			modeStr = "rb+";
			this->mCaps = CAN_READ | CAN_WRITE | CAN_SEEK;
			break;
		
		case READ_WRITE_AFFIRM:
			
			if ( exists ) {
				modeStr = "rb+";
			}
			else {
				affirmPath = true;
				modeStr = "wb+";
			}
			this->mCaps = CAN_READ | CAN_WRITE | CAN_SEEK;
			break;
		
		case READ_WRITE_NEW:
			
			affirmPath = true;
			modeStr = "wb+";
			this->mCaps = CAN_READ | CAN_WRITE | CAN_SEEK;
			break;
		
		case WRITE:
			
			modeStr = "rb+";
			this->mCaps = CAN_WRITE | CAN_SEEK;
			break;
	}
	
	if ( affirmPath ) {
		if ( !USFileSys::AffirmPath ( USFileSys::TruncateFilename ( filename ))) {
			modeStr = 0;
		}
	}
	
	if ( modeStr ) {
		
		this->mFile = ( ZLFILE* )zl_fopen ( filename, modeStr );
		
		if ( this->mFile && exists ) {
			this->mLength = fileStat.mSize;
		}
	}
	
	return this->mFile != NULL;
}

//----------------------------------------------------------------//
bool USFileStream::OpenRead ( cc8* filename ) {

	return this->Open ( filename, READ );
}

//----------------------------------------------------------------//
bool USFileStream::OpenWrite ( cc8* filename ) {

	return this->Open ( filename, READ_WRITE_NEW );
}

//----------------------------------------------------------------//
size_t USFileStream::ReadBytes ( void* buffer, size_t size ) {

	return zl_fread ( buffer, 1, size, this->mFile );
}

//----------------------------------------------------------------//
int USFileStream::SetCursor ( long offset ) {

	return zl_fseek ( this->mFile, offset, SEEK_SET );
}

//----------------------------------------------------------------//
USFileStream::USFileStream () :
	mFile ( 0 ),
	mLength ( 0 ),
	mCaps ( 0 ) {
}

//----------------------------------------------------------------//
USFileStream::~USFileStream () {
	Close ();
}

//----------------------------------------------------------------//
size_t USFileStream::WriteBytes ( const void* buffer, size_t size ) {

	size_t result = zl_fwrite ( buffer, 1, size, this->mFile );
	
	size_t cursor = ( size_t )zl_ftell ( this->mFile );
	if ( cursor > this->mLength ) {
		this->mLength = cursor;
	}
	return result;
}

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
u32 USFileStream::GetCursor () {

	return ( u32 )zl_ftell ( this->mFile );
}

//----------------------------------------------------------------//
ZLFILE* USFileStream::GetFile () {

	return this->mFile;
}

//----------------------------------------------------------------//
u32 USFileStream::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
bool USFileStream::Open ( cc8* filename, u32 mode ) {

	this->Close ();
	
	if ( !filename ) return false;
	if ( !filename [ 0 ]) return false;
	
	bool measure = false;
	bool affirmPath = false;

	cc8* modeStr = 0;

	switch ( mode ) {
	
		case READ:
			
			modeStr = "rb";
			measure = true;
			break;
		
		case READ_WRITE:
		
			modeStr = "rb+";
			measure = true;
			break;
		
		case READ_WRITE_AFFIRM:
			
			if ( USFileSys::CheckFileExists ( filename )) {
				this->mFile = ( ZLFILE* )zl_fopen ( filename, "rb+" );
				modeStr = "rb+";
			}
			else {
				affirmPath = true;
				modeStr = "wb+";
			}
			measure = true;
			break;
		
		case READ_WRITE_NEW:
			
			affirmPath = true;
			modeStr = "wb+";
			break;
		
		case WRITE:
			
			modeStr = "wb";
			break;
	}
	
	if ( !USFileSys::AffirmPath ( USFileSys::TruncateFilename ( filename ))) {
		modeStr = 0;
	}
	
	if ( modeStr ) {
		this->mFile = ( ZLFILE* )zl_fopen ( filename, modeStr );
		
		if ( this->mFile && measure ) {
			zl_fseek ( this->mFile, 0L, SEEK_END );
			this->mLength = ( u32 )zl_ftell ( this->mFile );
			zl_fseek ( this->mFile, 0L, SEEK_SET );
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

	return this->Open ( filename, WRITE );
}

//----------------------------------------------------------------//
u32 USFileStream::ReadBytes ( void* buffer, u32 size ) {

	return ( u32 )zl_fread ( buffer, 1, size, this->mFile );
}

//----------------------------------------------------------------//
void USFileStream::Seek ( long offset, int origin ) {

	zl_fseek ( this->mFile, offset, origin );
}

//----------------------------------------------------------------//
USFileStream::USFileStream () :
	mFile ( 0 ),
	mLength ( 0 ) {
}

//----------------------------------------------------------------//
USFileStream::~USFileStream () {
	Close ();
}

//----------------------------------------------------------------//
u32 USFileStream::WriteBytes ( const void* buffer, u32 size ) {

	u32 result = ( u32 )zl_fwrite ( buffer, 1, size, this->mFile );
	
	u32 cursor = ( u32 )zl_ftell ( this->mFile );
	if ( cursor > this->mLength ) {
		this->mLength = cursor;
	}

	return result;
}

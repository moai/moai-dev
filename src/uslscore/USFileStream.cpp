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
bool USFileStream::OpenRead ( cc8* filename ) {

	Close ();
	
	this->mFile = ( ZLFILE* )zl_fopen ( filename, "rb" );
	if ( this->mFile ) {

		zl_fseek ( this->mFile, 0L, SEEK_END );
		this->mLength = ( u32 )zl_ftell ( this->mFile );
		zl_fseek ( this->mFile, 0L, SEEK_SET );
	}

	return this->mFile != NULL;
}

//----------------------------------------------------------------//
bool USFileStream::OpenWrite ( cc8* filename, bool affirmPath ) {

	Close ();
	
	if ( affirmPath ) {
		bool result = USFileSys::AffirmPath ( USFileSys::TruncateFilename ( filename ));
		if ( !result ) return false;
	}

	this->mFile = ( ZLFILE* )zl_fopen ( filename, "wb" );
	return this->mFile != NULL;
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

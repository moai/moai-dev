// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USFilename.h>
#include <uslscore/USFileSys.h>
#include <uslscore/USFileStream.h>

//----------------------------------------------------------------//
void USFileStream::Close () {

	if ( this->mFile ) {
		moaio_fclose ( this->mFile );
	}
	
	this->mLength = 0;
	this->mFile = 0;
}

//----------------------------------------------------------------//
void USFileStream::Flush () {

	moaio_fflush ( this->mFile );
}

//----------------------------------------------------------------//
u32 USFileStream::GetCursor () {

	return ( u32 )moaio_ftell ( this->mFile );
}

//----------------------------------------------------------------//
MOAIFILE* USFileStream::GetFile () {

	return this->mFile;
}

//----------------------------------------------------------------//
u32 USFileStream::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
bool USFileStream::OpenRead ( cc8* filename ) {

	Close ();
	
	this->mFile = ( MOAIFILE* )moaio_fopen ( filename, "rb" );
	if ( this->mFile ) {

		moaio_fseek ( this->mFile, 0L, SEEK_END );
		this->mLength = ( u32 )moaio_ftell ( this->mFile );
		moaio_fseek ( this->mFile, 0L, SEEK_SET );
	}

	return this->mFile != NULL;
}

//----------------------------------------------------------------//
bool USFileStream::OpenWrite ( cc8* filename, bool affirmPath ) {

	Close ();
	
	if ( affirmPath ) {
		USFileSys::AffirmPath ( USPathOps::GetPath ( filename, true ));
	}

	this->mFile = ( MOAIFILE* )moaio_fopen ( filename, "wb" );
	return this->mFile != NULL;
}

//----------------------------------------------------------------//
u32 USFileStream::ReadBytes ( void* buffer, u32 size ) {

	return ( u32 )moaio_fread ( buffer, 1, size, this->mFile );
}

//----------------------------------------------------------------//
void USFileStream::Seek ( long offset, int origin ) {

	moaio_fseek ( this->mFile, offset, origin );
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

	u32 result = ( u32 )moaio_fwrite ( buffer, 1, size, this->mFile );
	
	u32 cursor = ( u32 )moaio_ftell ( this->mFile );
	if ( cursor > this->mLength ) {
		this->mLength = cursor;
	}

	return result;
}

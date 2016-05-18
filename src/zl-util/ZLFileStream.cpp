// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLFileSys.h>
#include <zl-util/ZLFileStream.h>
#include <zl-vfs/ZLVfsFileSystem.h>

//----------------------------------------------------------------//
void ZLFileStream::Close () {

	if ( this->mFile ) {
		zl_fclose ( this->mFile );
	}
	
	this->mLength = 0;
	this->mFile = 0;
}

//----------------------------------------------------------------//
void ZLFileStream::Flush () {

	zl_fflush ( this->mFile );
}

//----------------------------------------------------------------//
u32 ZLFileStream::GetCaps () {

	return this->mFile ? this->mCaps : 0;
}

//----------------------------------------------------------------//
size_t ZLFileStream::GetCursor () {

	return ( size_t )zl_ftell ( this->mFile );
}

//----------------------------------------------------------------//
ZLFILE* ZLFileStream::GetFile () {

	return this->mFile;
}

//----------------------------------------------------------------//
size_t ZLFileStream::GetLength () {

	return this->mLength;
}

//----------------------------------------------------------------//
bool ZLFileStream::IsOpen () {

	return this->mFile != NULL;
}

//----------------------------------------------------------------//
bool ZLFileStream::Open ( cc8* filename, u32 mode ) {

	this->Close ();
	
	if ( !filename ) return false;
	if ( !filename [ 0 ]) return false;
	
	zl_stat fileStat;
	bool exists = ZLFileSys::GetFileStat ( filename, fileStat );
	
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
		if ( !ZLFileSys::AffirmPath ( ZLFileSys::TruncateFilename ( filename ))) {
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
bool ZLFileStream::OpenRead ( cc8* filename ) {

	return this->Open ( filename, READ );
}

//----------------------------------------------------------------//
bool ZLFileStream::OpenWrite ( cc8* filename ) {

	return this->Open ( filename, READ_WRITE_NEW );
}

//----------------------------------------------------------------//
ZLSizeResult ZLFileStream::ReadBytes ( void* buffer, size_t size ) {

	ZL_RETURN_SIZE_RESULT ( zl_fread ( buffer, 1, size, this->mFile ), ZL_OK );
}

//----------------------------------------------------------------//
ZLResultCode ZLFileStream::SetCursor ( size_t offset ) {

	return zl_fseek ( this->mFile, ( long )offset, SEEK_SET ) == 0 ? ZL_OK : ZL_ERROR;
}

//----------------------------------------------------------------//
ZLSizeResult ZLFileStream::SetLength ( size_t length ) {
	UNUSED ( length );
	ZL_RETURN_SIZE_RESULT ( 0, ZL_UNSUPPORTED );
}

//----------------------------------------------------------------//
ZLSizeResult ZLFileStream::WriteBytes ( const void* buffer, size_t size ) {

	size_t writeSize = zl_fwrite ( buffer, 1, size, this->mFile );
	
	size_t cursor = ( size_t )zl_ftell ( this->mFile );
	if ( cursor > this->mLength ) {
		this->mLength = cursor;
	}
	ZL_RETURN_SIZE_RESULT ( writeSize, ZL_OK );
}

//----------------------------------------------------------------//
ZLFileStream::ZLFileStream () :
	mFile ( 0 ),
	mLength ( 0 ),
	mCaps ( 0 ) {
}

//----------------------------------------------------------------//
ZLFileStream::~ZLFileStream () {
	Close ();
}

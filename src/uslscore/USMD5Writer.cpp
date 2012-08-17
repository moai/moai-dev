// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USMD5Writer.h>

//================================================================//
// USMD5Writer
//================================================================//

//----------------------------------------------------------------//
void USMD5Writer::Close () {

	if ( this->mOutputStream ) {
		
		// Finalize hash
		this->mFinalMD5 = ( u8* )malloc ( MD5_DIGEST_LENGTH );
		MD5_Final ( this->mFinalMD5, this->mMD5Context );		
	}
	
	this->mOutputStream = 0;
	this->mCursor = 0;
}

//----------------------------------------------------------------//
u32 USMD5Writer::GetCaps () {
	
	return this->mOutputStream ? CAN_WRITE : 0;
}

//----------------------------------------------------------------//
u8* USMD5Writer::GetHash () {
	
	return this->mFinalMD5;
}

//----------------------------------------------------------------//
size_t USMD5Writer::GetCursor () {
	
	return this->mCursor;
}

//----------------------------------------------------------------//
size_t USMD5Writer::GetLength () {
	
	return this->mCursor;
}

//----------------------------------------------------------------//
bool USMD5Writer::Open ( USStream& stream ) {
	
	this->Close ();
	
	this->mOutputStream = &stream;
	
	this->mMD5Context = ( MD5_CTX* ) malloc ( sizeof ( MD5_CTX ));
	MD5_Init ( this->mMD5Context );
	
	//read in existing data, only applies to append
	if (( stream.GetCaps() & CAN_READ ) && ( stream.GetCaps() & CAN_WRITE ) && ( stream.GetCaps() & CAN_SEEK )) {

		stream.SetCursor( 0 );
		int bufferSize = 1000; //AJV TODO change hardcoded chunk size
		char * buffer =  ( char * ) malloc ( bufferSize );
		int bytesRead = bufferSize;
		
		while ( bytesRead == bufferSize ) {
			
			 bytesRead = stream.ReadBytes( buffer, bufferSize );
			 MD5_Update ( this->mMD5Context, buffer, bytesRead );
		}
	}
	
	return true;
}

//----------------------------------------------------------------//
USMD5Writer::USMD5Writer () :
	mOutputStream ( 0 ),
	mCursor ( 0 ) {
}

//----------------------------------------------------------------//
USMD5Writer::~USMD5Writer () {
	
}

//----------------------------------------------------------------//
size_t USMD5Writer::WriteBytes ( const void* buffer, size_t size ) {
	
	// Update the hash
	MD5_Update ( this->mMD5Context, buffer, size );
	
	// Pass the write through to stream
	if ( this->mOutputStream ) {
		
		this->mOutputStream->WriteBytes( buffer, size );
		return size;
	}
	
	return 0;
}

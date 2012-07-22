// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USHashWriter.h>

//================================================================//
// USHashWriter
//================================================================//


//----------------------------------------------------------------//
void USHashWriter::Close () {

	if ( this->mIsOpen ) {
		this->FinalizeHash ();
	}
	
	if ( this->mHMACKey ) {
	
		size_t blockSize = this->GetBlockSize ();
		size_t hashSize = this->GetHashSize ();
		
		u8* keyBytes = ( u8* )this->mHMACKey;
		u8* xorKey = ( u8* )alloca ( blockSize );
		for ( u32 i = 0; i < blockSize; ++i ) {
			xorKey [ i ] = keyBytes [ i ] ^ 0x5C;
		}
		
		void* hash = alloca ( hashSize );
		memcpy ( hash, this->GetHash (), hashSize );
	
		this->InitHash ();
		
		this->HashBytes ( xorKey, blockSize );
		this->HashBytes ( hash, hashSize );
		
		this->FinalizeHash ();
	}
	
	if ( this->mHMACKey ) {
		free ( this->mHMACKey );
		this->mHMACKey = 0;
	}
	
	this->mOutputStream = 0;
	this->mIsOpen = false;
}

//----------------------------------------------------------------//
u32 USHashWriter::GetCaps () {
	
	return this->mOutputStream ? CAN_WRITE : 0;
}

//----------------------------------------------------------------//
u32 USHashWriter::GetChecksum () {
	return 0;
}

//----------------------------------------------------------------//
size_t USHashWriter::GetCursor () {
	
	return 0;
}

//----------------------------------------------------------------//
size_t USHashWriter::GetLength () {
	
	return 0;
}

//----------------------------------------------------------------//
bool USHashWriter::Open ( USStream* stream ) {
	
	this->Close ();
	this->InitHash ();
	
	if ( this->mHMACKey ) {
		size_t blockSize = this->GetBlockSize ();
		u8* key = ( u8* )this->mHMACKey;
		u8* xorKey = ( u8* )alloca ( blockSize );
		for ( u32 i = 0; i < blockSize; ++i ) {
			xorKey [ i ] = key [ i ] ^ 0x36;
		}
		this->WriteBytes ( xorKey, blockSize );
	}
	
	this->mOutputStream = stream;
	this->mIsOpen = true;
	return true;
}

//----------------------------------------------------------------//
void USHashWriter::SetHMACKey ( void* key, size_t keySize ) {

	if ( !this->mIsOpen ) return;

	size_t blockSize = this->GetBlockSize ();
	
	if ( keySize > blockSize ) {
		
		this->InitHash ();
		this->HashBytes ( key, keySize );
		this->FinalizeHash ();
		this->InitHash ();

		keySize = this->GetHashSize ();
		key = alloca ( keySize );
		
		memcpy ( key, this->GetHash (), keySize );
	}
	
	this->mHMACKey = calloc ( 1, blockSize );
	memcpy ( this->mHMACKey, key, keySize );
	
	u8* keyBytes = ( u8* )this->mHMACKey;
	u8* xorKey = ( u8* )alloca ( blockSize );
	for ( u32 i = 0; i < blockSize; ++i ) {
		xorKey [ i ] = keyBytes [ i ] ^ 0x36;
	}
	this->HashBytes ( xorKey, blockSize );
}

//----------------------------------------------------------------//
USHashWriter::USHashWriter () :
	mIsOpen ( false ),
	mOutputStream ( 0 ),
	mHMACKey ( 0 ) {
}

//----------------------------------------------------------------//
USHashWriter::~USHashWriter () {
}

//----------------------------------------------------------------//
size_t USHashWriter::WriteBytes ( const void* buffer, size_t size ) {
	
	// Pass the write through to stream
	if ( this->mOutputStream ) {
		size = this->mOutputStream->WriteBytes( buffer, size );
	}
	
	// Update the hash
	if ( size ) {
		this->HashBytes ( buffer, size );
	}
	
	return size;
}

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLHashWriter.h>

//================================================================//
// ZLHashWriter
//================================================================//


//----------------------------------------------------------------//
void ZLHashWriter::Close () {

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
u32 ZLHashWriter::GetCaps () {
	
	return this->mOutputStream ? CAN_WRITE : 0;
}

//----------------------------------------------------------------//
u32 ZLHashWriter::GetChecksum () {
	return 0;
}

//----------------------------------------------------------------//
size_t ZLHashWriter::GetCursor () {
	
	return 0;
}

//----------------------------------------------------------------//
size_t ZLHashWriter::GetLength () {
	
	return 0;
}

//----------------------------------------------------------------//
bool ZLHashWriter::Open ( ZLStream& stream ) {
	
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
	
	this->mOutputStream = &stream;
	this->mIsOpen = true;
	return true;
}

//----------------------------------------------------------------//
void ZLHashWriter::SetHMACKey ( void* key, size_t keySize ) {

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
size_t ZLHashWriter::WriteBytes ( const void* buffer, size_t size ) {
	
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

//----------------------------------------------------------------//
ZLHashWriter::ZLHashWriter () :
	mIsOpen ( false ),
	mOutputStream ( 0 ),
	mHMACKey ( 0 ) {
}

//----------------------------------------------------------------//
ZLHashWriter::~ZLHashWriter () {
}

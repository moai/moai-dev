// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLHashWriter.h>

//================================================================//
// ZLHashWriter
//================================================================//

//----------------------------------------------------------------//
u32 ZLHashWriter::GetCaps () {

	return this->mProxiedStream ? this->mProxiedStream->GetCaps () | CAN_WRITE : CAN_WRITE;
}

//----------------------------------------------------------------//
u32 ZLHashWriter::GetChecksum () {

	return 0;
}

//----------------------------------------------------------------//
void ZLHashWriter::OnClose () {

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
}

//----------------------------------------------------------------//
ZLResultCode ZLHashWriter::OnOpen () {
	
	this->InitHash ();
	
	if ( this->mHMACKey ) {
		size_t blockSize = this->GetBlockSize ();
		u8* key = ( u8* )this->mHMACKey;
		u8* xorKey = ( u8* )alloca ( blockSize );
		for ( u32 i = 0; i < blockSize; ++i ) {
			xorKey [ i ] = key [ i ] ^ 0x36;
		}
		return ( this->WriteBytes ( xorKey, blockSize ).mCode == ZL_OK ) ? ZL_OK : ZL_ERROR;
	}
	return ZL_OK;
}

//----------------------------------------------------------------//
ZLSizeResult ZLHashWriter::WriteBytes ( const void* buffer, size_t size ) {
	
	size_t writeSize = size;
	
	// Pass the write through to stream
	if ( this->mProxiedStream ) {
		writeSize = this->mProxiedStream->WriteBytes( buffer, writeSize );
	}
	
	// Update the hash
	if ( writeSize ) {
		this->HashBytes ( buffer, writeSize );
	}
	
	this->mCursor += writeSize;
	if ( this->mLength < this->mCursor ) {
		this->mLength = this->mCursor;
	}

	ZL_RETURN_SIZE_RESULT ( writeSize, ZL_OK );
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
ZLHashWriter::ZLHashWriter () :
	mHMACKey ( 0 ) {
}

//----------------------------------------------------------------//
ZLHashWriter::~ZLHashWriter () {
}

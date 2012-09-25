// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zlib.h>
#include <uslscore/USHashWriterAdler32.h>

//================================================================//
// USHashWriterAdler32
//================================================================//

//----------------------------------------------------------------//
void USHashWriterAdler32::FinalizeHash () {
}

//----------------------------------------------------------------//
size_t USHashWriterAdler32::GetBlockSize () {
	return 1;
}

//----------------------------------------------------------------//
u32 USHashWriterAdler32::GetChecksum () {
	return this->mSum;
}

//----------------------------------------------------------------//
void* USHashWriterAdler32::GetHash () {
	
	return &this->mHash;
}

//----------------------------------------------------------------//
size_t USHashWriterAdler32::GetHashSize () {

	return 4;
}

//----------------------------------------------------------------//
void USHashWriterAdler32::HashBytes ( const void* buffer, size_t size ) {

	this->mSum = adler32 ( this->mSum, ( Bytef* )buffer, size );
	
	this->mHash [ 0 ] = ( this->mSum >> 24 ) & 0x000000ff;
	this->mHash [ 1 ] = ( this->mSum >> 16 ) & 0x000000ff;
	this->mHash [ 2 ] = ( this->mSum >> 8 ) & 0x000000ff;
	this->mHash [ 3 ] = this->mSum & 0x000000ff;
}

//----------------------------------------------------------------//
void USHashWriterAdler32::InitHash () {

	this->mSum = 0x00000001;
	memset ( this->mHash, 0, sizeof ( this->mHash ));
}

//----------------------------------------------------------------//
USHashWriterAdler32::USHashWriterAdler32 () {
}

//----------------------------------------------------------------//
USHashWriterAdler32::~USHashWriterAdler32 () {
	this->Close ();
}

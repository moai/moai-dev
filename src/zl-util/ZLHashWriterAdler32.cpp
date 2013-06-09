// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zlib.h>
#include <zl-util/ZLHashWriterAdler32.h>

//================================================================//
// ZLHashWriterAdler32
//================================================================//

//----------------------------------------------------------------//
void ZLHashWriterAdler32::FinalizeHash () {
}

//----------------------------------------------------------------//
size_t ZLHashWriterAdler32::GetBlockSize () {
	return 1;
}

//----------------------------------------------------------------//
u32 ZLHashWriterAdler32::GetChecksum () {
	return this->mSum;
}

//----------------------------------------------------------------//
void* ZLHashWriterAdler32::GetHash () {
	
	return &this->mHash;
}

//----------------------------------------------------------------//
size_t ZLHashWriterAdler32::GetHashSize () {

	return 4;
}

//----------------------------------------------------------------//
void ZLHashWriterAdler32::HashBytes ( const void* buffer, size_t size ) {

	this->mSum = adler32 ( this->mSum, ( Bytef* )buffer, size );
	
	this->mHash [ 0 ] = ( this->mSum >> 24 ) & 0x000000ff;
	this->mHash [ 1 ] = ( this->mSum >> 16 ) & 0x000000ff;
	this->mHash [ 2 ] = ( this->mSum >> 8 ) & 0x000000ff;
	this->mHash [ 3 ] = this->mSum & 0x000000ff;
}

//----------------------------------------------------------------//
void ZLHashWriterAdler32::InitHash () {

	this->mSum = 0x00000001;
	memset ( this->mHash, 0, sizeof ( this->mHash ));
}

//----------------------------------------------------------------//
ZLHashWriterAdler32::ZLHashWriterAdler32 () {
}

//----------------------------------------------------------------//
ZLHashWriterAdler32::~ZLHashWriterAdler32 () {
	this->Close ();
}

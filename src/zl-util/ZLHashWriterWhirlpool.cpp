// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLHashWriterWhirlpool.h>

extern "C" {
	#include <contrib/moai_whirlpool.h>
}

//================================================================//
// ZLHashWriterWhirlpool
//================================================================//

//----------------------------------------------------------------//
void ZLHashWriterWhirlpool::FinalizeHash () {

	MOAIWhirlpool_Finalize (( Whirlpool* )this->mAlgorithm, ( u8* )this->mHash );
}

//----------------------------------------------------------------//
size_t ZLHashWriterWhirlpool::GetBlockSize () {
	return WBLOCKBYTES;
}

//----------------------------------------------------------------//
void* ZLHashWriterWhirlpool::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t ZLHashWriterWhirlpool::GetHashSize () {

	return DIGESTBYTES;
}

//----------------------------------------------------------------//
void ZLHashWriterWhirlpool::HashBytes ( const void* buffer, size_t size ) {
	
	// size in *bits*
	MOAIWhirlpool_Add (( const u8* )buffer, ( unsigned long )size << 3, ( Whirlpool* )this->mAlgorithm );
}

//----------------------------------------------------------------//
void ZLHashWriterWhirlpool::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( Whirlpool ));
	MOAIWhirlpool_Init (( Whirlpool* )this->mAlgorithm );
}

//----------------------------------------------------------------//
ZLHashWriterWhirlpool::ZLHashWriterWhirlpool () {

	this->mAlgorithm = ( Whirlpool* )calloc ( 1, sizeof ( Whirlpool ));
}

//----------------------------------------------------------------//
ZLHashWriterWhirlpool::~ZLHashWriterWhirlpool () {
	this->Close ();
	free ( this->mAlgorithm );
}

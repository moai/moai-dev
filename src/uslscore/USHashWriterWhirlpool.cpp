// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USHashWriterWhirlpool.h>

extern "C" {
	#include <contrib/whirlpool.h>
}

//================================================================//
// USHashWriterWhirlpool
//================================================================//

//----------------------------------------------------------------//
void USHashWriterWhirlpool::FinalizeHash () {

	Whirlpool_Finalize (( Whirlpool* )this->mAlgorithm, ( u8* )this->mHash );
}

//----------------------------------------------------------------//
size_t USHashWriterWhirlpool::GetBlockSize () {
	return WBLOCKBYTES;
}

//----------------------------------------------------------------//
void* USHashWriterWhirlpool::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t USHashWriterWhirlpool::GetHashSize () {

	return DIGESTBYTES;
}

//----------------------------------------------------------------//
void USHashWriterWhirlpool::HashBytes ( const void* buffer, size_t size ) {
	
	// size in *bits*
	Whirlpool_Add (( const u8* )buffer, ( unsigned long )size << 3, ( Whirlpool* )this->mAlgorithm );
}

//----------------------------------------------------------------//
void USHashWriterWhirlpool::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( Whirlpool ));
	Whirlpool_Init (( Whirlpool* )this->mAlgorithm );
}

//----------------------------------------------------------------//
USHashWriterWhirlpool::USHashWriterWhirlpool () {

	this->mAlgorithm = ( Whirlpool* )calloc ( 1, sizeof ( Whirlpool ));
}

//----------------------------------------------------------------//
USHashWriterWhirlpool::~USHashWriterWhirlpool () {
	this->Close ();
	free ( this->mAlgorithm );
}

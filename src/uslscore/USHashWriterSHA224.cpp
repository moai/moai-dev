// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USHashWriterSHA224.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_LIBCRYPTO

#include <openssl/sha.h>

//================================================================//
// USHashWriterSHA224
//================================================================//

//----------------------------------------------------------------//
void USHashWriterSHA224::FinalizeHash () {

	SHA224_Final ( this->mHash, ( SHA256_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
size_t USHashWriterSHA224::GetBlockSize () {

	return SHA_CBLOCK;
}

//----------------------------------------------------------------//
void* USHashWriterSHA224::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t USHashWriterSHA224::GetHashSize () {

	return SHA224_DIGEST_LENGTH;
}

//----------------------------------------------------------------//
void USHashWriterSHA224::HashBytes ( const void* buffer, size_t size ) {

	SHA224_Update (( SHA256_CTX* )this->mAlgorithm, buffer, size );
}

//----------------------------------------------------------------//
void USHashWriterSHA224::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( SHA256_CTX ));
	SHA224_Init (( SHA256_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
USHashWriterSHA224::USHashWriterSHA224 () {

	this->mAlgorithm = ( SHA256_CTX* )calloc ( 1, sizeof ( SHA256_CTX ));
}

//----------------------------------------------------------------//
USHashWriterSHA224::~USHashWriterSHA224 () {

	this->Close ();
	free ( this->mAlgorithm );
}

#endif
// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USHashWriterSHA256.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_LIBCRYPTO

#include <openssl/sha.h>

//================================================================//
// USHashWriterSHA256
//================================================================//

//----------------------------------------------------------------//
void USHashWriterSHA256::FinalizeHash () {

	SHA256_Final ( this->mHash, ( SHA256_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
size_t USHashWriterSHA256::GetBlockSize () {

	return SHA_CBLOCK;
}

//----------------------------------------------------------------//
void* USHashWriterSHA256::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t USHashWriterSHA256::GetHashSize () {

	return SHA256_DIGEST_LENGTH;
}

//----------------------------------------------------------------//
void USHashWriterSHA256::HashBytes ( const void* buffer, size_t size ) {

	SHA256_Update (( SHA256_CTX* )this->mAlgorithm, buffer, size );
}

//----------------------------------------------------------------//
void USHashWriterSHA256::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( SHA256_CTX ));
	SHA256_Init (( SHA256_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
USHashWriterSHA256::USHashWriterSHA256 () {

	this->mAlgorithm = ( SHA256_CTX* )calloc ( 1, sizeof ( SHA256_CTX ));
}

//----------------------------------------------------------------//
USHashWriterSHA256::~USHashWriterSHA256 () {

	this->Close ();
	free ( this->mAlgorithm );
}

#endif
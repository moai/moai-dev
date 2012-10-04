// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <openssl/sha.h>
#include <uslscore/USHashWriterSHA224.h>

//================================================================//
// USHashWriterSHA224
//================================================================//

//----------------------------------------------------------------//
void USHashWriterSHA224::FinalizeHash () {
#if !MOAI_OS_NACL
	SHA224_Final ( this->mHash, ( SHA256_CTX* )this->mAlgorithm );
#endif
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
#if !MOAI_OS_NACL
	SHA224_Update (( SHA256_CTX* )this->mAlgorithm, buffer, size );
#endif
}

//----------------------------------------------------------------//
void USHashWriterSHA224::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( SHA256_CTX ));
#if !MOAI_OS_NACL
	SHA224_Init (( SHA256_CTX* )this->mAlgorithm );
#endif
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

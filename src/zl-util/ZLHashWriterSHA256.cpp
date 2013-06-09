// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLHashWriterSHA256.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_LIBCRYPTO

#include <openssl/sha.h>

//================================================================//
// ZLHashWriterSHA256
//================================================================//

//----------------------------------------------------------------//
void ZLHashWriterSHA256::FinalizeHash () {

	SHA256_Final ( this->mHash, ( SHA256_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
size_t ZLHashWriterSHA256::GetBlockSize () {

	return SHA_CBLOCK;
}

//----------------------------------------------------------------//
void* ZLHashWriterSHA256::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t ZLHashWriterSHA256::GetHashSize () {

	return SHA256_DIGEST_LENGTH;
}

//----------------------------------------------------------------//
void ZLHashWriterSHA256::HashBytes ( const void* buffer, size_t size ) {

	SHA256_Update (( SHA256_CTX* )this->mAlgorithm, buffer, size );
}

//----------------------------------------------------------------//
void ZLHashWriterSHA256::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( SHA256_CTX ));
	SHA256_Init (( SHA256_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
ZLHashWriterSHA256::ZLHashWriterSHA256 () {

	this->mAlgorithm = ( SHA256_CTX* )calloc ( 1, sizeof ( SHA256_CTX ));
}

//----------------------------------------------------------------//
ZLHashWriterSHA256::~ZLHashWriterSHA256 () {

	this->Close ();
	free ( this->mAlgorithm );
}

#endif
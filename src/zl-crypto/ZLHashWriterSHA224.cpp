// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/ZLHashWriterSHA224.h>
#include <openssl/sha.h>

//================================================================//
// ZLHashWriterSHA224
//================================================================//

//----------------------------------------------------------------//
void ZLHashWriterSHA224::FinalizeHash () {

	SHA224_Final ( this->mHash, ( SHA256_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
size_t ZLHashWriterSHA224::GetBlockSize () {

	return SHA_CBLOCK;
}

//----------------------------------------------------------------//
void* ZLHashWriterSHA224::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t ZLHashWriterSHA224::GetHashSize () {

	return SHA224_DIGEST_LENGTH;
}

//----------------------------------------------------------------//
void ZLHashWriterSHA224::HashBytes ( const void* buffer, size_t size ) {

	SHA224_Update (( SHA256_CTX* )this->mAlgorithm, buffer, size );
}

//----------------------------------------------------------------//
void ZLHashWriterSHA224::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( SHA256_CTX ));
	SHA224_Init (( SHA256_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
ZLHashWriterSHA224::ZLHashWriterSHA224 () {

	this->mAlgorithm = ( SHA256_CTX* )calloc ( 1, sizeof ( SHA256_CTX ));
}

//----------------------------------------------------------------//
ZLHashWriterSHA224::~ZLHashWriterSHA224 () {

	this->Close ();
	free ( this->mAlgorithm );
}

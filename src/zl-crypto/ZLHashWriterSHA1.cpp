// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/ZLHashWriterSHA1.h>
#include <openssl/sha.h>

//================================================================//
// ZLHashWriterSHA1
//================================================================//

//----------------------------------------------------------------//
void ZLHashWriterSHA1::FinalizeHash () {

	SHA1_Final ( this->mHash, ( SHA_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
size_t ZLHashWriterSHA1::GetBlockSize () {

	return SHA_CBLOCK;
}

//----------------------------------------------------------------//
void* ZLHashWriterSHA1::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t ZLHashWriterSHA1::GetHashSize () {

	return SHA_DIGEST_LENGTH;
}

//----------------------------------------------------------------//
void ZLHashWriterSHA1::HashBytes ( const void* buffer, size_t size ) {

	SHA1_Update (( SHA_CTX* )this->mAlgorithm, buffer, size );
}

//----------------------------------------------------------------//
void ZLHashWriterSHA1::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( SHA_CTX ));
	SHA1_Init (( SHA_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
ZLHashWriterSHA1::ZLHashWriterSHA1 () {

	this->mAlgorithm = ( SHA_CTX* )calloc ( 1, sizeof ( SHA_CTX ));
}

//----------------------------------------------------------------//
ZLHashWriterSHA1::~ZLHashWriterSHA1 () {

	this->Close ();
	free ( this->mAlgorithm );
}

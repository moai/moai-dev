// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/ZLHashWriterSHA512.h>
#include <openssl/sha.h>

//================================================================//
// ZLHashWriterSHA512
//================================================================//

//----------------------------------------------------------------//
void ZLHashWriterSHA512::FinalizeHash () {

	SHA512_Final ( this->mHash, ( SHA512_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
size_t ZLHashWriterSHA512::GetBlockSize () {

	return SHA512_CBLOCK;
}

//----------------------------------------------------------------//
void* ZLHashWriterSHA512::GetHash () {

	return this->mHash;
}

//----------------------------------------------------------------//
size_t ZLHashWriterSHA512::GetHashSize () {

	return SHA512_DIGEST_LENGTH;
}

//----------------------------------------------------------------//
void ZLHashWriterSHA512::HashBytes ( const void* buffer, size_t size ) {

	SHA512_Update (( SHA512_CTX* )this->mAlgorithm, buffer, size );
}

//----------------------------------------------------------------//
void ZLHashWriterSHA512::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( SHA512_CTX ));
	SHA512_Init (( SHA512_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
ZLHashWriterSHA512::ZLHashWriterSHA512 () {

	this->mAlgorithm = ( SHA512_CTX* )calloc ( 1, sizeof ( SHA512_CTX ));
}

//----------------------------------------------------------------//
ZLHashWriterSHA512::~ZLHashWriterSHA512 () {

	this->Close ();
	free ( this->mAlgorithm );
}

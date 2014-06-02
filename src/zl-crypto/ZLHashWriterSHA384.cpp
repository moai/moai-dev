// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/ZLHashWriterSHA384.h>
#include <openssl/sha.h>

//================================================================//
// ZLHashWriterSHA384
//================================================================//

//----------------------------------------------------------------//
void ZLHashWriterSHA384::FinalizeHash () {

	SHA384_Final ( this->mHash, ( SHA512_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
size_t ZLHashWriterSHA384::GetBlockSize () {

	return SHA_CBLOCK;
}

//----------------------------------------------------------------//
void* ZLHashWriterSHA384::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t ZLHashWriterSHA384::GetHashSize () {

	return SHA384_DIGEST_LENGTH;
}

//----------------------------------------------------------------//
void ZLHashWriterSHA384::HashBytes ( const void* buffer, size_t size ) {

	SHA384_Update (( SHA512_CTX* )this->mAlgorithm, buffer, size );
}

//----------------------------------------------------------------//
void ZLHashWriterSHA384::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( SHA512_CTX ));
	SHA384_Init (( SHA512_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
ZLHashWriterSHA384::ZLHashWriterSHA384 () {

	this->mAlgorithm = ( SHA512_CTX* )calloc ( 1, sizeof ( SHA512_CTX ));
}

//----------------------------------------------------------------//
ZLHashWriterSHA384::~ZLHashWriterSHA384 () {

	this->Close ();
	free ( this->mAlgorithm );
}

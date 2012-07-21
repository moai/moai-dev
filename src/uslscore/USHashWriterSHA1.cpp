// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <openssl/sha.h>
#include <uslscore/USHashWriterSHA1.h>

//================================================================//
// USHashWriterSHA1
//================================================================//

//----------------------------------------------------------------//
void USHashWriterSHA1::FinalizeHash () {

	SHA1_Final ( this->mHash, ( SHA_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
void* USHashWriterSHA1::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t USHashWriterSHA1::GetHashSize () {

	return SHA_DIGEST_LENGTH;
}

//----------------------------------------------------------------//
void USHashWriterSHA1::HashBytes ( const void* buffer, size_t size ) {

	SHA1_Update (( SHA_CTX* )this->mAlgorithm, buffer, size );
}

//----------------------------------------------------------------//
void USHashWriterSHA1::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( SHA_CTX ));
	SHA1_Init (( SHA_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
USHashWriterSHA1::USHashWriterSHA1 () {

	this->mAlgorithm = ( SHA_CTX* )calloc ( 1, sizeof ( SHA_CTX ));
}

//----------------------------------------------------------------//
USHashWriterSHA1::~USHashWriterSHA1 () {
	this->Close ();
	free ( this->mAlgorithm );
}

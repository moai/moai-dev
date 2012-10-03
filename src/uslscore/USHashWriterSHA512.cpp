// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <openssl/sha.h>
#include <uslscore/USHashWriterSHA512.h>

//================================================================//
// USHashWriterSHA512
//================================================================//

//----------------------------------------------------------------//
void USHashWriterSHA512::FinalizeHash () {
#if !MOAI_OS_NACL
	SHA512_Final ( this->mHash, ( SHA512_CTX* )this->mAlgorithm );
#endif
}

//----------------------------------------------------------------//
size_t USHashWriterSHA512::GetBlockSize () {
	return SHA512_CBLOCK;
}

//----------------------------------------------------------------//
void* USHashWriterSHA512::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t USHashWriterSHA512::GetHashSize () {

	return SHA512_DIGEST_LENGTH;
}

//----------------------------------------------------------------//
void USHashWriterSHA512::HashBytes ( const void* buffer, size_t size ) {
#if !MOAI_OS_NACL
	SHA512_Update (( SHA512_CTX* )this->mAlgorithm, buffer, size );
#endif
}

//----------------------------------------------------------------//
void USHashWriterSHA512::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( SHA512_CTX ));
#if !MOAI_OS_NACL
	SHA512_Init (( SHA512_CTX* )this->mAlgorithm );
#endif
}

//----------------------------------------------------------------//
USHashWriterSHA512::USHashWriterSHA512 () {

	this->mAlgorithm = ( SHA512_CTX* )calloc ( 1, sizeof ( SHA512_CTX ));
}

//----------------------------------------------------------------//
USHashWriterSHA512::~USHashWriterSHA512 () {
	this->Close ();
	free ( this->mAlgorithm );
}

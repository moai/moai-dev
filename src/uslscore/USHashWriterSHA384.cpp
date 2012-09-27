// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <openssl/sha.h>
#include <uslscore/USHashWriterSHA384.h>

//================================================================//
// USHashWriterSHA384
//================================================================//

//----------------------------------------------------------------//
void USHashWriterSHA384::FinalizeHash () {
#if !MOAI_OS_NACL
	SHA384_Final ( this->mHash, ( SHA512_CTX* )this->mAlgorithm );
#endif
}

//----------------------------------------------------------------//
size_t USHashWriterSHA384::GetBlockSize () {
	return SHA_CBLOCK;
}

//----------------------------------------------------------------//
void* USHashWriterSHA384::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t USHashWriterSHA384::GetHashSize () {

	return SHA384_DIGEST_LENGTH;
}

//----------------------------------------------------------------//
void USHashWriterSHA384::HashBytes ( const void* buffer, size_t size ) {
#if !MOAI_OS_NACL
	SHA384_Update (( SHA512_CTX* )this->mAlgorithm, buffer, size );
#endif
}

//----------------------------------------------------------------//
void USHashWriterSHA384::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( SHA512_CTX ));
#if !MOAI_OS_NACL
	SHA384_Init (( SHA512_CTX* )this->mAlgorithm );
#endif
}

//----------------------------------------------------------------//
USHashWriterSHA384::USHashWriterSHA384 () {

	this->mAlgorithm = ( SHA512_CTX* )calloc ( 1, sizeof ( SHA512_CTX ));
}

//----------------------------------------------------------------//
USHashWriterSHA384::~USHashWriterSHA384 () {
	this->Close ();
	free ( this->mAlgorithm );
}

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USHashWriterMD5.h>

SUPPRESS_EMPTY_FILE_WARNING
#if MOAI_WITH_LIBCRYPTO

#include <openssl/md5.h>

//================================================================//
// USHashWriterMD5
//================================================================//

//----------------------------------------------------------------//
void USHashWriterMD5::FinalizeHash () {
	
	MD5_Final ( this->mHash, ( MD5_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
size_t USHashWriterMD5::GetBlockSize () {

	return MD5_CBLOCK;
}

//----------------------------------------------------------------//
void* USHashWriterMD5::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t USHashWriterMD5::GetHashSize () {

	return MD5_DIGEST_LENGTH;
}

//----------------------------------------------------------------//
void USHashWriterMD5::HashBytes ( const void* buffer, size_t size ) {

	MD5_Update (( MD5_CTX* )this->mAlgorithm, buffer, size );
}

//----------------------------------------------------------------//
void USHashWriterMD5::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( MD5_CTX ));
	MD5_Init (( MD5_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
USHashWriterMD5::USHashWriterMD5 () {

	this->mAlgorithm = ( MD5_CTX* )calloc ( 1, sizeof ( MD5_CTX ));
}

//----------------------------------------------------------------//
USHashWriterMD5::~USHashWriterMD5 () {

	this->Close ();
	free ( this->mAlgorithm );
}

#endif
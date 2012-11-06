// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <openssl/md5.h>
#include <uslscore/USHashWriterMD5.h>

//================================================================//
// USHashWriterMD5
//================================================================//

//----------------------------------------------------------------//
void USHashWriterMD5::FinalizeHash () {
	
#if !MOAI_OS_NACL
	MD5_Final ( this->mHash, ( MD5_CTX* )this->mAlgorithm );
#endif
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

#if !MOAI_OS_NACL
	MD5_Update (( MD5_CTX* )this->mAlgorithm, buffer, size );
#endif
}

//----------------------------------------------------------------//
void USHashWriterMD5::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( MD5_CTX ));
#if !MOAI_OS_NACL
	MD5_Init (( MD5_CTX* )this->mAlgorithm );
#endif	
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

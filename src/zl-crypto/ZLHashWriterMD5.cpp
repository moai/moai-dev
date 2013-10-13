// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/ZLHashWriterMD5.h>
#include <openssl/md5.h>

//================================================================//
// ZLHashWriterMD5
//================================================================//

//----------------------------------------------------------------//
void ZLHashWriterMD5::FinalizeHash () {
	
	MD5_Final ( this->mHash, ( MD5_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
size_t ZLHashWriterMD5::GetBlockSize () {

	return MD5_CBLOCK;
}

//----------------------------------------------------------------//
void* ZLHashWriterMD5::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t ZLHashWriterMD5::GetHashSize () {

	return MD5_DIGEST_LENGTH;
}

//----------------------------------------------------------------//
void ZLHashWriterMD5::HashBytes ( const void* buffer, size_t size ) {

	MD5_Update (( MD5_CTX* )this->mAlgorithm, buffer, size );
}

//----------------------------------------------------------------//
void ZLHashWriterMD5::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mAlgorithm, 0, sizeof ( MD5_CTX ));
	MD5_Init (( MD5_CTX* )this->mAlgorithm );
}

//----------------------------------------------------------------//
ZLHashWriterMD5::ZLHashWriterMD5 () {

	this->mAlgorithm = ( MD5_CTX* )calloc ( 1, sizeof ( MD5_CTX ));
}

//----------------------------------------------------------------//
ZLHashWriterMD5::~ZLHashWriterMD5 () {

	this->Close ();
	free ( this->mAlgorithm );
}

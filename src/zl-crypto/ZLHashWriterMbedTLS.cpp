// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/ZLHashWriterMBedTLS.h>
#include <mbedtls/md.h>

//================================================================//
// ZLHashWriterMBedTLS
//================================================================//

//----------------------------------------------------------------//
void ZLHashWriterMbedTLS::FinalizeHash () {
	
//	MD5_Final ( this->mHash, ( MD5_CTX* )this->mAlgorithm );
	int res = mbedtls_md_finish((mbedtls_md_context_t *) this->mDigestContext, this->mHash);
	zl_assert(0 == res);
	mbedtls_md_free((mbedtls_md_context_t *) this->mDigestContext);
}


//----------------------------------------------------------------//
void* ZLHashWriterMbedTLS::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t ZLHashWriterMbedTLS::GetHashSize () {
	mbedtls_md_context_t *ctx = (mbedtls_md_context_t *) this->mDigestContext;
	return mbedtls_md_get_size(ctx->md_info);
}

//----------------------------------------------------------------//
void ZLHashWriterMbedTLS::HashBytes ( const void* buffer, size_t size ) {

	//MD5_Update (( MD5_CTX* )this->mAlgorithm, buffer, size );
	zl_assert(0 == mbedtls_md_update((mbedtls_md_context_t *) this->mDigestContext, (const unsigned char *) buffer, size));
}

//----------------------------------------------------------------//
void ZLHashWriterMbedTLS::InitHash () {

	memset ( &this->mHash, 0, sizeof ( this->mHash ));
	memset ( this->mDigestContext, 0, sizeof ( mbedtls_md_context_t ));
	//MD5_Init ((mbedtls_md_context_t * )this->mAlgorithm );

	mbedtls_md_context_t *ctx = (mbedtls_md_context_t *) this->mDigestContext;
	mbedtls_md_info_t *md = (mbedtls_md_info_t *) this->mDigestInfo;

	mbedtls_md_init(ctx);
	zl_assert(0 == mbedtls_md_setup(ctx, md, 0));
	zl_assert(0 == mbedtls_md_starts(ctx));
}

//----------------------------------------------------------------//
ZLHashWriterMbedTLS::ZLHashWriterMbedTLS(cc8 *algo) {
	
	//get digest info from algo param
	const mbedtls_md_info_t *md = NULL;
	md = mbedtls_md_info_from_string(algo);
	zl_assert(md);
	this->mDigestInfo = md;

	//init storage for context
	this->mDigestContext = (mbedtls_md_context_t *)calloc(1, sizeof(mbedtls_md_context_t));
	
	//init storage for output
	this->mHash = new u8[mbedtls_md_get_size(md)];
	
		//this->mAlgorithm = ( MD5_CTX* )calloc ( 1, sizeof ( MD5_CTX ));
}

//----------------------------------------------------------------//
ZLHashWriterMbedTLS::~ZLHashWriterMbedTLS() {
	this->Close ();
	delete[] this->mHash;
	free ( this->mDigestContext );
}

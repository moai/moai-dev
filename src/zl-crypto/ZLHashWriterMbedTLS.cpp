// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-crypto/ZLHashWriterMbedTLS.h>
#include <mbedtls/md.h>

//================================================================//
// ZLHashWriterMBedTLS
//================================================================//

//----------------------------------------------------------------//
void ZLHashWriterMbedTLS::FinalizeHash () {
	
	int res = mbedtls_md_finish (( mbedtls_md_context_t* )this->mDigestContext, this->mHash );
	zl_assert ( 0 == res );
}

//----------------------------------------------------------------//
void* ZLHashWriterMbedTLS::GetHash () {
	
	return this->mHash;
}

//----------------------------------------------------------------//
size_t ZLHashWriterMbedTLS::GetHashSize () {
	mbedtls_md_context_t *ctx = ( mbedtls_md_context_t* )this->mDigestContext;
	return mbedtls_md_get_size ( ctx->md_info );
}

//----------------------------------------------------------------//
void ZLHashWriterMbedTLS::HashBytes ( const void* buffer, size_t size ) {

	int res = mbedtls_md_update (( mbedtls_md_context_t* )this->mDigestContext, ( const unsigned char* ) buffer, size );
	zl_assert( 0 == res );
}

//----------------------------------------------------------------//
void ZLHashWriterMbedTLS::InitHash () {

	int res = mbedtls_md_starts (( mbedtls_md_context_t* )this->mDigestContext );
	zl_assert ( 0 == res );
}

//----------------------------------------------------------------//
ZLHashWriterMbedTLS::ZLHashWriterMbedTLS ( cc8 *algo ) {
	
	//get digest info from algo param
	const mbedtls_md_info_t* md = NULL;
	md = mbedtls_md_info_from_string ( algo );
	zl_assert ( md );
	this->mDigestInfo = md;

	//init storage for context
	mbedtls_md_context_t *ctx = ( mbedtls_md_context_t* )calloc ( 1, sizeof ( mbedtls_md_context_t ));
	memset ( ctx, 0, sizeof ( mbedtls_md_context_t ));
	this->mDigestContext = ctx;

	//init structures
	mbedtls_md_init ( ctx );
	int res = mbedtls_md_setup ( ctx, md, 0 );
	zl_assert ( 0 == res );

	//init storage for output
	this->mHash = new u8 [ mbedtls_md_get_size ( md )] ();
}

//----------------------------------------------------------------//
ZLHashWriterMbedTLS::~ZLHashWriterMbedTLS () {
	this->Close ();
	mbedtls_md_free (( mbedtls_md_context_t* )this->mDigestContext );
	delete [] this->mHash;
	free ( this->mDigestContext );
}

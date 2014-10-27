// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <zl-util/ZLBase64Encoder.h>

//================================================================//
// ZLBase64Encoder
//================================================================//

//----------------------------------------------------------------//
u32 ZLBase64Encoder::Decode ( u8* plain, const u8* crypt ) {

	this->FormatPlainBlock ( plain );
	
	if ( crypt [ 0 ] == '=' ) return 0;

	u32 bits = 0;
	
	bits += ( this->mASCIIToBits [ crypt [ 0 ]] & 0x3f ) << 18;
	bits += ( this->mASCIIToBits [ crypt [ 1 ]] & 0x3f ) << 12;
	plain [ 0 ] = ( u8 )(( bits >> 16 ) & 0xff );
	
	if ( crypt [ 2 ] == '=' ) return 1;
	
	bits += ( this->mASCIIToBits [ crypt [ 2 ]] & 0x3f ) << 6;
	plain [ 1 ] = ( u8 )(( bits >> 8 ) & 0xff );
	
	if ( crypt [ 3 ] == '=' ) return 2;
	
	bits += ( this->mASCIIToBits [ crypt [ 3 ]] & 0x3f );
	plain [ 2 ] = ( u8 )( bits & 0xff );
	
	return 3;
}

//----------------------------------------------------------------//
void ZLBase64Encoder::Encode ( u8* crypt, const u8* plain, u32 size ) {

	this->FormatCryptBlock ( crypt );

	if ( size == 0 ) return;

	u32 bits = 0;
	
	bits += plain [ 0 ] << 16;
	bits += plain [ 1 ] << 8;
	
	crypt [ 0 ] = this->mBitsToASCII [( bits >> 18 ) & 0x3f ];
	crypt [ 1 ] = this->mBitsToASCII [( bits >> 12 ) & 0x3f ];
	
	if ( size == 1 ) return;
	
	bits += plain [ 2 ];
	crypt [ 2 ] = this->mBitsToASCII [( bits >> 6 ) & 0x3f ];
	
	if ( size == 2 ) return;
	crypt [ 3 ] = this->mBitsToASCII [ bits & 0x3f ];
}

//----------------------------------------------------------------//
void ZLBase64Encoder::FormatCryptBlock ( u8* block ) {
	
	memset ( block, '=', CRYPT_BLOCK_SIZE );
}

//----------------------------------------------------------------//
void ZLBase64Encoder::FormatPlainBlock ( u8* block ) {
	
	memset ( block, 0, PLAIN_BLOCK_SIZE );
}

//----------------------------------------------------------------//
ZLBase64Encoder::ZLBase64Encoder () {

	// TODO: provide web safe alternatives, overrides
	// TODO: keep this in a static
	memcpy ( this->mBitsToASCII, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/", 64 );
	memset ( this->mASCIIToBits, 0, 256 );
	
	for ( u32 i = 0; i < 64; ++i ) {
		char ascii = this->mBitsToASCII [ i ];
		this->mASCIIToBits [( u32 )ascii ] = ( u8 )i;
	}
}

//----------------------------------------------------------------//
ZLBase64Encoder::~ZLBase64Encoder () {
}

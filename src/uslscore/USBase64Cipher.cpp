// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USBase64Cipher.h>
#include <stdio.h>

//================================================================//
// USBase64Cipher
//================================================================//

//----------------------------------------------------------------//
u32 USBase64Cipher::Decode ( const u8* crypt, u8* plain, u32 blockID ) {

	this->FormatPlainBlock ( plain, blockID );
	
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
void USBase64Cipher::Encode ( const u8* plain, u8* crypt, u32 blockID, u32 size ) {

	this->FormatCryptBlock ( crypt, blockID );

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
void USBase64Cipher::FormatCryptBlock ( u8* crypt, u32 blockID ) {
	UNUSED ( blockID );
	
	memset ( crypt, '=', CRYPT_BLOCK_SIZE );
}

//----------------------------------------------------------------//
u32 USBase64Cipher::GetCryptBlockSize () {
	return CRYPT_BLOCK_SIZE;
}

//----------------------------------------------------------------//
u32 USBase64Cipher::GetPlainBlockSize () {
	return PLAIN_BLOCK_SIZE;
}

//----------------------------------------------------------------//
USBase64Cipher::USBase64Cipher () {

	memcpy ( this->mBitsToASCII, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/", 64 );
	memset ( this->mASCIIToBits, 0, 256 );
	
	for ( u32 i = 0; i < 64; ++i ) {
		char ascii = this->mBitsToASCII [ i ];
		this->mASCIIToBits [( u32 )ascii ] = ( u8 )i;
	}
}

//----------------------------------------------------------------//
USBase64Cipher::~USBase64Cipher () {
}

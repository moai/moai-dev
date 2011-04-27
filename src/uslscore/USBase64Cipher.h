// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef BASE64CIPHER_H
#define BASE64CIPHER_H

#include <uslscore/USCipher.h>

//================================================================//
// USBase64Cipher
//================================================================//
class USBase64Cipher :
	public USCipher {
private:

	static const u32 PLAIN_BLOCK_SIZE = 3;
	static const u32 CRYPT_BLOCK_SIZE = 4;

	u8 mBitsToASCII [ 64 ];
	u8 mASCIIToBits [ 256 ];

public:

	//----------------------------------------------------------------//
	u32				Decode					( const u8* crypt, u8* plain, u32 blockID );
	void			Encode					( const u8* plain, u8* crypt, u32 blockID, u32 size );
	void			FormatCryptBlock		( u8* crypt, u32 blockID );
	u32				GetCryptBlockSize		();
	u32				GetPlainBlockSize		();
					USBase64Cipher			();
					~USBase64Cipher			();
};

#endif

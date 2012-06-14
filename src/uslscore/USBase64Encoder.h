// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef USBASE64ENCODER_H
#define USBASE64ENCODER_H

//================================================================//
// USBase64Encoder
//================================================================//
class USBase64Encoder {
private:

	u8 mBitsToASCII [ 64 ];
	u8 mASCIIToBits [ 256 ];

public:

	static const u32 PLAIN_BLOCK_SIZE = 3;
	static const u32 CRYPT_BLOCK_SIZE = 4;

	//----------------------------------------------------------------//
	u32				Decode					( u8* plain, const u8* crypt );
	void			Encode					( u8* crypt, const u8* plain, u32 size );
	void			FormatCryptBlock		( u8* block );
	void			FormatPlainBlock		( u8* block );
					USBase64Encoder			();
					~USBase64Encoder		();
};

#endif

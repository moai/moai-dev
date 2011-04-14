// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef CIPHER_H
#define CIPHER_H

#include <uslscore/USStream.h>

//================================================================//
// USCipher
//================================================================//
class USCipher {
protected:

	friend class USCipherStream;

	//----------------------------------------------------------------//
	virtual u32			Decode					( const u8* crypt, u8* plain, u32 blockID ) = 0;
	virtual void		Encode					( const u8* plain, u8* crypt, u32 blockID, u32 size ) = 0;
	virtual void		FormatCryptBlock		( u8* crypt, u32 blockID );
	virtual void		FormatPlainBlock		( u8* plain, u32 blockID );
	virtual u32			GetCryptBlockSize		() = 0;
	virtual u32			GetPlainBlockSize		() = 0;

public:
	
	//----------------------------------------------------------------//
						USCipher				();
	virtual				~USCipher				();
};

#endif

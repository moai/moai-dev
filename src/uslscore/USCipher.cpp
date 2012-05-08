// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <uslscore/USFileSys.h>
#include <uslscore/USDirectoryItr.h>
#include <uslscore/USCipher.h>
#include <stdio.h>

//================================================================//
// USCipher
//================================================================//

//----------------------------------------------------------------//
void USCipher::FormatCryptBlock ( u8* crypt, u32 blockID ) {
	UNUSED ( blockID );

	memset ( crypt, 0, this->GetCryptBlockSize ());
}

//----------------------------------------------------------------//
void USCipher::FormatPlainBlock ( u8* plain, u32 blockID ) {
	UNUSED ( blockID );

	memset ( plain, 0, this->GetPlainBlockSize ());
}

//----------------------------------------------------------------//
USCipher::USCipher () {
}

//----------------------------------------------------------------//
USCipher::~USCipher () {
}

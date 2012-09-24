// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USMD5Writer.h>

//================================================================//
// USMD5WriterNaCl
//================================================================//

//AJV TODO: This class ( or get openSSL lib crypto compiling on NaCl )

//----------------------------------------------------------------//
void USMD5Writer::Close () {

}

//----------------------------------------------------------------//
u32 USMD5Writer::GetCaps () {
	
	return 0;
}

//----------------------------------------------------------------//
u8* USMD5Writer::GetHash () {

	return NULL;
}

//----------------------------------------------------------------//
size_t USMD5Writer::GetCursor () {
	
	return 0;
}

//----------------------------------------------------------------//
size_t USMD5Writer::GetLength () {

	return 0;
}

//----------------------------------------------------------------//
bool USMD5Writer::Open ( USStream& stream ) {
	
	return false;
}

//----------------------------------------------------------------//
USMD5Writer::USMD5Writer () {

}

//----------------------------------------------------------------//
USMD5Writer::~USMD5Writer () {
	
}

//----------------------------------------------------------------//
size_t USMD5Writer::WriteBytes ( const void* buffer, size_t size ) {
	
	return 0;
}

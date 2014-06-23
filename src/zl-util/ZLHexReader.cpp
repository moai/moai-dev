// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLHexReader.h>

//================================================================//
// ZLHexReader
//================================================================//

//----------------------------------------------------------------//
void ZLHexReader::Close () {
	
	if ( this->mInputStream ) {
		this->mCursor = 0;
		this->mInputStream = 0;
	}
}

//----------------------------------------------------------------//
u32 ZLHexReader::GetCaps () {

	return this->mInputStream ? CAN_READ : 0;
}

//----------------------------------------------------------------//
size_t ZLHexReader::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
size_t ZLHexReader::GetDecodedLength ( size_t encodedLength ) {

	return encodedLength >> 1;
}

//----------------------------------------------------------------//
size_t ZLHexReader::GetLength () {

	return this->mCursor;
}

//----------------------------------------------------------------//
u8 ZLHexReader::HexToByte ( u32 c ) {

	if (( c >= '0' ) && ( c <= '9' )) return ( u8 )( c - '0' );
	if (( c >= 'a' ) && ( c <= 'f' )) return ( u8 )( c + 10 - 'a' );
	if (( c >= 'A' ) && ( c <= 'F' )) return ( u8 )( c + 10 - 'A' );

	return 0xff;
}

//----------------------------------------------------------------//
bool ZLHexReader::Open ( ZLStream& stream ) {

	this->Close ();

	this->mInputStream = &stream;
	return true;
}

//----------------------------------------------------------------//
size_t ZLHexReader::ReadBytes ( void* buffer, size_t size ) {
	
	if ( !this->mInputStream ) return 0;
	
	char hexByte [ 2 ];
	
	u8* bytes = ( u8* )buffer;
	for ( u32 i = 0; i < size; ++i, ++this->mCursor ) {
		
		size_t result = this->mInputStream->ReadBytes ( hexByte, 2 );
		if ( result != 2 ) {
			return i;
		}
		
		u32 hi = ZLHexReader::HexToByte ( hexByte [ 0 ]);
		u32 lo = ZLHexReader::HexToByte ( hexByte [ 1 ]);
		
		bytes [ i ] = ( u8 )(( hi << 4 ) + lo );
	}
	return size;
}

//----------------------------------------------------------------//
ZLHexReader::ZLHexReader () :
	mInputStream ( 0 ),
	mCursor ( 0 ) {
}

//----------------------------------------------------------------//
ZLHexReader::~ZLHexReader () {

	this->Close ();
}

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLHexAdapter.h>

//================================================================//
// ZLHexAdapter
//================================================================//

//----------------------------------------------------------------//
size_t ZLHexAdapter::EstimateDecodedLength ( size_t encodedLength ) {

	return encodedLength >> 1;
}

//----------------------------------------------------------------//
size_t ZLHexAdapter::EstimateEncodedLength ( size_t plainLength ) {

	return plainLength << 1;
}

//----------------------------------------------------------------//
u32 ZLHexAdapter::GetCaps () {

	return this->mProxiedStream ? ( CAN_READ | CAN_WRITE ) : 0;
}

//----------------------------------------------------------------//
u8 ZLHexAdapter::HexToByte ( u32 c ) {

	if (( c >= '0' ) && ( c <= '9' )) return ( u8 )( c - '0' );
	if (( c >= 'a' ) && ( c <= 'f' )) return ( u8 )( c + 10 - 'a' );
	if (( c >= 'A' ) && ( c <= 'F' )) return ( u8 )( c + 10 - 'A' );

	return 0xff;
}

//----------------------------------------------------------------//
size_t ZLHexAdapter::ReadBytes ( void* buffer, size_t size ) {
	
	if ( !this->mProxiedStream ) return 0;
	
	char hexByte [ 2 ];
	
	u8* bytes = ( u8* )buffer;
	for ( u32 i = 0; i < size; ++i, ++this->mCursor ) {
		
		size_t result = this->mProxiedStream->ReadBytes ( hexByte, 2 );
		if ( result != 2 ) {
			return i;
		}
		
		u32 hi = ZLHexAdapter::HexToByte ( hexByte [ 0 ]);
		u32 lo = ZLHexAdapter::HexToByte ( hexByte [ 1 ]);
		
		bytes [ i ] = ( u8 )(( hi << 4 ) + lo );
	}
	return size;
}

//----------------------------------------------------------------//
size_t ZLHexAdapter::WriteBytes ( const void* buffer, size_t size ) {
	
	if ( !this->mProxiedStream ) return 0;
	
	const char* hexTable = "0123456789abcdef";
	char hexByte [ 2 ];
	
	const u8* bytes = ( u8* )buffer;
	for ( u32 i = 0; i < size; ++i, ++this->mCursor ) {
	
		u32 byte = bytes [ i ];
		
		hexByte [ 0 ] = hexTable [( byte >> 4 ) & 0x0f ];
		hexByte [ 1 ] = hexTable [ byte & 0x0f ];
		
		size_t result = this->mProxiedStream->WriteBytes ( hexByte, 2 );
		if ( result != 2 ) {
			return i;
		}
	}
	
	if ( this->mLength < this->mCursor ) {
		this->mLength = this->mCursor;
	}
	return size;
}

//----------------------------------------------------------------//
ZLHexAdapter::ZLHexAdapter () {
}

//----------------------------------------------------------------//
ZLHexAdapter::~ZLHexAdapter () {
}

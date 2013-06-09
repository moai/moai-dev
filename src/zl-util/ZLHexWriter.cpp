// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <zl-util/ZLHexWriter.h>

//================================================================//
// ZLHexWriter
//================================================================//

//----------------------------------------------------------------//
void ZLHexWriter::Close () {
	
	if ( this->mOutputStream ) {
		this->mCursor = 0;
		this->mOutputStream = 0;
	}
}

//----------------------------------------------------------------//
u32 ZLHexWriter::GetCaps () {

	return this->mOutputStream ? CAN_WRITE : 0;
}

//----------------------------------------------------------------//
size_t ZLHexWriter::GetCursor () {

	return this->mCursor;
}

//----------------------------------------------------------------//
size_t ZLHexWriter::GetLength () {

	return this->mCursor;
}

//----------------------------------------------------------------//
bool ZLHexWriter::Open ( ZLStream& stream ) {

	this->Close ();

	this->mOutputStream = &stream;
	return true;
}

//----------------------------------------------------------------//
ZLHexWriter::ZLHexWriter () :
	mOutputStream ( 0 ),
	mCursor ( 0 ) {
}

//----------------------------------------------------------------//
ZLHexWriter::~ZLHexWriter () {

	this->Close ();
}

//----------------------------------------------------------------//
size_t ZLHexWriter::WriteBytes ( const void* buffer, size_t size ) {
	
	if ( !this->mOutputStream ) return 0;
	
	const char* hexTable = "0123456789abcdef";
	char hexByte [ 2 ];
	
	const u8* bytes = ( u8* )buffer;
	for ( u32 i = 0; i < size; ++i, ++this->mCursor ) {
	
		u32 byte = bytes [ i ];
		
		hexByte [ 0 ] = hexTable [( byte >> 4 ) & 0x0f ];
		hexByte [ 1 ] = hexTable [ byte & 0x0f ];
		
		size_t result = this->mOutputStream->WriteBytes ( hexByte, 2 );
		if ( result != 2 ) {
			return i;
		}
	}
	return size;
}

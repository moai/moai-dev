// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPkmHeader.h>

#define BIG_ENDIAN_2_BYTE_TO_U16(x) ( x [ 0 ] << 8 | x [ 1 ] )

//================================================================//
// MOAIPkmHeader
//================================================================//

//----------------------------------------------------------------//
u32 MOAIPkmHeader::GetDataSize () {

	u16 dataWidth = BIG_ENDIAN_2_BYTE_TO_U16 ( this->mDataWidth );
	u16 dataHeight = BIG_ENDIAN_2_BYTE_TO_U16 ( this->mDataHeight );
	return dataWidth * dataHeight / 2;
}

//----------------------------------------------------------------//
u32 MOAIPkmHeader::GetHeaderSize () {

	return 16;
}

//----------------------------------------------------------------//
void* MOAIPkmHeader::GetHeaderStart () {

	return &this->mPKM;
}

//----------------------------------------------------------------//
u32 MOAIPkmHeader::GetHeight () {

	return BIG_ENDIAN_2_BYTE_TO_U16 ( this->mHeight );
}

//----------------------------------------------------------------//
u16 MOAIPkmHeader::GetType () {

	return BIG_ENDIAN_2_BYTE_TO_U16 ( this->mType );
}

//----------------------------------------------------------------//
u32 MOAIPkmHeader::GetWidth () {

	return BIG_ENDIAN_2_BYTE_TO_U16 ( this->mWidth );
}

//----------------------------------------------------------------//
void MOAIPkmHeader::Invalidate () {

	this->mPKM = 0;
}

//----------------------------------------------------------------//
bool MOAIPkmHeader::IsValid () {

	return this->mPKM == PKM_FILE_MAGIC && this->mVersion == PKM_VERSION_1;
}

//----------------------------------------------------------------//
MOAIPkmHeader::MOAIPkmHeader () {

	this->Invalidate ();
}

//----------------------------------------------------------------//
MOAIPkmHeader::~MOAIPkmHeader () {
}

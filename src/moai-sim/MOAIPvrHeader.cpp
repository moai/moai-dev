// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPvrHeader.h>

//================================================================//
// MOAIPvrHeader
//================================================================//

//----------------------------------------------------------------//
u32 MOAIPvrHeader::GetDataSize () {

	return this->mDataSize;
}

//----------------------------------------------------------------//
u32 MOAIPvrHeader::GetHeaderSize () {

	return 52;
}

//----------------------------------------------------------------//
void* MOAIPvrHeader::GetHeaderStart () {

	return &this->mHeaderSize;
}

//----------------------------------------------------------------//
u32 MOAIPvrHeader::GetHeight () {

	return this->mHeight;
}

//----------------------------------------------------------------//
u32 MOAIPvrHeader::GetWidth () {

	return this->mWidth;
}

//----------------------------------------------------------------//
void MOAIPvrHeader::Invalidate () {

	this->mPVR = 0;
}

//----------------------------------------------------------------//
bool MOAIPvrHeader::IsValid () {

	return this->mPVR == PVR_FILE_MAGIC;
}

//----------------------------------------------------------------//
MOAIPvrHeader::MOAIPvrHeader () {

	this->Invalidate ();
}

//----------------------------------------------------------------//
MOAIPvrHeader::~MOAIPvrHeader () {
}

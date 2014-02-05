// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAITextureData.h>

//================================================================//
// MOAITextureData
//================================================================//

//----------------------------------------------------------------//
void MOAITextureData::Clear () {

	this->mData.Clear ();
	this->mFormat = FORMAT_INVALID;
}

//----------------------------------------------------------------//
u8* MOAITextureData::GetData () {

	return this->mData.Data() + this->mHeaders [ this->mFormat ]->GetHeaderSize ();
}

//----------------------------------------------------------------//
u32 MOAITextureData::GetHeight () {

	return this->mHeaders [ this->mFormat ]->GetHeight ();
}

//----------------------------------------------------------------//
MOAIPkmHeader* MOAITextureData::GetPkmHeader () {

	assert ( this->mFormat == FORMAT_PKM );
	return &this->mPkmHeader;
}

//----------------------------------------------------------------//
MOAIPvrHeader* MOAITextureData::GetPvrHeader () {
	assert ( this->mFormat == FORMAT_PVR );
	return &this->mPvrHeader;
}

//----------------------------------------------------------------//
u32 MOAITextureData::GetWidth () {

	return this->mHeaders [ this->mFormat ]->GetWidth ();
}

//----------------------------------------------------------------//
bool MOAITextureData::IsValid () {

	return this->mFormat != FORMAT_INVALID;
}

//----------------------------------------------------------------//
MOAITextureData::MOAITextureData () :
	mFormat ( FORMAT_INVALID ) {
	
	this->mHeaders [ FORMAT_INVALID ] = &this->mInvalidHeader;
	this->mHeaders [ FORMAT_PKM ] = &this->mPkmHeader;
	this->mHeaders [ FORMAT_PVR ] = &this->mPvrHeader;
}

//----------------------------------------------------------------//
MOAITextureData::~MOAITextureData () {
}

//----------------------------------------------------------------//
void MOAITextureData::Init ( ZLStream& stream ) {

	this->Clear ();

	for ( u32 format = FORMAT_INVALID + 1; format < TOTAL_FORMATS; ++format ) {
		MOAITextureHeader* header = this->mHeaders [ format ];

		header->Load ( stream );
		
		if ( header->IsValid ()) {
			this->mFormat = format;
			break;
		}
	}

	if ( this->mFormat != FORMAT_INVALID ) {
		u32 size = this->mHeaders [ this->mFormat ]->GetTotalSize ();
		this->mData.Init ( size );
		u32 actualSize = stream.ReadBytes ( this->mData.Data (), this->mData.Size ());

		if ( actualSize != size ) {
			this->Clear ();
		}
	}
}

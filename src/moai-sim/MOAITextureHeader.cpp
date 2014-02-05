// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAITextureHeader.h>

//================================================================//
// MOAITextureHeader
//================================================================//

//----------------------------------------------------------------//
u32 MOAITextureHeader::GetTotalSize () {

	return this->GetHeaderSize () + this->GetDataSize ();
}

//----------------------------------------------------------------//
void MOAITextureHeader::Load ( ZLStream& stream ) {

	this->Invalidate ();
	stream.PeekBytes ( this->GetHeaderStart (), this->GetHeaderSize ());
}

//----------------------------------------------------------------//
MOAITextureHeader::MOAITextureHeader () {
}

//----------------------------------------------------------------//
MOAITextureHeader::~MOAITextureHeader () {
}

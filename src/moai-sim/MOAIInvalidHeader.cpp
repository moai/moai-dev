// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIInvalidHeader.h>

//================================================================//
// MOAIInvalidHeader
//================================================================//

//----------------------------------------------------------------//
u32 MOAIInvalidHeader::GetDataSize () {

	return 0;
}

//----------------------------------------------------------------//
u32 MOAIInvalidHeader::GetHeaderSize () {

	return 0;
}

//----------------------------------------------------------------//
void* MOAIInvalidHeader::GetHeaderStart () {

	return this;
}

//----------------------------------------------------------------//
u32 MOAIInvalidHeader::GetHeight () {

	return 0;
}

//----------------------------------------------------------------//
u32 MOAIInvalidHeader::GetWidth () {

	return 0;
}

//----------------------------------------------------------------//
void MOAIInvalidHeader::Invalidate () {
}

//----------------------------------------------------------------//
bool MOAIInvalidHeader::IsValid () {

	return false;
}

//----------------------------------------------------------------//
MOAIInvalidHeader::MOAIInvalidHeader () {
}

//----------------------------------------------------------------//
MOAIInvalidHeader::~MOAIInvalidHeader () {
}

// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIShaderUniformDescriptor.h>

//================================================================//
// MOAIShaderUniformDescriptor
//================================================================//

//----------------------------------------------------------------//
size_t MOAIShaderUniformDescriptor::GetSize () const {

	return this->mCount * this->mWidth * ELEMENT_SIZE;
}

//----------------------------------------------------------------//
bool MOAIShaderUniformDescriptor::Init ( u32 type, u32 width, u32 count ) {
	
	this->mType			= type;
	this->mWidth		= width;
	this->mCount		= count;
	this->mCPUOffset	= 0;
	
	return true;
}

//----------------------------------------------------------------//
MOAIShaderUniformDescriptor::MOAIShaderUniformDescriptor () :
	mType ( UNIFORM_TYPE_FLOAT ),
	mWidth ( 1 ),
	mCount ( 0 ),
	mCPUOffset ( 0 ) {
}

//----------------------------------------------------------------//
MOAIShaderUniformDescriptor::~MOAIShaderUniformDescriptor () {
}

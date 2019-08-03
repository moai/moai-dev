// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIShaderUniformDescriptor.h>

//================================================================//
// MOAIShaderUniformDescriptor
//================================================================//

//----------------------------------------------------------------//
size_t MOAIShaderUniformDescriptor::GetSize () const {

	return this->mWidth * ELEMENT_SIZE;
}

//----------------------------------------------------------------//
bool MOAIShaderUniformDescriptor::Init ( u32 type, u32 width ) {
	
	this->mType		= type;
	this->mWidth	= width;
	
	return true;
}

//----------------------------------------------------------------//
MOAIShaderUniformDescriptor::MOAIShaderUniformDescriptor () :
	mType ( UNIFORM_TYPE_FLOAT ),
	mWidth ( 1 ) {
}

//----------------------------------------------------------------//
MOAIShaderUniformDescriptor::~MOAIShaderUniformDescriptor () {
}

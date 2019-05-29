// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIShaderUniform.h>

//================================================================//
// MOAIShaderUniform
//================================================================//

//----------------------------------------------------------------//
size_t MOAIShaderUniform::GetSize () const {

	return this->mWidth * ELEMENT_SIZE;
}

//----------------------------------------------------------------//
bool MOAIShaderUniform::Init ( u32 type, u32 width ) {
	
	this->mType		= type;
	this->mWidth	= width;
	
	return true;
}

//----------------------------------------------------------------//
MOAIShaderUniform::MOAIShaderUniform () :
	mType ( UNIFORM_TYPE_FLOAT ),
	mWidth ( 1 ) {
}

//----------------------------------------------------------------//
MOAIShaderUniform::~MOAIShaderUniform () {
}

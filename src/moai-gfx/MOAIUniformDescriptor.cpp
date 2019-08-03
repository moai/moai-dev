// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIUniformDescriptor.h>

//================================================================//
// MOAIUniformDescriptor
//================================================================//

//----------------------------------------------------------------//
ZLSize MOAIUniformDescriptor::GetBufferSize () const {

	return this->mCount * this->mWidth * COMPONENT_SIZE;
}

//----------------------------------------------------------------//
ZLSize MOAIUniformDescriptor::GetComponentSize () const {

	return COMPONENT_SIZE;
}

//----------------------------------------------------------------//
ZLSize MOAIUniformDescriptor::GetUniformSize () const {

	return this->mWidth * COMPONENT_SIZE;
}

//----------------------------------------------------------------//
bool MOAIUniformDescriptor::Init ( u32 type, u32 width, u32 count ) {
	
	this->mType			= type;
	this->mWidth		= width;
	this->mCount		= count;
	this->mCPUOffset	= 0;
	
	return true;
}

//----------------------------------------------------------------//
MOAIUniformDescriptor::MOAIUniformDescriptor () :
	mType ( UNIFORM_TYPE_FLOAT ),
	mWidth ( 1 ),
	mCount ( 0 ),
	mCPUOffset ( 0 ) {
}

//----------------------------------------------------------------//
MOAIUniformDescriptor::~MOAIUniformDescriptor () {
}

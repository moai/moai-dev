// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIShaderUniform.h>

//================================================================//
// MOAIShaderUniformBase
//================================================================//

//----------------------------------------------------------------//
size_t MOAIShaderUniformBase::GetSize () const {

	return this->mWidth * ELEMENT_SIZE;
}

//----------------------------------------------------------------//
bool MOAIShaderUniformBase::Init ( u32 type, u32 width ) {
	
	this->mType		= type;
	this->mWidth	= width;
	
	return true;
}

//----------------------------------------------------------------//
MOAIShaderUniformBase::MOAIShaderUniformBase () :
	mType ( UNIFORM_TYPE_FLOAT ),
	mWidth ( 1 ) {
}

//----------------------------------------------------------------//
MOAIShaderUniformBase::~MOAIShaderUniformBase () {
}

//================================================================//
// MOAIShaderUniform
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniform::Bind ( const void* buffer ) const {

	if ( this->mGPUBase == ZGL_INVALID_UNIFORM_ADDR ) return;

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {

		gfx.UniformFloat ( this->mGPUBase, 0, this->mWidth, this->mCount, ( float* )buffer );
	}
	else if ( this->mType == UNIFORM_TYPE_INT ) {
		
		gfx.UniformInt ( this->mGPUBase, 0, this->mWidth, this->mCount, ( s32* )buffer );
	}
}

//----------------------------------------------------------------//
size_t MOAIShaderUniform::GetSize () const {

	return this->MOAIShaderUniformBase::GetSize () * this->mCount;
}

//----------------------------------------------------------------//
void MOAIShaderUniform::Init ( u32 type, u32 width, u32 count ) {
	
	this->MOAIShaderUniformHandle::Init ( type, width );
	this->mCount = count;
	this->mCPUOffset = 0;
}

//----------------------------------------------------------------//
MOAIShaderUniform::MOAIShaderUniform () :
	mCount ( 0 ),
	mGPUBase (( u32 )-1 ) {
}

//----------------------------------------------------------------//
MOAIShaderUniform::~MOAIShaderUniform () {
}

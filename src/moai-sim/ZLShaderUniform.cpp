// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/ZLShaderUniform.h>

//================================================================//
// ZLShaderUniformBase
//================================================================//

//----------------------------------------------------------------//
size_t ZLShaderUniformBase::GetSize () const {

	return this->mWidth * ELEMENT_SIZE;
}

//----------------------------------------------------------------//
bool ZLShaderUniformBase::Init ( u32 type, u32 width ) {
	
	this->mType		= type;
	this->mWidth	= width;
	
	return true;
}

//----------------------------------------------------------------//
ZLShaderUniformBase::ZLShaderUniformBase () :
	mType ( UNIFORM_TYPE_FLOAT ),
	mWidth ( 1 ) {
}

//----------------------------------------------------------------//
ZLShaderUniformBase::~ZLShaderUniformBase () {
}

//================================================================//
// ZLShaderUniform
//================================================================//

//----------------------------------------------------------------//
void ZLShaderUniform::Bind ( const void* buffer ) const {

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
size_t ZLShaderUniform::GetSize () const {

	return this->ZLShaderUniformBase::GetSize () * this->mCount;
}

//----------------------------------------------------------------//
void ZLShaderUniform::Init ( u32 type, u32 width, u32 count ) {
	
	this->ZLShaderUniformBase::Init ( type, width );
	this->mCount = count;
	this->mCPUOffset = 0;
}

//----------------------------------------------------------------//
ZLShaderUniform::ZLShaderUniform () :
	mCount ( 0 ),
	mGPUBase (( u32 )-1 ) {
}

//----------------------------------------------------------------//
ZLShaderUniform::~ZLShaderUniform () {
}

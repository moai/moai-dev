// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIColor.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIShaderUniform.h>

//================================================================//
// MOAIShaderUniform
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniform::Bind ( MOAIShaderUniformInstance& instance ) {

	if ( this->mGPUBase == ZGL_INVALID_UNIFORM_ADDR ) return;

	ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {

		gfx.UniformFloat ( this->mGPUBase, 0, this->mWidth, instance.mCount, ( float* )instance.mBuffer );
	}
	else if ( this->mType == UNIFORM_TYPE_INT ) {
		
		gfx.UniformInt ( this->mGPUBase, 0, this->mWidth, instance.mCount, ( s32* )instance.mBuffer );
	}
}

//----------------------------------------------------------------//
void MOAIShaderUniform::Default ( void* buffer ) const {

	this->MOAIShaderUniformFormatter::Default ( buffer, this->mCount );
}

//----------------------------------------------------------------//
void MOAIShaderUniform::Init ( u32 type, u32 width, u32 count ) {
	
	this->MOAIShaderUniformFormatter::Init ( type, width );
	this->mCount = count;
}

//----------------------------------------------------------------//
MOAIShaderUniform::MOAIShaderUniform () :
	mCount ( 0 ) {
}

//----------------------------------------------------------------//
MOAIShaderUniform::~MOAIShaderUniform () {
}

// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIShaderUniformGL.h>

//================================================================//
// MOAIShaderUniformGL
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniformGL::Bind ( ZLGfx&gfx, const void* buffer ) const {

	if ( this->mGPUBase == ZGL_INVALID_UNIFORM_ADDR ) return;

	if ( this->mType == UNIFORM_TYPE_FLOAT ) {

		gfx.UniformFloat ( this->mGPUBase, 0, this->mWidth, this->mCount, ( float* )buffer );
	}
	else if ( this->mType == UNIFORM_TYPE_INT ) {
		
		gfx.UniformInt ( this->mGPUBase, 0, this->mWidth, this->mCount, ( s32* )buffer );
	}
}

//----------------------------------------------------------------//
size_t MOAIShaderUniformGL::GetSize () const {

	return this->MOAIShaderUniform::GetSize () * this->mCount;
}

//----------------------------------------------------------------//
void MOAIShaderUniformGL::Init ( u32 type, u32 width, u32 count ) {
	
	this->MOAIShaderUniform::Init ( type, width );
	this->mCount = count;
	this->mCPUOffset = 0;
}

//----------------------------------------------------------------//
MOAIShaderUniformGL::MOAIShaderUniformGL () :
	mCount ( 0 ),
	mGPUBase (( u32 )-1 ) {
}

//----------------------------------------------------------------//
MOAIShaderUniformGL::~MOAIShaderUniformGL () {
}

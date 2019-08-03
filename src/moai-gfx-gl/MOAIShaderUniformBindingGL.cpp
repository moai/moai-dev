// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIShaderUniformBindingGL.h>

//================================================================//
// MOAIShaderUniformBindingGL
//================================================================//

//----------------------------------------------------------------//
void MOAIShaderUniformBindingGL::Bind ( ZLGfx&gfx, const MOAIShaderUniformDescriptor& descriptor, const void* buffer ) const {

	if ( this->mGPUBase == ZGL_INVALID_UNIFORM_ADDR ) return;

	if ( descriptor.mType == MOAIShaderUniformDescriptor::UNIFORM_TYPE_FLOAT ) {

		gfx.UniformFloat ( this->mGPUBase, 0, descriptor.mWidth, this->mCount, ( float* )buffer );
	}
	else if ( descriptor.mType == MOAIShaderUniformDescriptor::UNIFORM_TYPE_INT ) {
		
		gfx.UniformInt ( this->mGPUBase, 0, descriptor.mWidth, this->mCount, ( s32* )buffer );
	}
}

//----------------------------------------------------------------//
size_t MOAIShaderUniformBindingGL::GetSize ( const MOAIShaderUniformDescriptor& descriptor ) const {

	return descriptor.GetSize () * this->mCount;
}

//----------------------------------------------------------------//
void MOAIShaderUniformBindingGL::Init ( u32 count ) {
	
	this->mCount = count;
	this->mCPUOffset = 0;
}

//----------------------------------------------------------------//
MOAIShaderUniformBindingGL::MOAIShaderUniformBindingGL () :
	mCount ( 0 ),
	mGPUBase (( u32 )-1 ) {
}

//----------------------------------------------------------------//
MOAIShaderUniformBindingGL::~MOAIShaderUniformBindingGL () {
}

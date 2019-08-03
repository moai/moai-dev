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
		gfx.UniformFloat ( this->mGPUBase, 0, descriptor.mWidth, descriptor.mCount, ( float* )buffer );
	}
	else if ( descriptor.mType == MOAIShaderUniformDescriptor::UNIFORM_TYPE_INT ) {
		gfx.UniformInt ( this->mGPUBase, 0, descriptor.mWidth, descriptor.mCount, ( s32* )buffer );
	}
}

//----------------------------------------------------------------//
MOAIShaderUniformBindingGL::MOAIShaderUniformBindingGL () :
	mGPUBase (( u32 )-1 ) {
}

//----------------------------------------------------------------//
MOAIShaderUniformBindingGL::~MOAIShaderUniformBindingGL () {
}

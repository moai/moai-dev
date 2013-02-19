// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIBlendMode.h>

//================================================================//
// MOAIBlendMode
//================================================================//

//----------------------------------------------------------------//
void MOAIBlendMode::Bind () {
	
	glEnable ( GL_BLEND );
	glBlendEquation ( this->mEquation );
	glBlendFunc ( this->mSourceFactor, this->mDestFactor );
}

//----------------------------------------------------------------//
void MOAIBlendMode::GetBlendFactors ( u32 blend, int& srcFactor, int& dstFactor ) {

	switch ( blend ) {
		case BLEND_NORMAL: {
			srcFactor = GL_ONE;
			dstFactor = GL_ONE_MINUS_SRC_ALPHA;
			break;
		}
		case BLEND_ADD: {
			srcFactor = GL_SRC_ALPHA;
			dstFactor = GL_ONE;
			break;
		}
		case BLEND_MULTIPLY: {
			srcFactor = GL_DST_COLOR;
			dstFactor = GL_ZERO;
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIBlendMode::SetBlend ( u32 blend ) {
	
	MOAIBlendMode::GetBlendFactors ( blend, this->mSourceFactor, this->mDestFactor );
	this->mEquation = GL_FUNC_ADD;
}

//----------------------------------------------------------------//
void MOAIBlendMode::SetBlend ( int srcFactor, int dstFactor ) {

	// GL_ZERO
	// GL_ONE
	// GL_DST_COLOR
	// GL_ONE_MINUS_DST_COLOR
	// GL_SRC_ALPHA
	// GL_ONE_MINUS_SRC_ALPHA
	// GL_DST_ALPHA
	// GL_ONE_MINUS_DST_ALPHA
	// GL_SRC_ALPHA_SATURATE
	this->mSourceFactor = srcFactor;
	
	// GL_ZERO
	// GL_ONE
	// GL_SRC_COLOR
	// GL_ONE_MINUS_SRC_COLOR
	// GL_SRC_ALPHA
	// GL_ONE_MINUS_SRC_ALPHA
	// GL_DST_ALPHA
	// GL_ONE_MINUS_DST_ALPHA
	this->mDestFactor = dstFactor;
}

//----------------------------------------------------------------//
void MOAIBlendMode::SetBlendEquation ( int equation ) {
	// GL_FUNC_ADD
	// GL_FUNC_SUBTRACT
	// GL_FUNC_REVERSE_SUBTRACT
	// (GL_MIN and GL_MAX don't exist on iOS)
	this->mEquation = equation;
}

//----------------------------------------------------------------//
MOAIBlendMode::MOAIBlendMode () :
	mSourceFactor ( GL_ONE ),
	mEquation ( GL_FUNC_ADD ),
	mDestFactor ( GL_ONE_MINUS_SRC_ALPHA ) {
}

//----------------------------------------------------------------//
MOAIBlendMode::~MOAIBlendMode () {
}

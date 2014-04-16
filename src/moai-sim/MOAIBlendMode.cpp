// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIBlendMode.h>

//================================================================//
// MOAIBlendMode
//================================================================//

//----------------------------------------------------------------//
void MOAIBlendMode::Bind () {
	
	zglEnable ( ZGL_PIPELINE_BLEND );
	zglBlendMode ( this->mEquation );
	zglBlendFunc ( this->mSourceFactor, this->mDestFactor );
}

//----------------------------------------------------------------//
void MOAIBlendMode::GetBlendFactors ( u32 blend, int& srcFactor, int& dstFactor ) {

	switch ( blend ) {
		case BLEND_NORMAL: {
			srcFactor = ZGL_BLEND_FACTOR_ONE;
			dstFactor = ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			break;
		}
		case BLEND_ADD: {
			srcFactor = ZGL_BLEND_FACTOR_SRC_ALPHA;
			dstFactor = ZGL_BLEND_FACTOR_ONE;
			break;
		}
		case BLEND_MULTIPLY: {
			srcFactor = ZGL_BLEND_FACTOR_DST_COLOR;
			dstFactor = ZGL_BLEND_FACTOR_ZERO;
			break;
		}
	}
}

//----------------------------------------------------------------//
void MOAIBlendMode::SetBlend ( u32 blend ) {
	
	MOAIBlendMode::GetBlendFactors ( blend, this->mSourceFactor, this->mDestFactor );
	this->mEquation = ZGL_BLEND_MODE_ADD;
}

//----------------------------------------------------------------//
void MOAIBlendMode::SetBlend ( int srcFactor, int dstFactor ) {

	// ZGL_BLEND_FACTOR_ZERO
	// ZGL_BLEND_FACTOR_ONE
	// ZGL_BLEND_FACTOR_DST_COLOR
	// ZGL_BLEND_FACTOR_ONE_MINUS_DST_COLOR
	// ZGL_BLEND_FACTOR_SRC_ALPHA
	// ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA
	// ZGL_BLEND_FACTOR_DST_ALPHA
	// ZGL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA
	// ZGL_BLEND_FACTOR_SRC_ALPHA_SATURATE
	this->mSourceFactor = srcFactor;
	
	// ZGL_BLEND_FACTOR_ZERO
	// ZGL_BLEND_FACTOR_ONE
	// ZGL_BLEND_FACTOR_SRC_COLOR
	// ZGL_BLEND_FACTOR_ONE_MINUS_SRC_COLOR
	// ZGL_BLEND_FACTOR_SRC_ALPHA
	// ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA
	// ZGL_BLEND_FACTOR_DST_ALPHA
	// ZGL_BLEND_FACTOR_ONE_MINUS_DST_ALPHA
	this->mDestFactor = dstFactor;
}

//----------------------------------------------------------------//
void MOAIBlendMode::SetBlendEquation ( int equation ) {
	// ZGL_BLEND_MODE_ADD
	// ZGL_BLEND_MODE_SUBTRACT
	// ZGL_BLEND_MODE_REVERSE_SUBTRACT
	// (ZGL_BLEND_MODE_MIN and GL_BLEND_MODE_MAX unsupported on iOS)
	this->mEquation = equation;
}

//----------------------------------------------------------------//
MOAIBlendMode::MOAIBlendMode () :
	mEquation ( ZGL_BLEND_MODE_ADD ),
	mSourceFactor ( ZGL_BLEND_FACTOR_ONE ),
	mDestFactor ( ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA ) {
}

//----------------------------------------------------------------//
MOAIBlendMode::~MOAIBlendMode () {
}

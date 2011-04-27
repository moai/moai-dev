// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslsext/USBlendMode.h>

//================================================================//
// USBlendMode
//================================================================//

//----------------------------------------------------------------//
void USBlendMode::Bind () {
	
	glEnable ( GL_BLEND );
	glBlendFunc ( this->mSourceFactor, this->mDestFactor );
}

//----------------------------------------------------------------//
void USBlendMode::GetBlendFactors ( u32 blend, int& srcFactor, int& dstFactor ) {

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
void USBlendMode::SetBlend ( u32 blend ) {
	
	USBlendMode::GetBlendFactors ( blend, this->mSourceFactor, this->mDestFactor );
}

//----------------------------------------------------------------//
void USBlendMode::SetBlend ( int srcFactor, int dstFactor ) {

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
USBlendMode::USBlendMode () :
	mSourceFactor ( GL_ONE ),
	mDestFactor ( GL_ONE_MINUS_SRC_ALPHA ) {
}

//----------------------------------------------------------------//
USBlendMode::~USBlendMode () {
}

// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIBlendMode.h>
#include <moai-sim/MOAIGfxMgr.h>

//================================================================//
// MOAIBlendMode
//================================================================//

//----------------------------------------------------------------//
void MOAIBlendMode::Init ( MOAILuaState& state, int idx ) {

	u32 materialID = 0;
	u32 stackSize = state.GetStackSize ( idx );

	if (( stackSize == 2 ) || ( stackSize == 4 )) {
		materialID = state.GetValue < u32 >( idx++, 1 ) - 1;
	}

	MOAIBlendMode blendMode;

	if ( state.CheckParams ( idx, "NNN" )) {

		u32 equation	= state.GetValue < u32 >( idx++, ZGL_BLEND_MODE_ADD );
		u32 srcFactor	= state.GetValue < u32 >( idx++, 0 );
		u32 dstFactor	= state.GetValue < u32 >( idx, 0 );
		
		this->SetBlend ( equation, srcFactor, dstFactor );
	}
	else {
		
		u32 preset = state.GetValue < u32 >( idx, MOAIBlendMode::BLEND_NORMAL );
		this->SetBlend ( preset );
	}
}

//----------------------------------------------------------------//
int MOAIBlendMode::Push ( MOAILuaState& state ) const {

	state.Push ( this->mEquation );
	state.Push ( this->mSourceFactor );
	state.Push ( this->mDestFactor );
	
	return 3;
}

//----------------------------------------------------------------//
void MOAIBlendMode::SetBlend ( u32 blend ) {
	
	switch ( blend ) {
	
		case BLEND_NORMAL:
			this->mEquation			= ZGL_BLEND_MODE_ADD;
			this->mSourceFactor		= ZGL_BLEND_FACTOR_ONE;
			this->mDestFactor		= ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			break;

		case BLEND_ADD:
			this->mEquation			= ZGL_BLEND_MODE_ADD;
			this->mSourceFactor		= ZGL_BLEND_FACTOR_SRC_ALPHA;
			this->mDestFactor		= ZGL_BLEND_FACTOR_ONE;
			break;

		case BLEND_MULTIPLY:
			this->mEquation			= ZGL_BLEND_MODE_ADD;
			this->mSourceFactor		= ZGL_BLEND_FACTOR_DST_COLOR;
			this->mDestFactor		= ZGL_BLEND_FACTOR_ZERO;
			break;
	}
}

//----------------------------------------------------------------//
void MOAIBlendMode::SetBlend ( int equation, int srcFactor, int dstFactor ) {

	// ZGL_BLEND_MODE_ADD
	// ZGL_BLEND_MODE_SUBTRACT
	// ZGL_BLEND_MODE_REVERSE_SUBTRACT
	// (ZGL_BLEND_MODE_MIN and GL_BLEND_MODE_MAX unsupported on iOS)
	this->mEquation = equation;

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
MOAIBlendMode::MOAIBlendMode () :
	mEquation ( ZGL_BLEND_MODE_ADD ),
	mSourceFactor ( ZGL_BLEND_FACTOR_ONE ),
	mDestFactor ( ZGL_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA ) {
}

//----------------------------------------------------------------//
MOAIBlendMode::~MOAIBlendMode () {
}

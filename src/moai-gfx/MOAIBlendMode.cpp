// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIBlendMode.h>

//================================================================//
// MOAIBlendMode
//================================================================//

////----------------------------------------------------------------//
//void MOAIBlendMode::Init ( MOAILuaState& state, int idx ) {
//
//	u32 equation	= state.GetValue < u32 >( idx++, MOAIBlendFunc::ADD );
//	u32 srcFactor	= state.GetValue < u32 >( idx++, MOAIBlendFactor::ONE );
//	u32 dstFactor	= state.GetValue < u32 >( idx, MOAIBlendFactor::ONE_MINUS_SRC_ALPHA );
//	
//	this->SetBlend ( equation, srcFactor, dstFactor );
//}
//
////----------------------------------------------------------------//
//int MOAIBlendMode::Push ( MOAILuaState& state ) const {
//
//	state.Push ( this->mEquation );
//	state.Push ( this->mSourceFactor );
//	state.Push ( this->mDestFactor );
//	
//	return 3;
//}

//----------------------------------------------------------------//
void MOAIBlendMode::SetBlend ( MOAIBlendFunc::Type equation, MOAIBlendFactor::Type srcFactor, MOAIBlendFactor::Type dstFactor ) {

	// MOAIBlendFunc::ADD
	// MOAIBlendFunc::SUBTRACT
	// MOAIBlendFunc::REVERSE_SUBTRACT
	// (MOAIBlendFunc::MIN and GL_BLEND_MODE_MAX unsupported on iOS)
	this->mEquation = equation;

	// MOAIBlendFactor::ZERO
	// MOAIBlendFactor::ONE
	// MOAIBlendFactor::DST_COLOR
	// MOAIBlendFactor::ONE_MINUS_DST_COLOR
	// MOAIBlendFactor::SRC_ALPHA
	// MOAIBlendFactor::ONE_MINUS_SRC_ALPHA
	// MOAIBlendFactor::DST_ALPHA
	// MOAIBlendFactor::ONE_MINUS_DST_ALPHA
	// MOAIBlendFactor::SRC_ALPHA_SATURATE
	this->mSourceFactor = srcFactor;
	
	// MOAIBlendFactor::ZERO
	// MOAIBlendFactor::ONE
	// MOAIBlendFactor::SRC_COLOR
	// MOAIBlendFactor::ONE_MINUS_SRC_COLOR
	// MOAIBlendFactor::SRC_ALPHA
	// MOAIBlendFactor::ONE_MINUS_SRC_ALPHA
	// MOAIBlendFactor::DST_ALPHA
	// MOAIBlendFactor::ONE_MINUS_DST_ALPHA
	this->mDestFactor = dstFactor;
}

//----------------------------------------------------------------//
MOAIBlendMode::MOAIBlendMode () :
	mEquation ( MOAIBlendFunc::ADD ),
	mSourceFactor ( MOAIBlendFactor::ONE ),
	mDestFactor ( MOAIBlendFactor::ONE_MINUS_SRC_ALPHA ) {
}

//----------------------------------------------------------------//
MOAIBlendMode::MOAIBlendMode ( MOAIBlendFunc::Type equation, MOAIBlendFactor::Type srcFactor, MOAIBlendFactor::Type dstFactor ) :
	mEquation ( equation ),
	mSourceFactor ( srcFactor ),
	mDestFactor ( dstFactor ) {
}

//----------------------------------------------------------------//
MOAIBlendMode::~MOAIBlendMode () {
}

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
//	u32 equation	= state.GetValue < u32 >( idx++, MOAIBlendFuncEnum::ADD );
//	u32 srcFactor	= state.GetValue < u32 >( idx++, MOAIBlendFactorEnum::ONE );
//	u32 dstFactor	= state.GetValue < u32 >( idx, MOAIBlendFactorEnum::ONE_MINUS_SRC_ALPHA );
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
void MOAIBlendMode::SetBlend ( MOAIBlendFuncEnum::_ equation, MOAIBlendFactorEnum::_ srcFactor, MOAIBlendFactorEnum::_ dstFactor ) {

	// MOAIBlendFuncEnum::ADD
	// MOAIBlendFuncEnum::SUBTRACT
	// MOAIBlendFuncEnum::REVERSE_SUBTRACT
	// (MOAIBlendFuncEnum::MIN and GL_BLEND_MODE_MAX unsupported on iOS)
	this->mEquation = equation;

	// MOAIBlendFactorEnum::ZERO
	// MOAIBlendFactorEnum::ONE
	// MOAIBlendFactorEnum::DST_COLOR
	// MOAIBlendFactorEnum::ONE_MINUS_DST_COLOR
	// MOAIBlendFactorEnum::SRC_ALPHA
	// MOAIBlendFactorEnum::ONE_MINUS_SRC_ALPHA
	// MOAIBlendFactorEnum::DST_ALPHA
	// MOAIBlendFactorEnum::ONE_MINUS_DST_ALPHA
	// MOAIBlendFactorEnum::SRC_ALPHA_SATURATE
	this->mSourceFactor = srcFactor;
	
	// MOAIBlendFactorEnum::ZERO
	// MOAIBlendFactorEnum::ONE
	// MOAIBlendFactorEnum::SRC_COLOR
	// MOAIBlendFactorEnum::ONE_MINUS_SRC_COLOR
	// MOAIBlendFactorEnum::SRC_ALPHA
	// MOAIBlendFactorEnum::ONE_MINUS_SRC_ALPHA
	// MOAIBlendFactorEnum::DST_ALPHA
	// MOAIBlendFactorEnum::ONE_MINUS_DST_ALPHA
	this->mDestFactor = dstFactor;
}

//----------------------------------------------------------------//
MOAIBlendMode::MOAIBlendMode () :
	mEquation ( MOAIBlendFuncEnum::ADD ),
	mSourceFactor ( MOAIBlendFactorEnum::ONE ),
	mDestFactor ( MOAIBlendFactorEnum::ONE_MINUS_SRC_ALPHA ) {
}

//----------------------------------------------------------------//
MOAIBlendMode::MOAIBlendMode ( MOAIBlendFuncEnum::_ equation, MOAIBlendFactorEnum::_ srcFactor, MOAIBlendFactorEnum::_ dstFactor ) :
	mEquation ( equation ),
	mSourceFactor ( srcFactor ),
	mDestFactor ( dstFactor ) {
}

//----------------------------------------------------------------//
MOAIBlendMode::~MOAIBlendMode () {
}

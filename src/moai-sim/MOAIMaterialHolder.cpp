// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIMaterialHolder.h>
#include <moai-sim/MOAIPartitionHull.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAITextureBase.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIMaterialBatchHolder
//================================================================//

//----------------------------------------------------------------//
MOAIMaterialHolder::MOAIMaterialHolder () {
	
//	RTTI_BEGIN
//		RTTI_EXTEND ( MOAILuaObject )
//	RTTI_END
}

//----------------------------------------------------------------//
MOAIMaterialHolder::~MOAIMaterialHolder () {
}

//----------------------------------------------------------------//
void MOAIMaterialHolder::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIMaterialHolder::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIMaterialHolderMixIn < MOAIMaterialHolder >::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIMaterialHolder::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMaterialHolder::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

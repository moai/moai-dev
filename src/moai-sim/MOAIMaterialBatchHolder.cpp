// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIImage.h>
#include <moai-sim/MOAIMaterialBatch.h>
#include <moai-sim/MOAIMaterialBatchHolder.h>
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
MOAIMaterialBatchHolder::MOAIMaterialBatchHolder () {
	
//	RTTI_BEGIN
//		RTTI_EXTEND ( MOAILuaObject )
//	RTTI_END
}

//----------------------------------------------------------------//
MOAIMaterialBatchHolder::~MOAIMaterialBatchHolder () {
}

//----------------------------------------------------------------//
void MOAIMaterialBatchHolder::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIMaterialBatchHolder::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIMaterialBatchHolderMixIn < MOAIMaterialBatchHolder >::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIMaterialBatchHolder::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

//----------------------------------------------------------------//
void MOAIMaterialBatchHolder::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	UNUSED ( state );
	UNUSED ( serializer );
}

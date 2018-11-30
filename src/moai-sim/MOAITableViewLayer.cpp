// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAITableViewLayer.h>
#include <moai-sim/MOAIMaterialMgr.h>
#include <moai-sim/MOAIPartitionHolder.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAITransform.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// local
//==========================================================s======//

//----------------------------------------------------------------//

//================================================================//
// MOAITableViewLayer
//================================================================//

//----------------------------------------------------------------//
MOAITableViewLayer::MOAITableViewLayer () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAITableLayer )
		RTTI_EXTEND ( MOAIViewLayer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITableViewLayer::~MOAITableViewLayer () {
}

//----------------------------------------------------------------//
void MOAITableViewLayer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITableLayer::RegisterLuaClass ( state );
	MOAIViewLayer::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITableViewLayer::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAITableLayer::RegisterLuaFuncs ( state );
	MOAIViewLayer::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAITableViewLayer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {

	MOAITableLayer::SerializeIn ( state, serializer );
	MOAIViewLayer::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAITableViewLayer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {

	MOAITableLayer::SerializeOut ( state, serializer );
	MOAIViewLayer::SerializeOut ( state, serializer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAIDrawable_Draw ( int subPrimID ) {

	this->MOAIViewLayer::MOAIDrawable_Draw ( subPrimID );
}

//----------------------------------------------------------------//
void MOAITableViewLayer::MOAIViewLayer_Draw () {
	
	MOAIDrawable::Draw ( this->mRenderTable );
	
	if ( MOAIDebugLinesMgr::Get ().IsVisible () && this->mShowDebugLines ) {
		MOAIDrawable::Draw ( this->mRenderTable, true );
	}
}

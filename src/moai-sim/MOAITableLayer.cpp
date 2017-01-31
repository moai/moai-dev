// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIDeck.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAITableLayer.h>
#include <moai-sim/MOAIMaterialStackMgr.h>
#include <moai-sim/MOAIPartitionHolder.h>
#include <moai-sim/MOAIPartitionResultBuffer.h>
#include <moai-sim/MOAIPartitionResultMgr.h>
#include <moai-sim/MOAIRenderMgr.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAITextureBase.h>
#include <moai-sim/MOAITransform.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIViewProj.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getRenderTable
	@text	Get the table of renderables to be drawn.
	
	@in		MOAITableLayer self
	@out	table renderTable
*/
int MOAITableLayer::_getRenderTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITableLayer, "U" )
	
	state.Push ( self->mRenderTable );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setTable
	@text	Set or clear the table of renderables to be drawn.
	
	@in		MOAITableLayer self
	@opt	table renderTable
	@out	nil
*/
int MOAITableLayer::_setRenderTable ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAITableLayer, "U" )
	
	self->mRenderTable.SetRef ( *self, state, 2 );
	return 0;
}

//================================================================//
// MOAITableLayer
//================================================================//

//----------------------------------------------------------------//
MOAITableLayer::MOAITableLayer () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAILayerBase )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITableLayer::~MOAITableLayer () {
}

//----------------------------------------------------------------//
void MOAITableLayer::RegisterLuaClass ( MOAILuaState& state ) {

	MOAILayerBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAITableLayer::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAILayerBase::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getRenderTable",				_getRenderTable },
		{ "setRenderTable",				_setRenderTable },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITableLayer::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAILayerBase::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAITableLayer::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAILayerBase::SerializeOut ( state, serializer );
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
void MOAITableLayer::MOAILayerBase_Draw ( int subPrimID ) {
	UNUSED ( subPrimID );
	
	MOAIDrawable::Draw ( this->mRenderTable );
	
	if ( MOAIDebugLinesMgr::Get ().IsVisible () && this->mShowDebugLines ) {
		MOAIDrawable::Draw ( this->mRenderTable, true );
	}
}

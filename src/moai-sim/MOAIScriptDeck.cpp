// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIScriptDeck.h>
#include <moai-sim/MOAIDeckRemapper.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxDevice.h>
#include <moai-sim/MOAIGrid.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAISurfaceSampler2D.h>
#include <moai-sim/MOAITransform.h>
#include <moai-sim/MOAIVertexFormatMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setDrawCallback
	@text	Sets the callback to be issued when draw events occur.
			The callback's parameters are ( number index, number xOff, number yOff, number xScale, number yScale ).

	@in		MOAIScriptDeck self
	@in		function callback
	@out	nil
*/
int MOAIScriptDeck::_setDrawCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptDeck, "UF" )
	
	self->mOnDraw.SetRef ( *self, state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRect
	@text	Set the model space dimensions of the deck's default rect.
	
	@in		MOAIScriptDeck self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
int MOAIScriptDeck::_setRect ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptDeck, "UNNNN" )
	
	self->mRect = state.GetRect < float >( 2 );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setRectCallback
	@text	Sets the callback to be issued when the size of a deck item
			needs to be determined.
			The callback's parameters are ( number index ).

	@in		MOAIScriptDeck self
	@in		function callback
	@out	nil
*/
int MOAIScriptDeck::_setRectCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptDeck, "UF" )
	
	self->mOnRect.SetRef ( *self, state, 2 );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setTotalRectCallback
	@text	Sets the callback to be issued when the size of a deck item
			needs to be determined.
			The callback's parameters are ( ).

	@in		MOAIScriptDeck self
	@in		function callback
	@out	nil
*/
int MOAIScriptDeck::_setTotalRectCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptDeck, "UF" )
	
	self->mOnTotalRect.SetRef ( *self, state, 2 );
	self->SetBoundsDirty ();
	return 0;
}

//================================================================//
// MOAIScriptDeck
//================================================================//

//----------------------------------------------------------------//
ZLBox MOAIScriptDeck::ComputeMaxBounds () {

	ZLRect rect = this->mRect;

	if ( this->mOnTotalRect ) {
	
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mOnTotalRect.PushRef ( state )) {
		
			state.DebugCall ( 0, 4 );
			
			rect.mXMin = state.GetValue < float >( -4, 0.0f );
			rect.mYMin = state.GetValue < float >( -3, 0.0f );
			rect.mXMax = state.GetValue < float >( -2, 0.0f );
			rect.mYMax = state.GetValue < float >( -1, 0.0f );
			
			rect.Bless ();
		}
	}
	
	ZLBox bounds;
	bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
	return bounds;
}

//----------------------------------------------------------------//
void MOAIScriptDeck::DrawIndex ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	UNUSED ( zOff );
	UNUSED ( zScl );
	
	if ( this->mOnDraw ) {
	
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mOnDraw.PushRef ( state )) {
	
			MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
			gfxDevice.SetVertexPreset ( MOAIVertexFormatMgr::XYZWC );
			
			gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
			gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		
			// TODO: fix this to take all offset/scale params
			lua_pushnumber ( state, idx );
			lua_pushnumber ( state, xOff );
			lua_pushnumber ( state, yOff );
			lua_pushnumber ( state, xScl );
			lua_pushnumber ( state, yScl );
			state.DebugCall ( 5, 0 );
		}
	}
}

//----------------------------------------------------------------//
ZLBox MOAIScriptDeck::GetItemBounds ( u32 idx ) {
	
	ZLRect rect = this->mRect;
	
	if ( this->mOnRect ) {
	
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mOnRect.PushRef ( state )) {
		
			lua_pushnumber ( state, idx );
			state.DebugCall ( 1, 4 );
			
			rect.mXMin = state.GetValue < float >( -4, 0.0f );
			rect.mYMin = state.GetValue < float >( -3, 0.0f );
			rect.mXMax = state.GetValue < float >( -2, 0.0f );
			rect.mYMax = state.GetValue < float >( -1, 0.0f );
			
			rect.Bless ();
		}
	}
	
	ZLBox bounds;
	bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
	return bounds;
}

//----------------------------------------------------------------//
MOAIScriptDeck::MOAIScriptDeck () {
	
	RTTI_SINGLE ( MOAIDeck )
	
	this->mRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mDefaultShaderID = MOAIShaderMgr::LINE_SHADER;
}

//----------------------------------------------------------------//
MOAIScriptDeck::~MOAIScriptDeck () {
}

//----------------------------------------------------------------//
void MOAIScriptDeck::RegisterLuaClass ( MOAILuaState& state ) {

	this->MOAIDeck::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIScriptDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	this->MOAIDeck::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setDrawCallback",		_setDrawCallback },
		{ "setRect",				_setRect },
		{ "setRectCallback",		_setRectCallback },
		{ "setTotalRectCallback",	_setTotalRectCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
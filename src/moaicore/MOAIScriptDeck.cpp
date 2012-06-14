// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIScriptDeck.h>
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIShader.h>
#include <moaicore/MOAIShaderMgr.h>
#include <moaicore/MOAISurfaceSampler2D.h>
#include <moaicore/MOAITransform.h>
#include <moaicore/MOAIVertexFormatMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	setDrawCallback
	@text	Sets the callback to be issued when draw events occur.
			The callback's parameters are ( number index, number xOff, number yOff, number xScale, number yScale ).

	@in		MOAIScriptDeck self
	@in		function callback
	@out	nil
*/
int MOAIScriptDeck::_setDrawCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptDeck, "UF" )
	
	self->SetLocal ( state, 2, self->mOnDraw );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setRect
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
/**	@name	setRectCallback
	@text	Sets the callback to be issued when the size of a deck item
			needs to be determined.
			The callback's parameters are ( number index ).

	@in		MOAIScriptDeck self
	@in		function callback
	@out	nil
*/
int MOAIScriptDeck::_setRectCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptDeck, "UF" )
	
	self->SetLocal ( state, 2, self->mOnRect );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTotalRectCallback
	@text	Sets the callback to be issued when the size of a deck item
			needs to be determined.
			The callback's parameters are ( ).

	@in		MOAIScriptDeck self
	@in		function callback
	@out	nil
*/
int MOAIScriptDeck::_setTotalRectCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIScriptDeck, "UF" )
	
	self->SetLocal ( state, 2, self->mOnTotalRect );
	self->SetBoundsDirty ();
	return 0;
}

//================================================================//
// MOAIScriptDeck
//================================================================//

//----------------------------------------------------------------//
USBox MOAIScriptDeck::ComputeMaxBounds () {

	USRect rect = this->mRect;

	if ( this->mOnTotalRect ) {
	
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, this->mOnTotalRect );
		
		state.DebugCall ( 0, 4 );
		
		rect.mXMin = state.GetValue < float >( -4, 0.0f );
		rect.mYMin = state.GetValue < float >( -3, 0.0f );
		rect.mXMax = state.GetValue < float >( -2, 0.0f );
		rect.mYMax = state.GetValue < float >( -1, 0.0f );
		
		rect.Bless ();
	}
	
	USBox bounds;
	bounds.Init ( rect.mXMin, rect.mYMax, rect.mXMax, rect.mYMin, 0.0f, 0.0f );	
	return bounds;
}

//----------------------------------------------------------------//
void MOAIScriptDeck::DrawIndex ( u32 idx, float xOff, float yOff, float zOff, float xScl, float yScl, float zScl ) {
	UNUSED ( zOff );
	UNUSED ( zScl );
	
	if ( this->mOnDraw ) {
	
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		gfxDevice.SetVertexPreset ( MOAIVertexFormatMgr::XYZWC );
		
		gfxDevice.SetVertexMtxMode ( MOAIGfxDevice::VTX_STAGE_MODEL, MOAIGfxDevice::VTX_STAGE_PROJ );
		gfxDevice.SetUVMtxMode ( MOAIGfxDevice::UV_STAGE_MODEL, MOAIGfxDevice::UV_STAGE_TEXTURE );
		
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, this->mOnDraw );
		
		// TODO: fix this to take all offset/scale params
		lua_pushnumber ( state, idx );
		lua_pushnumber ( state, xOff );
		lua_pushnumber ( state, yOff );
		lua_pushnumber ( state, xScl );
		lua_pushnumber ( state, yScl );
		state.DebugCall ( 5, 0 );
	}
}

//----------------------------------------------------------------//
USBox MOAIScriptDeck::GetItemBounds ( u32 idx ) {
	
	USRect rect = this->mRect;
	
	if ( this->mOnRect ) {
	
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, this->mOnRect );
		
		lua_pushnumber ( state, idx );
		state.DebugCall ( 1, 4 );
		
		rect.mXMin = state.GetValue < float >( -4, 0.0f );
		rect.mYMin = state.GetValue < float >( -3, 0.0f );
		rect.mXMax = state.GetValue < float >( -2, 0.0f );
		rect.mYMax = state.GetValue < float >( -1, 0.0f );
		
		rect.Bless ();
	}
	
	USBox bounds;
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
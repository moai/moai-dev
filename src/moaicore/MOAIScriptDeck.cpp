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
	return 0;
}

//================================================================//
// MOAIScriptDeck
//================================================================//

//----------------------------------------------------------------//
void MOAIScriptDeck::DrawPatch ( u32 idx, float xOff, float yOff, float xScale, float yScale ) {
	
	if ( this->mOnDraw ) {
	
		MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
		gfxDevice.SetVertexPreset ( MOAIVertexFormatMgr::XYZWC );
	
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, this->mOnDraw );
		
		lua_pushnumber ( state, idx );
		lua_pushnumber ( state, xOff );
		lua_pushnumber ( state, yOff );
		lua_pushnumber ( state, xScale );
		lua_pushnumber ( state, yScale );
		state.DebugCall ( 5, 0 );
	}
}

//----------------------------------------------------------------//
USRect MOAIScriptDeck::GetRect () {

	if ( this->mOnTotalRect ) {
	
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, this->mOnTotalRect );
		
		state.DebugCall ( 0, 4 );
		
		USRect rect;
		rect.mXMin = state.GetValue < float >( -4, 0.0f );
		rect.mYMin = state.GetValue < float >( -3, 0.0f );
		rect.mXMax = state.GetValue < float >( -2, 0.0f );
		rect.mYMax = state.GetValue < float >( -1, 0.0f );
		
		rect.Bless ();
		return rect;
	}
	return this->mRect;
}

//----------------------------------------------------------------//
USRect MOAIScriptDeck::GetRect ( u32 idx, MOAIDeckRemapper* remapper ) {
	
	if ( this->mOnRect ) {
	
		idx = remapper ? remapper->Remap ( idx ) : idx;
	
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		this->PushLocal ( state, this->mOnRect );
		
		lua_pushnumber ( state, idx );
		state.DebugCall ( 1, 4 );
		
		USRect rect;
		rect.mXMin = state.GetValue < float >( -4, 0.0f );
		rect.mYMin = state.GetValue < float >( -3, 0.0f );
		rect.mXMax = state.GetValue < float >( -2, 0.0f );
		rect.mYMax = state.GetValue < float >( -1, 0.0f );
		
		rect.Bless ();
		return rect;
	}
	return this->mRect;
}

//----------------------------------------------------------------//
MOAIGfxState* MOAIScriptDeck::GetShaderDefault () {

	return &MOAIShaderMgr::Get ().GetShader ( MOAIShaderMgr::LINE_SHADER );
}

//----------------------------------------------------------------//
MOAIScriptDeck::MOAIScriptDeck () {
	
	RTTI_SINGLE ( MOAIDeck2D )
	
	this->mRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
}

//----------------------------------------------------------------//
MOAIScriptDeck::~MOAIScriptDeck () {
}

//----------------------------------------------------------------//
void MOAIScriptDeck::RegisterLuaClass ( MOAILuaState& state ) {

	this->MOAIDeck2D::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIScriptDeck::RegisterLuaFuncs ( MOAILuaState& state ) {

	this->MOAIDeck2D::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setDrawCallback",		_setDrawCallback },
		{ "setRect",				_setRect },
		{ "setRectCallback",		_setRectCallback },
		{ "setTotalRectCallback",	_setTotalRectCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
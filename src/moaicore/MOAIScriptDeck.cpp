// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIScriptDeck.h>
#include <moaicore/MOAIDeckRemapper.h>
#include <moaicore/MOAIDraw.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGrid.h>
#include <moaicore/MOAILogMessages.h>
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
	
	self->mOnDraw.SetRef ( state, 2, false );
	
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
	
	self->mOnRect.SetRef ( state, 2, false );
	
	return 0;
}

//================================================================//
// MOAIScriptDeck
//================================================================//

//----------------------------------------------------------------//
bool MOAIScriptDeck::Bind () {

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();
	
	gfxDevice.SetTexture ();
	gfxDevice.SetVertexPreset ( MOAIVertexFormatMgr::XYC );

	return true;
}

//----------------------------------------------------------------//
void MOAIScriptDeck::DrawPatch ( u32 idx, float xOff, float yOff, float xScale, float yScale ) {
	
	if ( this->mOnDraw ) {
	
		USLuaStateHandle state = this->mOnDraw.GetSelf ();
		lua_pushnumber ( state, idx );
		lua_pushnumber ( state, xOff );
		lua_pushnumber ( state, yOff );
		lua_pushnumber ( state, xScale );
		lua_pushnumber ( state, yScale );
		state.DebugCall ( 5, 0 );
	}
}

//----------------------------------------------------------------//
USRect MOAIScriptDeck::GetBounds ( u32 idx, MOAIDeckRemapper* remapper ) {
	
	if ( this->mOnRect ) {
	
		idx = remapper ? remapper->Remap ( idx ) : idx;
	
		USLuaStateHandle state = this->mOnRect.GetSelf ();
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
void MOAIScriptDeck::LoadShader () {

	if ( this->mShader ) {
		MOAIGfxDevice::Get ().SetShader ( this->mShader );
	}
	else {
		MOAIShaderMgr::Get ().BindShader ( MOAIShaderMgr::LINE_SHADER );
	}
}

//----------------------------------------------------------------//
MOAIScriptDeck::MOAIScriptDeck () {
	
	RTTI_SINGLE ( MOAIDeck2D )
}

//----------------------------------------------------------------//
MOAIScriptDeck::~MOAIScriptDeck () {
}

//----------------------------------------------------------------//
void MOAIScriptDeck::RegisterLuaClass ( USLuaState& state ) {

	this->MOAIDeck2D::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIScriptDeck::RegisterLuaFuncs ( USLuaState& state ) {

	this->MOAIDeck2D::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setDrawCallback",		_setDrawCallback },
		{ "setRect",				_setRect },
		{ "setRectCallback",		_setRectCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
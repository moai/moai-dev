// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIProp.h>
#include <moaicore/MOAIRenderable.h>
#include <moaicore/MOAIRenderMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderMgr::_getRenderTable ( lua_State* L ) {
	MOAILuaState state ( L );
	state.Push ( MOAIRenderMgr::Get ().mRenderTable );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIRenderMgr::_setRenderTable ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAIRenderMgr::Get ().mRenderTable.SetStrongRef ( state, 1 );
	return 0;
}

//================================================================//
// MOAIRenderMgr
//================================================================//

//----------------------------------------------------------------//
MOAIRenderMgr::MOAIRenderMgr () :
	mRenderCounter ( 0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIRenderMgr::~MOAIRenderMgr () {
}

//----------------------------------------------------------------//
void MOAIRenderMgr::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getRenderTable",				_getRenderTable },
		{ "setRenderTable",				_setRenderTable },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIRenderMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIRenderMgr::Render () {

	this->mRenderCounter++;

	MOAIGfxDevice& gfxDevice = MOAIGfxDevice::Get ();

	gfxDevice.BeginDrawing ();
	
	if ( this->mRenderTable ) {
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mRenderTable );
		this->RenderTable ( state, -1 );
		state.Pop ( 1 );
	}
	
	gfxDevice.Flush ();
	gfxDevice.ProcessDeleters ();
}

//----------------------------------------------------------------//
void MOAIRenderMgr::RenderTable ( MOAILuaState& state, int idx ) {

	idx = state.AbsIndex ( idx );

	int n = 1;
	while ( n ) {
		
		lua_rawgeti ( state, idx, n++ );
		
		int valType = lua_type ( state, -1 );
			
		if ( valType == LUA_TUSERDATA ) {
			MOAIRenderable* renderable = state.GetLuaObject < MOAIRenderable >( -1 );
			if ( renderable ) {
				renderable->Render ();
			}
		}
		else if ( valType == LUA_TTABLE ) {
			this->RenderTable ( state, -1 );
		}
		else {
			n = 0;
		}
		
		lua_pop ( state, 1 );
	}
}
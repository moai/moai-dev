// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractDrawable.h>
#include <moai-gfx/MOAIAbstractRenderMgr.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractRenderMgr::_getRenderCount ( lua_State* L ) {

	MOAIAbstractRenderMgr& gfxMgr = MOAIAbstractRenderMgr::Get ();
	lua_pushnumber ( L, gfxMgr.mRenderCounter );

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractRenderMgr::_getRender ( lua_State* L ) {
	MOAILuaState state ( L );
	state.Push ( MOAIAbstractRenderMgr::Get ().mRenderRoot );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractRenderMgr::_setRender ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAIAbstractRenderMgr::Get ().mRenderRoot.SetRef ( state, 1 );
	return 0;
}

//================================================================//
// MOAIAbstractRenderMgr
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractRenderMgr::MOAIAbstractRenderMgr () :
	mRenderCounter ( 0 ),
	mRenderDuration ( 1.0 / 60.0 ),
	mRenderTime ( 0.0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIAbstractRenderMgr::~MOAIAbstractRenderMgr () {
}

//----------------------------------------------------------------//
void MOAIAbstractRenderMgr::PushDrawable ( MOAILuaObject* drawable ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	state.Push ( this->mRenderRoot );
	
	if ( !state.IsType ( -1, LUA_TTABLE )) {
		state.Pop ();
		lua_newtable ( state );
		this->mRenderRoot.SetRef ( state, -1 );
	}
	
	int top = ( int )state.GetTableSize ( -1 );
	state.SetField ( -1, top + 1, drawable );
}

//----------------------------------------------------------------//
void MOAIAbstractRenderMgr::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getRenderCount",				_getRenderCount },
		{ "getRender",					_getRender },
		{ "setRender",					_setRender },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAbstractRenderMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIAbstractRenderMgr::Render () {

	this->MOAIAbstractRenderMgr_Render ();
}

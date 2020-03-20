// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractDrawable.h>
#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIGfxMgr_RenderTree.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxMgr_RenderTree::_getRenderCount ( lua_State* L ) {

	MOAIGfxMgr_RenderTree& gfxMgr = MOAIGfxMgr::Get ();
	lua_pushnumber ( L, gfxMgr.mRenderCounter );

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxMgr_RenderTree::_getRender ( lua_State* L ) {
	MOAILuaState state ( L );
	state.Push ( MOAIGfxMgr::Get ().mRenderRoot );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxMgr_RenderTree::_setRender ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAIGfxMgr::Get ().mRenderRoot.SetRef ( state, 1 );
	return 0;
}

//================================================================//
// MOAIGfxMgr_RenderTree
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgr_RenderTree::MOAIGfxMgr_RenderTree () :
	mRenderCounter ( 0 ),
	mRenderDuration ( 1.0 / 60.0 ),
	mRenderTime ( 0.0 ) {
}

//----------------------------------------------------------------//
MOAIGfxMgr_RenderTree::~MOAIGfxMgr_RenderTree () {
}

//----------------------------------------------------------------//
void MOAIGfxMgr_RenderTree::PushDrawable ( MOAILuaObject* drawable ) {

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
void MOAIGfxMgr_RenderTree::Render () {

	this->MOAIGfxMgr_RenderTree_Render ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgr_RenderTree::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getRenderCount",				_getRenderCount },
		{ "getRender",					_getRender },
		{ "setRender",					_setRender },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxMgr_RenderTree::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

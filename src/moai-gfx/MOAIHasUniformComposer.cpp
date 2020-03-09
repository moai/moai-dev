// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIMaterialMgr.h>
#include <moai-gfx/MOAIHasUniformComposer.h>
#include <moai-gfx/MOAIUniformComposer.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasUniformComposer::_getComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasUniformComposer, "U" )
	
	state.Push (( MOAIUniformComposer* )self->mComposer );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIHasUniformComposer::_setComposer ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIHasUniformComposer, "U" )

	self->mComposer = state.GetLuaObject < MOAIUniformComposer >( 2, true );
	return 0;
}

//================================================================//
// MOAIHasUniformComposer
//================================================================//

//----------------------------------------------------------------//
bool MOAIHasUniformComposer::HasComposer () {

	return ( this->GetComposer () != NULL );
}

//----------------------------------------------------------------//
MOAIHasUniformComposer::MOAIHasUniformComposer () {

	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIHasUniformComposer )
		RTTI_EXTEND ( MOAIUniformComposerInterface )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIHasUniformComposer::~MOAIHasUniformComposer () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIHasUniformComposer::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIUniformComposerInterface, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIHasUniformComposer::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIUniformComposerInterface, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	
	luaL_Reg regTable [] = {
		{ "getComposer",				_getComposer },
		{ "setComposer",				_setComposer },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
MOAIUniformComposer& MOAIHasUniformComposer::MOAIUniformComposerInterface_AffirmComposer () {

	if ( !this->mComposer ) {
		this->mComposer = new MOAIUniformComposer ();
	}
	return *this->mComposer;
}

//----------------------------------------------------------------//
MOAIUniformComposer* MOAIHasUniformComposer::MOAIUniformComposerInterface_GetComposer () {

	return this->mComposer;
}

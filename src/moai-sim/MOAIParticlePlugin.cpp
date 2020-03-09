// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIParticlePlugin.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getSize
	@text	Return the particle size expected by the plugin.
	
	@in		MOAIParticlePlugin self
	@out	number size
*/
int MOAIParticlePlugin::_getSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIParticlePlugin, "U" )
	
	lua_pushnumber ( state, self->mSize );
	
	return 1;
}

//================================================================//
// MOAIParticlePlugin
//================================================================//

//----------------------------------------------------------------//
MOAIParticlePlugin::MOAIParticlePlugin () :
	mSize ( 0 ) {

	MOAI_LUA_OBJECT_RTTI_SINGLE ( MOAIParticlePlugin, MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIParticlePlugin::~MOAIParticlePlugin () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIParticlePlugin::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIParticlePlugin::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable[] = {
		{ "getSize",			_getSize },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}


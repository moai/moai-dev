// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIParticlePlugin.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getSize
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

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIParticlePlugin::~MOAIParticlePlugin () {
}

//----------------------------------------------------------------//
void MOAIParticlePlugin::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIParticlePlugin::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "getSize",			_getSize },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}


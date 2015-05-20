// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>
#include <moai-util/MOAILuaUtil.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILuaUtil::_convert ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAILuaUtil, "S" )
	
	// right now we just need Lua 5.1 32-bit to 5.1 64-bit
	
	// if you can take parameters, that is fine. or (maybe better) see if you
	// can figure out a way to tell the current Lua runtime and auto-convert to that
	
	size_t bufflen;
	cc8* buffer = lua_tolstring ( L, -1, &bufflen );
	
	ZLByteStream stream;
	stream.SetBuffer ( buffer, bufflen );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAILuaUtil::_getHeader ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAILuaUtil, "S" )
	
	// maybe just read the header and return a Lua table?
	
	size_t bufflen;
	cc8* buffer = lua_tolstring ( L, -1, &bufflen );
	
	ZLByteStream stream;
	stream.SetBuffer ( buffer, bufflen );
	
	return 0;
}

//================================================================//
// MOAILuaUtil
//================================================================//

//----------------------------------------------------------------//
MOAILuaUtil::MOAILuaUtil () {
}

//----------------------------------------------------------------//
MOAILuaUtil::~MOAILuaUtil () {
}

//----------------------------------------------------------------//
void MOAILuaUtil::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "_convert",					_convert },
		{ "_getHeader",					_getHeader },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAILuaUtil::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIMemStream.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
//int MOAIMemStream::_close ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIMemStream, "U" );
//	return 0;
//}

//----------------------------------------------------------------//
// TODO: doxygen
//int MOAIMemStream::_open ( lua_State* L ) {
//	MOAI_LUA_SETUP ( MOAIMemStream, "U" );
//	return 0;
//}

//================================================================//
// MOAIMemStream
//================================================================//

//----------------------------------------------------------------//
MOAIMemStream::MOAIMemStream () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIStream )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIMemStream::~MOAIMemStream () {
}

//----------------------------------------------------------------//
void MOAIMemStream::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIMemStream::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIStream::RegisterLuaFuncs ( state );

	//luaL_Reg regTable [] = {
	//	{ "close",				_close },
	//	{ "open",				_open },
	//	{ NULL, NULL }
	//};

	//luaL_register ( state, 0, regTable );
}


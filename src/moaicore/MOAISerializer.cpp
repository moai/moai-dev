// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAISerializer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	serializeToString
	@text	Serializes the specified table or userdata to a string.  Useful for sending data to a remote server.

	@in		MOAISerializer self
	@opt	table data				The table data to serialize.
	@opt	userdata data			The userdata (object) to serialize.  You must provide either a table or userdata, but not both.
	@out	string serialized		The serialized string.
*/
int MOAISerializer::_serializeToString ( lua_State* L ) {

	USLuaState state ( L );
	if ( !state.CheckParams ( 1, "U" )) return 0;
	if ( !( state.IsType ( 1, LUA_TTABLE ) || state.IsType ( 1, LUA_TUSERDATA ))) return 0;

	USLuaSerializer serializer;
	serializer.Affirm ( state, 1 );
	serializer.AddLuaReturn ( state, 1 );
	STLString result = serializer.SerializeToString ();

	lua_pushstring ( state, result );

	return 1;
}

//================================================================//
// MOAISerializer
//================================================================//

//----------------------------------------------------------------//
cc8* MOAISerializer::GetFileMagic () {

	return "--MOAI";
}

//----------------------------------------------------------------//
MOAISerializer::MOAISerializer () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( USLuaSerializer )
	RTTI_END
}

//----------------------------------------------------------------//
MOAISerializer::~MOAISerializer () {
}

//----------------------------------------------------------------//
void MOAISerializer::RegisterLuaClass ( USLuaState& state ) {

	USLuaSerializer::RegisterLuaClass ( state );
	
	luaL_Reg regTable [] = {
		{ "serializeToString",			_serializeToString },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISerializer::RegisterLuaFuncs ( USLuaState& state ) {
	
	USLuaSerializer::RegisterLuaFuncs ( state );
}

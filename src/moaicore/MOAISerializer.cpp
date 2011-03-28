// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAISerializer.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@brief <tt>serializeToString ( data )</tt>\n
\n
	Converts a lua data type to a string.  Useful for sending script to a server.
	@param data The data to be sent.  Can be a table.
	@return The string of converted script.
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
	
	LuaReg regTable [] = {
		{ "serializeToString",			_serializeToString },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAISerializer::RegisterLuaFuncs ( USLuaState& state ) {
	
	USLuaSerializer::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
STLString MOAISerializer::ToString () {

	STLString repr;
	
	return repr;
}

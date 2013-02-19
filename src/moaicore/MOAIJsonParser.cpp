// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <math.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIJsonParser.h>
#include <jansson.h>

//================================================================//
// json
//================================================================//

//----------------------------------------------------------------//
void		_jsonArrayToLua		( lua_State* L, json_t* json );
void		_jsonObjectToLua	( lua_State* L, json_t* json );
void		_jsonToLua			( lua_State* L, json_t* json );
json_t*		_luaToJSON			( lua_State* L, int idx );
json_t*		_luaToJSONArray		( lua_State* L, int idx );
json_t*		_luaToJSONObject	( lua_State* L, int idx );

//----------------------------------------------------------------//
void _jsonArrayToLua ( lua_State* L, json_t* json ) {
	
	assert ( json->type == JSON_ARRAY );
	
	lua_newtable ( L );
	
	int size = json_array_size ( json );
	for ( int i = 0; i < size; ++i ) {
		
		json_t* value = json_array_get ( json, i );
		
		if ( value ) {
		
			lua_pushnumber ( L, i + 1 );
			_jsonToLua ( L, value );
			lua_settable ( L, -3 );
		}
	}
}

//----------------------------------------------------------------//
void _jsonObjectToLua ( lua_State* L, json_t* json ) {
	
	assert ( json->type == JSON_OBJECT );
	
	lua_newtable ( L );

	void* iter = json_object_iter ( json );
	for ( ; iter; iter = json_object_iter_next ( json, iter )) {
	
		cc8* key = json_object_iter_key ( iter );
		json_t* value = json_object_iter_value ( iter );
		
		_jsonToLua ( L, value );
		lua_setfield ( L, -2, key );
	}
}

//----------------------------------------------------------------//
void _jsonToLua ( lua_State* L, json_t* json ) {

	switch ( json->type ) {
	
		case JSON_OBJECT:
			_jsonObjectToLua ( L, json );
			break;
		
		case JSON_ARRAY:
			_jsonArrayToLua ( L, json );
			break;
		
		case JSON_STRING:
			lua_pushstring ( L, json_string_value ( json ));
			break;
		
		case JSON_INTEGER:
			lua_pushnumber ( L, static_cast<lua_Number> ( static_cast<long long> ( json_integer_value ( json ) )));
			break;
		
		case JSON_REAL:
			lua_pushnumber ( L, ( lua_Number )json_real_value ( json ));
			break;
		
		case JSON_TRUE:
			lua_pushboolean ( L, 1 );
			break;
		
		case JSON_FALSE:
			lua_pushboolean ( L, 0 );
			break;
		
		case JSON_NULL:
			lua_pushlightuserdata ( L, 0 );
			break;
	};
}

//----------------------------------------------------------------//
json_t* _luaToJSON ( lua_State* L, int idx ) {
	
	switch ( lua_type ( L, idx )) {
		
		case LUA_TBOOLEAN: {
		
			bool value = lua_toboolean ( L, idx ) == 0 ? false : true;
			return value ? json_true () : json_false ();
		}
		case LUA_TTABLE: {
			
			return lua_objlen ( L, idx ) ? _luaToJSONArray ( L, idx ) : _luaToJSONObject ( L, idx );
		}
		case LUA_TSTRING: {
			
			cc8* str = lua_tostring ( L, idx );
			return json_string ( str );
		}
		case LUA_TNUMBER: {
		
			double real = lua_tonumber ( L, idx );
			double intpart;
			if ( modf ( real, &intpart ) == 0.0 ) {
				return json_integer (( json_int_t )intpart );
			}else{
				return json_real ( real );
			}
		}
		
		case LUA_TLIGHTUSERDATA: {
		
			return json_null ();
		}
	};

	return 0;
}

//----------------------------------------------------------------//
json_t* _luaToJSONArray ( lua_State* L, int idx ) {

	MOAILuaState state ( L );
	json_t* arr = json_array ();
	
	bool more = true;
	for ( int key = 1; more; ++key ) {
		
		state.GetField ( idx, key );
		json_t* value = _luaToJSON ( state, -1 );
		lua_pop ( state, 1 );
		
		if ( value ) {
			json_array_append_new ( arr, value );
		}
		else {
			more = false;
		}
	}
	return arr;
}

//----------------------------------------------------------------//
json_t* _luaToJSONObject ( lua_State* L, int idx ) {

	MOAILuaState state ( L );
	json_t* object = json_object ();
	
	u32 itr = state.PushTableItr ( idx );
	while ( state.TableItrNext ( itr )) {
		
		if ( lua_type ( state, -2 ) != LUA_TSTRING ) continue;
		
		STLString key = lua_tostring ( state, -2 );
		
		json_t* value = _luaToJSON ( state, -1 );
		
		if ( value ) {
			json_object_set_new ( object, key, value );
		}
	}
	return object;
}

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	decode
	@text	Decode a JSON string into a hierarchy of Lua tables.
	
	@in		string input
	@out	table result
*/
int MOAIJsonParser::_decode ( lua_State* L ) {
	UNUSED ( L );

	MOAILuaState state ( L );
	if ( state.IsType ( 1, LUA_TSTRING )) {
		
		size_t bufflen;
		cc8* buffer = lua_tolstring ( L, -1, &bufflen );
		
		json_error_t error;
		json_t* json = json_loadb ( buffer, bufflen, JSON_DISABLE_EOF_CHECK, &error );
		if ( json ) {
			
			_jsonToLua ( L, json );
			json_decref ( json );
			return 1;
		}
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	encode
	@text	Encode a hierarchy of Lua tables into a JSON string.
	
	@in		table input
	@out	string result
*/
int MOAIJsonParser::_encode ( lua_State* L ) {

	MOAILuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TTABLE )) {
	
		json_t* json = _luaToJSON ( state, 1 );
		if ( json ) {
			
			int flags = state.IsType ( 2, 0 );
			char* str = json_dumps ( json, flags );
			json_decref ( json );
			
			if ( str ) {
				lua_pushstring ( state, str );
				free ( str );
				return 1;
			}
		}
	}
	return 0;
}

//================================================================//
// MOAIJsonParser
//================================================================//

//----------------------------------------------------------------//
MOAIJsonParser::MOAIJsonParser () {
}

//----------------------------------------------------------------//
MOAIJsonParser::~MOAIJsonParser () {
}

//----------------------------------------------------------------//
void MOAIJsonParser::RegisterLuaClass ( MOAILuaState& state ) {

	lua_pushlightuserdata ( state, 0 );
	lua_setfield ( state, -2, "JSON_NULL" );

	state.SetField ( -1, "JSON_INDENT", JSON_INDENT ( 4 ));
	state.SetField ( -1, "JSON_COMPACT", JSON_COMPACT );
	state.SetField ( -1, "JSON_ENSURE_ASCII", JSON_ENSURE_ASCII );
	state.SetField ( -1, "JSON_SORT_KEYS", JSON_SORT_KEYS );
	state.SetField ( -1, "JSON_PRESERVE_ORDER", JSON_PRESERVE_ORDER );
	state.SetField ( -1, "JSON_ENCODE_ANY", JSON_ENCODE_ANY );
	
	luaL_Reg regTable[] = {
		{ "decode",					_decode },
		{ "encode",					_encode },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIJsonParser::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}


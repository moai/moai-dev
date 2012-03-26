// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEnvironment.h>
#include <moaicore/MOAIFileSystem.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMgr.h>
#include <aku/AKU.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	generateGUID
	@text	Generates a globally unique identifier

	@out	string GUID
*/
int MOAIEnvironment::_generateGUID ( lua_State* L ) {

	STLString guid = USUnique::GetGUID ();
	lua_pushstring ( L, guid );
	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIEnvironment::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TSTRING ) && ( state.IsType ( 2, LUA_TFUNCTION ) || state.IsNil ( 2 ))) {
		
		MOAIEnvironment& environment = MOAIEnvironment::Get ();

		if ( environment.mListeners ) {
			state.Push ( environment.mListeners );
		}
		else {
			lua_newtable ( state );
			environment.mListeners.SetStrongRef ( state, -1 );
		}
		
		lua_insert ( state, -3 );
		lua_settable ( state, -3 );
	}
	return 0;
}

//================================================================//
// MOAIEnvironment
//================================================================//

//----------------------------------------------------------------//
void MOAIEnvironment::CallListener ( cc8* key ) {

	if ( this->mListeners ) {
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		state.Push ( this->mListeners );
		if ( state.GetFieldWithType ( -1, key, LUA_TFUNCTION )) {
			
			this->PushLuaClassTable ( state );
			lua_pushstring ( state, key );
			lua_gettable ( state, -2 );
			lua_remove ( state, -2 );
			
			state.DebugCall ( 1, 0 );
		}
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
MOAIEnvironment::MOAIEnvironment () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIEnvironment::~MOAIEnvironment () {
}

//----------------------------------------------------------------//
void MOAIEnvironment::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "CONNECTION_TYPE_NONE", ( u32 )CONNECTION_TYPE_NONE );
	state.SetField ( -1, "CONNECTION_TYPE_WIFI", ( u32 )CONNECTION_TYPE_WIFI );
	state.SetField ( -1, "CONNECTION_TYPE_WWAN", ( u32 )CONNECTION_TYPE_WWAN );
	
	state.SetField ( -1, "OS_BRAND_ANDROID", ( u32 )OS_BRAND_ANDROID );
	state.SetField ( -1, "OS_BRAND_IOS", ( u32 )OS_BRAND_IOS );
	state.SetField ( -1, "OS_BRAND_UNAVAILABLE", ( u32 )OS_BRAND_UNAVAILABLE );

	luaL_Reg regTable [] = {
		{ "generateGUID",	_generateGUID },
		{ "setListener",	_setListener },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetValue ( cc8* key ) {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	this->PushLuaClassTable ( state );
	state.ClearField ( -1, key );
	state.Pop ( 1 );
}
/*
//----------------------------------------------------------------//
void MOAIEnvironment::SetValue ( cc8* key, bool value ) {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	this->PushLuaClassTable ( state );
	state.SetField < bool >( -1, key, value );
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetValue ( cc8* key, double value ) {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	this->PushLuaClassTable ( state );
	state.SetField < double >( -1, key, value );
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetValue ( cc8* key, int value ) {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	this->PushLuaClassTable ( state );
	state.SetField < int >( -1, key, value );
}

//----------------------------------------------------------------//
void MOAIEnvironment::SetValue ( cc8* key, cc8* value ) {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	this->PushLuaClassTable ( state );
	state.SetField < cc8* >( -1, key, value );
}
*/
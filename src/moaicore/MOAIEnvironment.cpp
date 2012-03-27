// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEnvironment.h>
#include <moaicore/MOAILogMgr.h>
#include <aku/AKU.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	generateGUID
	@text	Generates a globally unique identifier. This method will be
			moved to MOAIUnique in a future release.

	@out	string GUID
*/
int MOAIEnvironment::_generateGUID ( lua_State* L ) {

	STLString guid = USUnique::GetGUID ();
	lua_pushstring ( L, guid );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setValue
	@text	Sets an evironment value and also triggers the listener
			callback (if any).

	@in		string key
	@opt	variant value		Default value is nil.
	@out	nil
*/
int MOAIEnvironment::_setValue ( lua_State* L ) {
	MOAILuaState state ( L );
	
	if ( state.IsType ( 1, LUA_TSTRING )) {
	
		MOAIEnvironment& environment = MOAIEnvironment::Get ();
		environment.SetValue ( state );
	}
	return 0;
}

//================================================================//
// MOAIEnvironment
//================================================================//

//----------------------------------------------------------------//
MOAIEnvironment::MOAIEnvironment () {

	RTTI_SINGLE ( MOAIGlobalEventSource )
}

//----------------------------------------------------------------//
MOAIEnvironment::~MOAIEnvironment () {
}

//----------------------------------------------------------------//
void MOAIEnvironment::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIGlobalEventSource::RegisterLuaClass ( state );

	state.SetField ( -1, "EVENT_VALUE_CHANGED", ( u32 )EVENT_VALUE_CHANGED );

	state.SetField ( -1, "CONNECTION_TYPE_NONE", ( u32 )CONNECTION_TYPE_NONE );
	state.SetField ( -1, "CONNECTION_TYPE_WIFI", ( u32 )CONNECTION_TYPE_WIFI );
	state.SetField ( -1, "CONNECTION_TYPE_WWAN", ( u32 )CONNECTION_TYPE_WWAN );
	
	state.SetField ( -1, "OS_BRAND_ANDROID", ( u32 )OS_BRAND_ANDROID );
	state.SetField ( -1, "OS_BRAND_IOS", ( u32 )OS_BRAND_IOS );
	state.SetField ( -1, "OS_BRAND_UNAVAILABLE", ( u32 )OS_BRAND_UNAVAILABLE );

	luaL_Reg regTable [] = {
		{ "generateGUID",		_generateGUID },
		{ "setListener",		&MOAIGlobalEventSource::_setListener < MOAIEnvironment > },
		{ "setValue",			_setValue },
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

//----------------------------------------------------------------//
void MOAIEnvironment::SetValue ( lua_State* L ) {

	MOAILuaState state ( L );

	int top = state.GetTop ();

	this->PushLuaClassTable ( state );
		
	state.CopyToTop ( -3 ); // key
	state.CopyToTop ( -3 ); // value
	
	lua_settable ( state, -3 );
	state.Pop ( 1 );
	
	if ( this->PushListener ( EVENT_VALUE_CHANGED, state )) {
	
		state.CopyToTop ( -3 ); // key
		state.CopyToTop ( -3 ); // value
		
		state.DebugCall ( 2, 0 );
	}
	
	top = state.GetTop ();
}

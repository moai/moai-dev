// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEventSource.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// MOAIEventSource
//================================================================//

//----------------------------------------------------------------//
MOAIEventSource::MOAIEventSource () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIEventSource::~MOAIEventSource () {
}

//----------------------------------------------------------------//
bool MOAIEventSource::PushListener ( u32 eventID, MOAILuaState& state ) {

	if ( this->PushListenerTable ( state )) {
		if ( state.GetFieldWithType ( -1, eventID, LUA_TFUNCTION )) {
			lua_replace ( state, -2 );
			return true;
		}
		state.Pop ( 1 );
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIEventSource::PushListenerAndSelf ( u32 eventID, MOAILuaState& state ) {

	if ( this->PushListener ( eventID, state )) {
		this->PushLuaUserdata ( state );
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIEventSource::SetListener ( lua_State* L, u32 idx ) {

	MOAILuaState state ( L );
	idx = state.AbsIndex ( idx );

	this->AffirmListenerTable ( state );

	if ( this->PushListenerTable ( state )) {
		lua_pushvalue ( state, idx );
		lua_pushvalue ( state, idx + 1 );
		lua_settable ( state, -3 );
	}
	lua_pop ( state, 1 );
}

//================================================================//
// MOAIInstanceEventSource lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	setListener
	@text	Sets a listener callback for a given event ID. It is up
			to individual classes to declare their event IDs.

	@in		MOAIInstanceEventSource self
	@in		number eventID				The ID of the event.
	@opt	function callback			The callback to be called when the object emits the event. Default value is nil.
	@out	MOAIInstanceEventSource		self
*/
int MOAIInstanceEventSource::_setListener ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIInstanceEventSource, "UN" );

	self->SetListener ( state, 2 );

	return 0;
}

//================================================================//
// MOAIInstanceEventSource
//================================================================//

//----------------------------------------------------------------//
void MOAIInstanceEventSource::AffirmListenerTable ( MOAILuaState& state ) {

	if ( !this->mListenerTable ) {
		lua_newtable ( state );
		this->SetLocal ( state, -1, this->mListenerTable );
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
MOAIInstanceEventSource::MOAIInstanceEventSource () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIEventSource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIInstanceEventSource::~MOAIInstanceEventSource () {
}

//----------------------------------------------------------------//
bool MOAIInstanceEventSource::PushListenerTable ( MOAILuaState& state ) {

	if ( this->mListenerTable ) {
		return this->PushLocal ( state, this->mListenerTable );
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIInstanceEventSource::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "setListener",		_setListener },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//================================================================//
// MOAIGlobalEventSource
//================================================================//

//----------------------------------------------------------------//
void MOAIGlobalEventSource::AffirmListenerTable ( MOAILuaState& state ) {

	if ( !this->mListenerTable ) {
		lua_newtable ( state );
		this->mListenerTable.SetStrongRef ( state, -1 );
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
MOAIGlobalEventSource::MOAIGlobalEventSource () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIEventSource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGlobalEventSource::~MOAIGlobalEventSource () {
}

//----------------------------------------------------------------//
bool MOAIGlobalEventSource::PushListenerTable ( MOAILuaState& state ) {

	if ( this->mListenerTable ) {
		return this->mListenerTable.PushRef ( state );
	}
	return false;
}

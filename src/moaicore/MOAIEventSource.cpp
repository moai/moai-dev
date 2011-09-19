// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIEventSource.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	setListener
	@text	Sets a listener callback for a given event ID. It is up
			to individual classes to declare their event IDs. This function
			has two forms: one for instances and one for singletons. When
			using setListener with a singleton (such as MOAISim), simply
			omit the first parameter (self).

	@overload

		@in		MOAIEventSource self
		@in		number eventID			The ID of the event.
		@opt	function callback		The callback to be called when the object emits the event. Default value is nil.
		@out	nil
	
	@overload
		
		@in		number eventID			The ID of the event.
		@opt	function callback		The callback to be called when the object emits the event. Default value is nil.
		@out	nil
*/
int MOAIEventSource::_setListener ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIEventSource, "UN" );

	self->SetListener ( state, 2 );

	return 0;
}

//================================================================//
// MOAIEventSource
//================================================================//

//----------------------------------------------------------------//
void MOAIEventSource::AffirmListenerTable ( USLuaState& state ) {

	if ( !mListenerTable ) {
	
		lua_newtable ( state );
		this->mListenerTable.SetRef ( state, -1, false );
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
MOAIEventSource::MOAIEventSource () {

	RTTI_BEGIN
		RTTI_EXTEND ( USLuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIEventSource::~MOAIEventSource () {
}

//----------------------------------------------------------------//
bool MOAIEventSource::PushListener ( u32 eventID, USLuaState& state ) {

	if ( this->mListenerTable ) {
	
		this->mListenerTable.PushRef ( state );
		if ( state.GetFieldWithType ( -1, eventID, LUA_TFUNCTION )) {
			
			lua_replace ( state, -2 );
			return true;
		}
		state.Pop ( 1 );
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIEventSource::PushListenerAndSelf ( u32 eventID, USLuaState& state ) {

	if ( this->mListenerTable ) {
	
		this->mListenerTable.PushRef ( state );
		if ( state.GetFieldWithType ( -1, eventID, LUA_TFUNCTION )) {
			
			lua_replace ( state, -2 );
			this->PushLuaUserdata ( state );
			return true;
		}
		state.Pop ( 1 );
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIEventSource::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIEventSource::RegisterLuaFuncs ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "setListener",		_setListener },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIEventSource::SetListener ( lua_State* L, u32 idx ) {

	USLuaState state ( L );
	idx = state.AbsIndex ( idx );

	this->AffirmListenerTable ( state );

	this->mListenerTable.PushRef ( state );
	lua_pushvalue ( state, idx );
	lua_pushvalue ( state, idx + 1 );
	lua_settable ( state, -3 );
}
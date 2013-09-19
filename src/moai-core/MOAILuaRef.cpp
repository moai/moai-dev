// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAILuaObject.h>
#include <moai-core/MOAILuaRef.h>
#include <moai-core/MOAILuaRuntime.h>

//================================================================//
// MOAILuaRef
//================================================================//

static int sCounter = 0;

//----------------------------------------------------------------//
void MOAILuaRef::Clear () {

	if ( MOAILuaRuntime::IsValid ()) {
		if ( this->mRef ) {
			this->mRefTable->Unref ( this->mRef );
		}
	}

	this->mRefTable = 0;
	this->mRef = 0;
}

//----------------------------------------------------------------//
u32 MOAILuaRef::GetID () {

	return this->mRef;
}

//----------------------------------------------------------------//
MOAIScopedLuaState MOAILuaRef::GetSelf () {

	assert ( this->mRef );

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->mRefTable->PushRef ( state, this->mRef );
	return state;
}

//----------------------------------------------------------------//
MOAILuaRef::MOAILuaRef () :
	mRefTable ( 0 ),
	mRef ( 0 ) {
}

//----------------------------------------------------------------//
MOAILuaRef::~MOAILuaRef () {
	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAILuaRef::PushRef ( MOAILuaState& state ) {

	if ( this->mRef ) {
		lua_pushnil ( state );
		return false;
	}

	this->mRefTable->PushRef ( state, this->mRef );
	
	if ( lua_isnil ( state, -1 )) {
		this->mRefTable = 0;
		this->mRef = 0;
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
void MOAILuaRef::SetRef ( MOAILuaState& state, int idx, MOAILuaRefTable& refTable ) {

	this->Clear ();

	if ( lua_isnil ( state, idx ) == false ) {
		
		this->mRefTable = &refTable;
		this->mRef = this->mRefTable->Ref ( state, idx );
		
		cc8* tableName = &refTable == &MOAILuaRuntime::Get ().mStrongRefs ? "strong" : "weak";
		printf ( "TABLE: %s REF: %d\n", tableName, this->mRef );
	}
}

//================================================================//
// MOAILuaStrongRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaStrongRef::SetRef ( MOAILuaState& state, int idx ) {
	MOAILuaRef::SetRef ( state, idx, MOAILuaRuntime::Get ().mStrongRefs );
}

//================================================================//
// MOAILuaWeakRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaWeakRef::SetRef ( MOAILuaState& state, int idx ) {
	MOAILuaRef::SetRef ( state, idx, MOAILuaRuntime::Get ().mWeakRefs );
}

//================================================================//
// MOAILuaMemberRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaMemberRef::Clear () {

	if ( this->mRef != LUA_NOREF ) {

		if ( MOAILuaRuntime::IsValid ()) {
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			
			if ( this->mOwner->PushRefTable ( state )) {
				luaL_unref ( state, -1, this->mRef );
			}
		}
		this->mRef = LUA_NOREF;
		this->mOwner = 0;
	}
}

//----------------------------------------------------------------//
MOAILuaMemberRef::MOAILuaMemberRef () :
	mRef ( LUA_NOREF ),
	mOwner ( 0 ) {
}

//----------------------------------------------------------------//
MOAILuaMemberRef::~MOAILuaMemberRef () {
	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAILuaMemberRef::PushRef ( MOAILuaState& state ) {

	if ( this->mRef != LUA_NOREF ) {
	
		bool isNil = true;
	
		if ( this->mOwner->IsBound ()) {
			if ( this->mOwner->PushRefTable ( state )) {
				lua_rawgeti ( state, -1, this->mRef );
				lua_replace ( state, -2 );
			}
			isNil = lua_isnil ( state, -1 );
		}
		
		if ( isNil ) {
			this->mRef = LUA_NOREF;
			this->mOwner = 0;
			return false;
		}
		return true;
	}
	lua_pushnil ( state );
	return false;
}

//----------------------------------------------------------------//
void MOAILuaMemberRef::SetRef ( MOAILuaObject& owner, MOAILuaState& state, int idx ) {

	this->Clear ();

	if ( lua_isnil ( state, idx ) == false ) {

		idx = state.AbsIndex ( idx );

		this->mOwner = &owner;
		this->mOwner->PushRefTable ( state );
		
		lua_pushvalue ( state, idx );
		this->mRef = luaL_ref ( state, -2 );
		
		lua_pop ( state, 1 );
	}
}

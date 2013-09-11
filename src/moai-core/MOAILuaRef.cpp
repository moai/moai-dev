// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAILuaObject.h>
#include <moai-core/MOAILuaRef.h>
#include <moai-core/MOAILuaRuntime.h>

//================================================================//
// MOAILuaRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaRef::Clear () {

	if ( MOAILuaRuntime::IsValid ()) {
		if (( this->mRef != LUA_NOREF ) && this->mOwnsRef ) {
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			lua_rawgeti ( state, LUA_REGISTRYINDEX, this->mRefTableID );
			luaL_unref ( state, -1, this->mRef );
		}
	}

	this->mRef = LUA_NOREF;
	this->mOwnsRef = false;
	this->mRefTableID = LUA_NOREF;
}

//----------------------------------------------------------------//
u32 MOAILuaRef::GetID () {

	return this->mRef;
}

//----------------------------------------------------------------//
MOAIScopedLuaState MOAILuaRef::GetSelf () {

	assert ( this->mRef != LUA_NOREF );

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushRef ( state );
	return state;
}

//----------------------------------------------------------------//
MOAILuaRef::MOAILuaRef () :
	mRef ( LUA_NOREF ),
	mOwnsRef ( false ),
	mRefTableID ( LUA_NOREF ) {
}

//----------------------------------------------------------------//
MOAILuaRef::MOAILuaRef ( const MOAILuaRef& assign ) :
	mRef ( LUA_NOREF ),
	mOwnsRef ( false ),
	mRefTableID ( LUA_NOREF ) {
	this->Take ( assign );
}

//----------------------------------------------------------------//
MOAILuaRef::~MOAILuaRef () {
	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAILuaRef::PushRef ( MOAILuaState& state ) {

	if ( this->mRef == LUA_NOREF ) {
		lua_pushnil ( state );
		return false;
	}

	lua_rawgeti ( state, LUA_REGISTRYINDEX, this->mRefTableID );
	lua_rawgeti ( state, -1, this->mRef );
	lua_replace ( state, -2 );
	
	if ( lua_isnil ( state, -1 )) {
		this->mRef = LUA_NOREF;
		this->mOwnsRef = false;
		this->mRefTableID = LUA_NOREF;
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
void MOAILuaRef::SetRef ( MOAILuaState& state, int idx, int refTableID ) {

	this->Clear ();

	if ( lua_isnil ( state, idx ) == false ) {
		
		idx = state.AbsIndex ( idx );
		
		lua_rawgeti ( state, LUA_REGISTRYINDEX, refTableID );
		lua_pushvalue ( state, idx );
		this->mRef = luaL_ref ( state, -2 );
		lua_pop ( state, 1 );
		
		this->mOwnsRef = true;
		this->mRefTableID = refTableID;
	}
}

//----------------------------------------------------------------//
void MOAILuaRef::Take ( const MOAILuaRef& assign ) {

	this->Clear ();

	this->mRef = assign.mRef;
	this->mOwnsRef = assign.mOwnsRef;
	this->mRefTableID = assign.mRefTableID;

	// cast the const away
	(( MOAILuaRef& )assign ).mOwnsRef = false;
}

//================================================================//
// MOAILuaStrongRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaStrongRef::SetRef ( MOAILuaState& state, int idx ) {
	MOAILuaRef::SetRef ( state, idx, MOAILuaRuntime::Get ().mStrongRefTableID );
}


//================================================================//
// MOAILuaWeakRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaWeakRef::SetRef ( MOAILuaState& state, int idx ) {
	MOAILuaRef::SetRef ( state, idx, MOAILuaRuntime::Get ().mWeakRefTableID );
}

//================================================================//
// MOAILuaMemberRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaMemberRef::Clear () {

	if ( this->mRef != LUA_NOREF ) {

		if ( this->mOwner->mMemberTable && MOAILuaRuntime::IsValid ()) {
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

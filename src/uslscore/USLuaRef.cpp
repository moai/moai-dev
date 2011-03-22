// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USLuaRef.h>
#include <uslscore/USLuaRuntime.h>

//================================================================//
// USLuaRef
//================================================================//

//----------------------------------------------------------------//
void USLuaRef::Clear () {

	if ( USLuaRuntime::IsValid ()) {

		USLuaRuntime& luaRuntime = USLuaRuntime::Get ();

		if (( this->mRef != LUA_NOREF ) && this->mOwnsRef ) {

			USLuaStateHandle state = luaRuntime.State ();

			if ( this->mWeak ) {

				luaRuntime.WeakRefTable ( state );
				luaL_unref ( state, this->mRef, -1 );
			}
			else {
				
				luaL_unref ( state, this->mRef, LUA_REGISTRYINDEX );
				
				// explicitely set to nil...
				lua_pushnumber ( state, this->mRef );
				lua_pushnil ( state );
				lua_settable ( state, LUA_REGISTRYINDEX );
			}
		}
	}

	this->mRef = LUA_NOREF;
	this->mOwnsRef = false;
}

//----------------------------------------------------------------//
u32 USLuaRef::GetID () {

	return this->mRef;
}

//----------------------------------------------------------------//
USLuaStateHandle USLuaRef::GetSelf () {

	assert ( !this->IsNil ());

	USLuaStateHandle state = USLuaRuntime::Get ().State ();
	this->PushRef ( state );
	return state;
}

//----------------------------------------------------------------//
bool USLuaRef::IsNil () {

	return ( this->mRef == LUA_NOREF );
}

//----------------------------------------------------------------//
void USLuaRef::MakeStrong () {

	if ( !this->mWeak ) return;
	if ( this->mRef == LUA_NOREF ) return;
	if ( !USLuaRuntime::IsValid ()) return;

	USLuaRuntime& luaRuntime = USLuaRuntime::Get ();
	USLuaStateHandle state = luaRuntime.State ();

	// preserve the top
	int top = lua_gettop ( state );

	// push the original ref
	this->PushRef ( state );
		
	// get a strong ref to it
	int ref = luaL_ref ( state, LUA_REGISTRYINDEX );

	// push the weak ref table
	luaRuntime.WeakRefTable ( state );
	
	// release the weak ref
	luaL_unref ( state, this->mRef, -1 );

	// restore the top
	lua_settop ( state, top );

	this->mRef = ref;
	this->mWeak = false;
}

//----------------------------------------------------------------//
void USLuaRef::MakeWeak () {

	if ( this->mWeak ) return;
	if ( this->mRef == LUA_NOREF ) return;
	if ( !USLuaRuntime::IsValid ()) return;

	USLuaRuntime& luaRuntime = USLuaRuntime::Get ();
	USLuaStateHandle state = luaRuntime.State ();

	// preserve the top
	int top = lua_gettop ( state );

	// push the original ref
	this->PushRef ( state );
		
	// get a weak ref to it
	USLuaRuntime::Get ().WeakRefTable ( state );
	lua_pushvalue ( state, -2 );

	int ref = luaL_ref ( state, -2 );
	
	// release the strong ref
	luaL_unref ( state, this->mRef, LUA_REGISTRYINDEX );
				
	lua_pushnumber ( state, this->mRef );
	lua_pushnil ( state );
	lua_settable ( state, LUA_REGISTRYINDEX );

	// restore the top
	lua_settop ( state, top );

	this->mRef = ref;
	this->mWeak = true;
}

//----------------------------------------------------------------//
bool USLuaRef::PushRef ( USLuaState& state ) {

	if ( this->mRef == LUA_NOREF ) {
		lua_pushnil ( state );
		return false;
	}

	if ( this->mWeak ) {

		USLuaRuntime::Get ().WeakRefTable ( state );
		lua_rawgeti ( state, -1, this->mRef );
		lua_replace ( state, -2 );
	}
	else {
		lua_rawgeti ( state, LUA_REGISTRYINDEX, this->mRef );

	}

	return true;
}

//----------------------------------------------------------------//
void USLuaRef::SetRef ( USLuaState& state, int idx, bool weak ) {

	this->Clear ();

	this->mWeak = weak;

	int top = lua_gettop ( state );

	if ( weak ) {

		idx = state.AbsIndex ( idx );

		USLuaRuntime::Get ().WeakRefTable ( state );
		lua_pushvalue ( state, idx );

		this->mRef = luaL_ref ( state, -2 );
		this->mOwnsRef = true;
	}
	else {

		lua_pushvalue ( state, idx );
		this->mRef = luaL_ref ( state, LUA_REGISTRYINDEX );
		this->mOwnsRef = true;
	}

	lua_settop ( state, top );
}

//----------------------------------------------------------------//
void USLuaRef::Take ( const USLuaRef& assign ) {

	this->Clear ();

	this->mRef = assign.mRef;
	this->mOwnsRef = assign.mOwnsRef;
	this->mWeak = assign.mWeak;

	// cast the const away
	(( USLuaRef& )assign ).mOwnsRef = false;
}

//----------------------------------------------------------------//
USLuaRef::USLuaRef () :
	mRef ( LUA_NOREF ),
	mOwnsRef ( false ) {
}

//----------------------------------------------------------------//
USLuaRef::USLuaRef ( const USLuaRef& assign ) :
	mRef ( LUA_NOREF ),
	mOwnsRef ( false ) {
	this->Take ( assign );
}

//----------------------------------------------------------------//
USLuaRef::~USLuaRef () {

	this->Clear ();
}

//----------------------------------------------------------------//
void USLuaRef::WeakCopy ( const USLuaRef& assign ) {

	this->Clear ();

	this->mRef = assign.mRef;
	this->mOwnsRef = false;
	this->mWeak = assign.mWeak;
}

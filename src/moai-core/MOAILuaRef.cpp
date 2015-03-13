// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAILuaObject.h>
#include <moai-core/MOAILuaClass.h>
#include <moai-core/MOAILuaRef.h>
#include <moai-core/MOAILuaRuntime.h>

//================================================================//
// MOAILuaRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaRef::Clear () {

	if ( this->mOwnsRef && MOAILuaRuntime::IsValid () ) {
		MOAILuaRuntime::Get ().ClearRef ( this->mRefID );
	}
	this->mOwnsRef = false;
	this->mRefID = LUA_NOREF;
}

//----------------------------------------------------------------//
u32 MOAILuaRef::GetID () {

	return this->mRefID;
}

//----------------------------------------------------------------//
MOAIScopedLuaState MOAILuaRef::GetSelf () {

	assert ( !this->IsNil ());

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushRef ( state );
	return state;
}

//----------------------------------------------------------------//
bool MOAILuaRef::IsNil () {

	return ( this->mRefID == LUA_NOREF );
}

//----------------------------------------------------------------//
void MOAILuaRef::MakeStrong () {

	this->mRefID = MOAILuaRuntime::Get ().MakeStrong ( this->mRefID );
}

//----------------------------------------------------------------//
void MOAILuaRef::MakeWeak () {

	this->mRefID = MOAILuaRuntime::Get ().MakeWeak ( this->mRefID );
}

//----------------------------------------------------------------//
MOAILuaRef::MOAILuaRef () :
	mOwnsRef ( false ),
	mRefID ( LUA_NOREF ) {
}

//----------------------------------------------------------------//
MOAILuaRef::MOAILuaRef ( const MOAILuaRef& assign ) :
	mOwnsRef ( false ),
	mRefID ( LUA_NOREF ) {
	this->Take ( assign );
}

//----------------------------------------------------------------//
MOAILuaRef::~MOAILuaRef () {

	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAILuaRef::PushRef ( MOAILuaState& state ) {

	bool result = MOAILuaRuntime::Get ().PushRef ( state, this->mRefID );
	
	if ( !result ) {
		this->Clear ();
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
void  MOAILuaRef::SetRef ( MOAILuaObject* object ) {
	UNUSED ( object );
	assert ( false );
}

//----------------------------------------------------------------//
void MOAILuaRef::SetRef ( MOAILuaObject* object, u32 type ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	state.Push ( object );
	this->SetRef ( state, -1, type );
	state.Pop ( 1 );
}

//----------------------------------------------------------------//
void  MOAILuaRef::SetRef ( MOAILuaState& state, int idx ) {
	UNUSED ( state );
	UNUSED ( idx );
	assert ( false );
}

//----------------------------------------------------------------//
void MOAILuaRef::SetRef ( MOAILuaState& state, int idx, u32 type ) {

	this->Clear ();
	this->mRefID = MOAILuaRuntime::Get ().GetRef ( state, idx, type );
	this->mOwnsRef = ( this->mRefID != LUA_NOREF );
}

//----------------------------------------------------------------//
void MOAILuaRef::Take ( const MOAILuaRef& assign ) {

	this->Clear ();

	this->mRefID = assign.mRefID;
	this->mOwnsRef = assign.mOwnsRef;

	// cast the const away
	(( MOAILuaRef& )assign ).mOwnsRef = false;
}

//================================================================//
// MOAILuaStrongRef
//================================================================//

//----------------------------------------------------------------//
MOAILuaStrongRef::MOAILuaStrongRef () {
}

//----------------------------------------------------------------//
MOAILuaStrongRef::MOAILuaStrongRef ( MOAILuaObject* object ) {
	this->SetRef ( object );
}

//----------------------------------------------------------------//
void MOAILuaStrongRef::SetRef ( MOAILuaObject* object ) {
	MOAILuaRef::SetRef ( object, false );
}

//----------------------------------------------------------------//
void MOAILuaStrongRef::SetRef ( MOAILuaState& state, int idx ) {
	MOAILuaRef::SetRef ( state, idx, false );
}

//================================================================//
// MOAILuaWeakRef
//================================================================//

//----------------------------------------------------------------//
MOAILuaWeakRef::MOAILuaWeakRef () {
}

//----------------------------------------------------------------//
MOAILuaWeakRef::MOAILuaWeakRef ( MOAILuaObject* object ) {
	this->SetRef ( object );
}

//----------------------------------------------------------------//
void MOAILuaWeakRef::SetRef ( MOAILuaObject* object ) {
	MOAILuaRef::SetRef ( object, true );
}

//----------------------------------------------------------------//
void MOAILuaWeakRef::SetRef ( MOAILuaState& state, int idx ) {
	MOAILuaRef::SetRef ( state, idx, true );
}

//================================================================//
// MOAILuaMemberRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaMemberRef::Clear () {

	if ( this->mRefID != LUA_NOREF ) {

		if (( !this->mOwner->mCollected ) && MOAILuaRuntime::IsValid ()) {
			MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
			if ( this->mOwner->PushRefTable ( state )) {
				luaL_unref ( state, -1, this->mRefID );
			}
		}
		this->mRefID = LUA_NOREF;
		this->mOwner = 0;
	}
}

//----------------------------------------------------------------//
MOAILuaMemberRef::MOAILuaMemberRef () :
	mRefID ( LUA_NOREF ),
	mOwner ( 0 ) {
}

//----------------------------------------------------------------//
MOAILuaMemberRef::~MOAILuaMemberRef () {
	this->Clear ();
}

//----------------------------------------------------------------//
bool MOAILuaMemberRef::PushRef ( MOAILuaState& state ) {

	if ( this->mRefID != LUA_NOREF ) {

		bool isNil = true;

		if ( this->mOwner->IsBound ()) {
			if ( this->mOwner->PushRefTable ( state )) {
				lua_rawgeti ( state, -1, this->mRefID );
				lua_replace ( state, -2 );
			}
			isNil = lua_isnil ( state, -1 );
		}

		if ( isNil ) {
			this->mRefID = LUA_NOREF;
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
		this->mRefID = luaL_ref ( state, -2 );

		lua_pop ( state, 1 );
	}
}

// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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

	if ( this->mOwnsRef && this->mRuntime && this->mRuntime->IsValid () ) {
		this->mRuntime->ClearRef ( this->mRefID );
	}
	this->mRuntime 		= NULL;
	this->mOwnsRef 		= false;
	this->mRefID 		= LUA_NOREF;
}

//----------------------------------------------------------------//
u32 MOAILuaRef::GetID () {

	return this->mRefID;
}

//----------------------------------------------------------------//
MOAIScopedLuaState MOAILuaRef::GetSelf () {

	assert ( !this->IsNil ());

	MOAIScopedLuaState state = this->mRuntime->State ();
	this->PushRef ( state );
	return state;
}

//----------------------------------------------------------------//
bool MOAILuaRef::IsNil () {

	return ( this->mRefID == LUA_NOREF );
}

//----------------------------------------------------------------//
void MOAILuaRef::MakeStrong () {

	assert ( this->mRuntime );
	this->mRefID = this->mRuntime->MakeStrong ( this->mRefID );
}

//----------------------------------------------------------------//
void MOAILuaRef::MakeWeak () {

	assert ( this->mRuntime );
	this->mRefID = this->mRuntime->MakeWeak ( this->mRefID );
}

//----------------------------------------------------------------//
MOAILuaRef::MOAILuaRef () :
	mRuntime ( NULL ),
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

	assert ( this->mRuntime == &state.Get < MOAILuaRuntime >());

	bool result = this->mRuntime->PushRef ( state, this->mRefID );
	
	if ( !result ) {
		this->Clear ();
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
void MOAILuaRef::SetRef ( MOAILuaObject* object, u32 type ) {

	if ( !object ) {
		this->Clear ();
		return;
	}

	MOAIScopedLuaState state = object->Get < MOAILuaRuntime >().State ();
	state.Push ( object );
	this->SetRef ( state, -1, type );
	state.Pop ( 1 );
}

//----------------------------------------------------------------//
void MOAILuaRef::SetRef ( MOAILuaState& state, int idx, u32 type ) {

	this->Clear ();
	
	this->mRuntime = &state.Get < MOAILuaRuntime >();
	this->mRefID = this->mRuntime->GetRef ( state, idx, type );
	this->mOwnsRef = ( this->mRefID != LUA_NOREF );
}

//----------------------------------------------------------------//
void MOAILuaRef::Take ( const MOAILuaRef& assign ) {

	this->Clear ();

	this->mRuntime 		= assign.mRuntime;
	this->mRefID 		= assign.mRefID;
	this->mOwnsRef 		= assign.mOwnsRef;

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
	MOAILuaRef::SetRef ( object, MAKE_STRONG );
}

//----------------------------------------------------------------//
void MOAILuaStrongRef::SetRef ( MOAILuaState& state, int idx ) {
	MOAILuaRef::SetRef ( state, idx, MAKE_STRONG );
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
	MOAILuaRef::SetRef ( object, MAKE_WEAK );
}

//----------------------------------------------------------------//
void MOAILuaWeakRef::SetRef ( MOAILuaState& state, int idx ) {
	MOAILuaRef::SetRef ( state, idx, MAKE_WEAK );
}

//================================================================//
// MOAILuaMemberRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaMemberRef::Clear () {

	if ( this->mRefID != LUA_NOREF ) {
	
		assert ( this->mOwner );
		MOAILuaRuntime& runtime = this->mOwner->Get < MOAILuaRuntime >();

		if ( this->mOwner->IsBound () && runtime.IsValid ()) {
			MOAIScopedLuaState state = runtime.State ();
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
			isNil = state.IsNilOrNone ( -1 );
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

	if ( state.IsNilOrNone ( idx ) == false ) {

		idx = state.AbsIndex ( idx );

		this->mOwner = &owner;
		this->mOwner->PushRefTable ( state );

		lua_pushvalue ( state, idx );
		this->mRefID = luaL_ref ( state, -2 );

		lua_pop ( state, 1 );
	}
}

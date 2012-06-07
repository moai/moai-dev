// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAILuaRef.h>
#include <moaicore/MOAILuaRuntime.h>

//================================================================//
// MOAILuaRefTable
//================================================================//

//----------------------------------------------------------------//
void MOAILuaRefTable::Clear () {

	if ( this->mTableID != LUA_NOREF ) {
		
		MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
		
		luaL_unref ( state, LUA_REGISTRYINDEX, this->mTableID );
		this->mTableID = LUA_NOREF;
	}
	
	this->mRefIDStack.Clear ();
	this->mRefIDStackTop = 0;
}

//----------------------------------------------------------------//
void MOAILuaRefTable::InitStrong () {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();

	// create the table
	lua_newtable ( state );

	// and grab the table ref
	this->mTableID = luaL_ref ( state, LUA_REGISTRYINDEX );
}

//----------------------------------------------------------------//
void MOAILuaRefTable::InitWeak () {

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();

	// create the table
	lua_newtable ( state );

	// create the metatable
	lua_newtable ( state );

	// make it weak
	lua_pushstring ( state, "kv" );
	lua_setfield ( state, -2, "__mode" );

	//set the metatable
	lua_setmetatable ( state, -2 );

	// and grab the table ref
	this->mTableID = luaL_ref ( state, LUA_REGISTRYINDEX );
}

//----------------------------------------------------------------//
MOAILuaRefTable::MOAILuaRefTable () :
	mTableID ( LUA_NOREF ),
	mRefIDStackTop ( 0 ) {
}

//----------------------------------------------------------------//
MOAILuaRefTable::~MOAILuaRefTable () {
}

//----------------------------------------------------------------//
void MOAILuaRefTable::PushRef ( MOAILuaState& state, int refID ) {

	assert ( this->mTableID != LUA_NOREF );
	
	lua_rawgeti ( state, LUA_REGISTRYINDEX, this->mTableID );
	lua_rawgeti ( state, -1, refID );
	lua_replace ( state, -2 );
}

//----------------------------------------------------------------//
int MOAILuaRefTable::Ref ( MOAILuaState& state, int idx ) {

	assert ( this->mTableID != LUA_NOREF );

	idx = state.AbsIndex ( idx );
	int refID = this->ReserveRefID ();

	lua_rawgeti ( state, LUA_REGISTRYINDEX, this->mTableID );
	lua_pushnumber ( state, refID );
	lua_pushvalue ( state, idx );
	lua_settable ( state, -3 );
	
	lua_pop ( state, 1 );
	
	return refID;
}

//----------------------------------------------------------------//
void MOAILuaRefTable::ReleaseRefID ( int refID ) {

	this->mRefIDStack [ this->mRefIDStackTop++ ] = refID;
}

//----------------------------------------------------------------//
int MOAILuaRefTable::ReserveRefID () {

	if ( !this->mRefIDStackTop ) {

		u32 currentSize = this->mRefIDStack.Size ();

		assert ( currentSize <= ( MAX_REF_ID - REFID_CHUNK_SIZE ));

		u32 size = currentSize + REFID_CHUNK_SIZE;
		this->mRefIDStack.Init ( size );
		
		for ( u32 i = 0; i < REFID_CHUNK_SIZE; ++i ) {
			this->mRefIDStack [ i ] = size--;
		}
		this->mRefIDStackTop = REFID_CHUNK_SIZE;
	}
	
	assert ( this->mRefIDStackTop );
	
	return this->mRefIDStack [ --this->mRefIDStackTop ];
}

//----------------------------------------------------------------//
void MOAILuaRefTable::Unref ( MOAILuaState& state, int refID ) {

	assert ( this->mTableID != LUA_NOREF );

	lua_rawgeti ( state, LUA_REGISTRYINDEX, this->mTableID );
	lua_pushnumber ( state, refID );
	lua_pushnil ( state );
	lua_settable ( state, -3 );
	
	lua_pop ( state, 1 );
	
	this->ReleaseRefID ( refID );
}

//================================================================//
// MOAILuaRef
//================================================================//

//----------------------------------------------------------------//
void MOAILuaRef::Clear () {

	if ( MOAILuaRuntime::IsValid ()) {

		if (( this->mRef != LUA_NOREF ) && this->mOwnsRef ) {

			MOAILuaRuntime& luaRuntime = MOAILuaRuntime::Get ();
			MOAILuaStateHandle state = luaRuntime.State ();

			if ( this->mWeak ) {
				luaRuntime.mWeakRefTable.Unref ( state, this->mRef );
			}
			else {
				luaRuntime.mStrongRefTable.Unref ( state, this->mRef );
			}
		}
	}

	this->mRef = LUA_NOREF;
	this->mOwnsRef = false;
}

//----------------------------------------------------------------//
u32 MOAILuaRef::GetID () {

	return this->mRef;
}

//----------------------------------------------------------------//
MOAILuaStateHandle MOAILuaRef::GetSelf () {

	assert ( !this->IsNil ());

	MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
	this->PushRef ( state );
	return state;
}

//----------------------------------------------------------------//
bool MOAILuaRef::IsNil () {

	return ( this->mRef == LUA_NOREF );
}

//----------------------------------------------------------------//
bool MOAILuaRef::IsWeak () {

	return this->mWeak;
}

//----------------------------------------------------------------//
void MOAILuaRef::MakeStrong () {

	if ( !this->mWeak ) return;
	if ( this->mRef == LUA_NOREF ) return;
	
	this->mWeak = false;
	
	if ( MOAILuaRuntime::IsValid ()) {

		MOAILuaRuntime& luaRuntime = MOAILuaRuntime::Get ();
		MOAILuaStateHandle state = luaRuntime.State ();

		luaRuntime.mWeakRefTable.PushRef ( state, this->mRef );
		luaRuntime.mWeakRefTable.Unref ( state, this->mRef );
		
		this->mRef = luaRuntime.mStrongRefTable.Ref ( state, -1 );
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
void MOAILuaRef::MakeWeak () {

	if ( this->mWeak ) return;
	if ( this->mRef == LUA_NOREF ) return;
	
	this->mWeak = true;
	
	if ( MOAILuaRuntime::IsValid ()) {

		MOAILuaRuntime& luaRuntime = MOAILuaRuntime::Get ();
		MOAILuaStateHandle state = luaRuntime.State ();

		luaRuntime.mStrongRefTable.PushRef ( state, this->mRef );
		luaRuntime.mStrongRefTable.Unref ( state, this->mRef );
		
		this->mRef = luaRuntime.mWeakRefTable.Ref ( state, -1 );
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
MOAILuaRef::MOAILuaRef () :
	mRef ( LUA_NOREF ),
	mOwnsRef ( false ) {
}

//----------------------------------------------------------------//
MOAILuaRef::MOAILuaRef ( const MOAILuaRef& assign ) :
	mRef ( LUA_NOREF ),
	mOwnsRef ( false ) {
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

	MOAILuaRuntime& luaRuntime = MOAILuaRuntime::Get ();

	if ( this->mWeak ) {
		luaRuntime.mWeakRefTable.PushRef ( state, this->mRef );
	}
	else {
		luaRuntime.mStrongRefTable.PushRef ( state, this->mRef );
	}
	
	if ( lua_isnil ( state, -1 )) {
		this->mRef = LUA_NOREF;
		this->mOwnsRef = false;
		return false;
	}
	return true;
}

//----------------------------------------------------------------//
void MOAILuaRef::SetRef ( MOAILuaState& state, int idx, bool weak ) {

	this->Clear ();
	this->mWeak = weak;

	if ( lua_isnil ( state, idx ) == false ) {

		MOAILuaRuntime& luaRuntime = MOAILuaRuntime::Get ();

		if ( weak ) {
			this->mRef = luaRuntime.mWeakRefTable.Ref ( state, idx );
		}
		else {
			this->mRef = luaRuntime.mStrongRefTable.Ref ( state, idx );	
		}
		
		this->mOwnsRef = true;
	}
}

//----------------------------------------------------------------//
void MOAILuaRef::SetStrongRef ( MOAILuaState& state, int idx ) {

	this->SetRef ( state, idx, false );
}

//----------------------------------------------------------------//
void MOAILuaRef::SetWeakRef ( MOAILuaState& state, int idx ) {

	this->SetRef ( state, idx, true );
}

//----------------------------------------------------------------//
void MOAILuaRef::Take ( const MOAILuaRef& assign ) {

	this->Clear ();

	this->mRef = assign.mRef;
	this->mOwnsRef = assign.mOwnsRef;
	this->mWeak = assign.mWeak;

	// cast the const away
	(( MOAILuaRef& )assign ).mOwnsRef = false;
}

//----------------------------------------------------------------//
void MOAILuaRef::WeakCopy ( const MOAILuaRef& assign ) {

	this->Clear ();

	this->mRef = assign.mRef;
	this->mOwnsRef = false;
	this->mWeak = assign.mWeak;
}

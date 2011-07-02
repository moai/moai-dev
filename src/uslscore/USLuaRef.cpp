// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USLuaRef.h>
#include <uslscore/USLuaRuntime.h>

//================================================================//
// USLuaRefTable
//================================================================//

//----------------------------------------------------------------//
void USLuaRefTable::Clear () {

	if ( this->mTableID != LUA_NOREF ) {
		
		USLuaStateHandle state = USLuaRuntime::Get ().State ();
		
		luaL_unref ( state, LUA_REGISTRYINDEX, this->mTableID );
		this->mTableID = LUA_NOREF;
	}
	
	this->mRefIDStack.Clear ();
	this->mRefIDStackTop = 0;
}

//----------------------------------------------------------------//
void USLuaRefTable::InitStrong () {

	USLuaStateHandle state = USLuaRuntime::Get ().State ();

	// create the table
	lua_newtable ( state );

	// and grab the table ref
	this->mTableID = luaL_ref ( state, LUA_REGISTRYINDEX );
}

//----------------------------------------------------------------//
void USLuaRefTable::InitWeak () {

	USLuaStateHandle state = USLuaRuntime::Get ().State ();

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
void USLuaRefTable::PushRef ( USLuaState& state, int refID ) {

	assert ( this->mTableID != LUA_NOREF );
	
	lua_rawgeti ( state, LUA_REGISTRYINDEX, this->mTableID );
	lua_rawgeti ( state, -1, refID );
	lua_replace ( state, -2 );
}

//----------------------------------------------------------------//
int USLuaRefTable::Ref ( USLuaState& state, int idx ) {

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
void USLuaRefTable::ReleaseRefID ( int refID ) {

	this->mRefIDStack [ this->mRefIDStackTop++ ] = refID;
}

//----------------------------------------------------------------//
int USLuaRefTable::ReserveRefID () {

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
void USLuaRefTable::Unref ( USLuaState& state, int refID ) {

	assert ( this->mTableID != LUA_NOREF );

	lua_rawgeti ( state, LUA_REGISTRYINDEX, this->mTableID );
	lua_pushnumber ( state, refID );
	lua_pushnil ( state );
	lua_settable ( state, -3 );
	
	lua_pop ( state, 1 );
	
	this->ReleaseRefID ( refID );
}

//----------------------------------------------------------------//
USLuaRefTable::USLuaRefTable () :
	mTableID ( LUA_NOREF ),
	mRefIDStackTop ( 0 ) {
}

//----------------------------------------------------------------//
USLuaRefTable::~USLuaRefTable () {
}

//================================================================//
// USLuaRef
//================================================================//

//----------------------------------------------------------------//
void USLuaRef::Clear () {

	if ( USLuaRuntime::IsValid ()) {

		if (( this->mRef != LUA_NOREF ) && this->mOwnsRef ) {

			USLuaRuntime& luaRuntime = USLuaRuntime::Get ();
			USLuaStateHandle state = luaRuntime.State ();

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
bool USLuaRef::IsWeak () {

	return this->mWeak;
}

//----------------------------------------------------------------//
void USLuaRef::MakeStrong () {

	if ( !this->mWeak ) return;
	if ( this->mRef == LUA_NOREF ) return;
	
	this->mWeak = false;
	
	if ( USLuaRuntime::IsValid ()) {

		USLuaRuntime& luaRuntime = USLuaRuntime::Get ();
		USLuaStateHandle state = luaRuntime.State ();

		luaRuntime.mWeakRefTable.PushRef ( state, this->mRef );
		luaRuntime.mWeakRefTable.Unref ( state, this->mRef );
		
		this->mRef = luaRuntime.mStrongRefTable.Ref ( state, -1 );
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
void USLuaRef::MakeWeak () {

	if ( this->mWeak ) return;
	if ( this->mRef == LUA_NOREF ) return;
	
	this->mWeak = true;
	
	if ( USLuaRuntime::IsValid ()) {

		USLuaRuntime& luaRuntime = USLuaRuntime::Get ();
		USLuaStateHandle state = luaRuntime.State ();

		luaRuntime.mStrongRefTable.PushRef ( state, this->mRef );
		luaRuntime.mStrongRefTable.Unref ( state, this->mRef );
		
		this->mRef = luaRuntime.mWeakRefTable.Ref ( state, -1 );
		state.Pop ( 1 );
	}
}

//----------------------------------------------------------------//
bool USLuaRef::PushRef ( USLuaState& state ) {

	if ( this->mRef == LUA_NOREF ) {
		lua_pushnil ( state );
		return false;
	}

	USLuaRuntime& luaRuntime = USLuaRuntime::Get ();

	if ( this->mWeak ) {
		luaRuntime.mWeakRefTable.PushRef ( state, this->mRef );
	}
	else {
		luaRuntime.mStrongRefTable.PushRef ( state, this->mRef );
	}

	// make sure
	return !lua_isnil ( state, -1 );
}

//----------------------------------------------------------------//
void USLuaRef::SetRef ( USLuaState& state, int idx, bool weak ) {

	this->Clear ();
	this->mWeak = weak;

	int top = state.GetTop ();

	if ( lua_isnil ( state, idx ) == false ) {

		USLuaRuntime& luaRuntime = USLuaRuntime::Get ();

		if ( weak ) {
			this->mRef = luaRuntime.mWeakRefTable.Ref ( state, idx );
		}
		else {
			this->mRef = luaRuntime.mStrongRefTable.Ref ( state, idx );	
		}
		
		this->mOwnsRef = true;
	}
	
	assert ( top == state.GetTop ());
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

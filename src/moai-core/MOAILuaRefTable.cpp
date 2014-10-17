// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAILuaClass.h>
#include <moai-core/MOAILuaRefTable.h>
#include <moai-core/MOAILuaRuntime.h>

//================================================================//
// MOAILuaRefTable
//================================================================//

//----------------------------------------------------------------//
void MOAILuaRefTable::Clear () {

	if ( this->mTableID != LUA_NOREF ) {

		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

		luaL_unref ( state, LUA_REGISTRYINDEX, this->mTableID );
		this->mTableID = LUA_NOREF;
	}

	this->mRefIDStack.Clear ();
	this->mRefIDStackTop = 0;
}

//----------------------------------------------------------------//
void MOAILuaRefTable::InitStrong () {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	// create the table
	lua_newtable ( state );

	// and grab the table ref
	this->mTableID = luaL_ref ( state, LUA_REGISTRYINDEX );
}

//----------------------------------------------------------------//
void MOAILuaRefTable::InitWeak () {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	// create the table
	lua_newtable ( state );

	// create the metatable
	lua_newtable ( state );

	// make it weak
	lua_pushstring ( state, "v" );
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
	
	// push table[refID]
	lua_rawgeti ( state, LUA_REGISTRYINDEX, this->mTableID );
	lua_rawgeti ( state, -1, refID );
	lua_replace ( state, -2 );
}

//----------------------------------------------------------------//
void MOAILuaRefTable::PushRefTable ( MOAILuaState& state ) {

	lua_rawgeti ( state, LUA_REGISTRYINDEX, this->mTableID );
}

//----------------------------------------------------------------//
int MOAILuaRefTable::Ref ( MOAILuaState& state, int idx ) {

	assert ( this->mTableID != LUA_NOREF );

	idx = state.AbsIndex ( idx );
	int refID = this->ReserveRefID ();

	// table[refID] = idx
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

		// TODO: 64-bit
		u32 currentSize = ( u32 )this->mRefIDStack.Size ();

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
void MOAILuaRefTable::Unref ( int refID ) {

	assert ( this->mTableID != LUA_NOREF );

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();

	// table[refID] = NULL
	lua_rawgeti ( state, LUA_REGISTRYINDEX, this->mTableID );
	lua_pushnumber ( state, refID );
	lua_pushnil ( state );
	lua_settable ( state, -3 );

	lua_pop ( state, 1 );

	this->ReleaseRefID ( refID );
}

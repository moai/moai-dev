// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAILuaObject.h>
#include <moai-core/MOAILuaState.h>
#include <moai-core/MOAIScopedLuaState.h>
#include <moai-core/MOAILuaRuntime.h>
#include <moai-core/MOAILogMessages.h>
#include <moai-core/MOAILuaState-impl.h>

//================================================================//
// MOAIScopedLuaState
//================================================================//

//----------------------------------------------------------------//
void MOAIScopedLuaState::PinTop () {

	this->PinTop ( this->GetTop ());
}

//----------------------------------------------------------------//
void MOAIScopedLuaState::PinTop ( int top ) {

	this->mRestoreTop = this->AbsIndex ( top );
}

//----------------------------------------------------------------//
void MOAIScopedLuaState::Set ( lua_State* L ) {

	this->Restore ();
	
	this->mState = L;
	this->mRestoreTop = lua_gettop ( this->mState );
}

//----------------------------------------------------------------//
void MOAIScopedLuaState::Take ( const MOAIScopedLuaState& assign ) {

	this->Restore ();

	this->mState = assign.mState;
	this->mRestoreTop = assign.mRestoreTop;

	(( MOAIScopedLuaState& )assign ).mState = 0;
	(( MOAIScopedLuaState& )assign ).mRestoreTop = 0;
}

//----------------------------------------------------------------//
MOAIScopedLuaState::MOAIScopedLuaState () :
	mRestoreTop ( 0 ) {
}

//----------------------------------------------------------------//
MOAIScopedLuaState::MOAIScopedLuaState ( lua_State* L ) {

	this->mState = L;
	this->mRestoreTop = lua_gettop ( this->mState );
}

//----------------------------------------------------------------//
MOAIScopedLuaState::MOAIScopedLuaState ( MOAILuaState& state ) {

	this->mState = state.mState;
	this->mRestoreTop = lua_gettop ( this->mState );
}

//----------------------------------------------------------------//
MOAIScopedLuaState::MOAIScopedLuaState ( const MOAIScopedLuaState& assign ) :
	MOAILuaState () {

	this->Take ( assign );
}

//----------------------------------------------------------------//
MOAIScopedLuaState::~MOAIScopedLuaState () {

	this->Restore ();
}

//----------------------------------------------------------------//
void MOAIScopedLuaState::Restore () {

	if ( this->mState ) {
		if ( lua_gettop ( this->mState ) != this->mRestoreTop ) {
			lua_settop ( this->mState, this->mRestoreTop );
		}
	}
}


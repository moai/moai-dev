// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-core/MOAILuaObject.h>
#include <moai-core/MOAILuaState.h>
#include <moai-core/MOAILuaStateHandle.h>
#include <moai-core/MOAILuaRuntime.h>
#include <moai-core/MOAILogMessages.h>
#include <moai-core/MOAILuaState-impl.h>

//================================================================//
// MOAILuaStateHandle
//================================================================//

//----------------------------------------------------------------//
void MOAILuaStateHandle::PinTop () {

	this->PinTop ( this->GetTop ());
}

//----------------------------------------------------------------//
void MOAILuaStateHandle::PinTop ( int top ) {

	this->mRestoreTop = this->AbsIndex ( top );
}

//----------------------------------------------------------------//
void MOAILuaStateHandle::Take ( const MOAILuaStateHandle& assign ) {

	this->mState = assign.mState;
	this->mRestoreTop = assign.mRestoreTop;

	(( MOAILuaStateHandle& )assign ).mState = 0;
	(( MOAILuaStateHandle& )assign ).mRestoreTop = 0;
}

//----------------------------------------------------------------//
MOAILuaStateHandle::MOAILuaStateHandle () :
	mRestoreTop ( 0 ) {
}

//----------------------------------------------------------------//
MOAILuaStateHandle::MOAILuaStateHandle ( lua_State* L ) {

	this->mState = L;
	this->mRestoreTop = lua_gettop ( this->mState );
}

//----------------------------------------------------------------//
MOAILuaStateHandle::MOAILuaStateHandle ( MOAILuaState& state ) {

	this->mState = state.mState;
	this->mRestoreTop = lua_gettop ( this->mState );
}

//----------------------------------------------------------------//
MOAILuaStateHandle::MOAILuaStateHandle ( const MOAILuaStateHandle& assign ) :
	MOAILuaState () {

	this->Take ( assign );
}

//----------------------------------------------------------------//
MOAILuaStateHandle::~MOAILuaStateHandle () {

	if ( this->mState ) {
		if ( lua_gettop ( this->mState ) != this->mRestoreTop ) {
			lua_settop ( this->mState, this->mRestoreTop );
		}
	}
}

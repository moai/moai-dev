// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <uslscore/USLuaObject.h>
#include <uslscore/USLuaState.h>
#include <uslscore/USLuaStateHandle.h>
#include <uslscore/USLuaRuntime.h>

#include <uslscore/USLuaState-impl.h>

//================================================================//
// USLuaStateHandle
//================================================================//

//----------------------------------------------------------------//
void USLuaStateHandle::PinTop () {

	this->PinTop ( this->GetTop ());
}

//----------------------------------------------------------------//
void USLuaStateHandle::PinTop ( int top ) {

	this->mRestoreTop = this->AbsIndex ( top );
}

//----------------------------------------------------------------//
void USLuaStateHandle::Take ( const USLuaStateHandle& assign ) {

	this->mState = assign.mState;
	this->mRestoreTop = assign.mRestoreTop;

	(( USLuaStateHandle& )assign ).mState = 0;
	(( USLuaStateHandle& )assign ).mRestoreTop = 0;
}

//----------------------------------------------------------------//
USLuaStateHandle::USLuaStateHandle () :
	mRestoreTop ( 0 ) {
}

//----------------------------------------------------------------//
USLuaStateHandle::USLuaStateHandle ( lua_State* L ) {

	this->mState = L;
	this->mRestoreTop = lua_gettop ( this->mState );
}

//----------------------------------------------------------------//
USLuaStateHandle::USLuaStateHandle ( USLuaState& state ) {

	this->mState = state.mState;
	this->mRestoreTop = lua_gettop ( this->mState );
}

//----------------------------------------------------------------//
USLuaStateHandle::USLuaStateHandle ( const USLuaStateHandle& assign ) :
	USLuaState () {

	this->Take ( assign );
}

//----------------------------------------------------------------//
USLuaStateHandle::~USLuaStateHandle () {

	if ( this->mState ) {
		if ( lua_gettop ( this->mState ) != this->mRestoreTop ) {
			lua_settop ( this->mState, this->mRestoreTop );
		}
	}
}

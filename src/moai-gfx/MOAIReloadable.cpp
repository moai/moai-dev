// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIReloadable.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setReloader
	@text	The reloaded is called prior to recreating the resource. It should
			in turn call the resources regular load or init methods.
 
	@in		MOAIAbstractRecoverableGL self
	@opt	function reloader
	@out	nil
*/
int MOAIReloadable::_setReloader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIReloadable, "U" )

	self->mReloader.SetRef ( *self, state, 2 );
	self->Reload ();
	return 0;
}

//================================================================//
// MOAIReloadable
//================================================================//

//----------------------------------------------------------------//
bool MOAIReloadable::ClearReloadable () {
	this->mIsLoaded = false;
	this->MOAIReloadable_ClearRecoverable ();
}

//----------------------------------------------------------------//
bool MOAIReloadable::FinishLoading () {
	if ( !this->mIsLoaded ) {
		this->mIsLoaded = this->MOAIReloadable_FinishLoading ();
	}
	return this->mIsLoaded;
}

//----------------------------------------------------------------//
bool MOAIReloadable::HasReloader () const {
	return ( bool )this->mReloader;
}

//----------------------------------------------------------------//
MOAIReloadable::MOAIReloadable () :
	mIsLoaded ( false ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIReloadable::~MOAIReloadable () {
}

//----------------------------------------------------------------//
bool MOAIReloadable::Reload () {

	this->ClearReloadable ();

	if ( this->mReloader ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mReloader.PushRef ( state )) {
			state.DebugCall ( 0, 0 );
			return true;
		}
	}
	return this->MOAIReloadable_FinishLoading ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIReloadable::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( composer );
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIReloadable::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( composer );
	UNUSED ( state );

	luaL_Reg regTable [] = {
		{ "setReloader",				_setReloader },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIReloadable::MOAIReloadable_ClearRecoverable () {
}

//----------------------------------------------------------------//
bool MOAIReloadable::MOAIReloadable_FinishLoading () {
	return true;
}

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPathStepper.h>
#include <moai-sim/MOAIPathStepper.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
// TODO: path
int MOAIPathStepper::_reset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathStepper, "U" );
	
	self->Reset ();
	return 0;
}

//----------------------------------------------------------------//
// TODO: path
int MOAIPathStepper::_setPath ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathStepper, "U" );
	
	MOAIPath* path = state.GetLuaObject < MOAIPath >( 2, true );
	self->mPath.Set ( *self, path );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: path
int MOAIPathStepper::_setStepSize ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathStepper, "U" );
	
	self->mStep					= state.GetValue < float >( 2, 0.0f );
	self->mStepByDistance		= state.GetValue < bool >( 3, false );
	
	return 0;
}

//----------------------------------------------------------------//
// TODO: path
int MOAIPathStepper::_step ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIPathStepper, "U" );
	return 0;
}

//================================================================//
// MOAIPathStepper
//================================================================//

//----------------------------------------------------------------//
MOAIPathStepper::MOAIPathStepper () :
	mT ( 0.0f ),
	mStep ( 0.0f ),
	mIsStepping ( false ),
	mStepByDistance ( false ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
void MOAIPathStepper::RegisterLuaClass ( MOAILuaState& state ) {

	this->mPath.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIPathStepper::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "reset",				_reset },
		{ "setPath",			_setPath },
		{ "setStepSize",		_setStepSize },
		{ "step",				_step },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIPathStepper::Reset () {

	this->mIsStepping = false;
}

//----------------------------------------------------------------//
void MOAIPathStepper::Step () {

	if ( !this->mIsStepping ) {
	}

	if ( this->mStepByDistance ) {
	}
	else {
	}
}

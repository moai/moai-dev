// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGfxResource.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAISim.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxResource::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxResource::_getAge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	u32 age = MOAISim::Get ().GetRenderCounter () - self->mLastRenderCount;
	lua_pushnumber ( state, age );

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxResource::_preload ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	return 0;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIGfxResource::_setRenewCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	return 0;
}

//----------------------------------------------------------------//
/**	@name	softRelease
	@text	Attempt to release the resource. Generally this is used when
			responding to a memory warning from the system. A resource
			will only be released if it is reneweable (i.e. has a renew
			callback or contains all information needed to reload the
			resources on demand).
			
			Using soft release can save an app in extreme memory
			circumstances, but may trigger reloads of resources during
			runtime which can significantly degrade performance.
 
	@in		MOAITexture self
	@opt	int age				Release only if the texture hasn't been used in X frames.
	@out	boolean				True if the texture was actually released.
*/
int MOAIGfxResource::_softRelease ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	int age = state.GetValue < int >( 2, 0 );
	lua_pushboolean ( L, self->SoftReleaseGfxResource ( age ));

	return 1;
}

//================================================================//
// MOAIGfxResource
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxResource::Affirm () {

	if ( this->mState == STATE_RENEW ) {
		this->mState = STATE_CLEAR;
	
		if ( this->mOnRenew ) {
			MOAILuaStateHandle state = MOAILuaRuntime::Get ().State ();
			this->PushLocal ( state, this->mOnRenew );
			this->PushLuaUserdata ( state );
			state.DebugCall ( 1, 0 );
		}
		else if ( this->IsRenewable ()) {
			this->OnRenew ();
		}
	}
	
	if ( this->IsValid ()) return true;
	if ( this->mState != STATE_CLEAR ) return false;
	
	this->OnLoad ();
	
	return this->IsValid ();
}

//----------------------------------------------------------------//
bool MOAIGfxResource::Bind () {

	if ( this->Affirm ()) {
		this->OnBind ();
		
		this->mLastRenderCount = MOAISim::Get ().GetRenderCounter ();
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxResource::Clear () {

	this->OnUnload ();
	this->OnClear ();
	this->mState = STATE_CLEAR;
}

//----------------------------------------------------------------//
MOAIGfxResource::MOAIGfxResource () :
	mState ( STATE_CLEAR ),
	mLastRenderCount ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )

	this->mLink.Data ( this );
	
	MOAIGfxDevice::Get ().InsertGfxResource ( *this );
}

//----------------------------------------------------------------//
MOAIGfxResource::~MOAIGfxResource () {

	if ( MOAIGfxDevice::IsValid ()) {
		MOAIGfxDevice::Get ().RemoveGfxResource ( *this );
	}
}

//----------------------------------------------------------------//
void MOAIGfxResource::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIGfxResource::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "clear",					_clear },
		{ "getAge",					_getAge },
		{ "preload",				_preload },
		{ "setRenewCallback",		_setRenewCallback },
		{ "softRelease",			_softRelease },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIGfxResource::ReleaseGfxResource () {

	if ( this->mState != STATE_CLEAR ) return;

	this->OnUnload ();
	this->mState = STATE_WAIT_RENEW;
}

//----------------------------------------------------------------//
void MOAIGfxResource::RenewGfxResource () {

	if ( this->mState == STATE_WAIT_RENEW ) {
		this->mState = STATE_RENEW;
	}
}

//----------------------------------------------------------------//
void MOAIGfxResource::SetError () {

	this->mState = STATE_ERROR;	
}

//----------------------------------------------------------------//
bool MOAIGfxResource::SoftReleaseGfxResource ( u32 age ) {

	if ( this->mState != STATE_CLEAR ) return false;

	age = MOAISim::Get ().GetRenderCounter () - age;

	if ( this->mLastRenderCount <= age ) {
		if ( this->IsRenewable ()) {
			this->OnUnload ();
			this->mState = STATE_RENEW;
			return true;
		}
	}
	return false;
}

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAIGfxResource.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIRenderMgr.h>

//================================================================//
// MOAIGfxState
//================================================================//

//----------------------------------------------------------------//
MOAIGfxState::MOAIGfxState () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGfxState::~MOAIGfxState () {
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	getAge
	@text	Returns the 'age' of the graphics resource. The age
			is the number of times MOAIRenderMgr has rendered a scene
			since the resource was last bound. It is part of the
			render count, not a timestamp. This may change to be
			time-based in futurecreleases.
 
	@in		MOAIGfxResource self
	@out	number age
*/
int MOAIGfxResource::_getAge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	u32 age = MOAIRenderMgr::Get ().GetRenderCounter () - self->mLastRenderCount;
	lua_pushnumber ( state, age );

	return 1;
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
 
	@in		MOAIGfxResource self
	@opt	int age				Release only if the texture hasn't been used in X frames.
	@out	boolean				True if the texture was actually released.
*/
int MOAIGfxResource::_softRelease ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	int age = state.GetValue < int >( 2, 0 );
	lua_pushboolean ( L, self->SoftRelease ( age ));

	return 1;
}

//================================================================//
// MOAIGfxResource
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxResource::Affirm () {
	
	if ( this->mState == STATE_PRECREATE ) {
		this->OnCreate ();
		this->mState = this->IsValid () ? STATE_READY : STATE_ERROR;
	}
	return this->mState == STATE_READY;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::Bind () {

	if ( !MOAIGfxDevice::Get ().GetHasContext ()) {
		MOAILog ( 0, MOAILogMessages::MOAIGfxResource_MissingDevice );
		return false;
	}

	if ( this->Affirm ()) {
		this->OnBind ();
		
		this->mLastRenderCount = MOAIRenderMgr::Get ().GetRenderCounter ();
		
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxResource::Clear () {

	if ( MOAIGfxDevice::IsValid ()) {
		this->OnDestroy ();
	}
	this->OnInvalidate ();
	this->OnClear ();
	this->mState = STATE_PRELOAD;
}

//----------------------------------------------------------------//
void MOAIGfxResource::Destroy () {

	if ( this->mState == STATE_READY ) {

		this->OnDestroy ();
		this->OnInvalidate ();

		this->mState = STATE_PRELOAD;
	}
	else if ( this->mState == STATE_PRECREATE ) {

		this->mState = STATE_PRELOAD;
	}
}

//----------------------------------------------------------------//
void MOAIGfxResource::Invalidate () {

	if ( this->mState == STATE_READY ) {

		this->OnInvalidate ();

		this->mState = STATE_PRELOAD;
	}
	else if ( this->mState == STATE_PRECREATE ) {

		this->mState = STATE_PRELOAD;
	}
}

//----------------------------------------------------------------//
void MOAIGfxResource::Load () {

	if ( this->mState != STATE_ERROR ) {

		if ( this->mState == STATE_READY ) {
			this->OnDestroy ();
			this->OnInvalidate ();
		}

		this->OnLoad ();
		this->mState = STATE_PRECREATE;
	}
}

//----------------------------------------------------------------//
MOAIGfxResource::MOAIGfxResource () :
	mState ( STATE_PRELOAD ),
	mLastRenderCount ( 0 ) {

	RTTI_SINGLE ( MOAIGfxState )

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
		{ "getAge",					_getAge },
		{ "softRelease",			_softRelease },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
bool MOAIGfxResource::SoftRelease ( u32 age ) {

	if ( this->mState != STATE_READY ) return false;

	age = MOAIRenderMgr::Get ().GetRenderCounter () - age;

	if ( this->mLastRenderCount <= age ) {
		if ( this->IsRenewable ()) {
			this->Load ();
			return true;
		}
	}
	return false;
}

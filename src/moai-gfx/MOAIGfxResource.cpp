// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxResource.h>

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
int MOAIGfxResource::_setReloader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	self->mReloader.SetRef ( *self, state, 2 );
	self->Reload ();
	return 0;
}

//================================================================//
// MOAIGfxResource
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxResource::ClearReloadable () {
	this->MOAIGfxResource_ClearReloadable ();
}

//----------------------------------------------------------------//
bool MOAIGfxResource::FinishLoading () {
	if ( this->MOAIGfxResource_FinishLoading ()) {
		this->ScheduleForGPUUpdate ();
		return true;
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::HasReloader () const {
	return ( bool )this->mReloader;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::IsReadyForUse () const {
	return this->MOAIGfxResource_IsReadyForUse ();
}

//----------------------------------------------------------------//
MOAIGfxResource::MOAIGfxResource () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGfxResource::~MOAIGfxResource () {
}

//----------------------------------------------------------------//
bool MOAIGfxResource::Reload () {

	this->ClearReloadable ();

	if ( this->mReloader ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mReloader.PushRef ( state )) {
			state.DebugCall ( 0, 0 );
			return true;
		}
	}
	return this->MOAIGfxResource_FinishLoading ();
}

//----------------------------------------------------------------//
void MOAIGfxResource::ScheduleForGPUDestroy () {
	this->MOAIGfxResource_ScheduleForGPUDestroy ();
}

//----------------------------------------------------------------//
bool MOAIGfxResource::ScheduleForGPUUpdate () {
	return this->MOAIGfxResource_ScheduleForGPUUpdate ();
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxResource::MOAIGfxResource_ClearReloadable () {
}

//----------------------------------------------------------------//
bool MOAIGfxResource::MOAIGfxResource_FinishLoading () {
	return true;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::MOAIGfxResource_IsReadyForUse () const {
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxResource::MOAIGfxResource_ScheduleForGPUDestroy () {
}

//----------------------------------------------------------------//
bool MOAIGfxResource::MOAIGfxResource_ScheduleForGPUUpdate () {
	return false;
}

//----------------------------------------------------------------//
void MOAIGfxResource::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( composer );
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIGfxResource::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	UNUSED ( composer );
	UNUSED ( state );

	luaL_Reg regTable [] = {
		{ "setReloader",				_setReloader },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

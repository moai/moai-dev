// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIGfxResource.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	destroy
	@text	Releases any CPU memory associated with buffer and schedules
			the GPU resource (if any) for destruction.
 
	@in		MOAIGfxBuffer self
	@out	nil
*/
int	MOAIGfxResource::_destroy ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )
	
	self->Destroy ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	scheduleForGPUUpdate
	@text	Trigger an update of the GPU-side buffer. Call this when
			the backing buffer has been altered.
 
	@in		MOAIGfxResource self
	@out	nil
*/
int MOAIGfxResource::_scheduleForGPUUpdate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )
	
	self->ScheduleForGPUUpdate ();
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setReloader
	@text	The reloaded is called prior to recreating the resource. It should
			in turn call the resources regular load or init methods.
 
	@in		MOAIGfxResourceGL self
	@opt	function reloader
	@out	nil
*/
int MOAIGfxResource::_setReloader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIGfxResource, "U" )

	self->mReloader.SetRef ( *self, state, 2 );
	self->InvokeLoader ();
	return 0;
}

//================================================================//
// MOAIGfxResource
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxResource::Destroy () {
	this->MOAIGfxResource_OnCPUPurgeRecoverable ();
	this->MOAIGfxResource_OnCPUDestroy ();
	this->MOAIGfxResource_ScheduleForGPUDestroy ();
}

//----------------------------------------------------------------//
bool MOAIGfxResource::DoCPUCreate () {
	return this->MOAIGfxResource_DoCPUCreate ();
}

//----------------------------------------------------------------//
bool MOAIGfxResource::HasLoader () {

	return ( bool )this->mReloader;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::InvokeLoader () {

	if ( this->mReloader ) {
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		if ( this->mReloader.PushRef ( state )) {
			state.DebugCall ( 0, 0 );
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------//
bool MOAIGfxResource::IsReady () const {

	return this->MOAIGfxResource_IsReady ();
}

//----------------------------------------------------------------//
MOAIGfxResource::MOAIGfxResource () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGfxResource::~MOAIGfxResource () {
}

//----------------------------------------------------------------//
bool MOAIGfxResource::ScheduleForGPUUpdate ( PipelineHint hint ) {
	return this->MOAIGfxResource_ScheduleForGPUUpdate ( hint );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxResource::MOAIGfxResource_OnCPUDestroy () {
}

//----------------------------------------------------------------//
void MOAIGfxResource::MOAIGfxResource_OnCPUPurgeRecoverable () {
}

//----------------------------------------------------------------//
void MOAIGfxResource::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
}

//----------------------------------------------------------------//
void MOAIGfxResource::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "clear",						_destroy }, // TODO: deprecate
		{ "destroy",					_destroy },
		{ "release",					_destroy }, // TODO: deprecate
		{ "scheduleFlush",				_scheduleForGPUUpdate },
		{ "scheduleForGPUUpdate",		_scheduleForGPUUpdate },
		{ "setReloader",				_setReloader },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIRenderMgr.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getAge
	@text	Returns the 'age' of the graphics resource. The age
			is the number of times MOAIRenderMgr has rendered a scene
			since the resource was last bound. It is part of the
			render count, not a timestamp. This may change to be
			time-based in future releases.
 
	@in		MOAIAbstractGfxResource self
	@out	number age
*/
int MOAIAbstractGfxResource::_getAge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResource, "U" )

	u32 age = MOAIRenderMgr::Get ().GetRenderCounter () - self->ZLGfxResource ().GetLastRenderCount ();
	lua_pushnumber ( state, age );

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractGfxResource::_getResourceState ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResource, "U" )

	state.Push ( self->ZLGfxResource ().GetState ());
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	purge
	@text	Attempt to release the resource. Generally this is used when
			responding to a memory warning from the system. A resource
			will only be released if it is renewable (i.e. has a renew
			callback or contains all information needed to reload the
			resources on demand).
			
			Using soft release can save an app in extreme memory
			circumstances, but may trigger reloads of resources during
			runtime which can significantly degrade performance.
 
	@in		MOAIAbstractGfxResource self
	@opt	number age				Release only if the texture hasn't been used in X frames.
	@out	boolean released		True if the texture was actually released.
*/
int MOAIAbstractGfxResource::_purge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResource, "U" )

	int age = state.GetValue < int >( 2, 0 );
	lua_pushboolean ( L, self->ZLGfxResource ().Purge ( age ));

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractGfxResource::_scheduleForGPUCreate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResource, "U" )

	u32 listID = state.GetValue < u32 >( 2, ZLGfxPipelineClerk::DRAWING_PIPELINE );
	self->ZLGfxResource ().ScheduleForGPUCreate ( listID );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setReloader
	@text	The reloaded is called prior to recreating the resource. It should
			in turn call the resources regular load or init methods.
 
	@in		MOAIAbstractGfxResource self
	@opt	function reloader
	@out	nil
*/
int MOAIAbstractGfxResource::_setReloader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResource, "U" )

	self->mReloader.SetRef ( *self, state, 2 );
	self->ZLGfxResource ().InvokeLoader ();
	return 0;
}

//================================================================//
// MOAIAbstractGfxResource
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxResource::MOAIAbstractGfxResource () {

	RTTI_SINGLE ( MOAIInstanceEventSource )
}

//----------------------------------------------------------------//
MOAIAbstractGfxResource::~MOAIAbstractGfxResource () {

	this->mReloader.Clear ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxResource::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIInstanceEventSource::RegisterLuaClass ( state );
	
	state.SetField ( -1, "STATE_UNINITIALIZED",					( u32 )ZLAbstractGfxResource::STATE_UNINITIALIZED );
	state.SetField ( -1, "STATE_READY_FOR_CPU_CREATE",			( u32 )ZLAbstractGfxResource::STATE_READY_FOR_CPU_CREATE );
	state.SetField ( -1, "STATE_READY_FOR_GPU_CREATE",			( u32 )ZLAbstractGfxResource::STATE_READY_FOR_GPU_CREATE );
	state.SetField ( -1, "STATE_READY_TO_BIND",					( u32 )ZLAbstractGfxResource::STATE_READY_TO_BIND );
	state.SetField ( -1, "STATE_ERROR",							( u32 )ZLAbstractGfxResource::STATE_ERROR );
	
	state.SetField ( -1, "GFX_EVENT_CREATED",					( u32 )ZLAbstractGfxResource::GFX_EVENT_CREATED );
	
	state.SetField ( -1, "DRAWING_PIPELINE",					( u32 )ZLGfxPipelineClerk::DRAWING_PIPELINE );
	state.SetField ( -1, "LOADING_PIPELINE",					( u32 )ZLGfxPipelineClerk::LOADING_PIPELINE );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxResource::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIInstanceEventSource::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getAge",					_getAge },
		{ "getResourceState",		_getResourceState },
		{ "purge",					_purge },
		{ "softRelease",			_purge }, // back compat
		{ "scheduleForGPUCreate",	_scheduleForGPUCreate },
		{ "setReloader",			_setReloader },
		{ NULL, NULL }
	};
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
ZLAbstractGfxResource& MOAIAbstractGfxResource::ZLGfxResource () {

	return this->MOAIAbstractGfxResource_ZLAbstractGfxResource ();
}

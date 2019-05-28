// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxResourceGL.h>
#include <moai-gfx/MOAIRenderMgr.h>

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
 
	@in		MOAIAbstractGfxResourceGL self
	@out	number age
*/
int MOAIAbstractGfxResourceGL::_getAge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResourceGL, "U" )

	u32 age = MOAIRenderMgr::Get ().GetRenderCounter () - self->ZLGfxResourceGL ().GetLastRenderCount ();
	lua_pushnumber ( state, age );

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractGfxResourceGL::_getResourceState ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResourceGL, "U" )

	state.Push ( self->ZLGfxResourceGL ().GetState ());
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
 
	@in		MOAIAbstractGfxResourceGL self
	@opt	number age				Release only if the texture hasn't been used in X frames.
	@out	boolean released		True if the texture was actually released.
*/
int MOAIAbstractGfxResourceGL::_purge ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResourceGL, "U" )

	int age = state.GetValue < int >( 2, 0 );
	lua_pushboolean ( L, self->ZLGfxResourceGL ().Purge ( age ));

	return 1;
}

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIAbstractGfxResourceGL::_scheduleForGPUCreate ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResourceGL, "U" )

	u32 listID = state.GetValue < u32 >( 2, ZLGfxPipelineClerkGL::DRAWING_PIPELINE );
	self->ZLGfxResourceGL ().ScheduleForGPUCreate ( listID );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setReloader
	@text	The reloaded is called prior to recreating the resource. It should
			in turn call the resources regular load or init methods.
 
	@in		MOAIAbstractGfxResourceGL self
	@opt	function reloader
	@out	nil
*/
int MOAIAbstractGfxResourceGL::_setReloader ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIAbstractGfxResourceGL, "U" )

	self->mReloader.SetRef ( *self, state, 2 );
	self->ZLGfxResourceGL ().InvokeLoader ();
	return 0;
}

//================================================================//
// MOAIAbstractGfxResourceGL
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractGfxResourceGL::MOAIAbstractGfxResourceGL () {

	RTTI_SINGLE ( MOAIInstanceEventSource )
}

//----------------------------------------------------------------//
MOAIAbstractGfxResourceGL::~MOAIAbstractGfxResourceGL () {

	this->mReloader.Clear ();
}

//----------------------------------------------------------------//
void MOAIAbstractGfxResourceGL::RegisterLuaClass ( MOAILuaState& state ) {
	
	MOAIInstanceEventSource::RegisterLuaClass ( state );
	
	state.SetField ( -1, "STATE_UNINITIALIZED",					( u32 )ZLAbstractGfxResourceGL::STATE_UNINITIALIZED );
	state.SetField ( -1, "STATE_READY_FOR_CPU_CREATE",			( u32 )ZLAbstractGfxResourceGL::STATE_READY_FOR_CPU_CREATE );
	state.SetField ( -1, "STATE_READY_FOR_GPU_CREATE",			( u32 )ZLAbstractGfxResourceGL::STATE_READY_FOR_GPU_CREATE );
	state.SetField ( -1, "STATE_READY_TO_BIND",					( u32 )ZLAbstractGfxResourceGL::STATE_READY_TO_BIND );
	state.SetField ( -1, "STATE_ERROR",							( u32 )ZLAbstractGfxResourceGL::STATE_ERROR );
	
	state.SetField ( -1, "GFX_EVENT_CREATED",					( u32 )ZLAbstractGfxResourceGL::GFX_EVENT_CREATED );
	
	state.SetField ( -1, "DRAWING_PIPELINE",					( u32 )ZLGfxPipelineClerkGL::DRAWING_PIPELINE );
	state.SetField ( -1, "LOADING_PIPELINE",					( u32 )ZLGfxPipelineClerkGL::LOADING_PIPELINE );
}

//----------------------------------------------------------------//
void MOAIAbstractGfxResourceGL::RegisterLuaFuncs ( MOAILuaState& state ) {

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
ZLAbstractGfxResourceGL& MOAIAbstractGfxResourceGL::ZLGfxResourceGL () {

	return this->MOAIAbstractGfxResource_ZLAbstractGfxResource ();
}

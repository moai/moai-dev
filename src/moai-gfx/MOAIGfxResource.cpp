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
MOAIGfxResource::MOAIGfxResource ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ) {
	
	RTTI_BEGIN ( MOAIGfxResource )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIGfxResource >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxResource::~MOAIGfxResource () {
}

//----------------------------------------------------------------//
bool MOAIGfxResource::Reload () {

	this->ClearReloadable ();

	if ( this->mReloader ) {
		MOAIScopedLuaState state = this->Get < MOAILuaRuntime >().State ();
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
void MOAIGfxResource::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIGfxResource::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "setReloader",				_setReloader },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

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

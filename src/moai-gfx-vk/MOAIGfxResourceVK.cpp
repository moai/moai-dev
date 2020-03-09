// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIGfxResourceVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_RenderTreeVK.h>

//================================================================//
// MOAIGfxResourceVK
//================================================================//

//----------------------------------------------------------------//
MOAIGfxResourceVK::MOAIGfxResourceVK () {

	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIGfxResourceVK )
		RTTI_EXTEND ( MOAIGfxResource )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIGfxResourceVK::~MOAIGfxResourceVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxResourceVK::MOAIGfxResource_IsReadyForUse () const {
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxResourceVK::MOAIGfxResource_ScheduleForGPUDestroy () {
}

//----------------------------------------------------------------//
bool MOAIGfxResourceVK::MOAIGfxResource_ScheduleForGPUUpdate () {
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxResourceVK::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIGfxResourceVK::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

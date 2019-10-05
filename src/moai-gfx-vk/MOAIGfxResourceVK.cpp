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

	RTTI_BEGIN
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
void MOAIGfxResourceVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResource, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIGfxResourceVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResource, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}

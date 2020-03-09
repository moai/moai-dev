// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>

//================================================================//
// MOAITextureVK
//================================================================//

//----------------------------------------------------------------//
MOAITextureVK::MOAITextureVK () {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAITextureVK )
		RTTI_EXTEND ( MOAITexture )
		RTTI_EXTEND ( MOAIGfxResourceVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITextureVK::~MOAITextureVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAITextureVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAITexture, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAITextureVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAITexture, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxResourceVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}

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
void MOAITextureVK::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAITextureVK::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

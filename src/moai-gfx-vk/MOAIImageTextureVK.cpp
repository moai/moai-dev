// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIImageTextureVK.h>

//================================================================//
// MOAIImageTextureVK
//================================================================//

//----------------------------------------------------------------//
MOAIImageTextureVK::MOAIImageTextureVK () {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIImageTextureVK )
		RTTI_EXTEND ( MOAIImageTexture )
		RTTI_EXTEND ( MOAITextureVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIImageTextureVK::~MOAIImageTextureVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIImageTextureVK::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIImageTextureVK::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

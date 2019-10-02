// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAITexture2DVK.h>
#include <moai-gfx-vk/ZLTextureFormat.h>

//================================================================//
// MOAITexture2DVK
//================================================================//

//----------------------------------------------------------------//
MOAITexture2DVK::MOAITexture2DVK () {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAITexture2D )
		RTTI_EXTEND ( MOAITextureVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAITexture2DVK::~MOAITexture2DVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAITexture2DVK::MOAIGfxResource_Clear () {

	this->MOAITextureVK::MOAIGfxResource_Clear ();
	this->MOAITexture2D::MOAIGfxResource_Clear ();
}

//----------------------------------------------------------------//
void MOAITexture2DVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAITexture2D, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAITexture2DVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAITexture2D, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}

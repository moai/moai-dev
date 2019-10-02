// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIFrameBufferTextureVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>

//================================================================//
// MOAIFrameBufferTextureVK
//================================================================//

//----------------------------------------------------------------//
MOAIFrameBufferTextureVK::MOAIFrameBufferTextureVK () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIFrameBufferVK )
		RTTI_EXTEND ( MOAITextureVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBufferTextureVK::~MOAIFrameBufferTextureVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIFrameBufferTextureVK::MOAIFrameBufferVK_AffirmBuffers () {
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIFrameBufferVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIFrameBufferVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureVK::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureVK, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureVK::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAITextureVK, MOAILuaObject_SerializeOut ( composer, state, serializer ));
}

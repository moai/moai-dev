// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>
#include <moai-gfx-vk/MOAIVertexFormatMgrVK.h>

//================================================================//
// MOAIVertexBufferVK
//================================================================//

//----------------------------------------------------------------//
MOAIVertexBufferVK::MOAIVertexBufferVK () {
		
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIVertexBuffer )
		RTTI_EXTEND ( MOAIGfxBufferVK )
	RTTI_END
	
//	this->mTarget = ZGL_BUFFER_TARGET_ARRAY;
}

//----------------------------------------------------------------//
MOAIVertexBufferVK::~MOAIVertexBufferVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexBufferVK::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIVertexBuffer, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBufferVK, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIVertexBufferVK::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIVertexBuffer, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBufferVK, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIVertexBufferVK::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBufferVK, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIVertexBufferVK::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBufferVK, MOAILuaObject_SerializeOut ( composer, state, serializer ));
}

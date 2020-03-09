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
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIFrameBufferTextureVK )
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
void MOAIFrameBufferTextureVK::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureVK::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureVK::MOAILuaObject_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureVK::MOAILuaObject_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
}

// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIVertexBufferVK
//================================================================//

//----------------------------------------------------------------//
MOAIVertexBufferVK::MOAIVertexBufferVK () {
		
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIVertexBufferVK )
		RTTI_EXTEND ( MOAIVertexBuffer )
		RTTI_EXTEND ( MOAIGfxBufferVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIVertexBufferVK::~MOAIVertexBufferVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexBufferVK::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIVertexBufferVK::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIVertexBufferVK::MOAILuaObject_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIVertexBufferVK::MOAILuaObject_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
}

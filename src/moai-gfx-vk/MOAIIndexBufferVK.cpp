// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIIndexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//================================================================//
// MOAIIndexBufferVK
//================================================================//

//----------------------------------------------------------------//
MOAIIndexBufferVK::MOAIIndexBufferVK () {
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIIndexBufferVK )
		RTTI_EXTEND ( MOAIIndexBuffer )
		RTTI_EXTEND ( MOAIGfxBufferVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIIndexBufferVK::~MOAIIndexBufferVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIIndexBufferVK::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIIndexBufferVK::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIIndexBufferVK::MOAILuaObject_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIIndexBufferVK::MOAILuaObject_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
}

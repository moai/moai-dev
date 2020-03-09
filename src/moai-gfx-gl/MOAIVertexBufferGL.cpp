// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIVertexBufferGL.h>
#include <moai-gfx-gl/MOAIVertexFormatGL.h>

//================================================================//
// MOAIVertexBufferGL
//================================================================//

//----------------------------------------------------------------//
MOAIVertexBufferGL::MOAIVertexBufferGL () {
		
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIVertexBufferGL )
		RTTI_EXTEND ( MOAIVertexBuffer )
		RTTI_EXTEND ( MOAIGfxBufferGL )
	RTTI_END
	
	this->mTarget = ZLGfxEnum::BUFFER_TARGET_ARRAY;
}

//----------------------------------------------------------------//
MOAIVertexBufferGL::~MOAIVertexBufferGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIVertexBufferGL::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::MOAILuaObject_SerializeIn ( RTTIVisitorHistory& history, MOAILuaState& state, MOAIDeserializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::MOAILuaObject_SerializeOut ( RTTIVisitorHistory& history, MOAILuaState& state, MOAISerializer& serializer ) {
	if ( history.DidVisit ( *this )) return;
}

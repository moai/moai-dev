// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIIndexBufferGL.h>
#include <moai-gfx-gl/MOAIVertexFormatGL.h>
#include <moai-gfx-gl/MOAIVertexFormatMgrGL.h>

//================================================================//
// MOAIIndexBufferGL
//================================================================//

//----------------------------------------------------------------//
MOAIIndexBufferGL::MOAIIndexBufferGL () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIIndexBuffer )
		RTTI_EXTEND ( MOAIGfxBufferGL )
	RTTI_END
	
	this->mTarget = ZGL_BUFFER_TARGET_ELEMENT_ARRAY;
}

//----------------------------------------------------------------//
MOAIIndexBufferGL::~MOAIIndexBufferGL () {
}

//----------------------------------------------------------------//
void MOAIIndexBufferGL::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIIndexBuffer::RegisterLuaClass ( state );
	MOAIGfxBufferGL::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIIndexBufferGL::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIIndexBuffer::RegisterLuaFuncs ( state );
	MOAIGfxBufferGL::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIIndexBufferGL::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIIndexBuffer::SerializeIn ( state, serializer );
	MOAIGfxBufferGL::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIIndexBufferGL::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIIndexBuffer::SerializeOut ( state, serializer );
	MOAIGfxBufferGL::SerializeOut ( state, serializer );
}

// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIVertexBufferGL.h>
#include <moai-gfx-gl/MOAIVertexFormatGL.h>
#include <moai-gfx-gl/MOAIVertexFormatMgrGL.h>

//================================================================//
// MOAIVertexBufferGL
//================================================================//

//----------------------------------------------------------------//
MOAIVertexBufferGL::MOAIVertexBufferGL () {
		
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIVertexBuffer )
		RTTI_EXTEND ( MOAIGfxBufferGL )
	RTTI_END
	
	this->mTarget = ZGL_BUFFER_TARGET_ARRAY;
}

//----------------------------------------------------------------//
MOAIVertexBufferGL::~MOAIVertexBufferGL () {
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIVertexBuffer::RegisterLuaClass ( state );
	MOAIGfxBufferGL::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIVertexBuffer::RegisterLuaFuncs ( state );
	MOAIGfxBufferGL::RegisterLuaFuncs ( state );
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::SerializeIn ( MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAIGfxBufferGL::SerializeIn ( state, serializer );
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::SerializeOut ( MOAILuaState& state, MOAISerializer& serializer ) {
	MOAIGfxBufferGL::SerializeOut ( state, serializer );
}

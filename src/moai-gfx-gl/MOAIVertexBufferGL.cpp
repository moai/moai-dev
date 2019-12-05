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
		
	RTTI_BEGIN
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
void MOAIVertexBufferGL::MOAILuaObject_RegisterLuaClass ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIVertexBuffer, MOAILuaObject_RegisterLuaClass ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBufferGL, MOAILuaObject_RegisterLuaClass ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::MOAILuaObject_RegisterLuaFuncs ( MOAIComposer& composer, MOAILuaState& state ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIVertexBuffer, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBufferGL, MOAILuaObject_RegisterLuaFuncs ( composer, state ));
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::MOAILuaObject_SerializeIn ( MOAIComposer& composer, MOAILuaState& state, MOAIDeserializer& serializer ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBufferGL, MOAILuaObject_SerializeIn ( composer, state, serializer ));
}

//----------------------------------------------------------------//
void MOAIVertexBufferGL::MOAILuaObject_SerializeOut ( MOAIComposer& composer, MOAILuaState& state, MOAISerializer& serializer ) {
	MOAI_CALL_SUPER_ONCE ( composer, MOAIGfxBufferGL, MOAILuaObject_SerializeOut ( composer, state, serializer ));
}

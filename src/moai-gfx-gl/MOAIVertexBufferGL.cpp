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
MOAIVertexBufferGL::MOAIVertexBufferGL ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIGfxResource ( context ),
	MOAIStream ( context ),
	MOAIGfxBuffer ( context ),
	MOAIVertexBuffer ( context ),
	MOAIEventSource ( context ),
	MOAIInstanceEventSource ( context ),
	MOAIGfxResourceGL ( context ),
	MOAIGfxBufferGL ( context ) {
		
	RTTI_BEGIN ( MOAIVertexBufferGL )
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

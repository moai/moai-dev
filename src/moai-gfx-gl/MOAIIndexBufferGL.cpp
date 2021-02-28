// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-gl/MOAIGfxMgrGL.h>
#include <moai-gfx-gl/MOAIIndexBufferGL.h>
#include <moai-gfx-gl/MOAIVertexFormatGL.h>

//================================================================//
// MOAIIndexBufferGL
//================================================================//

//----------------------------------------------------------------//
MOAIIndexBufferGL::MOAIIndexBufferGL ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIGfxResource ( context ),
	MOAIStream ( context ),
	MOAIGfxBuffer ( context ),
	MOAIIndexBuffer ( context ),
	MOAIEventSource ( context ),
	MOAIInstanceEventSource ( context ),
	MOAIGfxResourceGL ( context ),
	MOAIGfxBufferGL ( context ) {
	
	RTTI_BEGIN ( MOAIIndexBufferGL )
		RTTI_EXTEND ( MOAIIndexBuffer )
		RTTI_EXTEND ( MOAIGfxBufferGL )
	RTTI_END
	
	this->mTarget = ZLGfxEnum::BUFFER_TARGET_ELEMENT_ARRAY;
}

//----------------------------------------------------------------//
MOAIIndexBufferGL::~MOAIIndexBufferGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//

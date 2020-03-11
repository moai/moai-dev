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

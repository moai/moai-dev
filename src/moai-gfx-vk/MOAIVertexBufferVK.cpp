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
		
	RTTI_BEGIN ( MOAIVertexBufferVK )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIVertexBufferVK >)
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

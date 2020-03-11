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
	
	RTTI_BEGIN ( MOAIIndexBufferVK )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIIndexBufferVK >)
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

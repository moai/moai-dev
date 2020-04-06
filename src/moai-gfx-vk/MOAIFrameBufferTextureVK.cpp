// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIFrameBufferTextureVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK.h>

//================================================================//
// MOAIFrameBufferTextureVK
//================================================================//

//----------------------------------------------------------------//
MOAIFrameBufferTextureVK::MOAIFrameBufferTextureVK () {
	
	RTTI_BEGIN ( MOAIFrameBufferTextureVK )
		RTTI_EXTEND ( MOAIFrameBufferVK )
		RTTI_EXTEND ( MOAITextureVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIFrameBufferTextureVK::~MOAIFrameBufferTextureVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractDescriptorElementVK* MOAIFrameBufferTextureVK::MOAIAbstractDescriptorElementStateVK_GetElement ( MOAIMutableWriteDescriptorSetVK& writeDescriptorSet, ZLIndex index ) {
	UNUSED ( writeDescriptorSet );
	UNUSED ( index );
	return NULL;
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureVK::MOAIFrameBufferVK_AffirmBuffers () {
}

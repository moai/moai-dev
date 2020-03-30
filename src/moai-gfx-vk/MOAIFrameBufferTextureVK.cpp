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
void MOAIFrameBufferTextureVK::MOAIAbstractDescriptorElementVK_GetPinnedData ( VkWriteDescriptorSet& writeDescriptorSet, ZLIndex index, MOAICommandBufferVK& commandBuffer ) {
	UNUSED ( writeDescriptorSet );
	UNUSED ( index );
	UNUSED ( commandBuffer );
}

//----------------------------------------------------------------//
void MOAIFrameBufferTextureVK::MOAIFrameBufferVK_AffirmBuffers () {
}

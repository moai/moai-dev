// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK.h>
#include <moai-gfx-vk/MOAIImageTextureVK.h>

//================================================================//
// MOAIImageTextureVK
//================================================================//

//----------------------------------------------------------------//
MOAIImageTextureVK::MOAIImageTextureVK () {
	
	RTTI_BEGIN ( MOAIImageTextureVK )
		RTTI_EXTEND ( MOAIImageTexture )
		RTTI_EXTEND ( MOAITextureVK )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIImageTextureVK::~MOAIImageTextureVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
MOAIAbstractDescriptorElementVK* MOAIImageTextureVK::MOAIAbstractDescriptorElementStateVK_GetElement ( MOAIMutableWriteDescriptorSetVK& writeDescriptorSet, ZLIndex index ) {
	UNUSED ( writeDescriptorSet );
	UNUSED ( index );
	return NULL;
}

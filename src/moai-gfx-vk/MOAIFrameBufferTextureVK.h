// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFRAMEBUFFERTEXTUREVK_H
#define	MOAIFRAMEBUFFERTEXTUREVK_H

#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>

//================================================================//
// MOAIFrameBufferTextureVK
//================================================================//
class MOAIFrameBufferTextureVK :
	public virtual MOAIFrameBufferVK,
	public virtual MOAITextureVK {
private:
	
	//----------------------------------------------------------------//
	void				MOAIAbstractDescriptorElementVK_GetPinnedData	( VkWriteDescriptorSet& writeDescriptorSet, ZLIndex index, MOAICommandBufferVK& commandBuffer );
	void				MOAIFrameBufferVK_AffirmBuffers					();

public:
	
	DECL_LUA_FACTORY ( MOAIFrameBufferTextureVK )
	
	//----------------------------------------------------------------//
						MOAIFrameBufferTextureVK		();
						~MOAIFrameBufferTextureVK		();
};

#endif

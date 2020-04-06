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
	MOAIAbstractDescriptorElementVK*	MOAIAbstractDescriptorElementStateVK_GetElement		( MOAIMutableWriteDescriptorSetVK& writeDescriptorSet, ZLIndex index );
	void								MOAIFrameBufferVK_AffirmBuffers						();

public:
	
	DECL_LUA_FACTORY ( MOAIFrameBufferTextureVK )
	
	//----------------------------------------------------------------//
						MOAIFrameBufferTextureVK		();
						~MOAIFrameBufferTextureVK		();
};

#endif

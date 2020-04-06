// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIIMAGETEXTUREVK_H
#define	MOAIIMAGETEXTUREVK_H

#include <moai-gfx-vk/MOAITextureVK.h>

//================================================================//
// MOAIImageTextureVK
//================================================================//
/**	@lua	MOAIImageTextureVK
	@text	Binds an image (CPU memory) to a texture (GPU memory).
			Regions of the texture (or the entire texture) may be
			invalidated. Invalidated regions will be reloaded
			into GPU memory the next time the texture is bound.
*/
class MOAIImageTextureVK :
	public MOAIImageTexture,
	public MOAITextureVK {
private:

	//----------------------------------------------------------------//
	MOAIAbstractDescriptorElementVK*	MOAIAbstractDescriptorElementStateVK_GetElement		( MOAIMutableWriteDescriptorSetVK& writeDescriptorSet, ZLIndex index );

public:
	
	DECL_LUA_FACTORY ( MOAIImageTextureVK )
	
//	static const u32 DEFAULT_TRANSFORM = ZLImageTransform::TRUECOLOR | ZLImageTransform::PREMULTIPLY_ALPHA;
	
	//----------------------------------------------------------------//
					MOAIImageTextureVK		();
					~MOAIImageTextureVK		();
};

#endif

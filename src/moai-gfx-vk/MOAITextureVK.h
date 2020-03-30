// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTUREVK_H
#define	MOAITEXTUREVK_H

#include <moai-gfx-vk/MOAIAbstractDescriptorElementVK.h>
#include <moai-gfx-vk/MOAIDescriptorSetVK.h>
#include <moai-gfx-vk/MOAIGfxResourceVK.h>

class ZLImage;

//================================================================//
// MOAITextureVK
//================================================================//
/**	@lua	MOAITextureVK
	@text	Base class for texture resources.

	@const	VK_LINEAR
	@const	VK_LINEAR_MIPMAP_LINEAR
	@const	VK_LINEAR_MIPMAP_NEAREST
	@const	VK_NEAREST
	@const	VK_NEAREST_MIPMAP_LINEAR
	@const	VK_NEAREST_MIPMAP_NEAREST
*/
class MOAITextureVK :
	public virtual MOAITexture,
	public virtual MOAIGfxResourceVK,
	public virtual MOAIAbstractDescriptorElementVK {
public:
	
	//----------------------------------------------------------------//
						MOAITextureVK				();
						~MOAITextureVK				();
	
};

#endif

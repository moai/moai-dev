// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITEXTURE2DVK_H
#define	MOAITEXTURE2DVK_H

#include <moai-gfx-vk/MOAIAbstractDescriptorElementStateVK.h>
#include <moai-gfx-vk/MOAISnapshotVK.h>
#include <moai-gfx-vk/MOAILogicalDeviceVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>

class MOAILogicalDeviceVK;
class MOAITextureSnapshot2DVK;
class ZLTextureFormat;

//================================================================//
// MOAITexture2DVK
//================================================================//
/**	@lua	MOAITexture2DVK
	@text	Texture class.
*/
class MOAITexture2DVK :
	public virtual MOAITexture2D,
	public virtual MOAITextureVK,
	public virtual MOAISnapshotFactoryVK < MOAITextureSnapshot2DVK > {
protected:

	ZLStrongPtr < MOAITextureSnapshot2DVK > mSnapshot;

	//----------------------------------------------------------------//
	MOAITextureSnapshot2DVK*			MOAISnapshotFactoryVK_GetSnapshot			();
	MOAIAbstractDescriptorElementVK*	MOAIAbstractDescriptorElementStateVK_GetElement		( MOAIMutableWriteDescriptorSetVK& writeDescriptorSet, ZLIndex index );
	bool								MOAIGfxResource_FinishLoading						();
	bool								MOAIGfxResource_IsReadyForUse						() const;

public:
			
	DECL_LUA_FACTORY ( MOAITexture2DVK )
	
	//----------------------------------------------------------------//
								MOAITexture2DVK			();
								~MOAITexture2DVK		();
};

#endif

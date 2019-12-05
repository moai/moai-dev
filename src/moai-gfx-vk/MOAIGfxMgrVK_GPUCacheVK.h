// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRVK_GPUCACHEVK_H
#define	MOAIGFXMGRVK_GPUCACHEVK_H

#include <moai-gfx-vk/MOAIGfxMgrVKComponents.h>

class MOAIFrameBufferVK;
class MOAIIndexBufferVK;
class MOAIShaderVK;
class MOAIShaderProgramVK;
class MOAIVertexBufferVK;
class MOAIVertexFormatVK;
class MOAIGfxResourceVK;
class MOAIPipelineLayoutVK;
class MOAIVertexBufferWithFormatVK;
class MOAITextureVK;

//================================================================//
// MOAIGfxMgrVK_GPUCacheVK
//================================================================//
class MOAIGfxMgrVK_GPUCacheVK :
	public virtual MOAIGfxMgrVKComponents,
	public virtual MOAIGfxMgr_GPUCache {
protected:

	friend class MOAIGfxMgrVK_VertexCacheVK;

	//----------------------------------------------------------------//
	void			MOAIGfxMgr_GPUCache_ApplyStateChanges			();
	void			MOAIGfxMgr_GPUCache_ClearSurface				(); // takes zgl clear flags
	size_t			MOAIGfxMgr_GPUCache_CountTextureUnits			();
	void			MOAIGfxMgr_GPUCache_DrawPrims					( MOAITopology::Type primType, u32 base, u32 count );
	void			MOAIGfxMgr_GPUCache_ResetGPUState				();
	void			MOAIGfxMgr_GPUCache_UnbindAll					();

public:
	
	//----------------------------------------------------------------//
					MOAIGfxMgrVK_GPUCacheVK					();
	virtual			~MOAIGfxMgrVK_GPUCacheVK				();
};

#endif

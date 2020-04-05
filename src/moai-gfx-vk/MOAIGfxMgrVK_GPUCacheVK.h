// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRVK_GPUCACHEVK_H
#define	MOAIGFXMGRVK_GPUCACHEVK_H

#include <moai-gfx-vk/MOAIGfxMgrVKComponents.h>

class MOAIFrameBufferVK;
class MOAIIndexBufferVK;
class MOAIGfxResourceVK;
class MOAIPipelineLayoutVK;
class MOAIPipelineParamsVK;
class MOAIPipelineVK;
class MOAIShaderProgramVK;
class MOAIShaderVK;
class MOAITextureVK;
class MOAIVertexBufferVK;
class MOAIVertexFormatVK;

//================================================================//
// MOAIGfxMgrVK_GPUCacheVK
//================================================================//
class MOAIGfxMgrVK_GPUCacheVK :
	public virtual MOAIGfxMgrVKComponents,
	public virtual MOAIGfxMgr_GPUCache {
protected:

	friend class MOAIGfxMgrVK_VertexCacheVK;

	STLMap < MOAIPipelineParamsVK, ZLStrongPtr < MOAIPipelineVK > >		mPipelinePool; // TODO: VkPipelineCache

	//----------------------------------------------------------------//
	MOAIPipelineVK* 		AffirmPipeline 			( MOAIPipelineParamsVK& params );

	//----------------------------------------------------------------//
	void			MOAIGfxMgr_GPUCache_ApplyStateChanges			();
	void			MOAIGfxMgr_GPUCache_ClearSurface				(); // takes zgl clear flags
	size_t			MOAIGfxMgr_GPUCache_CountTextureUnits			();
	void			MOAIGfxMgr_GPUCache_DrawPrims					( MOAIGfxTopologyEnum::_ primType, u32 base, u32 count );
	void			MOAIGfxMgr_GPUCache_ResetGPUState				();
	void			MOAIGfxMgr_GPUCache_UnbindAll					();

public:
	
	//----------------------------------------------------------------//
					MOAIGfxMgrVK_GPUCacheVK					();
	virtual			~MOAIGfxMgrVK_GPUCacheVK				();
};

#endif

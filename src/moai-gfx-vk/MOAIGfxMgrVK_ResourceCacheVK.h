// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIGFXMGRVK_RESOURCECACHEVK_H
#define	MOAIGFXMGRVK_RESOURCECACHEVK_H

#include <moai-gfx-vk/MOAIDescriptorSetLayoutImplCacheVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVKComponents.h>
#include <moai-gfx-vk/MOAIPipelineCacheVK.h>

class MOAIGfxMgrVK;

//================================================================//
// MOAIGfxMgrVK_ResourceCacheVK
//================================================================//
class MOAIGfxMgrVK_ResourceCacheVK :
	public virtual MOAIGfxMgrVKComponents,
	public MOAIDescriptorSetLayoutImplCacheVK,
	public MOAIPipelineCacheVK {
protected:

	//----------------------------------------------------------------//
	
public:
	
	friend class MOAIGfxMgr;
	
	//----------------------------------------------------------------//
			MOAIGfxMgrVK_ResourceCacheVK			();
			~MOAIGfxMgrVK_ResourceCacheVK			();
};

#endif

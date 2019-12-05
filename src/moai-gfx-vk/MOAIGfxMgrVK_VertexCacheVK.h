// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRVK_VERTEXCACHEVK_H
#define	MOAIGFXMGRVK_VERTEXCACHEVK_H

#include <moai-gfx-vk/MOAIGfxMgrVKComponents.h>
#include <moai-gfx-vk/MOAIIndexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>

class MOAIGfxResourceVK;

//================================================================//
// MOAIGfxMgrVK_VertexCacheVK
//================================================================//
class MOAIGfxMgrVK_VertexCacheVK :
	public virtual MOAIGfxMgrVKComponents,
	public virtual MOAIGfxMgr_VertexCache {
protected:

	//----------------------------------------------------------------//
	void			MOAIGfxMgr_VertexCache_FlushToGPU		();

public:
	
	//----------------------------------------------------------------//
					MOAIGfxMgrVK_VertexCacheVK				();
					~MOAIGfxMgrVK_VertexCacheVK				();
};

#endif

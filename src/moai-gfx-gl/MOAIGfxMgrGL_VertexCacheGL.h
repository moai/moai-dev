// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGL_VERTEXCACHEGL_H
#define	MOAIGFXMGRGL_VERTEXCACHEGL_H

#include <moai-gfx-gl/MOAIGfxMgrGLComponents.h>
#include <moai-gfx-gl/MOAIIndexBufferGL.h>
#include <moai-gfx-gl/MOAIVertexBufferGL.h>

class MOAIGfxResourceGL;

//================================================================//
// MOAIGfxMgrGL_VertexCacheGL
//================================================================//
class MOAIGfxMgrGL_VertexCacheGL :
	public virtual MOAIGfxMgrGLComponents,
	public virtual MOAIGfxMgr_VertexCache {
protected:

	//----------------------------------------------------------------//
	bool			MOAIGfxMgr_VertexCache_BeginPrim 		( MOAIGfxTopologyEnum::_ primType, u32 vtxCount, u32 idxCount );
	void			MOAIGfxMgr_VertexCache_FlushToGPU		();

public:
	
	//----------------------------------------------------------------//
					MOAIGfxMgrGL_VertexCacheGL				( ZLContext& context );
					~MOAIGfxMgrGL_VertexCacheGL				();
};

#endif

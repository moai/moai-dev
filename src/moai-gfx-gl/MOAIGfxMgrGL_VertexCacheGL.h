// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXMGRGL_VERTEXCACHEGL_H
#define	MOAIGFXMGRGL_VERTEXCACHEGL_H

#include <moai-gfx-gl/MOAIGfxMgrGLComponents.h>
#include <moai-gfx-gl/MOAIIndexBufferGL.h>
#include <moai-gfx-gl/MOAIVertexBufferGL.h>

class MOAIAbstractGfxResourceGL;

//================================================================//
// MOAIGfxMgrGL_VertexCacheGL
//================================================================//
class MOAIGfxMgrGL_VertexCacheGL :
	virtual public MOAIGfxMgrGLComponents,
	virtual public MOAIGfxMgr_VertexCache {
protected:
		
	ZLStrongPtr < MOAIVertexBufferGL >		mVtxBuffer;
	ZLStrongPtr < MOAIIndexBufferGL >		mIdxBuffer;

	//----------------------------------------------------------------//
	void			MOAIGfxMgr_VertexCache_AffirmBuffers	();
	bool			MOAIGfxMgr_VertexCache_BeginPrim 		( u32 primType, u32 vtxCount, u32 idxCount );
	void			MOAIGfxMgr_VertexCache_FlushToGPU		();

public:
	
	//----------------------------------------------------------------//
					MOAIGfxMgrGL_VertexCacheGL				();
					~MOAIGfxMgrGL_VertexCacheGL				();
};

#endif

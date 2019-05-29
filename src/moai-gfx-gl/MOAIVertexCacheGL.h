// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIVERTEXCACHEGL_H
#define	MOAIVERTEXCACHEGL_H

#include <moai-gfx-gl/MOAIAbstractGfxMgrComponentsGL.h>
#include <moai-gfx-gl/MOAIIndexBufferGL.h>
#include <moai-gfx-gl/MOAIVertexBufferGL.h>

class MOAIAbstractGfxResourceGL;

//================================================================//
// MOAIVertexCacheGL
//================================================================//
class MOAIVertexCacheGL :
	virtual public MOAIAbstractGfxMgrComponentsGL,
	virtual public MOAIAbstractVertexCache {
protected:
		
	ZLStrongPtr < MOAIVertexBufferGL >		mVtxBuffer;
	ZLStrongPtr < MOAIIndexBufferGL >		mIdxBuffer;

	//----------------------------------------------------------------//
	void			ZLAbstractVertexCache_AffirmBuffers		();
	bool			ZLAbstractVertexCache_BeginPrim 		( u32 primType, u32 vtxCount, u32 idxCount );
	void			ZLAbstractVertexCache_FlushToGPU		();

public:
	
	//----------------------------------------------------------------//
					MOAIVertexCacheGL				();
					~MOAIVertexCacheGL				();
};

#endif

// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLABSTRACTGFXMGR_H
#define	ZLABSTRACTGFXMGR_H

#include <zl-gfx/ZLAbstractGPU.h>
#include <zl-gfx/ZLGfxStateCPUCache.h>
#include <zl-gfx/ZLGfxStateVertexCache.h>

//================================================================//
// ZLAbstractGfxMgr
//================================================================//
class ZLAbstractGfxMgr :
	public virtual ZLAbstractGPU,
	public virtual ZLGfxStateCPUCache,
	public virtual ZLGfxStateVertexCache {
public:
	
	//----------------------------------------------------------------//
	ZLAbstractGfxMgr () {
	}
	
	//----------------------------------------------------------------//
	virtual ~ZLAbstractGfxMgr () {
	}
};

#endif

// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	ZLGFXSTATECACHE_H
#define	ZLGFXSTATECACHE_H

#include <moai-sim/ZLAbstractGfxStateCache.h>
#include <moai-sim/ZLGfxStateCPUCache.h>
#include <moai-sim/ZLGfxStateGPUCache.h>
#include <moai-sim/ZLGfxStateVertexCache.h>

//================================================================//
// ZLGfxStateFrame
//================================================================//
class ZLGfxStateFrame :
	public ZLGfxStateCPUCacheFrame,
	public ZLGfxStateGPUCacheFrame {
protected:
};

//================================================================//
// ZLGfxStateCache
//================================================================//
class ZLGfxStateCache :
	virtual public ZLAbstractGfxStateCache,
	public ZLGfxStateGPUCache,
	public ZLGfxStateVertexCache {
protected:

	ZLLeanArray < ZLGfxStateFrame* >	mStateStack;
	ZLIndex								mStateStackTop;

	//----------------------------------------------------------------//
	ZLGfxStateCPUCache&			ZLAbstractGfxStateCache_GetGfxStateCacheCPU			();
	ZLGfxStateGPUCache&			ZLAbstractGfxStateCache_GetGfxStateCacheGPU			();
	ZLGfxStateVertexCache&		ZLAbstractGfxStateCache_GetGfxVertexCache			();

public:

	//----------------------------------------------------------------//
	void					FinishFrame					();
	
	ZLMatrix4x4				GetNormToWndMtx				();
	ZLMatrix4x4				GetNormToWndMtx				( const ZLRect& wndRect );
	
	ZLMatrix4x4				GetWorldToWndMtx			();
	ZLMatrix4x4				GetWorldToWndMtx			( const ZLRect& wndRect );
	
	ZLMatrix4x4				GetWndToNormMtx				();
	ZLMatrix4x4				GetWndToNormMtx				( const ZLRect& wndRect );
	
	ZLMatrix4x4				GetWndToWorldMtx			();
	ZLMatrix4x4				GetWndToWorldMtx			( const ZLRect& wndRect );
	
							ZLGfxStateCache				();
							~ZLGfxStateCache			();
	
	void					PopState					();
	void					PushState					();
};

#endif

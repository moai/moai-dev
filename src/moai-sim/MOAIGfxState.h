// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSTATE_H
#define	MOAIGFXSTATE_H

#include <moai-sim/MOAIAbstractGfxStateCache.h>
#include <moai-sim/MOAIGfxStateCPUCache.h>
#include <moai-sim/MOAIGfxStateGPUCache.h>
#include <moai-sim/MOAIGfxStateVertexCache.h>

//================================================================//
// MOAIGfxState
//================================================================//
class MOAIGfxState :
	virtual public MOAIAbstractGfxStateCache,
	public MOAIGfxStateCPUCache,
	public MOAIGfxStateGPUCache,
	public MOAIGfxStateVertexCache {
protected:

	//----------------------------------------------------------------//
	MOAIGfxStateCPUCache&			MOAIAbstractGfxStateCache_GetGfxStateCacheCPU			();
	MOAIGfxStateGPUCache&			MOAIAbstractGfxStateCache_GetGfxStateCacheGPU			();
	MOAIGfxStateVertexCache&		MOAIAbstractGfxStateCache_GetGfxVertexCache				();

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
	
							MOAIGfxState				();
							~MOAIGfxState				();
};

#endif

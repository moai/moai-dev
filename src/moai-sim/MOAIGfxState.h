// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSTATE_H
#define	MOAIGFXSTATE_H

#include <moai-sim/MOAIAbstractGfxStateCache.h>
#include <moai-sim/MOAIGfxStateCPU.h>
#include <moai-sim/MOAIGfxStateGPU.h>
#include <moai-sim/MOAIGfxVertexCache.h>

//================================================================//
// MOAIGfxState
//================================================================//
class MOAIGfxState :
	virtual public MOAIAbstractGfxStateCache,
	public MOAIGfxStateCPU,
	public MOAIGfxStateGPU,
	public MOAIGfxVertexCache {
protected:

	//----------------------------------------------------------------//
	MOAIGfxStateCPU&			MOAIAbstractGfxStateCache_GetGfxStateCacheCPU			();
	MOAIGfxStateGPU&			MOAIAbstractGfxStateCache_GetGfxStateCacheGPU			();
	MOAIGfxVertexCache&			MOAIAbstractGfxStateCache_GetGfxVertexCache				();

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

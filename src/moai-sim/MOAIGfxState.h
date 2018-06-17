// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSTATE_H
#define	MOAIGFXSTATE_H

#include <moai-sim/MOAIGfxStateCPU.h>
#include <moai-sim/MOAIGfxStateGPU.h>

//================================================================//
// MOAIGfxState
//================================================================//
class MOAIGfxState :
	public MOAIGfxStateCPU,
	public MOAIGfxStateGPU {
public:

	//----------------------------------------------------------------//
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

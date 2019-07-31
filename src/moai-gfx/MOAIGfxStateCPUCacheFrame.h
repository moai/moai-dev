// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIGFXSTATECPUCACHEFRAME_H
#define	MOAIGFXSTATECPUCACHEFRAME_H

#include <moai-gfx/MOAIGfxStateCPUConsts.h>

//================================================================//
// MOAIGfxStateCPUCacheFrame
//================================================================//
class MOAIGfxStateCPUCacheFrame {
protected:

	friend class MOAIGfxMgr_CPUCache;
	friend class MOAIGfxMgr_VertexCache;
	
	u64						mDirtyFlags;
	
	ZLMatrix4x4				mMatrices [ MOAIGfxStateConstsCPU::TOTAL_MATRICES ];
	
	ZLFrustum				mViewVolume;
	
	ZLColorVec				mAmbientColor;
	ZLColorVec				mPenColor;
	ZLColorVec				mFinalColor;
	u32						mFinalColor32;
	
	u32						mClearFlags;
	ZLColorVec				mClearColor;
	double					mClearDepth;
};

#endif

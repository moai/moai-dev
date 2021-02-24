// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIAbstractMesh.h>
#include <moai-gfx/MOAIFrameBuffer.h>
#include <moai-gfx/MOAIGfxStateGPUCacheFrame.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>

//================================================================//
// MOAIGfxStateGPUCacheFrame
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxStateGPUCacheFrame::Clear () {

	this->mShader			= NULL;
	this->mFrameBuffer		= NULL;
	this->mMesh				= NULL;
	
	this->mTextureUnits.Clear ();
}

//----------------------------------------------------------------//
MOAIGfxStateGPUCacheFrame::MOAIGfxStateGPUCacheFrame () :
	mCullFunc ( MOAICullFuncEnum::NONE ),
	mDepthFunc ( MOAIDepthFuncEnum::NONE ),
	mDepthMask ( true ),
	mBlendEnabled ( 0 ),
	mPenWidth ( 1.0f ),
	mScissorEnabled ( false ),
	mScissorRect ( ZLRect::EMPTY ),
	mViewRect ( ZLRect::EMPTY ) {
	
	RTTI_SINGLE ( MOAIGfxStateGPUCacheFrame, RTTIBase )
}

//----------------------------------------------------------------//
MOAIGfxStateGPUCacheFrame::~MOAIGfxStateGPUCacheFrame () {
}

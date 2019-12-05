// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx/MOAIFrameBuffer.h>
#include <moai-gfx/MOAIGfxStateGPUCacheFrame.h>
#include <moai-gfx/MOAIIndexBuffer.h>
#include <moai-gfx/MOAIShader.h>
#include <moai-gfx/MOAITexture.h>
#include <moai-gfx/MOAIVertexArray.h>
#include <moai-gfx/MOAIVertexBuffer.h>
#include <moai-gfx/MOAIVertexFormat.h>

//================================================================//
// MOAIGfxStateGPUCacheFrame
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxStateGPUCacheFrame::Clear () {

	this->mShader			= NULL;
	this->mFrameBuffer		= NULL;
	this->mIdxBuffer		= NULL;
	this->mVtxArray			= NULL;
	this->mVtxBuffer		= NULL;
	this->mVtxFormat		= NULL;
	
	this->mTextureUnits.Clear ();
}

//----------------------------------------------------------------//
MOAIGfxStateGPUCacheFrame::MOAIGfxStateGPUCacheFrame () :
	mCullFunc ( MOAICullFunc::NONE ),
	mDepthFunc ( MOAIDepthFunc::NONE ),
	mDepthMask ( true ),
	mBlendEnabled ( 0 ),
	mPenWidth ( 1.0f ),
	mScissorEnabled ( false ),
	mScissorRect ( ZLRect::EMPTY ),
	mViewRect ( ZLRect::EMPTY ) {
	
	RTTI_SINGLE ( RTTIBase )
}

//----------------------------------------------------------------//
MOAIGfxStateGPUCacheFrame::~MOAIGfxStateGPUCacheFrame () {
}

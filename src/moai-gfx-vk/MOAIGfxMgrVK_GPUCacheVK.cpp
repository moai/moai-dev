// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx-vk/MOAIFrameBufferVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_GPUCacheVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_VertexCacheVK.h>
#include <moai-gfx-vk/MOAIGfxStateGPUCacheFrameVK.h>
#include <moai-gfx-vk/MOAIIndexBufferVK.h>
#include <moai-gfx-vk/MOAIPipelineLayoutVK.h>
#include <moai-gfx-vk/MOAIShaderVK.h>
#include <moai-gfx-vk/MOAIShaderProgramVK.h>
#include <moai-gfx-vk/MOAITextureVK.h>
#include <moai-gfx-vk/MOAIVertexBufferVK.h>
#include <moai-gfx-vk/MOAIVertexFormatVK.h>

//#define MOAIGFXSTATECACHE_DEBUG_LOG

#ifdef MOAIGFXSTATECACHE_DEBUG_LOG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

//================================================================//
// MOAIGfxStateGPUCacheFrameVK
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::ApplyStateChanges () {

	if ( !this->mApplyingStateChanges ) {

		if ( this->mDirtyFlags || this->mTextureDirtyFlags ) {

			this->SuspendChanges ();
			this->mCurrentState = this->mActiveState;

			u32 dirtyFlags = this->mDirtyFlags;
			this->mDirtyFlags = 0;

			this->GfxStateWillChange ();

			this->mCurrentState = this->mPendingState;
			this->ResumeChanges ();
		}
	}
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgrVK_GPUCacheVK () {

	this->mDefaultFrameBuffer = new MOAIFrameBufferVK ();
	
	this->mActiveState = new MOAIGfxStateGPUCacheFrameVK ();
	this->mPendingState = new MOAIGfxStateGPUCacheFrameVK ();
	
	this->mCurrentState = this->mPendingState;
	
	this->InitTextureUnits ( 16 );
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_GPUCacheVK::~MOAIGfxMgrVK_GPUCacheVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_ClearSurface () {
}

//----------------------------------------------------------------//
size_t MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_CountTextureUnits () {

//	return this->mActiveState.mTextureUnits.Size ();
	return 0;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_DrawPrims ( u32 primType, u32 base, u32 count ) {
	UNUSED ( primType );
	UNUSED ( base );
	UNUSED ( count );
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_ResetGPUState () {
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_GPUCacheVK::MOAIGfxMgr_GPUCache_UnbindAll () {
}


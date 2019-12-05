// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-vk/MOAIGfxMgrVK_GPUCacheVK.h>
#include <moai-gfx-vk/MOAIGfxMgrVK_VertexCacheVK.h>

//================================================================//
// MOAIGfxMgrVK_VertexCacheVK
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgrVK_VertexCacheVK::MOAIGfxMgrVK_VertexCacheVK () {
}

//----------------------------------------------------------------//
MOAIGfxMgrVK_VertexCacheVK::~MOAIGfxMgrVK_VertexCacheVK () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrVK_VertexCacheVK::MOAIGfxMgr_VertexCache_FlushToGPU () {

	if ( this->mPrimCount == 0 ) return;
	
	MOAIGfxMgrVK_GPUCacheVK& gpuCache = this->GetGPUCacheVK ();
	gpuCache.SuspendChanges (); // don't apply any pending state changes;

	if ( !this->mIsDrawing ) {
		
		this->mIsDrawing = true;
		
		u32 count = 0;
		u32 offset = 0;
		
		if ( this->mUseIdxBuffer ) {
			count = ( u32 )( this->mIdxBuffer->GetCursor () / INDEX_SIZE ) - this->mIdxBase;
			offset = this->mIdxBase;
			this->mIdxBase += count;
		}
		else {
			count = ( u32 )( this->mVtxBuffer->GetCursor () / this->mVtxSize );
		}
		
		if ( count > 0 ) {
		
			// force the buffers into the cache; they will now be active (but pending will not match).
			// it's OK to leave these; will get set back to zero for the next cached prim.
			// setting back to zero won't trigger a redraw, since the vertex prim cache will be empty.
			if ( this->mUseIdxBuffer ) {
				MOAIIndexBufferVK* idxBuffer = MOAICastAssert < MOAIIndexBufferVK >(( MOAIIndexBuffer* )this->mIdxBuffer );
//				gpuCache.FlushIndexBuffer ( idxBuffer );
			}
			MOAIVertexBufferVK* vtxBuffer = MOAICastAssert < MOAIVertexBufferVK >(( MOAIVertexBuffer* )this->mVtxBuffer );
//			gpuCache.FlushVertexBuffer ( vtxBuffer );
			
			gpuCache.DrawPrims ( this->mPrimType, offset, count );
		}
		
		this->mIsDrawing = false;
		this->mPrimCount = 0;

		this->Reset ();
	}
	
	gpuCache.ResumeChanges (); // business as usual
}

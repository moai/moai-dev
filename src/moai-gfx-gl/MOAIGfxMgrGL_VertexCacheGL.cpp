// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxMgrGL_DisplayListClerkGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_GPUCacheGL.h>
#include <moai-gfx-gl/MOAIGfxMgrGL_VertexCacheGL.h>

//================================================================//
// MOAIGfxMgrGL_VertexCacheGL
//================================================================//

//----------------------------------------------------------------//
MOAIGfxMgrGL_VertexCacheGL::MOAIGfxMgrGL_VertexCacheGL () {
}

//----------------------------------------------------------------//
MOAIGfxMgrGL_VertexCacheGL::~MOAIGfxMgrGL_VertexCacheGL () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgrGL_VertexCacheGL::MOAIGfxMgr_VertexCache_FlushToGPU () {

	if ( this->mPrimCount == 0 ) return;
	
	MOAIGfxMgrGL_GPUCacheGL& gpuCache = this->GetGPUCacheGL ();
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
				MOAIIndexBufferGL* idxBuffer = MOAICastAssert < MOAIIndexBufferGL >(( MOAIIndexBuffer* )this->mIdxBuffer );
				gpuCache.ForceIndexBuffer ( idxBuffer );
			}
			MOAIVertexBufferGL* vtxBuffer = MOAICastAssert < MOAIVertexBufferGL >(( MOAIVertexBuffer* )this->mVtxBuffer );
			gpuCache.ForceVertexBuffer ( vtxBuffer );
			
			gpuCache.DrawPrims ( this->mPrimType, offset, count );
		}
		
		this->mIsDrawing = false;
		this->mPrimCount = 0;

		if ( this->GetDisplayListClerkGL ().GetDrawingAPI ().IsImmediate ()) {
			this->Reset ();
		}
	}
	
	gpuCache.ResumeChanges (); // business as usual
}

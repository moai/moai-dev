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
MOAIGfxMgrGL_VertexCacheGL::MOAIGfxMgrGL_VertexCacheGL ( ZLContext& context) {
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

	MOAIIndexBufferGL* idxBuffer = MOAICast < MOAIIndexBufferGL >( this->mIdxBuffer );
	MOAIVertexBufferGL* vtxBuffer = MOAICast < MOAIVertexBufferGL >( this->mVtxBuffer );

	if ( !this->mIsDrawing ) {
		
		this->mIsDrawing = true;
		
		u32 count = 0;
		u32 offset = 0;
		
		if ( this->mUseIdxBuffer ) {
		
			idxBuffer->Update ();
		
			count = ( u32 )( this->mIdxBuffer->GetCursor () / INDEX_SIZE ) - this->mIdxBase;
			offset = this->mIdxBase;
			this->mIdxBase += count;
		}
		else {
			count = ( u32 )( this->mVtxBuffer->GetCursor () / this->mVtxSize );
		}
		
		if ( count > 0 ) {
			vtxBuffer->Update ();

			this->mMesh->DrawPrims ( offset, count );
		}
		
		this->mIsDrawing = false;
		this->mPrimCount = 0;

		if ( this->GetDisplayListClerkGL ().GetDrawingAPI ().IsImmediate ()) {
			this->Reset ();
		}
	}
	
	gpuCache.ResumeChanges (); // business as usual
}

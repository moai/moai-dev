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
bool MOAIGfxMgrVK_VertexCacheVK::MOAIGfxMgr_VertexCache_BeginPrim ( u32 primType, u32 vtxCount, u32 idxCount ) {
	
//	MOAIGfxMgrVK_GPUCacheVK& gpuCache = this->GetGPUCacheVK ();
//	MOAIVertexFormat* format = gpuCache.GetCurrentVtxFormat ();
//
//	u32 vtxSize = format ? format->GetVertexSize () : 0;
//	if ( !vtxSize ) return false;
//
//	bool useIdxBuffer = ( idxCount > 0 );
//
//	// flush if ya gotta
//	if (( this->mPrimType != primType ) || ( this->mVtxSize != vtxSize ) || ( this->mUseIdxBuffer != useIdxBuffer )) {
//		this->FlushToGPU ();
//	}
//	this->mFlushOnPrimEnd = !(( primType == ZGL_PRIM_POINTS ) || ( primType == ZGL_PRIM_LINES ) || ( primType == ZGL_PRIM_TRIANGLES ));
//
//	// these will get bound later, just before drawing; clear them for now
//	// we have to bind them later since their contents will change as the primitive is drawn
//	gpuCache.SetIndexBuffer ();
//	gpuCache.SetVertexBuffer ();
//
//	gpuCache.ApplyStateChanges (); // must happen here in case there needs to be a flush
//
//	this->mPrimType = primType;
//	this->mVtxSize = vtxSize;
//	this->mUseIdxBuffer = useIdxBuffer;
//
//	if ( useIdxBuffer ) {
//		u32 vtxCursor = ( u32 )this->mVtxBuffer->GetCursor ();
//		this->mVtxBase = vtxCursor / vtxSize;
//
//		// if we're on a boundary, bump on up to the next vert
//		if ( vtxCursor % this->mVtxSize ) {
//			this->mVtxBase++;
//		}
//	}
//	return this->ContinuePrim ( vtxCount, idxCount ) != CONTINUE_FAIL;
	return true;
}

//----------------------------------------------------------------//
void MOAIGfxMgrVK_VertexCacheVK::MOAIGfxMgr_VertexCache_FlushToGPU () {

//	if ( this->mPrimCount == 0 ) return;
//	
//	MOAIGfxMgrVK_GPUCacheVK& gpuCache = this->GetGPUCacheVK ();
//	gpuCache.SuspendChanges (); // don't apply any pending state changes;
//
//	if ( !this->mIsDrawing ) {
//	
////		DEBUG_LOG ( "FLUSH BUFFERED PRIMS\n" );
//	
//		this->mIsDrawing = true;
//		
//		u32 count = 0;
//		u32 offset = 0;
//		
//		if ( this->mUseIdxBuffer ) {
//			count = ( u32 )( this->mIdxBuffer->GetCursor () / INDEX_SIZE ) - this->mIdxBase;
//			offset = this->mIdxBase;
//			this->mIdxBase += count;
//		}
//		else {
//			count = ( u32 )( this->mVtxBuffer->GetCursor () / this->mVtxSize );
//		}
//		
//		if ( count > 0 ) {
//		
//			// force the buffers into the cache; they will now be active (but pending will not match).
//			// it's OK to leave these; will get set back to zero for the next cached prim.
//			// setting back to zero won't trigger a redraw, since the vertex prim cache will be empty.
//			if ( this->mUseIdxBuffer ) {
//				MOAIIndexBufferVK* idxBuffer = MOAICastAssert < MOAIIndexBufferVK >(( MOAIIndexBuffer* )this->mIdxBuffer );
//				gpuCache.FlushIndexBuffer ( idxBuffer );
//			}
//			MOAIVertexBufferVK* vtxBuffer = MOAICastAssert < MOAIVertexBufferVK >(( MOAIVertexBuffer* )this->mVtxBuffer );
//			gpuCache.FlushVertexBuffer ( vtxBuffer );
//			
//			gpuCache.DrawPrims ( this->mPrimType, offset, count );
//		}
//		
//		this->mIsDrawing = false;
//		this->mPrimCount = 0;
//
//		if ( this->GetPipelineClerkVK ().GetDrawingAPI ().IsImmediate ()) {
//			this->Reset ();
//		}
//	}
//	
//	gpuCache.ResumeChanges (); // business as usual
}

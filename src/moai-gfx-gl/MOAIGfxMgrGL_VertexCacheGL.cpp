// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-gfx-gl/MOAIGfxMgrGL_PipelineClerkGL.h>
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
void MOAIGfxMgrGL_VertexCacheGL::ZLAbstractVertexCache_AffirmBuffers () {

	if ( !this->mVtxBuffer ) {

		this->mVtxBuffer = new MOAIVertexBufferGL ();
		this->mVtxBuffer->SetGfxMgr ( this->GetGfxMgrGL ());
		
		this->mIdxBuffer = new MOAIIndexBufferGL ();
		this->mIdxBuffer->SetGfxMgr ( this->GetGfxMgrGL ());
		
		this->mIdxBuffer->SetIndexSize ( INDEX_SIZE );

		this->mVtxBuffer->Reserve ( DEFAULT_VERTEX_BUFFER_SIZE );
		this->mIdxBuffer->Reserve ( DEFAULT_INDEX_BUFFER_SIZE );
	}
}

//----------------------------------------------------------------//
bool MOAIGfxMgrGL_VertexCacheGL::ZLAbstractVertexCache_BeginPrim ( u32 primType, u32 vtxCount, u32 idxCount ) {
	
	MOAIGfxMgrGL_GPUCacheGL& gpuCache = this->GetGPUCacheGL ();
	MOAIVertexFormat* format = gpuCache.GetCurrentVtxFormat ();
	
	u32 vtxSize = format ? format->GetVertexSize () : 0;
	if ( !vtxSize ) return false;
	
	bool useIdxBuffer = ( idxCount > 0 );

	// flush if ya gotta
	if (( this->mPrimType != primType ) || ( this->mVtxSize != vtxSize ) || ( this->mUseIdxBuffer != useIdxBuffer )) {
		this->FlushToGPU ();
	}
	this->mFlushOnPrimEnd = !(( primType == ZGL_PRIM_POINTS ) || ( primType == ZGL_PRIM_LINES ) || ( primType == ZGL_PRIM_TRIANGLES ));
	
	// these will get bound later, just before drawing; clear them for now
	// we have to bind them later since their contents will change as the primitive is drawn
	gpuCache.SetIndexBuffer ();
	gpuCache.SetVertexBuffer ();
	
	gpuCache.ApplyStateChanges (); // must happen here in case there needs to be a flush
	
	this->mPrimType = primType;
	this->mVtxSize = vtxSize;
	this->mUseIdxBuffer = useIdxBuffer;

	if ( useIdxBuffer ) {
		u32 vtxCursor = ( u32 )this->mVtxBuffer->GetCursor ();
		this->mVtxBase = vtxCursor / vtxSize;

		// if we're on a boundary, bump on up to the next vert
		if ( vtxCursor % this->mVtxSize ) {
			this->mVtxBase++;
		}
	}
	return this->ContinuePrim ( vtxCount, idxCount ) != CONTINUE_FAIL;
}

//----------------------------------------------------------------//
void MOAIGfxMgrGL_VertexCacheGL::ZLAbstractVertexCache_FlushToGPU () {

	if ( this->mPrimCount == 0 ) return;
	
	MOAIGfxMgrGL_GPUCacheGL& gpuCache = this->GetGPUCacheGL ();
	gpuCache.SuspendChanges (); // don't apply any pending state changes;

	if ( !this->mIsDrawing ) {
	
//		DEBUG_LOG ( "FLUSH BUFFERED PRIMS\n" );
	
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
				gpuCache.FlushIndexBuffer ( this->mIdxBuffer );
			}
			gpuCache.FlushVertexBuffer ( this->mVtxBuffer );
			
			gpuCache.DrawPrims ( this->mPrimType, offset, count );
		}
		
		this->mIsDrawing = false;
		this->mPrimCount = 0;

		if ( this->GetPipelineClerkGL ().GetDrawingAPI ().IsImmediate ()) {
			this->Reset ();
		}
	}
	
	gpuCache.ResumeChanges (); // business as usual
}

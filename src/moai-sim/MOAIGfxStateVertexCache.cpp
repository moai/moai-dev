// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
#include <moai-sim/MOAIGfxStateCPUCache.h>
#include <moai-sim/MOAIGfxStateGPUCache.h>
#include <moai-sim/MOAIGfxStateVertexCache.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/MOAISim.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIViewport.h>

//#define MOAIGFXVERTEXCACHE_DEBUG_LOG

#ifdef MOAIGFXVERTEXCACHE_DEBUG_LOG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

//================================================================//
// MOAIGfxStateVertexCache
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxStateVertexCache::BeginPrim ( u32 primType, u32 vtxCount, u32 idxCount ) {
	
	DEBUG_LOG ( "BEGIN INDEXED PRIM: %d %d %d\n", primType, vtxCount, idxCount );
	
	MOAIGfxState& gfxState = MOAIGfxMgr::Get ().mGfxState;
	MOAIVertexFormat* format = gfxState.GetCurrentVtxFormat ();
	
	u32 vtxSize = format ? format->GetVertexSize () : 0;
	if ( !vtxSize ) return false;
	
	bool useIdxBuffer = ( idxCount > 0 );

	// flush if ya gotta
	if (( this->mPrimType != primType ) || ( this->mVtxSize != vtxSize ) || ( this->mUseIdxBuffer != useIdxBuffer )) {
		this->FlushVertexCache ();
	}
	this->mFlushOnPrimEnd = !(( primType == ZGL_PRIM_POINTS ) || ( primType == ZGL_PRIM_LINES ) || ( primType == ZGL_PRIM_TRIANGLES ));
	
	// these will get bound later, just before drawing; clear them for now
	// we have to bind them later since their contents will change as the primitive is drawn
	gfxState.SetIndexBuffer ();
	gfxState.SetVertexBuffer ();
	
	gfxState.ApplyStateChanges (); // must happen here in case there needs to be a flush
	
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
u32 MOAIGfxStateVertexCache::ContinuePrim ( u32 vtxCount, u32 idxCount ) {

	u32 idxCursor = ( u32 )this->mIdxBuffer->GetCursor ();
	u32 vtxCursor = ( u32 )this->mVtxBuffer->GetCursor ();

	u32 idxBufferSize = ( u32 )this->mIdxBuffer->GetLength ();
	u32 vtxBufferSize = ( u32 )this->mVtxBuffer->GetLength ();
	
	u32 idxBytes = idxCount * INDEX_SIZE;
	u32 vtxBytes = vtxCount * this->mVtxSize;
	
	// really, this should never happen
	if (( vtxBufferSize < vtxBytes ) || ( idxBufferSize < idxBytes )) {
		return CONTINUE_FAIL;
	}
	
	if (( vtxBufferSize < ( vtxCursor + vtxBytes )) || ( idxBufferSize < ( idxCursor + idxBytes ))) {
		
		this->FlushVertexCache ();
		this->Reset ();
		return CONTINUE_ROLLOVER;
	}
	
	return CONTINUE_OK;
}

//----------------------------------------------------------------//
u32 MOAIGfxStateVertexCache::CountPrims () {

	return this->mPrimCount;
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::EndPrim () {
	
	this->mPrimCount++;
	
	if ( this->mFlushOnPrimEnd ) {
		this->FlushVertexCache ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::FlushVertexCache () {

	if ( this->mPrimCount == 0 ) return;
	
	MOAIGfxState& gfxState = MOAIGfxMgr::Get ().mGfxState;
	gfxState.SuspendChanges (); // don't apply any pending state changes;

	if ( !this->mIsDrawing ) {
	
		DEBUG_LOG ( "FLUSH BUFFERED PRIMS\n" );
	
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
				gfxState.FlushIndexBuffer ( this->mIdxBuffer );
			}
			gfxState.FlushVertexBuffer ( this->mVtxBuffer );
			
			gfxState.DrawPrims ( this->mPrimType, offset, count );
			
			gfxState.FlushIndexBuffer ( NULL );
			gfxState.FlushVertexBuffer ( NULL );
		}
		
		this->mIsDrawing = false;
		this->mPrimCount = 0;
				
		if ( MOAIGfxMgr::GetDrawingAPI ().IsImmediate ()) {
			this->Reset ();
		}
	}
	
	gfxState.ResumeChanges (); // business as usual
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::InitBuffers () {

	if ( !this->mVtxBuffer ) {

		this->mVtxBuffer = new MOAIVertexBuffer ();
		this->mIdxBuffer = new MOAIIndexBuffer ();
		
		this->mIdxBuffer->SetIndexSize ( INDEX_SIZE );

		this->mVtxBuffer->Reserve ( DEFAULT_VERTEX_BUFFER_SIZE );
		this->mIdxBuffer->Reserve ( DEFAULT_INDEX_BUFFER_SIZE );
	}
}

//----------------------------------------------------------------//
MOAIGfxStateVertexCache::MOAIGfxStateVertexCache () :
	mIsDrawing ( false ),
	mVtxBuffer ( 0 ),
	mIdxBuffer ( 0 ),
	mVtxBase ( 0 ),
	mIdxBase ( 0 ),
	mVtxSize ( 0 ),
	mPrimType ( 0 ),
	mFlushOnPrimEnd ( false ),
	mUseIdxBuffer ( false ),
	mPrimCount ( 0 ),
	mApplyVertexTransform ( false ),
	mApplyUVTransform ( false ) {
	
	this->mVertexTransform.Ident ();
	this->mUVTransform.Ident ();
}

//----------------------------------------------------------------//
MOAIGfxStateVertexCache::~MOAIGfxStateVertexCache () {
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::Reset () {

	this->mVtxBuffer->Seek ( 0 );
	this->mIdxBuffer->Seek ( 0 );
	
	this->mVtxBase = 0;
	this->mIdxBase = 0;
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::SetUVTransform () {

	this->mApplyUVTransform = false;
	this->mUVTransform.Ident ();
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::SetUVTransform ( u32 mtxID ) {

	this->SetUVTransform ( this->GetMtx ( mtxID ));
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::SetUVTransform ( const ZLMatrix4x4& uvTransform ) {

	this->mApplyUVTransform = !uvTransform.IsIdent ();
	this->mUVTransform = uvTransform;
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::SetVertexTransform () {

	this->mApplyVertexTransform = false;
	this->mVertexTransform.Ident ();
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::SetVertexTransform ( u32 mtxID ) {

	this->SetVertexTransform ( this->GetMtx ( mtxID ));
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::SetVertexTransform ( const ZLMatrix4x4& vertexTransform ) {

	this->mApplyVertexTransform = !vertexTransform.IsIdent ();
	this->mVertexTransform = vertexTransform;
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::TransformAndWriteQuad ( ZLVec4D* vtx, ZLVec2D* uv ) {

	if ( this->mApplyVertexTransform ) {
		this->mVertexTransform.TransformQuad ( vtx );
	}
	
	if ( this->mApplyUVTransform ) {
		this->mUVTransform.TransformQuad ( uv );
	}
	
	this->BeginPrim ( ZGL_PRIM_TRIANGLES, 4, 6 );
	
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
	
		// left top
		this->mVtxBuffer->Write ( vtx [ 0 ]);
		this->mVtxBuffer->Write ( uv [ 0 ]);
		this->mVtxBuffer->Write < u32 >( this->mStateFrameCPU.mFinalColor32 );
	
		// right top
		this->mVtxBuffer->Write ( vtx [ 1 ]);
		this->mVtxBuffer->Write ( uv [ 1 ]);
		this->mVtxBuffer->Write < u32 >( this->mStateFrameCPU.mFinalColor32 );
	
		// right bottom
		this->mVtxBuffer->Write ( vtx[ 2 ]);
		this->mVtxBuffer->Write ( uv [ 2 ]);
		this->mVtxBuffer->Write < u32 >( this->mStateFrameCPU.mFinalColor32 );
	
		// left bottom
		this->mVtxBuffer->Write ( vtx [ 3 ]);
		this->mVtxBuffer->Write ( uv [ 3 ]);
		this->mVtxBuffer->Write < u32 >( this->mStateFrameCPU.mFinalColor32 );

		// indices
		this->WriteIndex ( 0 ); // left top
		this->WriteIndex ( 3 ); // left bottom
		this->WriteIndex ( 2 ); // right bottom
	
		this->WriteIndex ( 0 ); // left top
		this->WriteIndex ( 2 ); // right bottom
		this->WriteIndex ( 1 ); // right top
	
	this->EndPrim ();
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv ) {

	ZLVec4D vtxBuffer [ 4 ];
	
	vtxBuffer [ 0 ].mX = vtx [ 0 ].mX;
	vtxBuffer [ 0 ].mY = vtx [ 0 ].mY;
	vtxBuffer [ 0 ].mZ = 0.0f;
	vtxBuffer [ 0 ].mW = 1.0f;
	
	vtxBuffer [ 1 ].mX = vtx [ 1 ].mX;
	vtxBuffer [ 1 ].mY = vtx [ 1 ].mY;
	vtxBuffer [ 1 ].mZ = 0.0f;
	vtxBuffer [ 1 ].mW = 1.0f;
	
	vtxBuffer [ 2 ].mX = vtx [ 2 ].mX;
	vtxBuffer [ 2 ].mY = vtx [ 2 ].mY;
	vtxBuffer [ 2 ].mZ = 0.0f;
	vtxBuffer [ 2 ].mW = 1.0f;
	
	vtxBuffer [ 3 ].mX = vtx [ 3 ].mX;
	vtxBuffer [ 3 ].mY = vtx [ 3 ].mY;
	vtxBuffer [ 3 ].mZ = 0.0f;
	vtxBuffer [ 3 ].mW = 1.0f;

	ZLVec2D uvBuffer [ 4 ];
	memcpy ( uvBuffer, uv, sizeof ( ZLVec2D ) * 4 );
	
	this->TransformAndWriteQuad ( vtxBuffer, uvBuffer );
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff ) {

	ZLVec4D vtxBuffer [ 4 ];
	
	vtxBuffer [ 0 ].mX = vtx [ 0 ].mX + xOff;
	vtxBuffer [ 0 ].mY = vtx [ 0 ].mY + yOff;
	vtxBuffer [ 0 ].mZ = zOff;
	vtxBuffer [ 0 ].mW = 1.0f;
	
	vtxBuffer [ 1 ].mX = vtx [ 1 ].mX + xOff;
	vtxBuffer [ 1 ].mY = vtx [ 1 ].mY + yOff;
	vtxBuffer [ 1 ].mZ = zOff;
	vtxBuffer [ 1 ].mW = 1.0f;
	
	vtxBuffer [ 2 ].mX = vtx [ 2 ].mX + xOff;
	vtxBuffer [ 2 ].mY = vtx [ 2 ].mY + yOff;
	vtxBuffer [ 2 ].mZ = zOff;
	vtxBuffer [ 2 ].mW = 1.0f;
	
	vtxBuffer [ 3 ].mX = vtx [ 3 ].mX + xOff;
	vtxBuffer [ 3 ].mY = vtx [ 3 ].mY + yOff;
	vtxBuffer [ 3 ].mZ = zOff;
	vtxBuffer [ 3 ].mW = 1.0f;
	
	ZLVec2D uvBuffer [ 4 ];
	memcpy ( uvBuffer, uv, sizeof ( ZLVec2D ) * 4 );
	
	this->TransformAndWriteQuad ( vtxBuffer, uvBuffer );
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale ) {

	ZLVec4D vtxBuffer [ 4 ];
	
	vtxBuffer [ 0 ].mX = ( vtx [ 0 ].mX * xScale ) + xOff;
	vtxBuffer [ 0 ].mY = ( vtx [ 0 ].mY * yScale ) + yOff;
	vtxBuffer [ 0 ].mZ = zOff;
	vtxBuffer [ 0 ].mW = 1.0f;
	
	vtxBuffer [ 1 ].mX = ( vtx [ 1 ].mX * xScale ) + xOff;
	vtxBuffer [ 1 ].mY = ( vtx [ 1 ].mY * yScale ) + yOff;
	vtxBuffer [ 1 ].mZ = zOff;
	vtxBuffer [ 1 ].mW = 1.0f;

	vtxBuffer [ 2 ].mX = ( vtx [ 2 ].mX * xScale ) + xOff;
	vtxBuffer [ 2 ].mY = ( vtx [ 2 ].mY * yScale ) + yOff;
	vtxBuffer [ 2 ].mZ = zOff;
	vtxBuffer [ 2 ].mW = 1.0f;
	
	vtxBuffer [ 3 ].mX = ( vtx [ 3 ].mX * xScale ) + xOff;
	vtxBuffer [ 3 ].mY = ( vtx [ 3 ].mY * yScale ) + yOff;
	vtxBuffer [ 3 ].mZ = zOff;
	vtxBuffer [ 3 ].mW = 1.0f;
	
	ZLVec2D uvBuffer [ 4 ];
	memcpy ( uvBuffer, uv, sizeof ( ZLVec2D ) * 4 );
	
	this->TransformAndWriteQuad ( vtxBuffer, uvBuffer );
}

//----------------------------------------------------------------//
void MOAIGfxStateVertexCache::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale, float uOff, float vOff, float uScale, float vScale ) {

	ZLVec4D vtxBuffer [ 4 ];
	
	vtxBuffer [ 0 ].mX = ( vtx [ 0 ].mX * xScale ) + xOff;
	vtxBuffer [ 0 ].mY = ( vtx [ 0 ].mY * yScale ) + yOff;
	vtxBuffer [ 0 ].mZ = zOff;
	vtxBuffer [ 0 ].mW = 1.0f;
	
	vtxBuffer [ 1 ].mX = ( vtx [ 1 ].mX * xScale ) + xOff;
	vtxBuffer [ 1 ].mY = ( vtx [ 1 ].mY * yScale ) + yOff;
	vtxBuffer [ 1 ].mZ = zOff;
	vtxBuffer [ 1 ].mW = 1.0f;

	vtxBuffer [ 2 ].mX = ( vtx [ 2 ].mX * xScale ) + xOff;
	vtxBuffer [ 2 ].mY = ( vtx [ 2 ].mY * yScale ) + yOff;
	vtxBuffer [ 2 ].mZ = zOff;
	vtxBuffer [ 2 ].mW = 1.0f;
	
	vtxBuffer [ 3 ].mX = ( vtx [ 3 ].mX * xScale ) + xOff;
	vtxBuffer [ 3 ].mY = ( vtx [ 3 ].mY * yScale ) + yOff;
	vtxBuffer [ 3 ].mZ = zOff;
	vtxBuffer [ 3 ].mW = 1.0f;
	
	ZLVec2D uvBuffer [ 4 ];
	
	uvBuffer [ 0 ].mX = ( uv [ 0 ].mX * uScale ) + uOff;
	uvBuffer [ 0 ].mY = ( uv [ 0 ].mY * vScale ) + vOff;
	
	uvBuffer [ 1 ].mX = ( uv [ 1 ].mX * uScale ) + uOff;
	uvBuffer [ 1 ].mY = ( uv [ 1 ].mY * vScale ) + vOff;

	uvBuffer [ 2 ].mX = ( uv [ 2 ].mX * uScale ) + uOff;
	uvBuffer [ 2 ].mY = ( uv [ 2 ].mY * vScale ) + vOff;
	
	uvBuffer [ 3 ].mX = ( uv [ 3 ].mX * uScale ) + uOff;
	uvBuffer [ 3 ].mY = ( uv [ 3 ].mY * vScale ) + vOff;
	
	this->TransformAndWriteQuad ( vtxBuffer, uvBuffer );
}

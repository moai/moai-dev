// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-gfx/MOAIGfxMgr.h>
#include <moai-gfx/MOAIGfxMgr_VertexCache.h>

//#define MOAIGFXVERTEXCACHE_DEBUG_LOG

#ifdef MOAIGFXVERTEXCACHE_DEBUG_LOG
	#define DEBUG_LOG printf
#else
	#define DEBUG_LOG(...)
#endif

//================================================================//
// MOAIGfxMgr_VertexCache
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxMgr_VertexCache::AffirmBuffers () {

	if ( !this->mVtxBuffer ) {

		this->mVtxBuffer = this->GetGfxMgr ().CreateVertexBuffer ();
		this->mIdxBuffer = this->GetGfxMgr ().CreateIndexBuffer ();
		
		this->mIdxBuffer->SetIndexSize ( INDEX_SIZE );

		this->mVtxBuffer->Reserve ( DEFAULT_VERTEX_BUFFER_SIZE );
		this->mIdxBuffer->Reserve ( DEFAULT_INDEX_BUFFER_SIZE );
	}

	assert ( this->mVtxBuffer );
	assert ( this->mIdxBuffer);
}

//----------------------------------------------------------------//
bool MOAIGfxMgr_VertexCache::BeginPrim ( u32 primType, u32 vtxCount, u32 idxCount ) {
	
	DEBUG_LOG ( "BEGIN INDEXED PRIM: %d %d %d\n", ( int )primType, vtxCount, idxCount );
	
	this->MOAIGfxMgr_VertexCache_BeginPrim ( primType, vtxCount, idxCount );
}

////----------------------------------------------------------------//
//void MOAIGfxMgr_VertexCache::Clear () {
//
//	this->mVtxBuffer = NULL;
//	this->mIdxBuffer = NULL;
//}

//----------------------------------------------------------------//
u32 MOAIGfxMgr_VertexCache::ContinuePrim ( u32 vtxCount, u32 idxCount ) {

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
		
		this->FlushToGPU ();
		this->Reset ();
		return CONTINUE_ROLLOVER;
	}
	
	return CONTINUE_OK;
}

//----------------------------------------------------------------//
u32 MOAIGfxMgr_VertexCache::CountPrims () {

	return this->mPrimCount;
}

//----------------------------------------------------------------//
void MOAIGfxMgr_VertexCache::EndPrim () {
	
	this->mPrimCount++;
	
	if ( this->mFlushAlways || this->mFlushOnPrimEnd ) {
		this->FlushToGPU ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxMgr_VertexCache::FlushToGPU () {

	this->MOAIGfxMgr_VertexCache_FlushToGPU ();
}

//----------------------------------------------------------------//
MOAIGfxMgr_VertexCache::MOAIGfxMgr_VertexCache () :
	mIsDrawing ( false ),
	mVtxBase ( 0 ),
	mIdxBase ( 0 ),
	mVtxSize ( 0 ),
	mPrimType ( 0 ),
	mFlushOnPrimEnd ( false ),
	mFlushAlways ( true ),
	mUseIdxBuffer ( false ),
	mPrimCount ( 0 ),
	mApplyVertexTransform ( false ),
	mApplyUVTransform ( false ) {
	
	this->mVertexTransform.Ident ();
	this->mUVTransform.Ident ();
}

//----------------------------------------------------------------//
MOAIGfxMgr_VertexCache::~MOAIGfxMgr_VertexCache () {
}

//----------------------------------------------------------------//
void MOAIGfxMgr_VertexCache::Reset () {

	this->mVtxBuffer->Seek ( 0 );
	this->mIdxBuffer->Seek ( 0 );
	
	this->mVtxBase = 0;
	this->mIdxBase = 0;
}

//----------------------------------------------------------------//
void MOAIGfxMgr_VertexCache::SetFlushAlways ( bool flushAlways ) {

	this->mFlushAlways = flushAlways;
}

//----------------------------------------------------------------//
void MOAIGfxMgr_VertexCache::SetUVTransform () {

	this->mApplyUVTransform = false;
	this->mUVTransform.Ident ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_VertexCache::SetUVTransform ( u32 mtxID ) {

	this->SetUVTransform ( this->GetCPUCache ().GetMtx ( mtxID ));
}

//----------------------------------------------------------------//
void MOAIGfxMgr_VertexCache::SetUVTransform ( const ZLMatrix4x4& uvTransform ) {

	this->mApplyUVTransform = !uvTransform.IsIdent ();
	this->mUVTransform = uvTransform;
}

//----------------------------------------------------------------//
void MOAIGfxMgr_VertexCache::SetVertexTransform () {

	this->mApplyVertexTransform = false;
	this->mVertexTransform.Ident ();
}

//----------------------------------------------------------------//
void MOAIGfxMgr_VertexCache::SetVertexTransform ( u32 mtxID ) {

	this->SetVertexTransform ( this->GetCPUCache ().GetMtx ( mtxID ));
}

//----------------------------------------------------------------//
void MOAIGfxMgr_VertexCache::SetVertexTransform ( const ZLMatrix4x4& vertexTransform ) {

	this->mApplyVertexTransform = !vertexTransform.IsIdent ();
	this->mVertexTransform = vertexTransform;
}

//----------------------------------------------------------------//
void MOAIGfxMgr_VertexCache::TransformAndWriteQuad ( ZLVec4D* vtx, ZLVec2D* uv ) {

	if ( this->mApplyVertexTransform ) {
		this->mVertexTransform.TransformQuad ( vtx );
	}
	
	if ( this->mApplyUVTransform ) {
		this->mUVTransform.TransformQuad ( uv );
	}
	
	// TODO: PrimType
	this->BeginPrim ( ZGL_PRIM_TRIANGLES, 4, 6 );
	
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
	
		u32 finalColor32 = this->GetCPUCache ().GetFinalColor32 ();
	
		// left top
		this->mVtxBuffer->Write ( vtx [ 0 ]);
		this->mVtxBuffer->Write ( uv [ 0 ]);
		this->mVtxBuffer->Write < u32 >( finalColor32 );
	
		// right top
		this->mVtxBuffer->Write ( vtx [ 1 ]);
		this->mVtxBuffer->Write ( uv [ 1 ]);
		this->mVtxBuffer->Write < u32 >( finalColor32 );
	
		// right bottom
		this->mVtxBuffer->Write ( vtx[ 2 ]);
		this->mVtxBuffer->Write ( uv [ 2 ]);
		this->mVtxBuffer->Write < u32 >( finalColor32 );
	
		// left bottom
		this->mVtxBuffer->Write ( vtx [ 3 ]);
		this->mVtxBuffer->Write ( uv [ 3 ]);
		this->mVtxBuffer->Write < u32 >( finalColor32 );

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
void MOAIGfxMgr_VertexCache::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv ) {

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
void MOAIGfxMgr_VertexCache::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff ) {

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
void MOAIGfxMgr_VertexCache::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale ) {

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
void MOAIGfxMgr_VertexCache::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale, float uOff, float vOff, float uScale, float vScale ) {

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

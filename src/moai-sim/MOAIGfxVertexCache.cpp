// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
#include <moai-sim/MOAIMultiTexture.h>
#include <moai-sim/MOAIShader.h>
#include <moai-sim/MOAIShaderMgr.h>
#include <moai-sim/MOAIShaderProgram.h>
#include <moai-sim/MOAISim.h>
#include <moai-sim/MOAITexture.h>
#include <moai-sim/MOAIVertexFormat.h>
#include <moai-sim/MOAIVertexFormatMgr.h>
#include <moai-sim/MOAIViewport.h>

//================================================================//
// MOAIGfxVertexCache
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxVertexCache::BeginPrim () {

	// TODO: this was commented out to deal with prims improperly batching in Vavius's
	// spine implementation. this code did not handle indexed prims correctly. the
	// fix was to introduce BeginPrimIndexed (). The TODO is to make sure that the
	// original behavior is preserved, and to see if there's way to reconcile these
	// ideas (to get back to one BeginPrim () to handle both cases).

//	if ( this->mPrimSize ) {
//
//		u32 totalIndices	= this->mIdxBuffer->GetLength () / INDEX_SIZE;
//		u32 totalVertices	= this->mVtxBuffer->GetLength() / this->mVertexSize;
//
//		u32 maxVertices		= MIN ( totalIndices, totalVertices );
//		
//		this->mMaxPrims = ( u32 )( maxVertices / this->mPrimSize );
//	
//		this->mPrimTopIdx = this->mIdxBuffer->GetCursor () + ( this->mPrimSize * INDEX_SIZE );
//		this->mPrimTopVtx = this->mVtxBuffer->GetCursor () + ( this->mPrimSize * this->mVertexSize );
//	}
	
	if ( this->mUseIdxBuffer ) {
		this->FlushBufferedPrims ();
	}
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	gfxMgr.mGfxState.SetShaderFlags ( gfxMgr.mGfxState.GetShaderGlobalsMask ());
	gfxMgr.mGfxState.SetClient ( this );
	
	this->mUseIdxBuffer = false;
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::BeginPrim ( u32 primType, u32 primSize ) {

	this->SetPrimType ( primType, primSize );
	this->BeginPrim ();
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::BeginPrimIndexed ( u32 primType, u32 vtxCount, u32 idxCount ) {
	
	this->SetPrimType ( primType );
	
	if ( !this->mUseIdxBuffer ||
		(( this->mTotalIndices + idxCount ) > this->mMaxIndices ) ||
		(( this->mTotalVertices + vtxCount ) > this->mMaxVertices )) {
		
		this->FlushBufferedPrims ();
	}
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	gfxMgr.mGfxState.SetShaderFlags ( gfxMgr.mGfxState.GetShaderGlobalsMask ());
	gfxMgr.mGfxState.SetClient ( this );
	
	this->mUseIdxBuffer		= true;
	this->mIndexBase		= ( u16 )( this->mTotalVertices );
	
	this->mTotalIndices	   += idxCount;
	this->mTotalVertices   += vtxCount;
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::BindBufferedDrawing ( MOAIVertexFormat& format ) {
	
	this->mVertexFormat = &format;
	this->mVertexSize = format.GetVertexSize ();
	this->UpdateLimits ();
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::BindBufferedDrawing ( u32 preset ) {

	MOAIVertexFormat* format = MOAIVertexFormatMgr::Get ().GetFormat ( preset );
	
	assert ( format );
	this->BindBufferedDrawing ( *format );
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::EndPrim () {
	
	++this->mPrimCount;
	
	if (( this->mPrimSize == 0 ) || ( this->mPrimCount >= this->mMaxPrims )) {
	
		// this forces a flush immediately
		this->FlushBufferedPrims ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::EndPrimIndexed () {
	
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::FlushBufferedPrims () {

	if (( !this->mIsDrawing ) && this->mVertexFormat && this->mVertexSize ) {
	
		this->mIsDrawing = true;
		
		ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
		
		if ( this->mUseIdxBuffer ) {
			
			u32 count = ( u32 )( this->mIdxBuffer->GetCursor () / INDEX_SIZE );
			
			if ( count > 0 ) {
				
				this->mVtxBuffer->OnGPUUpdate ();
				this->mIdxBuffer->OnGPUUpdate ();
				
				//gfxCache.BindVertexFormat ();
				
				gfxMgr.mGfxState.BindVertexBuffer ( this->mVtxBuffer );
				gfxMgr.mGfxState.BindVertexFormat ( this->mVertexFormat, true );
				gfxMgr.mGfxState.BindIndexBuffer ( this->mIdxBuffer );
				
				gfxMgr.mGfxState.UpdateAndBindUniforms ();

				ZLSharedConstBuffer* buffer = gfx.CopyBuffer ( this->mIdxBuffer->GetBuffer ());

				gfx.DrawElements ( this->mPrimType, count, ZGL_TYPE_UNSIGNED_SHORT, buffer, 0 );
				//this->mDrawCount++;
			}
		}
		else {
			
			u32 count = this->mPrimSize ? this->mPrimCount * this->mPrimSize : ( u32 )( this->mVtxBuffer->GetCursor () / this->mVertexSize );
			
			if ( count > 0 ) {
				
				this->mVtxBuffer->OnGPUUpdate ();
				
				//gfxCache.BindVertexFormat ();
				
				gfxMgr.mGfxState.BindVertexBuffer ( this->mVtxBuffer );
				gfxMgr.mGfxState.BindVertexFormat ( this->mVertexFormat, true );
				
				gfxMgr.mGfxState.UpdateAndBindUniforms ();
				
				gfx.DrawArrays ( this->mPrimType, 0, count );
				//this->mDrawCount++;
			}
		}
		
		this->mIsDrawing = false;
		
		this->mVtxBuffer->SetLength ( 0 );
		this->mIdxBuffer->SetLength ( 0 );
	
//		this->mPrimTopIdx		= 0;
//		this->mPrimTopVtx		= 0;
		this->mPrimCount		= 0;
		
		this->mPrimCount		= 0;
		this->mTotalIndices		= 0;
		this->mTotalVertices	= 0;
		
		gfxMgr.mGfxState.SetShaderFlags ( 0 );
	}
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::InitBuffers () {

	if ( !this->mVtxBuffer ) {

		this->mVtxBuffer = new MOAIVertexBuffer ();
		this->mIdxBuffer = new MOAIIndexBuffer ();

		this->mVtxBuffer->Reserve ( DEFAULT_VERTEX_BUFFER_SIZE );
		this->mIdxBuffer->Reserve ( DEFAULT_INDEX_BUFFER_SIZE );
		
		this->mVtxBuffer->SetCopyOnUpdate ( true );
		this->mIdxBuffer->SetCopyOnUpdate ( true );
	}
}

//----------------------------------------------------------------//
MOAIGfxVertexCache::MOAIGfxVertexCache () :
	mIsDrawing ( false ),
	mUseIdxBuffer ( false ),
	mVtxBuffer ( 0 ),
	mIdxBuffer ( 0 ),
	mVertexSize ( 0 ),
	mMaxVertices ( 0 ),
	mMaxIndices ( 0 ),
	mMaxPrims ( 0 ),
	mPrimCount ( 0 ),
	mPrimSize ( 0 ),
	mPrimType ( 0xffffffff ),
	mTotalVertices ( 0 ),
	mTotalIndices ( 0 ),
	mVertexFormat ( 0 ),
	mApplyVertexTransform ( false ),
	mApplyUVTransform ( false ),
	mVertexColor32 ( 0xffffffff ) {
	
	this->mVertexTransform.Ident ();
	this->mUVTransform.Ident ();
	
	this->mVertexColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
MOAIGfxVertexCache::~MOAIGfxVertexCache () {

	if ( this->mVtxBuffer ) {
	
		delete this->mVtxBuffer;
		delete this->mIdxBuffer;
	}
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::OnGfxStateWillChange () {

	this->FlushBufferedPrims ();
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::SetPrimType ( u32 primType, u32 primSize ) {

	if ( !primSize ) {

		switch ( primType ) {
			
			case ZGL_PRIM_LINES:
				primSize = 2;
				break;
			
			case ZGL_PRIM_TRIANGLES:
				primSize = 3;
				break;
			
			case ZGL_PRIM_POINTS:
			case ZGL_PRIM_LINE_LOOP:
			case ZGL_PRIM_LINE_STRIP:
			case ZGL_PRIM_TRIANGLE_FAN:
			case ZGL_PRIM_TRIANGLE_STRIP:
			default:
				break;
		}
	}
	
	if (( this->mPrimType != primType ) || ( this->mPrimSize != primSize )) {

		this->FlushBufferedPrims ();
		
		this->mPrimType = primType;
		this->mPrimSize = primSize;
		this->UpdateLimits ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::SetUVTransform () {

	this->mApplyUVTransform = false;
	this->mUVTransform.Ident ();
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::SetUVTransform ( const ZLMatrix4x4& uvTransform ) {

	this->mApplyUVTransform = !uvTransform.IsIdent ();
	this->mUVTransform = uvTransform;
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::SetVertexTransform () {

	this->mApplyVertexTransform = false;
	this->mVertexTransform.Ident ();
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::SetVertexTransform ( const ZLMatrix4x4& vertexTransform ) {

	this->mApplyVertexTransform = !vertexTransform.IsIdent ();
	this->mVertexTransform = vertexTransform;
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::TransformAndWriteQuad ( ZLVec4D* vtx, ZLVec2D* uv ) {

	if ( this->mApplyVertexTransform ) {
		this->mVertexTransform.TransformQuad ( vtx );
	}
	
	if ( this->mApplyUVTransform ) {
		this->mUVTransform.TransformQuad ( uv );
	}
	
//	u32 base = this->mVtxBuffer->GetCursor () / this->mVertexSize;
	this->BeginPrimIndexed ( ZGL_PRIM_TRIANGLES, 4, 6 );
	
		// TODO: put back an optimized write (i.e. WriteUnsafe or an equivalent)
	
		// left top
		this->mVtxBuffer->Write ( vtx [ 0 ]);
		this->mVtxBuffer->Write ( uv [ 0 ]);
		this->mVtxBuffer->Write < u32 >( this->mVertexColor32 );
	
		// right top
		this->mVtxBuffer->Write ( vtx [ 1 ]);
		this->mVtxBuffer->Write ( uv [ 1 ]);
		this->mVtxBuffer->Write < u32 >( this->mVertexColor32 );
	
		// right bottom
		this->mVtxBuffer->Write ( vtx[ 2 ]);
		this->mVtxBuffer->Write ( uv [ 2 ]);
		this->mVtxBuffer->Write < u32 >( this->mVertexColor32 );
	
		// left bottom
		this->mVtxBuffer->Write ( vtx [ 3 ]);
		this->mVtxBuffer->Write ( uv [ 3 ]);
		this->mVtxBuffer->Write < u32 >( this->mVertexColor32 );

		// indices
		this->WriteIndex ( 0 ); // left top
		this->WriteIndex ( 3 ); // left bottom
		this->WriteIndex ( 2 ); // right bottom
	
		this->WriteIndex ( 0 ); // left top
		this->WriteIndex ( 2 ); // right bottom
		this->WriteIndex ( 1 ); // right top
	
	this->EndPrimIndexed ();
}

//----------------------------------------------------------------//
//void MOAIGfxVertexCache::UnbindBufferedDrawing () {
//
//	MOAIGfxStateCache& gfxCache = MOAIGfxStateCache::Get ();
//
//	this->FlushBufferedPrims ();
//
//	this->BindVertexBuffer ();
//	this->BindIndexBuffer ();
//	
//	this->mVertexFormat = 0;
//	this->mVertexSize = 0;
//}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::UpdateLimits () {
	
	u32 primSize	= this->mPrimSize ? this->mPrimSize : 1;
	u32 vertexSize	= this->mVertexSize ? this->mVertexSize : 1;
	
	this->mMaxIndices	= ( u32 )( this->mIdxBuffer->GetSize () / INDEX_SIZE );
	this->mMaxVertices	= ( u32 )( this->mVtxBuffer->GetSize () / vertexSize );
	
	u32 maxElements		= MIN ( this->mMaxIndices, this->mMaxVertices );
	this->mMaxPrims		= ( u32 )( maxElements / primSize );
}
	
//----------------------------------------------------------------//
void MOAIGfxVertexCache::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv ) {

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
void MOAIGfxVertexCache::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff ) {

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
void MOAIGfxVertexCache::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale ) {

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
void MOAIGfxVertexCache::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale, float uOff, float vOff, float uScale, float vScale ) {

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

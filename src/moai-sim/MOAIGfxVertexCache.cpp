// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxResourceClerk.h>
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
// MOAIGfxVertexCache
//================================================================//

//----------------------------------------------------------------//
bool MOAIGfxVertexCache::BeginPrim ( u32 primType, u32 vtxCount, u32 idxCount ) {
	
	DEBUG_LOG ( "BEGIN INDEXED PRIM: %d %d %d\n", primType, vtxCount, idxCount );
	
	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	MOAIVertexFormat* format = gfxMgr.mGfxState.GetCurrentVtxFormat ();
	
	u32 vtxSize = format ? format->GetVertexSize () : 0;
	if ( !vtxSize ) return false;
	
	bool useIdxBuffer = ( idxCount > 0 );

	// flush if ya gotta
	if (( this->mPrimType != primType ) || ( this->mVtxSize != vtxSize ) || ( this->mUseIdxBuffer != useIdxBuffer )) {
		this->FlushBufferedPrims ();
	}
	this->mFlushOnPrimEnd = !(( primType == ZGL_PRIM_POINTS ) || ( primType == ZGL_PRIM_LINES ) || ( primType == ZGL_PRIM_TRIANGLES ));
	
	// these will get bound later, just before drawing; clear them for now
	gfxMgr.mGfxState.SetIndexBuffer ();
	gfxMgr.mGfxState.SetVertexBuffer ();
	gfxMgr.mGfxState.ApplyStateChanges (); // must happen here in case there needs to be a flush
	
	// check to see if the shader uniforms changed
	MOAIShader* shader = gfxMgr.mGfxState.GetCurrentShader ();
	assert ( shader );
	size_t uniformBufferSize = shader->GetUniformBufferSize ();
	const void* uniformBuffer = shader->GetUniformBuffer ();
	
	if ( this->mCurrentShader ) {
	
		if ( uniformBufferSize ) {
	
			assert ( shader == this->mCurrentShader );
			assert ( uniformBufferSize <= this->mUniformBufferSize );
		
			if ( memcmp ( this->mUniformBuffer, uniformBuffer, uniformBufferSize )) {
				this->FlushBufferedPrims ();
				memcpy ( this->mUniformBuffer, uniformBuffer, uniformBufferSize );
			}
		}
	}
	else {
		
		// take a snapshot of the uniform buffer
		this->mCurrentShader = shader;
		
		if ( uniformBufferSize ) {
			this->ResizeUniformBuffer ( uniformBufferSize );
			memcpy ( this->mUniformBuffer, uniformBuffer, uniformBufferSize );
		}
	}
	
	gfxMgr.mGfxState.SetClient ( this );
	
	// OK, *now* we can start to change the state
	
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
u32 MOAIGfxVertexCache::ContinuePrim ( u32 vtxCount, u32 idxCount ) {

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
		
		this->FlushBufferedPrims ();
		this->Reset ();
		return CONTINUE_ROLLOVER;
	}
	
	return CONTINUE_OK;
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::EndPrim () {
	
	this->mPrimCount++;
	
	if ( this->mFlushOnPrimEnd ) {
		this->FlushBufferedPrims ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::FlushBufferedPrims () {

	if ( this->mPrimCount == 0 ) return;

	MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
	gfxMgr.mGfxState.SuspendChanges (); // don't apply any pending state changes;

	if ( !this->mIsDrawing ) {
	
		DEBUG_LOG ( "FLUSH BUFFERED PRIMS\n" );
	
		this->mIsDrawing = true;
		
		//ZLGfx& gfx = MOAIGfxMgr::GetDrawingAPI ();
		
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
		
			if ( this->mUseIdxBuffer ) {
				gfxMgr.mGfxState.SetIndexBuffer ( this->mIdxBuffer );
			}
			gfxMgr.mGfxState.SetVertexBuffer ( this->mVtxBuffer );
		
			gfxMgr.mGfxState.DrawPrims ( this->mPrimType, offset, count );
		}
		
		this->mIsDrawing = false;
		this->mPrimCount = 0;
		
		gfxMgr.mGfxState.SetShaderFlags ( 0 );
		
		if ( MOAIGfxMgr::GetDrawingAPI ().IsImmediate ()) {
			this->Reset ();
		}
	}
	
	this->mCurrentShader = 0;
	
	gfxMgr.mGfxState.ResumeChanges (); // business as usual
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::InitBuffers () {

	if ( !this->mVtxBuffer ) {

		this->mVtxBuffer = new MOAIVertexBuffer ();
		this->mIdxBuffer = new MOAIIndexBuffer ();
		
		this->mIdxBuffer->SetIndexSize ( INDEX_SIZE );

		this->mVtxBuffer->Reserve ( DEFAULT_VERTEX_BUFFER_SIZE );
		this->mIdxBuffer->Reserve ( DEFAULT_INDEX_BUFFER_SIZE );
	}
}

//----------------------------------------------------------------//
MOAIGfxVertexCache::MOAIGfxVertexCache () :
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
	mApplyUVTransform ( false ),
	mVertexColor32 ( 0xffffffff ),
	mCurrentShader ( 0 ),
	mUniformBuffer ( 0 ),
	mUniformBufferSize ( 0 ) {
	
	this->mVertexTransform.Ident ();
	this->mUVTransform.Ident ();
	
	this->mVertexColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
MOAIGfxVertexCache::~MOAIGfxVertexCache () {

	this->ResizeUniformBuffer ( 0 );
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::OnGfxStateWillChange () {

	this->FlushBufferedPrims ();
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::Reset () {

	this->mVtxBuffer->Seek ( 0 );
	this->mIdxBuffer->Seek ( 0 );
	
	this->mVtxBase = 0;
	this->mIdxBase = 0;
}

//----------------------------------------------------------------//
void MOAIGfxVertexCache::ResizeUniformBuffer ( size_t size ) {

	if ( size && ( this->mUniformBufferSize < size )) {

		if ( this->mUniformBuffer ) {
			free ( this->mUniformBuffer );
			this->mUniformBuffer = 0;
			this->mUniformBufferSize = 0;
		}

		this->mUniformBufferSize = (( size / UNIFORM_BUFFER_CHUNK_SIZE ) + 1 ) * UNIFORM_BUFFER_CHUNK_SIZE;
		this->mUniformBuffer = malloc ( this->mUniformBufferSize );
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
	
	this->BeginPrim ( ZGL_PRIM_TRIANGLES, 4, 6 );
	
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
	
	this->EndPrim ();
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

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIFrameBuffer.h>
#include <moai-sim/MOAIFrameBufferTexture.h>
#include <moai-sim/MOAIGfxDeviceVertexWriter.h>
#include <moai-sim/MOAIGfxResource.h>
#include <moai-sim/MOAIGfxResourceMgr.h>
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
// MOAIGfxDeviceVertexWriter
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::BeginPrim () {

	// TODO: this was commented out to deal with prims improperly batching in Vavius's
	// spine implementation. this code did not handle indexed prims correctly. the
	// fix was to introduce BeginPrimIndexed (). The TODO is to make sure that the
	// original behavior is preserved, and to see if there's way to reconcile these
	// ideas (to get back to one BeginPrim () to handle both cases).

//	if ( this->mPrimSize ) {
//
//		u32 totalIndices	= this->mIdxBuffer.GetLength () / INDEX_SIZE;
//		u32 totalVertices	= this->mVtxBuffer.GetLength() / this->mVertexSize;
//
//		u32 maxVertices		= MIN ( totalIndices, totalVertices );
//		
//		this->mMaxPrims = ( u32 )( maxVertices / this->mPrimSize );
//	
//		this->mPrimTopIdx = this->mIdxBuffer.GetCursor () + ( this->mPrimSize * INDEX_SIZE );
//		this->mPrimTopVtx = this->mVtxBuffer.GetCursor () + ( this->mPrimSize * this->mVertexSize );
//	}
	
	if ( this->mUseIdxBuffer ) {
		this->FlushBufferedPrims ();
	}
	this->mUseIdxBuffer = false;
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::BeginPrim ( u32 primType, u32 primSize ) {

	this->SetPrimType ( primType, primSize );
	this->BeginPrim ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::BeginPrimIndexed ( u32 primType, u32 vtxCount, u32 idxCount ) {
	
	if ( !this->mUseIdxBuffer ||
		( this->mTotalIndices + idxCount > this->mMaxIndices ) ||
		( this->mTotalVertices + vtxCount > this->mMaxVertices )) {
		
		this->FlushBufferedPrims ();
	}
	
	this->SetPrimType ( primType );
	
	this->mUseIdxBuffer		= true;
	this->mIndexBase		= ( u16 )( this->mTotalVertices );
	
	this->mTotalIndices	   += idxCount;
	this->mTotalVertices   += vtxCount;
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::BindBufferedDrawing ( MOAIVertexFormat& format ) {
	
	this->mVertexFormat = &format;
	this->mVertexSize = format.GetVertexSize ();
	this->UpdateLimits ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::BindBufferedDrawing ( u32 preset ) {

	MOAIVertexFormat* format = MOAIVertexFormatMgr::Get ().GetFormat ( preset );
	
	assert ( format );
	this->BindBufferedDrawing ( *format );
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::EndPrim () {
	
	++this->mPrimCount;
	
	if (( this->mPrimSize == 0 ) || ( this->mPrimCount >= this->mMaxPrims )) {
	
		// this forces a flush immediately
		this->FlushBufferedPrims ();
	}
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::EndPrimIndexed () {
	
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::FlushBufferedPrims () {

	if (( !this->mIsDrawing ) && this->mVertexFormat && this->mVertexSize ) {
	
		this->mIsDrawing = true;
		
		if ( this->mUseIdxBuffer ) {
			
			u32 count = ( u32 )( this->mIdxBuffer.GetCursor () / INDEX_SIZE );
			
			if ( count > 0 ) {
				
				this->mVtxBuffer.ScheduleFlush ();
				this->mIdxBuffer.ScheduleFlush ();
				
				this->BindVertexBuffer ( &this->mVtxBuffer );
				this->BindVertexFormat ( this->mVertexFormat );
				this->BindIndexBuffer ( &this->mIdxBuffer );
				
				this->UpdateShaderGlobals ();
				zglDrawElements ( this->mPrimType, count, ZGL_TYPE_UNSIGNED_SHORT, this->mIdxBuffer.GetAddress ());
				this->mDrawCount++;
			}
		}
		else {
			
			u32 count = this->mPrimSize ? this->mPrimCount * this->mPrimSize : ( u32 )( this->mVtxBuffer.GetCursor () / this->mVertexSize );
			
			if ( count > 0 ) {
				
				this->mVtxBuffer.ScheduleFlush ();
				
				this->BindVertexBuffer ( &this->mVtxBuffer );
				this->BindVertexFormat ( this->mVertexFormat );
				
				this->UpdateShaderGlobals ();
				zglDrawArrays ( this->mPrimType, 0, count );
				this->mDrawCount++;
			}
		}
		
		this->mIsDrawing = false;
		
		this->mVtxBuffer.Seek ( 0 );
		this->mIdxBuffer.Seek ( 0 );
		
		this->mPrimCount		= 0;
		this->mTotalIndices		= 0;
		this->mTotalVertices	= 0;
	}
}

//----------------------------------------------------------------//
MOAIGfxDeviceVertexWriter::MOAIGfxDeviceVertexWriter () :
	mIsDrawing ( false ),
	mFinalColor32 ( 0xffffffff ),
	mVertexSize ( 0 ),
	mMaxVertices ( 0 ),
	mMaxIndices ( 0 ),
	mMaxPrims ( 0 ),
	mPrimCount ( 0 ),
	mPrimSize ( 0 ),
	mPrimType ( 0xffffffff ),
	mTotalVertices ( 0 ),
	mTotalIndices ( 0 ),
	mVertexFormat ( 0 ) {
	
	this->mAmbientColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mFinalColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mPenColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	
	this->mVtxBuffer.Reserve ( DEFAULT_VERTEX_BUFFER_SIZE );
	this->mIdxBuffer.Reserve ( DEFAULT_INDEX_BUFFER_SIZE );
}

//----------------------------------------------------------------//
MOAIGfxDeviceVertexWriter::~MOAIGfxDeviceVertexWriter () {
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::OnGfxStateWillChange () {

	this->FlushBufferedPrims ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::SetAmbientColor ( u32 color ) {

	this->mAmbientColor.SetRGBA ( color );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::SetAmbientColor ( const ZLColorVec& colorVec ) {

	this->mAmbientColor = colorVec;
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::SetAmbientColor ( float r, float g, float b, float a ) {

	this->mAmbientColor.Set ( r, g, b, a );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::SetPenColor ( u32 color ) {

	this->mPenColor.SetRGBA ( color );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::SetPenColor ( const ZLColorVec& colorVec ) {

	this->mPenColor = colorVec;
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::SetPenColor ( float r, float g, float b, float a ) {

	this->mPenColor.Set ( r, g, b, a );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::SetPrimType ( u32 primType, u32 primSize ) {

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
void MOAIGfxDeviceVertexWriter::TransformAndWriteQuad ( ZLVec4D* vtx, ZLVec2D* uv ) {

	if ( this->mCpuVertexTransform ) {
		this->mCpuVertexTransformMtx.TransformQuad ( vtx );
	}
	
	if ( this->mCpuUVTransform ) {
		this->mUVTransform.TransformQuad ( uv );
	}
	
//	u32 base = this->mVtxBuffer.GetCursor () / this->mVertexSize;
	this->BeginPrimIndexed ( ZGL_PRIM_TRIANGLES, 4, 6 );
	
		// left top
		this->mVtxBuffer.WriteUnsafe ( vtx [ 0 ]);
		this->mVtxBuffer.WriteUnsafe ( uv [ 0 ]);
		this->mVtxBuffer.WriteUnsafe < u32 >( this->mFinalColor32 );
	
		// right top
		this->mVtxBuffer.WriteUnsafe ( vtx [ 1 ]);
		this->mVtxBuffer.WriteUnsafe ( uv [ 1 ]);
		this->mVtxBuffer.WriteUnsafe < u32 >( this->mFinalColor32 );
	
		// right bottom
		this->mVtxBuffer.WriteUnsafe ( vtx[ 2 ]);
		this->mVtxBuffer.WriteUnsafe ( uv [ 2 ]);
		this->mVtxBuffer.WriteUnsafe < u32 >( this->mFinalColor32 );
	
		// left bottom
		this->mVtxBuffer.WriteUnsafe ( vtx [ 3 ]);
		this->mVtxBuffer.WriteUnsafe ( uv [ 3 ]);
		this->mVtxBuffer.WriteUnsafe < u32 >( this->mFinalColor32 );

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
void MOAIGfxDeviceVertexWriter::UnbindBufferedDrawing () {

	this->FlushBufferedPrims ();

	this->BindVertexBuffer ();
	this->BindIndexBuffer ();
	
	this->mVertexFormat = 0;
	this->mVertexSize = 0;
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::UpdateFinalColor () {

	this->mFinalColor.mR = this->mAmbientColor.mR * this->mPenColor.mR;
	this->mFinalColor.mG = this->mAmbientColor.mG * this->mPenColor.mG;
	this->mFinalColor.mB = this->mAmbientColor.mB * this->mPenColor.mB;
	this->mFinalColor.mA = this->mAmbientColor.mA * this->mPenColor.mA;

	this->mFinalColor32 = this->mFinalColor.PackRGBA ();
	
	this->mShaderDirty = true;
}

//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::UpdateLimits () {
	
	u32 primSize	= this->mPrimSize ? this->mPrimSize : 1;
	u32 vertexSize	= this->mVertexSize ? this->mVertexSize : 1;
	
	this->mMaxIndices	= this->mIdxBuffer.GetLength () / INDEX_SIZE;
	this->mMaxVertices	= this->mVtxBuffer.GetLength() / vertexSize;
	
	u32 maxElements		= MIN ( this->mMaxIndices, this->mMaxVertices );
	this->mMaxPrims		= ( u32 )( maxElements / primSize );
}
	
//----------------------------------------------------------------//
void MOAIGfxDeviceVertexWriter::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv ) {

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
void MOAIGfxDeviceVertexWriter::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff ) {

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
void MOAIGfxDeviceVertexWriter::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale ) {

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
void MOAIGfxDeviceVertexWriter::WriteQuad ( const ZLVec2D* vtx, const ZLVec2D* uv, float xOff, float yOff, float zOff, float xScale, float yScale, float uOff, float vOff, float uScale, float vScale ) {

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

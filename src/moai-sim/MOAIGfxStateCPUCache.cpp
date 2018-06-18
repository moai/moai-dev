// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIGfxStateCPUCache.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIGfxStateCPUCache.h>
#include <moai-sim/MOAIGfxStateGPUCache.h>
#include <moai-sim/MOAIGfxStateVertexCache.h>
#include <moai-sim/MOAIShaderUniform.h>
#include <moai-sim/MOAIViewport.h>
#include <moai-sim/MOAIViewProj.h>

//================================================================//
// MOAIGfxStateCPUCache
//================================================================//

//----------------------------------------------------------------//
const ZLMatrix4x4& MOAIGfxStateCPUCache::GetMtx ( u32 mtxID ) {
	
	u64 mtxFlag = ID_TO_FLAG ( mtxID );
	
	if ( !( this->mStateFrameCPU.mDirtyFlags & mtxFlag )) {
		return this->mStateFrameCPU.mMatrices [ mtxID ];
	}
	
	u32 primaryMtxID = mtxID % MATRIX_SET_SIZE;
	u32 setID = ( u32 )( mtxID / MATRIX_SET_SIZE );

	switch ( setID ) {
	
		case PRIMARY_MATRICES:
			this->GetPrimaryMtx ( primaryMtxID, mtxFlag );
			break;
		
		case INVERSE_MATRICES:
		
			if ( this->mStateFrameCPU.mDirtyFlags & mtxFlag ) {
				
				this->mStateFrameCPU.mMatrices [ mtxID ] = this->GetPrimaryMtx ( primaryMtxID, mtxFlag );
				this->mStateFrameCPU.mMatrices [ mtxID ].Inverse ();
			}
			break;
		
		case NORMAL_MATRICES:
		
			if ( this->mStateFrameCPU.mDirtyFlags & mtxFlag ) {
			
				ZLMatrix3x3 mtx3x3 = ZLMatrix3x3 ( this->GetPrimaryMtx ( primaryMtxID, mtxFlag ));
				mtx3x3.Inverse ();
				mtx3x3.Transpose ();
				this->mStateFrameCPU.mMatrices [ mtxID ] = ZLMatrix4x4 ( mtx3x3 );
			}
			break;
		
		case NORMAL_INVERSE_MATRICES:
		
			if ( this->mStateFrameCPU.mDirtyFlags & mtxFlag ) {
			
				ZLMatrix3x3 mtx3x3 = ZLMatrix3x3 ( this->GetPrimaryMtx ( primaryMtxID, mtxFlag ));
				mtx3x3.Inverse ();
				mtx3x3.Inverse ();
				mtx3x3.Transpose ();
				this->mStateFrameCPU.mMatrices [ mtxID ] = ZLMatrix4x4 ( mtx3x3 );
			}
			break;
	}

	this->mStateFrameCPU.mDirtyFlags &= ~mtxFlag;
	
	return this->mStateFrameCPU.mMatrices [ mtxID ];
}

//----------------------------------------------------------------//
const ZLMatrix4x4& MOAIGfxStateCPUCache::GetPrimaryMtx ( u32 mtxID, u64 mtxFlag ) {
	UNUSED(mtxFlag);
	switch ( mtxID ) {
	
		case CLIP_TO_DISPLAY_MTX:
	
			if ( this->mStateFrameCPU.mDirtyFlags & CLIP_TO_DISPLAY_MTX_MASK ) {
			
				this->mStateFrameCPU.mMatrices [ mtxID ] = this->GetMtx ( CLIP_TO_WORLD_MTX ); // back to world space
				this->mStateFrameCPU.mMatrices [ mtxID ].Append ( this->GetMtx ( WORLD_TO_DISPLAY_MTX )); // and forward to display space
			}
			return this->mStateFrameCPU.mMatrices [ mtxID ];
		
		case CLIP_TO_WINDOW_MTX:
			
			return this->mStateFrameCPU.mMatrices [ mtxID ];
		
		case MODEL_TO_CLIP_MTX:
			
			if ( this->mStateFrameCPU.mDirtyFlags & MODEL_TO_CLIP_MTX_MASK ) {
			
				this->mStateFrameCPU.mMatrices [ mtxID ] = this->mStateFrameCPU.mMatrices [ MODEL_TO_WORLD_MTX ];
				this->mStateFrameCPU.mMatrices [ mtxID ].Append ( this->GetMtx ( WORLD_TO_CLIP_MTX ));
			}
			return this->mStateFrameCPU.mMatrices [ mtxID ];
		
		case MODEL_TO_DISPLAY_MTX:
	
			if ( this->mStateFrameCPU.mDirtyFlags & MODEL_TO_DISPLAY_MTX_MASK ) {
			
				this->mStateFrameCPU.mMatrices [ mtxID ] = this->GetMtx ( MODEL_TO_WORLD_MTX );
				this->mStateFrameCPU.mMatrices [ mtxID ].Append ( this->GetMtx ( WORLD_TO_DISPLAY_MTX ));
			}
			return this->mStateFrameCPU.mMatrices [ mtxID ];
		
		case MODEL_TO_UV_MTX:
			
			return this->mStateFrameCPU.mMatrices [ MODEL_TO_UV_MTX ];
		
		case MODEL_TO_VIEW_MTX:
			
			if ( this->mStateFrameCPU.mDirtyFlags & MODEL_TO_VIEW_MTX_MASK ) {
			
				this->mStateFrameCPU.mMatrices [ mtxID ] = this->mStateFrameCPU.mMatrices [ MODEL_TO_WORLD_MTX ];
				this->mStateFrameCPU.mMatrices [ mtxID ].Append ( this->mStateFrameCPU.mMatrices [ WORLD_TO_VIEW_MTX ]);
			}
			return this->mStateFrameCPU.mMatrices [ mtxID ];
		
		case MODEL_TO_WORLD_MTX:
			
			return this->mStateFrameCPU.mMatrices [ mtxID ];
		
		case VIEW_TO_CLIP_MTX:
			
			return this->mStateFrameCPU.mMatrices [ mtxID ];
		
		case VIEW_TO_DISPLAY_MTX:
	
			if ( this->mStateFrameCPU.mDirtyFlags & VIEW_TO_DISPLAY_MTX_MASK ) {
					
				this->mStateFrameCPU.mMatrices [ mtxID ].Inverse ( this->GetMtx ( VIEW_TO_WORLD_MTX )); // back to world space
				this->mStateFrameCPU.mMatrices [ mtxID ].Append ( this->GetMtx ( WORLD_TO_DISPLAY_MTX )); // and forward to display space
			}
			return this->mStateFrameCPU.mMatrices [ VIEW_TO_DISPLAY_MTX ];
		
		case WORLD_TO_CLIP_MTX:
			
			if ( this->mStateFrameCPU.mDirtyFlags & WORLD_TO_CLIP_MTX_MASK ) {
			
				this->mStateFrameCPU.mMatrices [ mtxID ] = this->mStateFrameCPU.mMatrices [ WORLD_TO_VIEW_MTX ];
				this->mStateFrameCPU.mMatrices [ mtxID ].Append ( this->mStateFrameCPU.mMatrices [ VIEW_TO_CLIP_MTX ]);
			}
			return this->mStateFrameCPU.mMatrices [ mtxID ];
		
		case WORLD_TO_DISPLAY_MTX:
			
			return this->mStateFrameCPU.mMatrices [ mtxID ];
		
		case WORLD_TO_VIEW_MTX:
			
			return this->mStateFrameCPU.mMatrices [ mtxID ];
	}
	
	assert ( false );
	return ZLMatrix4x4::IDENT;
}

//----------------------------------------------------------------//
const ZLFrustum& MOAIGfxStateCPUCache::GetViewVolume () {

	if ( this->mStateFrameCPU.mDirtyFlags & VIEW_VOLUME_MASK ) {
		this->mStateFrameCPU.mViewVolume.Init ( this->GetMtx ( CLIP_TO_WORLD_MTX ));
		this->mStateFrameCPU.mDirtyFlags &= ~VIEW_VOLUME_MASK;
	}
	return this->mStateFrameCPU.mViewVolume;
}

//----------------------------------------------------------------//
bool MOAIGfxStateCPUCache::IsInputMtx ( u32 mtxID ) {

	return (
		( mtxID == CLIP_TO_WINDOW_MTX )		||
		( mtxID == MODEL_TO_WORLD_MTX )		||
		( mtxID == MODEL_TO_UV_MTX )		||
		( mtxID == WORLD_TO_VIEW_MTX )		||
		( mtxID == VIEW_TO_CLIP_MTX )		||
		( mtxID == WORLD_TO_DISPLAY_MTX )
	);
}

//----------------------------------------------------------------//
MOAIGfxStateCPUCache::MOAIGfxStateCPUCache () {

	assert ( TOTAL_GLOBALS < MAX_GLOBALS );

	for ( u32 i = 0; i < TOTAL_MATRICES; ++i ) {
		this->mStateFrameCPU.mMatrices [ i ].Ident ();
	}
	
	this->mStateFrameCPU.mDirtyFlags = 0;
	
	this->mStateFrameCPU.mAmbientColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mStateFrameCPU.mFinalColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mStateFrameCPU.mPenColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mStateFrameCPU.mClearColor.Set ( 0.0f, 0.0f, 0.0f, 1.0f );
	this->mStateFrameCPU.mClearFlags = 0;
	this->mStateFrameCPU.mClearDepth = 0.0;
	
	this->mStateFrameCPU.mFinalColor32 = 0xffffffff;
}

//----------------------------------------------------------------//
MOAIGfxStateCPUCache::~MOAIGfxStateCPUCache () {
}

//----------------------------------------------------------------//
void MOAIGfxStateCPUCache::SetAmbientColor ( u32 color ) {

	this->mStateFrameCPU.mAmbientColor.SetRGBA ( color );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCPUCache::SetAmbientColor ( const ZLColorVec& colorVec ) {

	this->mStateFrameCPU.mAmbientColor = colorVec;
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCPUCache::SetAmbientColor ( float r, float g, float b, float a ) {

	this->mStateFrameCPU.mAmbientColor.Set ( r, g, b, a );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCPUCache::SetMtx ( u32 mtxID ) {

	ZLMatrix4x4 mtx;
	mtx.Ident ();
	this->SetMtx ( mtxID, mtx );
}

//----------------------------------------------------------------//
void MOAIGfxStateCPUCache::SetMtx ( u32 mtxID, const ZLAffine3D& transform ) {

	this->SetMtx ( mtxID, ZLMatrix4x4 ( transform ));
}

//----------------------------------------------------------------//
void MOAIGfxStateCPUCache::SetMtx ( u32 mtxID, const ZLMatrix4x4& mtx ) {

	u64 dirtyMask = 0;

	switch ( mtxID ) {
	
		case CLIP_TO_WINDOW_MTX:
		
			dirtyMask = CLIP_TO_WINDOW_MTX_DIRTY_MASK;
			break;
	
		case MODEL_TO_UV_MTX:
		
			dirtyMask = MODEL_TO_UV_MTX_DIRTY_MASK;
			break;
		
		case MODEL_TO_WORLD_MTX:
		
			dirtyMask = MODEL_TO_WORLD_MTX_DIRTY_MASK;
			break;
	
		case VIEW_TO_CLIP_MTX:
		
			dirtyMask = VIEW_TO_CLIP_MTX_DIRTY_MASK;
			break;
			
		case WORLD_TO_DISPLAY_MTX:
				
			dirtyMask = WORLD_TO_DISPLAY_MTX_DIRTY_MASK;
			break;
			
		case WORLD_TO_VIEW_MTX:
			
			dirtyMask = WORLD_TO_VIEW_MTX_DIRTY_MASK;
			break;

		default:
		
			assert ( false ); // read only
			return;
	}

	if ( !this->mStateFrameCPU.mMatrices [ mtxID ].IsSame ( mtx )) {
	
		this->mStateFrameCPU.mDirtyFlags |= dirtyMask;
		this->mStateFrameCPU.mMatrices [ mtxID ] = mtx;
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCPUCache::SetPenColor ( u32 color ) {

	this->mStateFrameCPU.mPenColor.SetRGBA ( color );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCPUCache::SetPenColor ( const ZLColorVec& colorVec ) {

	this->mStateFrameCPU.mPenColor = colorVec;
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCPUCache::SetPenColor ( float r, float g, float b, float a ) {

	this->mStateFrameCPU.mPenColor.Set ( r, g, b, a );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCPUCache::SetViewProj ( MOAIViewport* viewport, MOAICamera* camera, MOAICamera* debug, const ZLVec3D& parallax ) {

	ZLMatrix4x4 view = MOAIViewProj::GetViewMtx ( camera, parallax );
	ZLMatrix4x4 proj = MOAIViewProj::GetProjectionMtx ( viewport, camera );
	
	this->SetMtx ( MOAIGfxStateCPUCache::WORLD_TO_VIEW_MTX, view );
	this->SetMtx ( MOAIGfxStateCPUCache::VIEW_TO_CLIP_MTX, proj );
	this->SetMtx ( MOAIGfxStateCPUCache::CLIP_TO_WINDOW_MTX, viewport ? viewport->GetProjMtx () : ZLMatrix4x4::IDENT );
	
	if ( debug ) {
		ZLMatrix4x4 display = MOAIViewProj::GetViewMtx ( debug );
		display.Append ( MOAIViewProj::GetProjectionMtx ( viewport, debug ));
		this->SetMtx ( MOAIGfxStateCPUCache::WORLD_TO_DISPLAY_MTX, display );
	}
	else {
		view.Append ( proj );
		this->SetMtx ( MOAIGfxStateCPUCache::WORLD_TO_DISPLAY_MTX, view );
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCPUCache::UpdateFinalColor () {

	this->mStateFrameCPU.mFinalColor.mR = this->mStateFrameCPU.mAmbientColor.mR * this->mStateFrameCPU.mPenColor.mR;
	this->mStateFrameCPU.mFinalColor.mG = this->mStateFrameCPU.mAmbientColor.mG * this->mStateFrameCPU.mPenColor.mG;
	this->mStateFrameCPU.mFinalColor.mB = this->mStateFrameCPU.mAmbientColor.mB * this->mStateFrameCPU.mPenColor.mB;
	this->mStateFrameCPU.mFinalColor.mA = this->mStateFrameCPU.mAmbientColor.mA * this->mStateFrameCPU.mPenColor.mA;

	this->mStateFrameCPU.mFinalColor32 = this->mStateFrameCPU.mFinalColor.PackRGBA ();
}

// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIGfxStateCPU.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIShaderUniform.h>
#include <moai-sim/MOAIViewport.h>
#include <moai-sim/MOAIViewProj.h>

//================================================================//
// MOAIGfxStateCPU
//================================================================//

//----------------------------------------------------------------//
const ZLMatrix4x4& MOAIGfxStateCPU::GetMtx ( u32 mtxID ) {
	
	u64 mtxFlag = ID_TO_FLAG ( mtxID );
	
	if ( !( this->mDirtyFlags & mtxFlag )) {
		return this->mMatrices [ mtxID ];
	}
	
	u32 primaryMtxID = mtxID % MATRIX_SET_SIZE;
	u32 setID = ( u32 )( mtxID / MATRIX_SET_SIZE );

	switch ( setID ) {
	
		case PRIMARY_MATRICES:
			this->GetPrimaryMtx ( primaryMtxID, mtxFlag );
			break;
		
		case INVERSE_MATRICES:
		
			if ( this->mDirtyFlags & mtxFlag ) {
				
				this->mMatrices [ mtxID ] = this->GetPrimaryMtx ( primaryMtxID, mtxFlag );
				this->mMatrices [ mtxID ].Inverse ();
			}
			break;
		
		case NORMAL_MATRICES:
		
			if ( this->mDirtyFlags & mtxFlag ) {
			
				ZLMatrix3x3 mtx3x3 = ZLMatrix3x3 ( this->GetPrimaryMtx ( primaryMtxID, mtxFlag ));
				mtx3x3.Inverse ();
				mtx3x3.Transpose ();
				this->mMatrices [ mtxID ] = ZLMatrix4x4 ( mtx3x3 );
			}
			break;
		
		case NORMAL_INVERSE_MATRICES:
		
			if ( this->mDirtyFlags & mtxFlag ) {
			
				ZLMatrix3x3 mtx3x3 = ZLMatrix3x3 ( this->GetPrimaryMtx ( primaryMtxID, mtxFlag ));
				mtx3x3.Inverse ();
				mtx3x3.Inverse ();
				mtx3x3.Transpose ();
				this->mMatrices [ mtxID ] = ZLMatrix4x4 ( mtx3x3 );
			}
			break;
	}

	this->mDirtyFlags &= ~mtxFlag;
	
	return this->mMatrices [ mtxID ];
}

//----------------------------------------------------------------//
const ZLMatrix4x4& MOAIGfxStateCPU::GetPrimaryMtx ( u32 mtxID, u64 mtxFlag ) {
	UNUSED(mtxFlag);
	switch ( mtxID ) {
	
		case CLIP_TO_DISPLAY_MTX:
	
			if ( this->mDirtyFlags & CLIP_TO_DISPLAY_MTX_MASK ) {
			
				this->mMatrices [ mtxID ] = this->GetMtx ( CLIP_TO_WORLD_MTX ); // back to world space
				this->mMatrices [ mtxID ].Append ( this->GetMtx ( WORLD_TO_DISPLAY_MTX )); // and forward to display space
			}
			return this->mMatrices [ mtxID ];
		
		case CLIP_TO_WINDOW_MTX:
			
			return this->mMatrices [ mtxID ];
		
		case MODEL_TO_CLIP_MTX:
			
			if ( this->mDirtyFlags & MODEL_TO_CLIP_MTX_MASK ) {
			
				this->mMatrices [ mtxID ] = this->mMatrices [ MODEL_TO_WORLD_MTX ];
				this->mMatrices [ mtxID ].Append ( this->GetMtx ( WORLD_TO_CLIP_MTX ));
			}
			return this->mMatrices [ mtxID ];
		
		case MODEL_TO_DISPLAY_MTX:
	
			if ( this->mDirtyFlags & MODEL_TO_DISPLAY_MTX_MASK ) {
			
				this->mMatrices [ mtxID ] = this->GetMtx ( MODEL_TO_WORLD_MTX );
				this->mMatrices [ mtxID ].Append ( this->GetMtx ( WORLD_TO_DISPLAY_MTX ));
			}
			return this->mMatrices [ mtxID ];
		
		case MODEL_TO_UV_MTX:
			
			return this->mMatrices [ MODEL_TO_UV_MTX ];
		
		case MODEL_TO_VIEW_MTX:
			
			if ( this->mDirtyFlags & MODEL_TO_VIEW_MTX_MASK ) {
			
				this->mMatrices [ mtxID ] = this->mMatrices [ MODEL_TO_WORLD_MTX ];
				this->mMatrices [ mtxID ].Append ( this->mMatrices [ WORLD_TO_VIEW_MTX ]);
			}
			return this->mMatrices [ mtxID ];
		
		case MODEL_TO_WORLD_MTX:
			
			return this->mMatrices [ mtxID ];
		
		case VIEW_TO_CLIP_MTX:
			
			return this->mMatrices [ mtxID ];
		
		case VIEW_TO_DISPLAY_MTX:
	
			if ( this->mDirtyFlags & VIEW_TO_DISPLAY_MTX_MASK ) {
					
				this->mMatrices [ mtxID ].Inverse ( this->GetMtx ( VIEW_TO_WORLD_MTX )); // back to world space
				this->mMatrices [ mtxID ].Append ( this->GetMtx ( WORLD_TO_DISPLAY_MTX )); // and forward to display space
			}
			return this->mMatrices [ VIEW_TO_DISPLAY_MTX ];
		
		case WORLD_TO_CLIP_MTX:
			
			if ( this->mDirtyFlags & WORLD_TO_CLIP_MTX_MASK ) {
			
				this->mMatrices [ mtxID ] = this->mMatrices [ WORLD_TO_VIEW_MTX ];
				this->mMatrices [ mtxID ].Append ( this->mMatrices [ VIEW_TO_CLIP_MTX ]);
			}
			return this->mMatrices [ mtxID ];
		
		case WORLD_TO_DISPLAY_MTX:
			
			return this->mMatrices [ mtxID ];
		
		case WORLD_TO_VIEW_MTX:
			
			return this->mMatrices [ mtxID ];
	}
	
	assert ( false );
	return ZLMatrix4x4::IDENT;
}

//----------------------------------------------------------------//
const ZLFrustum& MOAIGfxStateCPU::GetViewVolume () {

	if ( this->mDirtyFlags & VIEW_VOLUME_MASK ) {
		this->mViewVolume.Init ( this->GetMtx ( CLIP_TO_WORLD_MTX ));
		this->mDirtyFlags &= ~VIEW_VOLUME_MASK;
	}
	return this->mViewVolume;
}

//----------------------------------------------------------------//
bool MOAIGfxStateCPU::IsInputMtx ( u32 mtxID ) {

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
MOAIGfxStateCPU::MOAIGfxStateCPU () {

	assert ( TOTAL_GLOBALS < MAX_GLOBALS );

	for ( u32 i = 0; i < TOTAL_MATRICES; ++i ) {
		this->mMatrices [ i ].Ident ();
	}
	
	this->mDirtyFlags = 0;
	
	this->mAmbientColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mFinalColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mPenColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mClearColor.Set ( 0.0f, 0.0f, 0.0f, 1.0f );
	this->mClearFlags = 0;
	this->mClearDepth = 0.0;
}

//----------------------------------------------------------------//
MOAIGfxStateCPU::~MOAIGfxStateCPU () {
}

//----------------------------------------------------------------//
void MOAIGfxStateCPU::SetAmbientColor ( u32 color ) {

	this->mAmbientColor.SetRGBA ( color );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCPU::SetAmbientColor ( const ZLColorVec& colorVec ) {

	this->mAmbientColor = colorVec;
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCPU::SetAmbientColor ( float r, float g, float b, float a ) {

	this->mAmbientColor.Set ( r, g, b, a );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCPU::SetMtx ( u32 mtxID ) {

	ZLMatrix4x4 mtx;
	mtx.Ident ();
	this->SetMtx ( mtxID, mtx );
}

//----------------------------------------------------------------//
void MOAIGfxStateCPU::SetMtx ( u32 mtxID, const ZLAffine3D& transform ) {

	this->SetMtx ( mtxID, ZLMatrix4x4 ( transform ));
}

//----------------------------------------------------------------//
void MOAIGfxStateCPU::SetMtx ( u32 mtxID, const ZLMatrix4x4& mtx ) {

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

	if ( !this->mMatrices [ mtxID ].IsSame ( mtx )) {
	
		this->mDirtyFlags |= dirtyMask;
		this->mMatrices [ mtxID ] = mtx;
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCPU::SetPenColor ( u32 color ) {

	this->mPenColor.SetRGBA ( color );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCPU::SetPenColor ( const ZLColorVec& colorVec ) {

	this->mPenColor = colorVec;
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCPU::SetPenColor ( float r, float g, float b, float a ) {

	this->mPenColor.Set ( r, g, b, a );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxStateCPU::SetViewProj ( MOAIViewport* viewport, MOAICamera* camera, MOAICamera* debug, const ZLVec3D& parallax ) {

	ZLMatrix4x4 view = MOAIViewProj::GetViewMtx ( camera, parallax );
	ZLMatrix4x4 proj = MOAIViewProj::GetProjectionMtx ( viewport, camera );
	
	this->SetMtx ( MOAIGfxStateCPU::WORLD_TO_VIEW_MTX, view );
	this->SetMtx ( MOAIGfxStateCPU::VIEW_TO_CLIP_MTX, proj );
	this->SetMtx ( MOAIGfxStateCPU::CLIP_TO_WINDOW_MTX, viewport ? viewport->GetProjMtx () : ZLMatrix4x4::IDENT );
	
	if ( debug ) {
		ZLMatrix4x4 display = MOAIViewProj::GetViewMtx ( debug );
		display.Append ( MOAIViewProj::GetProjectionMtx ( viewport, debug ));
		this->SetMtx ( MOAIGfxStateCPU::WORLD_TO_DISPLAY_MTX, display );
	}
	else {
		view.Append ( proj );
		this->SetMtx ( MOAIGfxStateCPU::WORLD_TO_DISPLAY_MTX, view );
	}
}

//----------------------------------------------------------------//
void MOAIGfxStateCPU::UpdateFinalColor () {

	this->mFinalColor.mR = this->mAmbientColor.mR * this->mPenColor.mR;
	this->mFinalColor.mG = this->mAmbientColor.mG * this->mPenColor.mG;
	this->mFinalColor.mB = this->mAmbientColor.mB * this->mPenColor.mB;
	this->mFinalColor.mA = this->mAmbientColor.mA * this->mPenColor.mA;

	u32 finalColor = this->mFinalColor.PackRGBA ();
	
	if ( this->mFinalColor32 != finalColor ) {
		
		this->mFinalColor32 = finalColor;
		
		MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
		
		gfxMgr.mVertexCache.mVertexColor = this->mFinalColor;
		gfxMgr.mVertexCache.mVertexColor32 = this->mFinalColor32;
	}
}

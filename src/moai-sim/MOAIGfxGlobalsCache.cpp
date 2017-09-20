// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIGfxGlobalsCache.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIShaderUniform.h>

//================================================================//
// MOAIGfxGlobalsCache
//================================================================//

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::CheckGlobalsAndFlush () {

	bool needsFlush = ( this->mDirtyFlags & this->mShaderFlags ) != 0;
	
	// we only set these in the first place to trip the shader flags (if any); can clear these here
	this->mDirtyFlags &= ~( BASE_ATTRS_MASK );
	
	// flush if ya gotta
	if ( needsFlush ) {
		this->GfxStateWillChange ();
	}
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxGlobalsCache::GetNormToWndMtx () {

	return this->GetNormToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxGlobalsCache::GetNormToWndMtx ( const ZLRect& wndRect ) {

	float hWidth = wndRect.Width () * 0.5f;
	float hHeight = wndRect.Height () * 0.5f;

	// Wnd
	ZLMatrix4x4 normToWnd;
	normToWnd.Scale ( hWidth, -hHeight, 1.0f );
	
	ZLMatrix4x4 mtx;
	mtx.Translate ( hWidth + wndRect.mXMin, hHeight + wndRect.mYMin, 0.0f );
	normToWnd.Append ( mtx );
	
	return normToWnd;
}

//----------------------------------------------------------------//
const ZLMatrix4x4& MOAIGfxGlobalsCache::GetMtx ( u32 transformID ) {

	switch ( transformID ) {
	
		case INVERSE_PROJ_MTX:
		
			if ( this->mDirtyFlags & INVERSE_PROJ_MTX_MASK ) {
			
				this->mVertexTransforms [ INVERSE_PROJ_MTX ].Inverse ( this->GetMtx ( PROJ_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( INVERSE_PROJ_MTX );
			}
			return this->mVertexTransforms [ INVERSE_PROJ_MTX ];
			
		case INVERSE_UV_MTX:
			
			if ( this->mDirtyFlags & INVERSE_UV_MTX_MASK ) {
			
				this->mVertexTransforms [ INVERSE_UV_MTX ].Inverse ( this->GetMtx ( UV_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( INVERSE_UV_MTX );
			}
			return this->mVertexTransforms [ INVERSE_UV_MTX ];
			
		case INVERSE_VIEW_MTX:
			
			if ( this->mDirtyFlags & INVERSE_VIEW_MTX_MASK ) {
			
				this->mVertexTransforms [ INVERSE_VIEW_MTX ].Inverse ( this->GetMtx ( VIEW_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( INVERSE_VIEW_MTX );
			}
			return this->mVertexTransforms [ INVERSE_VIEW_MTX ];
			
		case INVERSE_VIEW_PROJ_MTX:
			
			if ( this->mDirtyFlags & INVERSE_VIEW_PROJ_MTX_MASK ) {
			
				this->mVertexTransforms [ INVERSE_VIEW_PROJ_MTX ].Inverse ( this->GetMtx ( VIEW_PROJ_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( INVERSE_VIEW_PROJ_MTX );
			}
			return this->mVertexTransforms [ INVERSE_VIEW_PROJ_MTX ];
			
		case INVERSE_WORLD_MTX:
			
			if ( this->mDirtyFlags & INVERSE_WORLD_MTX_MASK ) {
			
				this->mVertexTransforms [ INVERSE_WORLD_MTX ].Inverse ( this->GetMtx ( WORLD_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( INVERSE_WORLD_MTX );
			}
			return this->mVertexTransforms [ INVERSE_WORLD_MTX ];
			
		case INVERSE_WORLD_VIEW_MTX:
			
			if ( this->mDirtyFlags & INVERSE_WORLD_VIEW_MTX_MASK ) {
			
				this->mVertexTransforms [ INVERSE_WORLD_VIEW_MTX ].Inverse ( this->GetMtx ( WORLD_VIEW_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( INVERSE_WORLD_VIEW_MTX );
			}
			return this->mVertexTransforms [ INVERSE_WORLD_VIEW_MTX ];
			
		case INVERSE_WORLD_VIEW_PROJ_MTX:
			
			if ( this->mDirtyFlags & INVERSE_WORLD_VIEW_PROJ_MTX_MASK ) {
			
				this->mVertexTransforms [ INVERSE_WORLD_VIEW_PROJ_MTX ].Inverse ( this->GetMtx ( WORLD_VIEW_PROJ_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( INVERSE_WORLD_VIEW_PROJ_MTX );
			}
			return this->mVertexTransforms [ INVERSE_WORLD_VIEW_PROJ_MTX ];
			
		case PROJ_MTX:
			
			return this->mVertexTransforms [ PROJ_MTX ];
			
		case UV_MTX:
			
			return this->mVertexTransforms [ UV_MTX ];
			
		case VIEW_MTX:
			
			return this->mVertexTransforms [ VIEW_MTX ];
			
		case VIEW_PROJ_MTX:
			
			if ( this->mDirtyFlags & VIEW_PROJ_MTX_MASK ) {
			
				this->mVertexTransforms [ VIEW_PROJ_MTX ] = this->mVertexTransforms [ VIEW_MTX ];
				this->mVertexTransforms [ VIEW_PROJ_MTX ].Append ( this->mVertexTransforms [ PROJ_MTX ]);
				
				this->mDirtyFlags &= ~ID_TO_FLAG ( VIEW_PROJ_MTX );
			}
			return this->mVertexTransforms [ VIEW_PROJ_MTX ];
			
		case WORLD_MTX:
			
			return this->mVertexTransforms [ WORLD_MTX ];
			
		case WORLD_VIEW_MTX:
			
			if ( this->mDirtyFlags & WORLD_VIEW_MTX_MASK ) {
			
				this->mVertexTransforms [ WORLD_VIEW_MTX ] = this->mVertexTransforms [ WORLD_MTX ];
				this->mVertexTransforms [ WORLD_VIEW_MTX ].Append ( this->mVertexTransforms [ VIEW_MTX ]);
				
				this->mDirtyFlags &= ~ID_TO_FLAG ( WORLD_VIEW_MTX );
			}
			return this->mVertexTransforms [ WORLD_VIEW_MTX ];
			
		case WORLD_VIEW_PROJ_MTX:
			
			if ( this->mDirtyFlags & WORLD_VIEW_PROJ_MTX_MASK ) {
			
				this->mVertexTransforms [ WORLD_VIEW_PROJ_MTX ] = this->mVertexTransforms [ WORLD_MTX ];
				this->mVertexTransforms [ WORLD_VIEW_PROJ_MTX ].Append ( this->GetMtx ( VIEW_PROJ_MTX ));
				
				this->mDirtyFlags &= ~ID_TO_FLAG ( WORLD_VIEW_PROJ_MTX);
			}
			return this->mVertexTransforms [ WORLD_VIEW_PROJ_MTX ];
	}
	
	assert ( false );
	return this->mVertexTransforms [ WORLD_MTX ];
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxGlobalsCache::GetWorldToWndMtx () {

	return this->GetWorldToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxGlobalsCache::GetWorldToWndMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 worldToWnd = this->GetMtx ( VIEW_PROJ_MTX );
	worldToWnd.Append ( MOAIGfxGlobalsCache::GetNormToWndMtx ( wndRect ));
	
	return worldToWnd;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxGlobalsCache::GetWndToNormMtx () {

	return this->GetWndToNormMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxGlobalsCache::GetWndToNormMtx ( const ZLRect& wndRect ) {

	float hWidth = wndRect.Width () * 0.5f;
	float hHeight = wndRect.Height () * 0.5f;

	// Inv Wnd
	ZLMatrix4x4 wndToNorm;
	wndToNorm.Translate ( -hWidth - wndRect.mXMin, -hHeight - wndRect.mYMin, 0.0f );
	
	ZLMatrix4x4 mtx;
	mtx.Scale (( 1.0f / hWidth ), -( 1.0f / hHeight ), 1.0f );
	wndToNorm.Append ( mtx );
	
	return wndToNorm;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxGlobalsCache::GetWndToWorldMtx () {

	return this->GetWndToWorldMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxGlobalsCache::GetWndToWorldMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 wndToWorld = MOAIGfxGlobalsCache::GetWndToNormMtx ( wndRect );
	
	// inv viewproj
	ZLMatrix4x4 mtx = this->GetMtx ( VIEW_PROJ_MTX );
	mtx.Inverse ();
	wndToWorld.Append ( mtx );
	
	return wndToWorld;
}

//----------------------------------------------------------------//
const ZLFrustum& MOAIGfxGlobalsCache::GetViewVolume () {

	if ( this->mDirtyFlags & VIEW_VOLUME_MASK ) {
		this->mViewVolume.Init ( this->GetMtx ( INVERSE_VIEW_PROJ_MTX ));
		this->mDirtyFlags &= ~VIEW_VOLUME_MASK;
	}
	return this->mViewVolume;
}

//----------------------------------------------------------------//
MOAIGfxGlobalsCache::MOAIGfxGlobalsCache () :
	mDirtyFlags ( 0 ),
	mShaderFlags ( 0 ),
	mFinalColor32 ( 0xffffffff ),
	mBufferWidth ( 0 ),
	mBufferHeight ( 0 ) {

	for ( u32 i = 0; i < TOTAL_MATRICES; ++i ) {
		this->mVertexTransforms [ i ].Ident ();
		this->mDirtyFlags = 0;
	}
	
	this->mAmbientColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mFinalColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mPenColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
}

//----------------------------------------------------------------//
MOAIGfxGlobalsCache::~MOAIGfxGlobalsCache () {
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetAmbientColor ( u32 color ) {

	this->mAmbientColor.SetRGBA ( color );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetAmbientColor ( const ZLColorVec& colorVec ) {

	this->mAmbientColor = colorVec;
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetAmbientColor ( float r, float g, float b, float a ) {

	this->mAmbientColor.Set ( r, g, b, a );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetMtx ( u32 transformID ) {

	ZLMatrix4x4 mtx;
	mtx.Ident ();
	this->SetMtx ( transformID, mtx );
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetMtx ( u32 transformID, const ZLAffine3D& transform ) {

	ZLMatrix4x4 mtx;
	mtx.Init ( transform );
	this->SetMtx ( transformID, mtx );
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetMtx ( u32 transformID, const ZLMatrix4x4& mtx ) {

	u32 dirtyMask = 0;

	switch ( transformID ) {
	
		case PROJ_MTX:
		
			dirtyMask = PROJ_MTX_DIRTY_MASK;
			break;
			
		case UV_MTX:
		
			dirtyMask = UV_MTX_DIRTY_MASK;
			break;
			
		case VIEW_MTX:
		
			dirtyMask = VIEW_MTX_DIRTY_MASK;
			break;
			
		case WORLD_MTX:
		
			dirtyMask = WORLD_MTX_DIRTY_MASK;
			break;
	
		case INVERSE_PROJ_MTX:
		case INVERSE_UV_MTX:
		case INVERSE_VIEW_MTX:
		case INVERSE_VIEW_PROJ_MTX:
		case INVERSE_WORLD_MTX:
		case INVERSE_WORLD_VIEW_MTX:
		case INVERSE_WORLD_VIEW_PROJ_MTX:
		case VIEW_PROJ_MTX:
		case WORLD_VIEW_MTX:
		case WORLD_VIEW_PROJ_MTX:
		
			assert ( false ); // read only
			return;
	}
	
	if ( !this->mVertexTransforms [ transformID ].IsSame ( mtx )) {
		this->mVertexTransforms [ transformID ] = mtx;
		this->mDirtyFlags |= dirtyMask;
	}
	
	this->CheckGlobalsAndFlush ();
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetPenColor ( u32 color ) {

	this->mPenColor.SetRGBA ( color );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetPenColor ( const ZLColorVec& colorVec ) {

	this->mPenColor = colorVec;
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetPenColor ( float r, float g, float b, float a ) {

	this->mPenColor.Set ( r, g, b, a );
	this->UpdateFinalColor ();
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::UpdateFinalColor () {

	this->mFinalColor.mR = this->mAmbientColor.mR * this->mPenColor.mR;
	this->mFinalColor.mG = this->mAmbientColor.mG * this->mPenColor.mG;
	this->mFinalColor.mB = this->mAmbientColor.mB * this->mPenColor.mB;
	this->mFinalColor.mA = this->mAmbientColor.mA * this->mPenColor.mA;

	u32 finalColor = this->mFinalColor.PackRGBA ();
	
	if ( this->mFinalColor32 != finalColor ) {
		
		this->mFinalColor32 = finalColor;
		this->mDirtyFlags |= PEN_COLOR_DIRTY_MASK;
		
		MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
		
		gfxMgr.mVertexCache.mVertexColor = this->mFinalColor;
		gfxMgr.mVertexCache.mVertexColor32 = this->mFinalColor32;
		
		this->CheckGlobalsAndFlush ();
	}
}

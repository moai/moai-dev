// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIGfxGlobalsCache.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIShaderUniform.h>
#include <moai-sim/MOAIViewport.h>
#include <moai-sim/MOAIViewProj.h>

//================================================================//
// MOAIGfxGlobalsCache
//================================================================//

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
	
		case CLIP_TO_DISPLAY_MTX:
	
			if ( this->mDirtyFlags & CLIP_TO_DISPLAY_MTX_MASK ) {
			
				if ( this->mUseDebugMtx ) {
			
					this->mMatrices [ CLIP_TO_DISPLAY_MTX ] = this->GetMtx ( CLIP_TO_WORLD_MTX ); // back to world space
					this->mMatrices [ CLIP_TO_DISPLAY_MTX ].Append ( this->GetMtx ( WORLD_TO_DISPLAY_MTX )); // and forward to display space
				}
				else {
					this->mMatrices [ CLIP_TO_DISPLAY_MTX ].Ident ();
				}
				this->mDirtyFlags &= ~ID_TO_FLAG ( CLIP_TO_DISPLAY_MTX );
			}
			return this->mMatrices [ CLIP_TO_DISPLAY_MTX ];
	
		case CLIP_TO_MODEL_MTX:
			
			if ( this->mDirtyFlags & CLIP_TO_MODEL_MTX_MASK ) {
			
				this->mMatrices [ CLIP_TO_MODEL_MTX ].Inverse ( this->GetMtx ( MODEL_TO_CLIP_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( CLIP_TO_MODEL_MTX );
			}
			return this->mMatrices [ CLIP_TO_MODEL_MTX ];
	
		case CLIP_TO_VIEW_MTX:
		
			if ( this->mDirtyFlags & CLIP_TO_VIEW_MTX_MASK ) {
			
				this->mMatrices [ CLIP_TO_VIEW_MTX ].Inverse ( this->GetMtx ( VIEW_TO_CLIP_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( CLIP_TO_VIEW_MTX );
			}
			return this->mMatrices [ CLIP_TO_VIEW_MTX ];
		
		case CLIP_TO_WINDOW_MTX:
			
			return this->mMatrices [ CLIP_TO_WINDOW_MTX ];
		
		case CLIP_TO_WORLD_MTX:
			
			if ( this->mDirtyFlags & CLIP_TO_WORLD_MTX_MASK ) {
			
				this->mMatrices [ CLIP_TO_WORLD_MTX ].Inverse ( this->GetMtx ( WORLD_TO_CLIP_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( CLIP_TO_WORLD_MTX );
			}
			return this->mMatrices [ CLIP_TO_WORLD_MTX ];
		
		case MODEL_TO_CLIP_MTX:
			
			if ( this->mDirtyFlags & MODEL_TO_CLIP_MTX_MASK ) {
			
				this->mMatrices [ MODEL_TO_CLIP_MTX ] = this->mMatrices [ MODEL_TO_WORLD_MTX ];
				this->mMatrices [ MODEL_TO_CLIP_MTX ].Append ( this->GetMtx ( WORLD_TO_CLIP_MTX ));
				
				this->mDirtyFlags &= ~ID_TO_FLAG ( MODEL_TO_CLIP_MTX);
			}
			return this->mMatrices [ MODEL_TO_CLIP_MTX ];
		
		case MODEL_TO_DISPLAY_MTX:
	
			if ( this->mDirtyFlags & MODEL_TO_DISPLAY_MTX_MASK ) {
			
				if ( this->mUseDebugMtx ) {
			
					this->mMatrices [ MODEL_TO_DISPLAY_MTX ] = this->GetMtx ( MODEL_TO_WORLD_MTX );
					this->mMatrices [ MODEL_TO_DISPLAY_MTX ].Append ( this->GetMtx ( WORLD_TO_DISPLAY_MTX ));
				}
				else {
					this->mMatrices [ MODEL_TO_DISPLAY_MTX ] = this->GetMtx ( MODEL_TO_CLIP_MTX );
				}
				this->mDirtyFlags &= ~ID_TO_FLAG ( MODEL_TO_DISPLAY_MTX );
			}
			return this->mMatrices [ MODEL_TO_DISPLAY_MTX ];
		
		case MODEL_TO_VIEW_MTX:
			
			if ( this->mDirtyFlags & MODEL_TO_VIEW_MTX_MASK ) {
			
				this->mMatrices [ MODEL_TO_VIEW_MTX ] = this->mMatrices [ MODEL_TO_WORLD_MTX ];
				this->mMatrices [ MODEL_TO_VIEW_MTX ].Append ( this->mMatrices [ WORLD_TO_VIEW_MTX ]);
				
				this->mDirtyFlags &= ~ID_TO_FLAG ( MODEL_TO_VIEW_MTX );
			}
			return this->mMatrices [ MODEL_TO_VIEW_MTX ];
		
		case MODEL_TO_UV_MTX:
			
			return this->mMatrices [ MODEL_TO_UV_MTX ];
		
		case MODEL_TO_WORLD_MTX:
			
			return this->mMatrices [ MODEL_TO_WORLD_MTX ];
		
		case NORMALIZED_MODEL_TO_CLIP_MTX:
			
			if ( this->mDirtyFlags & NORMALIZED_MODEL_TO_CLIP_MTX_MASK ) {
			
				ZLMatrix3x3 mtx = ZLMatrix3x3 ( this->GetMtx ( MODEL_TO_CLIP_MTX ));
				mtx.Inverse ();
				mtx.Transpose ();
			
				this->mMatrices [ NORMALIZED_MODEL_TO_CLIP_MTX ] = ZLMatrix4x4 ( mtx );
				
				this->mDirtyFlags &= ~ID_TO_FLAG ( NORMALIZED_MODEL_TO_CLIP_MTX );
			}
			return this->mMatrices [ NORMALIZED_MODEL_TO_CLIP_MTX ];
		
		case NORMALIZED_MODEL_TO_VIEW_MTX:
			
			if ( this->mDirtyFlags & NORMALIZED_MODEL_TO_VIEW_MTX_MASK ) {
			
				ZLMatrix3x3 mtx = ZLMatrix3x3 ( this->GetMtx ( MODEL_TO_VIEW_MTX ));
				mtx.Inverse ();
				mtx.Transpose ();
			
				this->mMatrices [ NORMALIZED_MODEL_TO_VIEW_MTX ] = ZLMatrix4x4 ( mtx );
				
				this->mDirtyFlags &= ~ID_TO_FLAG ( NORMALIZED_MODEL_TO_VIEW_MTX );
			}
			return this->mMatrices [ NORMALIZED_MODEL_TO_VIEW_MTX ];
		
		case NORMALIZED_MODEL_TO_WORLD_MTX:
			
			if ( this->mDirtyFlags & NORMALIZED_MODEL_TO_WORLD_MTX_MASK ) {
			
				ZLMatrix3x3 mtx = ZLMatrix3x3 ( this->GetMtx ( MODEL_TO_WORLD_MTX ));
				mtx.Inverse ();
				mtx.Transpose ();
			
				this->mMatrices [ NORMALIZED_MODEL_TO_WORLD_MTX ] = ZLMatrix4x4 ( mtx );
				
				this->mDirtyFlags &= ~ID_TO_FLAG ( NORMALIZED_MODEL_TO_WORLD_MTX );
			}
			return this->mMatrices [ NORMALIZED_MODEL_TO_WORLD_MTX ];
		
		case UV_TO_MODEL_MTX:
			
			if ( this->mDirtyFlags & UV_TO_MODEL_MTX_MASK ) {
			
				this->mMatrices [ UV_TO_MODEL_MTX ].Inverse ( this->GetMtx ( MODEL_TO_UV_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( UV_TO_MODEL_MTX );
			}
			return this->mMatrices [ UV_TO_MODEL_MTX ];
		
		
		case VIEW_TO_CLIP_MTX:
			
			return this->mMatrices [ VIEW_TO_CLIP_MTX ];
		
		case VIEW_TO_DISPLAY_MTX:
	
			if ( this->mDirtyFlags & VIEW_TO_DISPLAY_MTX_MASK ) {
			
				if ( this->mUseDebugMtx ) {
					
					this->mMatrices [ VIEW_TO_DISPLAY_MTX ].Inverse ( this->GetMtx ( VIEW_TO_WORLD_MTX )); // back to world space
					this->mMatrices [ VIEW_TO_DISPLAY_MTX ].Append ( this->GetMtx ( WORLD_TO_DISPLAY_MTX )); // and forward to display space
				}
				else {
					this->mMatrices [ VIEW_TO_DISPLAY_MTX ] = this->GetMtx ( VIEW_TO_CLIP_MTX );
				}
				this->mDirtyFlags &= ~ID_TO_FLAG ( VIEW_TO_DISPLAY_MTX );
			}
			return this->mMatrices [ VIEW_TO_DISPLAY_MTX ];
		
		case VIEW_TO_MODEL_MTX:
			
			if ( this->mDirtyFlags & VIEW_TO_MODEL_MTX_MASK ) {
			
				this->mMatrices [ VIEW_TO_MODEL_MTX ].Inverse ( this->GetMtx ( MODEL_TO_VIEW_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( VIEW_TO_MODEL_MTX );
			}
			return this->mMatrices [ VIEW_TO_MODEL_MTX ];
		
		case VIEW_TO_WORLD_MTX:
			
			if ( this->mDirtyFlags & VIEW_TO_WORLD_MTX_MASK ) {
			
				this->mMatrices [ VIEW_TO_WORLD_MTX ].Inverse ( this->GetMtx ( WORLD_TO_VIEW_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( VIEW_TO_WORLD_MTX );
			}
			return this->mMatrices [ VIEW_TO_WORLD_MTX ];
		
		case WINDOW_TO_CLIP_MTX:
			
			if ( this->mDirtyFlags & WINDOW_TO_CLIP_MTX_MASK ) {
			
				this->mMatrices [ WINDOW_TO_CLIP_MTX ].Inverse ( this->GetMtx ( CLIP_TO_WINDOW_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( WINDOW_TO_CLIP_MTX );
			}
			return this->mMatrices [ WINDOW_TO_CLIP_MTX ];
		
		case WORLD_TO_CLIP_MTX:
			
			if ( this->mDirtyFlags & WORLD_TO_CLIP_MTX_MASK ) {
			
				this->mMatrices [ WORLD_TO_CLIP_MTX ] = this->mMatrices [ WORLD_TO_VIEW_MTX ];
				this->mMatrices [ WORLD_TO_CLIP_MTX ].Append ( this->mMatrices [ VIEW_TO_CLIP_MTX ]);
				
				this->mDirtyFlags &= ~ID_TO_FLAG ( WORLD_TO_CLIP_MTX );
			}
			return this->mMatrices [ WORLD_TO_CLIP_MTX ];
		
		case WORLD_TO_DEBUG_MTX:
			
			return this->mMatrices [ WORLD_TO_DEBUG_MTX ];
		
		case WORLD_TO_DISPLAY_MTX:
	
			if ( this->mDirtyFlags & WORLD_TO_DISPLAY_MTX_MASK ) {
			
				if ( this->mUseDebugMtx ) {
			
					this->mMatrices [ WORLD_TO_DISPLAY_MTX ] = this->GetMtx ( WORLD_TO_DEBUG_MTX );
				}
				else {
					this->mMatrices [ WORLD_TO_DISPLAY_MTX ] = this->GetMtx ( WORLD_TO_CLIP_MTX );
				}
				this->mDirtyFlags &= ~ID_TO_FLAG ( WORLD_TO_DISPLAY_MTX );
			}
			return this->mMatrices [ WORLD_TO_DISPLAY_MTX ];
		
		case WORLD_TO_MODEL_MTX:
			
			if ( this->mDirtyFlags & WORLD_TO_MODEL_MTX_MASK ) {
			
				this->mMatrices [ WORLD_TO_MODEL_MTX ].Inverse ( this->GetMtx ( MODEL_TO_WORLD_MTX ));
				this->mDirtyFlags &= ~ID_TO_FLAG ( WORLD_TO_MODEL_MTX );
			}
			return this->mMatrices [ WORLD_TO_MODEL_MTX ];
			
		case WORLD_TO_VIEW_MTX:
			
			return this->mMatrices [ WORLD_TO_VIEW_MTX ];
	}
	
	assert ( false );
	return this->mMatrices [ MODEL_TO_WORLD_MTX ];
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxGlobalsCache::GetWorldToWndMtx () {

	return this->GetWorldToWndMtx ( this->GetViewRect ());
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAIGfxGlobalsCache::GetWorldToWndMtx ( const ZLRect& wndRect ) {

	ZLMatrix4x4 worldToWnd = this->GetMtx ( WORLD_TO_CLIP_MTX );
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
	ZLMatrix4x4 mtx = this->GetMtx ( WORLD_TO_CLIP_MTX );
	mtx.Inverse ();
	wndToWorld.Append ( mtx );
	
	return wndToWorld;
}

//----------------------------------------------------------------//
const ZLFrustum& MOAIGfxGlobalsCache::GetViewVolume () {

	if ( this->mDirtyFlags & VIEW_VOLUME_MASK ) {
		this->mViewVolume.Init ( this->GetMtx ( CLIP_TO_WORLD_MTX ));
		this->mDirtyFlags &= ~VIEW_VOLUME_MASK;
	}
	return this->mViewVolume;
}

//----------------------------------------------------------------//
bool MOAIGfxGlobalsCache::IsInputMtx ( u32 transformID ) {

	return (( transformID == MODEL_TO_WORLD_MTX ) || ( transformID == WORLD_TO_VIEW_MTX ) || ( transformID == VIEW_TO_CLIP_MTX ) || ( transformID == MODEL_TO_UV_MTX ));
}

//----------------------------------------------------------------//
MOAIGfxGlobalsCache::MOAIGfxGlobalsCache () :
	mDirtyFlags ( 0 ),
	mShaderFlags ( 0 ),
	mFinalColor32 ( 0xffffffff ),
	mClearFlags ( 0 ),
	mClearDepth ( 0.0 ),
	mUseDebugMtx ( false ) {

	assert ( TOTAL_GLOBALS < MAX_GLOBALS );

	for ( u32 i = 0; i < TOTAL_MATRICES; ++i ) {
		this->mMatrices [ i ].Ident ();
		this->mDirtyFlags = 0;
	}
	
	this->mAmbientColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mFinalColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mPenColor.Set ( 1.0f, 1.0f, 1.0f, 1.0f );
	this->mClearColor.Set ( 0.0f, 0.0f, 0.0f, 1.0f );
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
void MOAIGfxGlobalsCache::SetDebug ( MOAIViewport* viewport, MOAICamera* camera ) {

	this->SetMtx ( WORLD_TO_DEBUG_MTX, MOAIViewProj::GetDebugMtx ( viewport, camera ));
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetDirtyFlags ( u32 dirtyFlags ) {

	bool needsFlush = ( dirtyFlags & this->mShaderFlags ) != 0;
	
	// flush if ya gotta
	if ( needsFlush ) {
		this->GfxStateWillChange ();
	}
	
	// we only set these in the first place to trip the shader flags (if any); can clear these here
	this->mDirtyFlags = dirtyFlags & ~( BASE_ATTRS_MASK );
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetMtx ( u32 transformID ) {

	ZLMatrix4x4 mtx;
	mtx.Ident ();
	this->SetMtx ( transformID, mtx );
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetMtx ( u32 transformID, const ZLAffine3D& transform ) {

	this->SetMtx ( transformID, ZLMatrix4x4 ( transform ));
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::SetMtx ( u32 transformID, const ZLMatrix4x4& mtx ) {

	u32 dirtyMask = 0;

	switch ( transformID ) {
	
		case VIEW_TO_CLIP_MTX:
		
			dirtyMask = VIEW_TO_CLIP_MTX_DIRTY_MASK;
			break;
			
		case MODEL_TO_UV_MTX:
		
			dirtyMask = MODEL_TO_UV_MTX_DIRTY_MASK;
			break;
			
		case WORLD_TO_DEBUG_MTX:
		
			this->mUseDebugMtx = !( mtx.IsIdent () || mtx.IsSame ( this->mMatrices [ WORLD_TO_VIEW_MTX ]));
		
			dirtyMask = WORLD_TO_DEBUG_MTX_DIRTY_MASK;
			break;
			
		case WORLD_TO_VIEW_MTX:
			
			this->mUseDebugMtx = !( this->mMatrices [ WORLD_TO_DEBUG_MTX ].IsIdent () || mtx.IsSame ( this->mMatrices [ WORLD_TO_DEBUG_MTX ]));
		
			dirtyMask = WORLD_TO_VIEW_MTX_DIRTY_MASK;
			break;
			
		case CLIP_TO_WINDOW_MTX:
		
			dirtyMask = CLIP_TO_WINDOW_MTX_DIRTY_MASK;
			break;
			
		case MODEL_TO_WORLD_MTX:
		
			dirtyMask = MODEL_TO_WORLD_MTX_DIRTY_MASK;
			break;
	
		default:
		
			assert ( false ); // read only
			return;
	}
		
	if ( !this->mMatrices [ transformID ].IsSame ( mtx )) {
	
		this->SetDirtyFlags ( this->mDirtyFlags | dirtyMask );
		this->mMatrices [ transformID ] = mtx;
	}
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
void MOAIGfxGlobalsCache::SetViewProj ( MOAIViewport* viewport, MOAICamera* camera, const ZLVec3D& parallax ) {

	ZLMatrix4x4 view = MOAIViewProj::GetViewMtx ( camera, parallax );
	ZLMatrix4x4 proj = MOAIViewProj::GetProjectionMtx ( viewport, camera );
	
	this->SetMtx ( MOAIGfxGlobalsCache::WORLD_TO_VIEW_MTX, view );
	this->SetMtx ( MOAIGfxGlobalsCache::VIEW_TO_CLIP_MTX, proj );
	this->SetMtx ( MOAIGfxGlobalsCache::CLIP_TO_WINDOW_MTX, viewport ? viewport->GetProjMtx () : ZLMatrix4x4::IDENT );
}

//----------------------------------------------------------------//
void MOAIGfxGlobalsCache::UpdateFinalColor () {

	this->mFinalColor.mR = this->mAmbientColor.mR * this->mPenColor.mR;
	this->mFinalColor.mG = this->mAmbientColor.mG * this->mPenColor.mG;
	this->mFinalColor.mB = this->mAmbientColor.mB * this->mPenColor.mB;
	this->mFinalColor.mA = this->mAmbientColor.mA * this->mPenColor.mA;

	u32 finalColor = this->mFinalColor.PackRGBA ();
	
	if ( this->mFinalColor32 != finalColor ) {
		
		this->SetDirtyFlags ( this->mDirtyFlags |= PEN_COLOR_DIRTY_MASK );
		
		this->mFinalColor32 = finalColor;
		
		MOAIGfxMgr& gfxMgr = MOAIGfxMgr::Get ();
		
		gfxMgr.mVertexCache.mVertexColor = this->mFinalColor;
		gfxMgr.mVertexCache.mVertexColor32 = this->mFinalColor32;
	}
}

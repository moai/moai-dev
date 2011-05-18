// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAICameraAnchor2D.h>
#include <moaicore/MOAICameraFitter2D.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIViewport.h>

//----------------------------------------------------------------//
int MOAICameraFitter2D::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	self->Clear ();

	return 0;
}

//----------------------------------------------------------------//
int MOAICameraFitter2D::_insertAnchor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "UU" )
	
	MOAICameraAnchor2D* anchor = state.GetLuaObject < MOAICameraAnchor2D >( 2 );
	if ( anchor ) {
		self->AddAnchor ( *anchor );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAICameraFitter2D::_removeAnchor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "UU" )
	
	MOAICameraAnchor2D* anchor = state.GetLuaObject < MOAICameraAnchor2D >( 2 );
	if ( anchor ) {
		self->RemoveAnchor ( *anchor );
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAICameraFitter2D::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "UNNNN" )

	float x0	= state.GetValue < float >( 2, 0.0f );
	float y0	= state.GetValue < float >( 3, 0.0f );
	float x1	= state.GetValue < float >( 4, 0.0f );
	float y1	= state.GetValue < float >( 5, 0.0f );
	
	self->mBounds.Init ( x0, y0, x1, y1 );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAICameraFitter2D::_setMin ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	self->mMin = state.GetValue < float >( 2, 0.0f );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAICameraFitter2D::_setViewport ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "UU" )
	
	MOAIViewport* viewport = state.GetLuaObject < MOAIViewport >( 2 );
	if ( viewport ) {
		self->mViewport = viewport;
		self->mViewRect = viewport->GetRect ();
	}
	return 0;
}

//================================================================//
// MOAICameraFitter2D
//================================================================//

//----------------------------------------------------------------//
void MOAICameraFitter2D::AddAnchor ( MOAICameraAnchor2D& anchor ) {

	if ( !this->mAnchors.contains ( &anchor )) {
		anchor.Retain ();
		this->mAnchors.insert ( &anchor );
	}
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::Center () {

	//this->mTargetLoc = camera.mLoc;
	//this->mTargetScale = camera.mScale;

	//if ( !this->mIsReady ) return;
	//
	//USCamera2D fitCam = camera;
	//
	//USAffine2D worldToWndMtx;
	//USCanvas::GetWorldToWndMtx ( viewport, fitCam, worldToWndMtx );
	//
	//USAffine2D wndToWorldMtx;
	//USCanvas::GetWndToWorldMtx ( viewport, fitCam, wndToWorldMtx );
	//
	//USRect worldRect = this->mWorldRect;
	//
	//if ( this->mFitRects.size ()) {
	//
	//	bool initFrame = true;
	//	USRect frameRect;
	//	
	//	FitRectIt fitRectIt = this->mFitRects.begin ();
	//	for ( ; fitRectIt != this->mFitRects.end (); ++fitRectIt ) {
	//		
	//		USFitRect& fitRect = *fitRectIt;
	//		
	//		USVec2D loc = fitRect.mWorldLoc;
	//		worldToWndMtx.Transform ( loc );
	//		
	//		USRect rect = fitRect.mScreenRect;
	//		rect.Offset ( loc.mX, loc.mY );
	//		
	//		if ( initFrame ) {
	//			frameRect = rect;
	//			initFrame = false;
	//		}
	//		else {
	//			frameRect.Grow ( rect );
	//		}
	//	}
	//	
	//	wndToWorldMtx.Transform ( frameRect );
	//	frameRect.Bless ();
	//	worldRect.Grow ( frameRect );
	//}
	//
	//worldRect.GetCenter ( this->mTargetLoc );
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::Clear () {

	AnchorIt anchorIt = this->mAnchors.begin ();
	while ( anchorIt != this->mAnchors.end ()) {
		MOAICameraAnchor2D* anchor = *anchorIt;
		anchorIt = this->mAnchors.erase ( anchorIt );
		anchor->Release ();
	} 
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::Fit () {

	if ( !this->mAnchors.size ()) return;
	if ( !this->mViewport ) return;

	// reset the fitter
	this->mTargetLoc.Init ( 0.0f, 0.0f );
	this->mTargetScale = 1.0f;

	// build the world rect
	USRect worldRect = this->GetWorldRect ();

	USAffine2D camera;
	USRect screenRect;
	
	// chase down the screen rect fit
	for ( u32 i = 0; i < 8; ++i ) {
		
		this->GetCamera ( camera );
		
		USAffine2D worldToWnd;
		USCanvas::GetWorldToWndMtx ( *this->mViewport, camera, worldToWnd );
		
		screenRect = worldRect;
		worldToWnd.Transform ( screenRect );
		
		USRect padRect = this->GetScreenRect ( worldToWnd );
		
		screenRect.Grow ( padRect );
		this->SetTarget ( camera, screenRect );
	}
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::GetCamera ( USAffine2D& camera ) {

	camera.ScRoTr ( this->mTargetScale, this->mTargetScale, 0.0f, this->mTargetLoc.mX, this->mTargetLoc.mY );
}

//----------------------------------------------------------------//
USRect MOAICameraFitter2D::GetScreenRect ( const USAffine2D& worldToWnd ) {

	// build the world rect
	AnchorIt anchorIt = this->mAnchors.begin ();
	USRect screenRect = ( *anchorIt )->GetScreenRect ( worldToWnd );
	++anchorIt;
	
	for ( ; anchorIt != this->mAnchors.end (); ++anchorIt ) {
		MOAICameraAnchor2D* anchor = *anchorIt;
		screenRect.Grow ( anchor->GetScreenRect ( worldToWnd ));
	}
	return screenRect;
}

//----------------------------------------------------------------//
USRect MOAICameraFitter2D::GetWorldRect () {

	// build the world rect
	AnchorIt anchorIt = this->mAnchors.begin ();
	USRect worldRect = ( *anchorIt )->GetWorldRect ();
	++anchorIt;
	
	for ( ; anchorIt != this->mAnchors.end (); ++anchorIt ) {
		MOAICameraAnchor2D* anchor = *anchorIt;
		worldRect.Grow ( anchor->GetWorldRect ());
	}
	
	float width = worldRect.Width ();
	float height = worldRect.Height ();
	float pad;
	
	if ( this->mMin > 0.0f ) {
	
		if ( width < this->mMin ) {
			pad = ( this->mMin - width ) * 0.5f;
			worldRect.mXMin -= pad;
			worldRect.mXMax += pad;
		}
		
		if ( height < this->mMin ) {
			pad = ( this->mMin - height ) * 0.5f;
			worldRect.mYMin -= pad;
			worldRect.mYMax += pad;
		}
	}
	
	if ( this->mMax > 0.0f ) {
	
		if ( width > this->mMax ) {
			pad = ( this->mMax - width ) * 0.5f;
			worldRect.mXMin -= pad;
			worldRect.mXMax += pad;
		}
		
		if ( height > this->mMax ) {
			pad = ( this->mMax - height ) * 0.5f;
			worldRect.mYMin -= pad;
			worldRect.mYMax += pad;
		}
	}
	
	return worldRect;
}

//----------------------------------------------------------------//
bool MOAICameraFitter2D::IsDone () {

	return false;
}

//----------------------------------------------------------------//
MOAICameraFitter2D::MOAICameraFitter2D () :
	mMin ( 0.0f ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAITransformBase )
		RTTI_EXTEND ( MOAIAction )
	RTTI_END
	
	this->mViewRect.Init ( 0.0f, 0.0f, 0.0f, 0.0f );
	this->mTargetLoc.Init ( 0.0f, 0.0f );
	this->mTargetScale = 1.0f;
}

//----------------------------------------------------------------//
MOAICameraFitter2D::~MOAICameraFitter2D () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::OnDepNodeUpdate () {

	this->Fit ();

	this->mLocalToWorldMtx.ScRoTr ( this->mTargetScale, this->mTargetScale, 0.0f, this->mTargetLoc.mX, this->mTargetLoc.mY );
	this->mWorldToLocalMtx.Inverse ( this->mLocalToWorldMtx );
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::OnUpdate ( float step ) {
	UNUSED ( step );
	
	this->ScheduleUpdate ();
	
	// make sure all the anchors are ahead of fitter in the schedule
	AnchorIt anchorIt = this->mAnchors.begin ();	
	for ( ; anchorIt != this->mAnchors.end (); ++anchorIt ) {
		MOAICameraAnchor2D* anchor = *anchorIt;
		anchor->Activate ( *this );
	}
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::RemoveAnchor ( MOAICameraAnchor2D& anchor ) {

	if ( this->mAnchors.contains ( &anchor )) {
		this->mAnchors.erase ( &anchor );
		anchor.Release ();
	}
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::SetTarget ( const USAffine2D& camera, const USRect& screenRect ) {
	
	USAffine2D worldToWnd;
	USCanvas::GetWorldToWndMtx ( *this->mViewport, camera, worldToWnd );
	
	USAffine2D wndToWorld;
	USCanvas::GetWndToWorldMtx ( *this->mViewport, camera, wndToWorld );
	
	// fit the viewport rect to the target rect
	USRect viewport = this->mViewport->GetRect ();
	screenRect.FitOutside ( viewport );
	
	// since aspect stayed constant, can use either width or height to fit
	this->mTargetScale *= viewport.Width () / this->mViewport->Width ();

	// TODO: take viewport offset into account
	
	// take the viewport back into world space
	wndToWorld.Transform ( viewport );
	this->mBounds.Constrain ( viewport );
	
	viewport.GetCenter ( this->mTargetLoc );
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::RegisterLuaClass ( USLuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
	MOAITransformBase::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::RegisterLuaFuncs ( USLuaState& state ) {

	MOAIAction::RegisterLuaFuncs ( state );
	MOAITransformBase::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "clear",				_clear },
		{ "insertAnchor",		_insertAnchor },
		{ "removeAnchor",		_removeAnchor },
		{ "setBounds",			_setBounds },
		{ "setMin",				_setMin },
		{ "setViewport",		_setViewport },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAICameraFitter2D::ToString () {

	STLString repr( MOAIAction::ToString ());
	return repr;
}

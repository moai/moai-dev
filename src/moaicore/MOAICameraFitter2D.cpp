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
	USRect screenRect;
	
	USAffine2D camera;
	
	// chase down the screen rect fit
	//for ( u32 i = 0; ( i < 8 ) && more; ++i ) {
		
		this->GetCamera ( camera );
		
		USAffine2D worldToWnd;
		USCanvas::GetWorldToWndMtx ( *this->mViewport, camera, worldToWnd );
		
		screenRect = worldRect;
		worldToWnd.Transform ( screenRect );
		
		USRect padRect = this->GetScreenRect ( worldToWnd );
		
		screenRect.Grow ( padRect );
		this->SetTarget ( camera, screenRect );
	//}
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
	return worldRect;
}

//----------------------------------------------------------------//
bool MOAICameraFitter2D::IsDone () {

	return false;
}

//----------------------------------------------------------------//
MOAICameraFitter2D::MOAICameraFitter2D () {

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
void MOAICameraFitter2D::NudgeLine ( const USPlane2D& plane, float step, USVec2D& v0, USVec2D& v1 ) {

	float dist;

	dist = -USDist::PointToPlane2D ( v0, plane );
	if ( dist > 0.0f ) {
		v0.mX += plane.mNorm.mX * dist * step;
		v0.mY += plane.mNorm.mY * dist * step;
	}
	
	dist = -USDist::PointToPlane2D ( v1, plane );
	if ( dist > 0.0f ) {
		v1.mX += plane.mNorm.mX * dist * step;
		v1.mY += plane.mNorm.mY * dist * step;
	}
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::NudgeRect ( const USPlane2D& plane, float step, USRect& rect ) {
	
	if ( plane.mNorm.mY == 0.0f ) {
		if ( plane.mNorm.mX > 0.0f ) {
			
			// normal facing right
			float dist = -plane.mDist;
			if ( rect.mXMin < dist ) rect.mXMin += ( dist - rect.mXMin ) * step;
			if ( rect.mXMax < dist ) rect.mXMax += ( dist - rect.mXMax ) * step;
			return;
		}
		else {
			
			// normal facing left
			float dist = plane.mDist;
			if ( rect.mXMin > dist ) rect.mXMin += ( dist - rect.mXMin ) * step;
			if ( rect.mXMax > dist ) rect.mXMax += ( dist - rect.mXMax ) * step;
			return;
		}
	}
	
	if ( plane.mNorm.mX == 0.0f ) {
		if ( plane.mNorm.mY > 0.0f ) {
			
			// normal facing up
			float dist = -plane.mDist;
			if ( rect.mYMin < dist ) rect.mYMin += ( dist - rect.mYMin ) * step;
			if ( rect.mYMax < dist ) rect.mYMax += ( dist - rect.mYMax ) * step;
			return;
		}
		else {
			
			// normal facing down
			float dist = plane.mDist;
			if ( rect.mYMin > dist ) rect.mYMin += ( dist - rect.mYMin ) * step;
			if ( rect.mYMax > dist ) rect.mYMax += ( dist - rect.mYMax ) * step;
			return;
		}
	}
	
	USVec2D v0;
	USVec2D v1;
	
	if ( plane.mNorm.mY > 0.0f ) {
		if ( plane.mNorm.mX > 0.0f ) {
		
			v0.Init ( rect.mXMin, rect.mYMin );
			v1.Init ( rect.mXMax, rect.mYMax );
			this->NudgeLine ( plane, step, v0, v1 );
			rect.Init ( v0.mX, v0.mY, v1.mX, v1.mY );
		}
		else {
		
			v0.Init ( rect.mXMax, rect.mYMin );
			v1.Init ( rect.mXMin, rect.mYMax );
			this->NudgeLine ( plane, step, v0, v1 );
			rect.Init ( v1.mX, v0.mY, v0.mX, v1.mY );
		}
	}
	else {
		if ( plane.mNorm.mX < 0.0f ) {
		
			v0.Init ( rect.mXMin, rect.mYMin );
			v1.Init ( rect.mXMax, rect.mYMax );
			this->NudgeLine ( plane, step, v0, v1 );
			rect.Init ( v0.mX, v0.mY, v1.mX, v1.mY );
		}
		else {
			
			v0.Init ( rect.mXMax, rect.mYMin );
			v1.Init ( rect.mXMin, rect.mYMax );
			this->NudgeLine ( plane, step, v0, v1 );
			rect.Init ( v1.mX, v0.mY, v0.mX, v1.mY );
		}
	}
	
	/*
	vtx.Init ( rect.mXMin, rect.mYMin );
	dist = USDist::PointToPlane2D ( vtx, plane );
	if ( dist < 0.0f ) {
		
		vtx.mX = plane.mNorm.mX * -dist;
		vtx.mY = plane.mNorm.mY * -dist;
		
		rect.mXMin = vtx.mX;
		rect.mYMin = vtx.mY;
	}
	
	vtx.Init ( rect.mXMin, rect.mYMax );
	dist = USDist::PointToPlane2D ( vtx, plane );
	if ( dist < 0.0f ) {
		
		vtx.mX = plane.mNorm.mX * -dist;
		vtx.mY = plane.mNorm.mY * -dist;
		
		rect.mXMin = vtx.mX;
		rect.mYMax = vtx.mY;
	}
	
	vtx.Init ( rect.mXMax, rect.mYMax );
	dist = USDist::PointToPlane2D ( vtx, plane );
	if ( dist < 0.0f ) {
		
		vtx.mX = plane.mNorm.mX * -dist;
		vtx.mY = plane.mNorm.mY * -dist;
		
		rect.mXMax = vtx.mX;
		rect.mYMax = vtx.mY;
	}
	
	vtx.Init ( rect.mXMax, rect.mYMin );
	dist = USDist::PointToPlane2D ( vtx, plane );
	if ( dist < 0.0f ) {
		
		vtx.mX = plane.mNorm.mX * -dist;
		vtx.mY = plane.mNorm.mY * -dist;
		
		rect.mXMax = vtx.mX;
		rect.mYMin = vtx.mY;
	}
	*/
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
	
	USRect targetRect = screenRect;
	
	// Get all the planes for the bounding rect
	USQuad boundsQuad;
	boundsQuad.Init ( this->mBounds.mXMin, this->mBounds.mYMin, this->mBounds.mXMax, this->mBounds.mYMax );
	boundsQuad.Transform ( worldToWnd );
	boundsQuad.ReverseWinding ();
	
	USPlane2D planes [ 4 ];
	
	planes [ 0 ] = boundsQuad.GetPlane ( 0 );
	planes [ 1 ] = boundsQuad.GetPlane ( 1 );
	planes [ 2 ] = boundsQuad.GetPlane ( 2 );
	planes [ 3 ] = boundsQuad.GetPlane ( 3 );
	
	planes [ 0 ].Flip ();
	planes [ 1 ].Flip ();
	planes [ 2 ].Flip ();
	planes [ 3 ].Flip ();
	
	// fit the rect into the bounds
	for ( u32 i = 0; i < 8; ++i ) {
		this->NudgeRect ( planes [ 0 ], 0.5f, targetRect );
		this->NudgeRect ( planes [ 1 ], 0.5f, targetRect );
		this->NudgeRect ( planes [ 2 ], 0.5f, targetRect );
		this->NudgeRect ( planes [ 3 ], 0.5f, targetRect );
	}
	
	this->NudgeRect ( planes [ 0 ], 1.0f, targetRect );
	this->NudgeRect ( planes [ 1 ], 1.0f, targetRect );
	this->NudgeRect ( planes [ 2 ], 1.0f, targetRect );
	this->NudgeRect ( planes [ 3 ], 1.0f, targetRect );
	
	// see how much space we have around target rect
	
	// TODO: fix me
	float xMax = 100000.0f;
	float xMin = -100000.0f;
	float yMax = 100000.0f;
	float yMin = -100000.0f;
	
	for ( u32 i = 0; i < 4; ++i ) {
		
		float x;
		float y;
		
		USPlane2D& plane = planes [ i ];
		
		if ( USSect::XAxisToPlane ( targetRect.mYMin, planes [ i ], x ) == USSect::SECT_HIT ) {
			if (( plane.mNorm.mX < 0.0f ) && ( x < xMax )) xMax = x;
			if (( plane.mNorm.mX > 0.0f ) && ( x > xMin )) xMin = x;
		}
		
		if ( USSect::XAxisToPlane ( targetRect.mYMax, planes [ i ], x ) == USSect::SECT_HIT ) {
			if (( plane.mNorm.mX < 0.0f ) && ( x < xMax )) xMax = x;
			if (( plane.mNorm.mX > 0.0f ) && ( x > xMin )) xMin = x;
		}
		
		if ( USSect::YAxisToPlane ( targetRect.mXMin, planes [ i ], y ) == USSect::SECT_HIT ) {
			if (( plane.mNorm.mY < 0.0f ) && ( y < yMax )) yMax = y;
			if (( plane.mNorm.mY > 0.0f ) && ( y > yMin )) yMin = y;
		}
		
		if ( USSect::YAxisToPlane ( targetRect.mXMax, planes [ i ], y ) == USSect::SECT_HIT ) {
			if (( plane.mNorm.mY < 0.0f ) && ( y < yMax )) yMax = y;
			if (( plane.mNorm.mY > 0.0f ) && ( y > yMin )) yMin = y;
		}
	}
	
	// fit the viewport rect to the target rect
	USRect viewport = this->mViewport->GetRect ();
	
	float targetAspect = targetRect.Aspect ();
	float viewAspect = viewport.Aspect ();
	
	float fitWidth = 0.0f;
	float fitHeight = 0.0f;
	
	if ( viewAspect <= targetAspect ) {
		// target is fatter than view
		fitWidth = targetRect.Width ();
		fitHeight = fitWidth / viewAspect;
	}
	else {
	}
	
	// resive the view rect
	viewport.mXMin = (( targetRect.mXMin + ( targetRect.Width () * 0.5f )) - ( fitWidth * 0.5f ));
	viewport.mYMin = (( targetRect.mYMin + ( targetRect.Height () * 0.5f )) - ( fitHeight * 0.5f ));
	
	viewport.mXMax = viewport.mXMin + fitWidth;
	viewport.mYMax = viewport.mYMin + fitHeight;
	
	if ( viewAspect <= targetAspect ) {
		
		if ( viewport.mYMin < yMin ) {
			float offset = yMin - viewport.mYMin;
			viewport.Offset ( 0.0f, offset );
		}
		
		if ( viewport.mYMax > yMax ) {
			float offset = yMax - viewport.mYMax;
			viewport.Offset ( 0.0f, offset );
		}
	}
	else {
	}
	
	// since aspect stayed constant, can use either width or height to fit
	this->mTargetScale *= viewport.Width () / this->mViewport->Width ();

	// TODO: take viewport offset into account
	viewport.GetCenter ( this->mTargetLoc );
	wndToWorld.Transform ( this->mTargetLoc );
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

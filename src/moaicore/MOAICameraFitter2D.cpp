// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAICameraAnchor2D.h>
#include <moaicore/MOAICameraFitter2D.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITransform.h>
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
int MOAICameraFitter2D::_setCamera ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	MOAITransform* camera = state.GetLuaObject < MOAITransform >( 2 );
	self->mCamera = camera;

	return 0;
}

//----------------------------------------------------------------//
int MOAICameraFitter2D::_setDamper ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	self->mDamper = state.GetValue < float >( 2, 0.0f );
	
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

//----------------------------------------------------------------//
int MOAICameraFitter2D::_snapToTarget ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	MOAITransform* camera = state.GetLuaObject < MOAITransform >( 2 );
	if ( camera ) {
		self->SnapToTarget ( *camera );
	}
	else {
		if ( self->mCamera ) {
			self->SnapToTarget ( *self->mCamera );
		}
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
void MOAICameraFitter2D::Clear () {

	while ( this->mAnchors.size ()) {
		AnchorIt anchorIt = this->mAnchors.begin ();
		MOAICameraAnchor2D* anchor = *anchorIt;
		this->mAnchors.erase ( anchorIt );
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

	USAffine2D ident;
	ident.Ident ();
	
	USAffine2D worldToWnd;
	USCanvas::GetWorldToWndMtx ( *this->mViewport, ident, worldToWnd );
	
	USAffine2D wndToWorld;
	USCanvas::GetWndToWorldMtx ( *this->mViewport, ident, wndToWorld );
	
	USRect screenRect = worldRect;
	worldToWnd.Transform ( screenRect );
	
	// fit the viewport rect to the target rect
	USRect viewport = this->mViewport->GetRect ();
	screenRect.FitOutside ( viewport );
	
	// since aspect stayed constant, can use either width or height to fit
	this->mTargetScale *= viewport.Width () / this->mViewport->Width ();

	// TODO: take viewport offset into account
	
	// take the viewport back into world space
	wndToWorld.Transform ( viewport );
	
	viewport.Bless ();
	this->mBounds.Constrain ( viewport );
	
	viewport.GetCenter ( this->mTargetLoc );
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::GetCamera ( USAffine2D& camera ) {

	camera.ScRoTr ( this->mTargetScale, this->mTargetScale, 0.0f, this->mTargetLoc.mX, this->mTargetLoc.mY );
}

//----------------------------------------------------------------//
USRect MOAICameraFitter2D::GetWorldRect () {

	// build the world rect
	AnchorIt anchorIt = this->mAnchors.begin ();
	USRect worldRect = ( *anchorIt )->GetRect ();
	++anchorIt;
	
	for ( ; anchorIt != this->mAnchors.end (); ++anchorIt ) {
		MOAICameraAnchor2D* anchor = *anchorIt;
		worldRect.Grow ( anchor->GetRect ());
	}
	
	this->mBounds.Clip ( worldRect );
	
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
	
	return worldRect;
}

//----------------------------------------------------------------//
bool MOAICameraFitter2D::IsDone () {

	return false;
}

//----------------------------------------------------------------//
MOAICameraFitter2D::MOAICameraFitter2D () :
	mMin ( 0.0f ),
	mDamper ( 0.0f ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAction )
		RTTI_EXTEND ( MOAINode )
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
	
	if ( this->mCamera ) {
		
		float d = 1.0f - USFloat::Clamp ( this->mDamper, 0.0f, 1.0f );
		
		USVec2D loc = this->mCamera->GetLoc ();
		float scale = this->mCamera->GetScl ().mX;
		
		loc.mX += ( this->mTargetLoc.mX - loc.mX ) * d;
		loc.mY += ( this->mTargetLoc.mY - loc.mY ) * d;
		scale += ( this->mTargetScale - scale ) * d;
		
		USVec2D scaleVec;
		scaleVec.Init ( scale, scale );
		this->mCamera->SetScl ( scaleVec );
		this->mCamera->SetLoc ( loc );
		this->mCamera->ScheduleUpdate ();
	}
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
void MOAICameraFitter2D::SnapToTarget ( MOAITransform& camera ) {
	
	camera.SetLoc ( this->mTargetLoc );
	
	USVec2D scaleVec;
	scaleVec.Init ( this->mTargetScale, this->mTargetScale );
	camera.SetScl ( scaleVec );
	
	camera.ScheduleUpdate ();
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::RegisterLuaClass ( USLuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
	MOAINode::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::RegisterLuaFuncs ( USLuaState& state ) {

	MOAIAction::RegisterLuaFuncs ( state );
	MOAINode::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "clear",				_clear },
		{ "insertAnchor",		_insertAnchor },
		{ "removeAnchor",		_removeAnchor },
		{ "setBounds",			_setBounds },
		{ "setCamera",			_setCamera },
		{ "setDamper",			_setDamper },
		{ "setMin",				_setMin },
		{ "setViewport",		_setViewport },
		{ "snapToTarget",		_snapToTarget },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
STLString MOAICameraFitter2D::ToString () {

	STLString repr( MOAIAction::ToString ());
	return repr;
}

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAICameraAnchor2D.h>
#include <moaicore/MOAICameraFitter2D.h>
#include <moaicore/MOAIGfxDevice.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAITransform.h>
#include <moaicore/MOAIViewport.h>

//----------------------------------------------------------------//
/**	@name	clear
	@text	Remove all camera anchors from the fitter.
	
	@in		MOAICameraFitter2D self
	@out	nil
*/
int MOAICameraFitter2D::_clear ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	self->Clear ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	getFitDistance
	@text	Returns the distance between the camera's current x, y, scale and
			the target x, y, scale. As the camera approaches its target, the
			distance approaches 0. Check the value returned by this function
			against a small epsilon value.
	
	@in		MOAICameraFitter2D self
	@out	number distance
*/
int MOAICameraFitter2D::_getFitDistance ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	float distance = self->GetFitDistance ();
	lua_pushnumber ( state, distance );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	insertAnchor
	@text	Add an anchor to the fitter.
	
	@in		MOAICameraFitter2D self
	@in		MOAICameraAnchor2D anchor
	@out	nil
*/
int MOAICameraFitter2D::_insertAnchor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "UU" )
	
	MOAICameraAnchor2D* anchor = state.GetLuaObject < MOAICameraAnchor2D >( 2 );
	if ( anchor ) {
		self->AddAnchor ( *anchor );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	removeAnchor
	@text	Remove an anchor from the fitter.
	
	@in		MOAICameraFitter2D self
	@in		MOAICameraAnchor2D anchor
	@out	nil
*/
int MOAICameraFitter2D::_removeAnchor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "UU" )
	
	MOAICameraAnchor2D* anchor = state.GetLuaObject < MOAICameraAnchor2D >( 2 );
	if ( anchor ) {
		self->RemoveAnchor ( *anchor );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setBounds
	@text	Set the world bounds of the fitter. The camera will not
			move outside of the fitter's bounds.
	
	@in		MOAICameraFitter2D self
	@in		number xMin
	@in		number yMin
	@in		number xMax
	@in		number yMax
	@out	nil
*/
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
/**	@name	setCamera
	@text	Set a MOAITransform for the fitter to use as a camera.
			The fitter will dynamically change the location and
			scale of the camera to keep all of the anchors on the
			screen.
	
	@in		MOAICameraFitter2D self
	@in		MOAITransform camera		Default value is nil.
	@out	nil
*/
int MOAICameraFitter2D::_setCamera ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	MOAITransform* camera = state.GetLuaObject < MOAITransform >( 2 );
	self->mCamera = camera;

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setDamper
	@text	Set's the fitter's damper coefficient. This is a scalar
			applied to the difference between the camera transform's
			location and the fitter's target location every frame.
			The smaller the coefficient, the tighter the fit will be.
			A value of '0' will not dampen at all; a value of '1' will
			never move the camera.
	
	@in		MOAICameraFitter2D self
	@opt	number damper		Default value is 0.
	@out	nil
*/
int MOAICameraFitter2D::_setDamper ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	self->mDamper = state.GetValue < float >( 2, 0.0f );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMin
	@text	Set the minimum number of world units to be displayed by
			the camera along either axis.
	
	@in		MOAICameraFitter2D self
	@opt	number min					Default value is 0.
	@out	nil
*/
int MOAICameraFitter2D::_setMin ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	self->mMin = state.GetValue < float >( 2, 0.0f );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setViewport
	@text	Set the viewport to be used for fitting.
	
	@in		MOAICameraFitter2D self
	@opt	MOAIViewport viewport			Default value is nil.
	@out	nil
*/
int MOAICameraFitter2D::_setViewport ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	self->mViewport = state.GetLuaObject < MOAIViewport >( 2 );
	if ( self->mViewport ) {
		self->mViewRect = self->mViewport->GetRect ();
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	snapToTarget
	@text	Snap the camera to the target fitting.
	
	@overload	Snap the fitter's camera transform to the target.
	
		@in		MOAICameraFitter2D self
		@out	nil
	
	@overload	Snap a passed in transform to the target.
	
		@in		MOAICameraFitter2D self
		@in		MOAITransform
		@out	nil
*/
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
		this->LuaRetain ( anchor );
		this->mAnchors.insert ( &anchor );
	}
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::Clear () {

	while ( this->mAnchors.size ()) {
		AnchorIt anchorIt = this->mAnchors.begin ();
		MOAICameraAnchor2D* anchor = *anchorIt;
		this->mAnchors.erase ( anchorIt );
		this->LuaRelease ( *anchor );
	} 
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::Fit () {

	if ( !this->mAnchors.size ()) return;
	if ( !this->mViewport ) return;

	// reset the fitter
	this->mTargetLoc.Init ( 0.0f, 0.0f );
	this->mTargetScale = 1.0f;

	USAffine2D ident;
	ident.Ident ();
	
	USAffine2D worldToWnd = this->mViewport->GetWorldToWndMtx ( ident );
	USAffine2D wndToWorld = this->mViewport->GetWndToWorldMtx ( ident );
	
	// take the view rect into world space
	USRect worldViewRect = this->mViewport->GetRect ();
	wndToWorld.Transform ( worldViewRect );
	worldViewRect.Bless ();
	
	// TODO: take viewport offset into account
	
	// build the anchor rect (clipped to bounds)
	USRect anchorRect = this->GetAnchorRect ();
	
	// fit the view rect around the world rect while preserving aspect ratio
	USRect fitViewRect = worldViewRect;
	anchorRect.FitOutside ( fitViewRect );
	
	// constrain the view rect to the bounds while preserving aspect
	this->mBounds.ConstrainWithAspect ( fitViewRect );
	
	// get the fitting
	this->mTargetScale *= fitViewRect.Width () / worldViewRect.Width ();	
	fitViewRect.GetCenter ( this->mTargetLoc );
}

//----------------------------------------------------------------//
USRect MOAICameraFitter2D::GetAnchorRect () {

	// expand the world rect to include all the anchors
	AnchorIt anchorIt = this->mAnchors.begin ();
	USRect worldRect = ( *anchorIt )->GetRect ();
	++anchorIt;
	
	for ( ; anchorIt != this->mAnchors.end (); ++anchorIt ) {
		MOAICameraAnchor2D* anchor = *anchorIt;
		worldRect.Grow ( anchor->GetRect ());
	}
	
	// clip the world rect to the bounds
	this->mBounds.Clip ( worldRect );
	
	// enforce the minimum
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
void MOAICameraFitter2D::GetCamera ( USAffine2D& camera ) {

	camera.ScRoTr ( this->mTargetScale, this->mTargetScale, 0.0f, this->mTargetLoc.mX, this->mTargetLoc.mY );
}

//----------------------------------------------------------------//
float MOAICameraFitter2D::GetFitDistance () {

	if ( this->mCamera ) {

		USVec2D loc = this->mCamera->GetLoc ();
		float scale = this->mCamera->GetScl ().mX;

		USVec3D current ( loc.mX, loc.mY, scale );
		USVec3D target ( this->mTargetLoc.mX, this->mTargetLoc.mY, this->mTargetScale );
		
		return USDist::VecToVec ( current, target );
	}
	return 0.0f;
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

	printf ( "~MOAICameraFitter2D ()\n" );

	// do not have to remove objects here as no objects are reference counted
	// trying to force removal will result in crash during Lua shutdown
	// since objects are only referenced by Lua table, Lua's GC will handle cleanup
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
	
	// make sure all the anchors are ahead of fitter in the update schedule
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
		this->LuaRelease ( anchor );
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
		{ "getFitDistance",		_getFitDistance },
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

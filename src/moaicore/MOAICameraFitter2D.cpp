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
/**	@name	clearAnchors
	@text	Remove all camera anchors from the fitter.
	
	@in		MOAICameraFitter2D self
	@out	nil
*/
int MOAICameraFitter2D::_clearAnchors ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	self->Clear ();

	return 0;
}

//----------------------------------------------------------------//
/**	@name	clearFitMode
	@text	Clears bits in the fitting mask.
	
	@in		MOAICameraFitter2D self
	@opt	number mask		Default value is FITTING_MODE_MASK
	@out	nil
*/
int MOAICameraFitter2D::_clearFitMode( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	u32 mask = state.GetValue < u32 >( 2, FITTING_MODE_MASK );
	
	self->mFittingMode &= ~mask;

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
/**	@name	getFitLoc
	@text	Get the fitter location.
	
	@in		MOAICameraFitter2D self
	@out	number x
	@out	number y
*/
int MOAICameraFitter2D::_getFitLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )

	lua_pushnumber ( state, self->mFitLoc.mX );
	lua_pushnumber ( state, self->mFitLoc.mY );
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getFitMode
	@text	Gets bits in the fitting mask.
	
	@in		MOAICameraFitter2D self
	@out	number mask
*/
int MOAICameraFitter2D::_getFitMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	state.Push ( self->mFittingMode );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getFitScale
	@text	Returns the fit scale
	
	@in		MOAICameraFitter2D self
	@out	number scale
*/
int MOAICameraFitter2D::_getFitScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )

	lua_pushnumber ( state, self->mFitScale );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getTargetLoc
 @text	Get the target location.
 
 @in	MOAICameraFitter2D self
 @out	number x
 @out	number y
 */
int MOAICameraFitter2D::_getTargetLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	lua_pushnumber ( state, self->mTargetLoc.mX );
	lua_pushnumber ( state, self->mTargetLoc.mY );
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getTargetScale
 @text	Returns the target scale
 
 @in	MOAICameraFitter2D self
 @out	number scale
 */
int MOAICameraFitter2D::_getTargetScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	lua_pushnumber ( state, self->mTargetScale );
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
	
	MOAICameraAnchor2D* anchor = state.GetLuaObject < MOAICameraAnchor2D >( 2, true );
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
	
	MOAICameraAnchor2D* anchor = state.GetLuaObject < MOAICameraAnchor2D >( 2, true );
	if ( anchor ) {
		self->RemoveAnchor ( *anchor );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setBounds
	@text	Sets or clears the world bounds of the fitter. The camera will not
			move outside of the fitter's bounds.
	
	@overload
	
		@in		MOAICameraFitter2D self
		@in		number xMin
		@in		number yMin
		@in		number xMax
		@in		number yMax
		@out	nil
	
	@overload
	
		@in		MOAICameraFitter2D self
		@out	nil
*/
int MOAICameraFitter2D::_setBounds ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )

	if ( state.CheckParams ( 2, "NNNN" )) {

		float x0	= state.GetValue < float >( 2, 0.0f );
		float y0	= state.GetValue < float >( 3, 0.0f );
		float x1	= state.GetValue < float >( 4, 0.0f );
		float y1	= state.GetValue < float >( 5, 0.0f );
		
		self->mBounds.Init ( x0, y0, x1, y1 );
		self->mFittingMode |= FITTING_MODE_APPLY_BOUNDS;
	}
	else {
	
		self->mFittingMode &= ~FITTING_MODE_APPLY_BOUNDS;
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setCamera
	@text	Set a MOAITransform for the fitter to use as a camera.
			The fitter will dynamically change the location and
			scale of the camera to keep all of the anchors on the
			screen.
	
	@in		MOAICameraFitter2D self
	@opt	MOAITransform camera		Default value is nil.
	@out	nil
*/
int MOAICameraFitter2D::_setCamera ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	self->mCamera.Set ( *self, state.GetLuaObject < MOAITransform >( 2, true ));
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
/**	@name	setFitLoc
	@text	Set the fitter's location.
	
	@in		MOAICameraFitter2D self
	@opt	number x		Default value is 0.
	@opt	number y		Default value is 0.
	@opt	boolean snap	Default value is false.
	@out	nil
*/
int MOAICameraFitter2D::_setFitLoc ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )

	self->mFitLoc.mX = state.GetValue < float >( 2, 0.0f );
	self->mFitLoc.mY = state.GetValue < float >( 3, 0.0f );
	
	self->mFittingMode &= ~FITTING_MODE_APPLY_ANCHORS;
	self->UpdateTarget ();
	
	bool snap = state.GetValue < bool >( 4, false );
	if ( snap && self->mCamera ) {
		self->SnapToTargetLoc ( *self->mCamera );
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFitMode
	@text	Sets bits in the fitting mask.
	
	@in		MOAICameraFitter2D self
	@opt	number mask		Default value is FITTING_MODE_DEFAULT
	@out	nil
*/
int MOAICameraFitter2D::_setFitMode ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )
	
	self->mFittingMode |= state.GetValue < u32 >( 2, FITTING_MODE_DEFAULT );
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFitScale
	@text	Set the fitter's scale.
	
	@in		MOAICameraFitter2D self
	@opt	number scale	Default value is 1.
	@opt	boolean snap	Default value is false.
	@out	nil
*/
int MOAICameraFitter2D::_setFitScale ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICameraFitter2D, "U" )

	self->mFitScale = state.GetValue < float >( 2, 1.0f );
	
	self->mFittingMode &= ~FITTING_MODE_APPLY_ANCHORS;
	self->UpdateTarget ();
	
	bool snap = state.GetValue < bool >( 3, false );
	if ( snap && self->mCamera ) {
		self->SnapToTargetScale ( *self->mCamera );
	}
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
	
	self->mViewport.Set ( *self, state.GetLuaObject < MOAIViewport >( 2, true ));
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
	
	MOAITransform* camera = state.GetLuaObject < MOAITransform >( 2, true );
	if ( camera ) {
		self->SnapToTargetLoc ( *camera );
		self->SnapToTargetScale ( *camera );
	}
	else {
		if ( self->mCamera ) {
			self->SnapToTargetLoc ( *self->mCamera );
			self->SnapToTargetScale ( *self->mCamera );
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
		this->LuaRetain ( &anchor );
		this->mAnchors.insert ( &anchor );
	}
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::Clear () {

	while ( this->mAnchors.size ()) {
		AnchorIt anchorIt = this->mAnchors.begin ();
		MOAICameraAnchor2D* anchor = *anchorIt;
		this->mAnchors.erase ( anchorIt );
		this->LuaRelease ( anchor );
	}
	
	this->mCamera.Set ( *this, 0 );
	this->mViewport.Set ( *this, 0 );
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
	if ( this->mFittingMode & FITTING_MODE_APPLY_BOUNDS ) {
		this->mBounds.Clip ( worldRect );
	}
	
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
void MOAICameraFitter2D::GetCamera ( USAffine3D& camera ) {

	camera.ScRoTr ( this->mTargetScale, this->mTargetScale, 1.0f, 0.0f, 0.0f, 0.0f, this->mTargetLoc.mX, this->mTargetLoc.mY, 0.0f );
}

//----------------------------------------------------------------//
float MOAICameraFitter2D::GetFitDistance () {

	if ( this->mCamera ) {

		USVec3D loc = this->mCamera->GetLoc ();
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
	mDamper ( 0.0f ),
	mFittingMode ( FITTING_MODE_DEFAULT ) {

	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAction )
		RTTI_EXTEND ( MOAINode )
	RTTI_END
	
	this->mFitLoc.Init ( 0.0f, 0.0f, 0.0f );
	this->mFitScale = 1.0f;
	
	this->mTargetLoc.Init ( 0.0f, 0.0f, 0.0f );
	this->mTargetScale = 1.0f;
}

//----------------------------------------------------------------//
MOAICameraFitter2D::~MOAICameraFitter2D () {

	this->Clear ();
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::OnDepNodeUpdate () {

	this->UpdateFit ();
	this->UpdateTarget ();
	
	if ( this->mCamera ) {
		
		float d = 1.0f - USFloat::Clamp ( this->mDamper, 0.0f, 1.0f );
		
		USVec3D loc = this->mCamera->GetLoc ();
		float scale = this->mCamera->GetScl ().mX;
		
		loc.mX += ( this->mTargetLoc.mX - loc.mX ) * d;
		loc.mY += ( this->mTargetLoc.mY - loc.mY ) * d;
		scale += ( this->mTargetScale - scale ) * d;
		
		USVec3D scaleVec;
		scaleVec.Init ( scale, scale, 1.0f );
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
		this->LuaRelease ( &anchor );
	}
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::SnapToTargetLoc ( MOAITransform& camera ) {
	
	camera.SetLoc ( this->mTargetLoc );
	
	camera.ScheduleUpdate ();
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::SnapToTargetScale ( MOAITransform& camera ) {
	
	USVec3D scaleVec;
	scaleVec.Init ( this->mTargetScale, this->mTargetScale, 1.0f );
	camera.SetScl ( scaleVec );
	
	camera.ScheduleUpdate ();
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::UpdateFit () {

	if ( !( this->mFittingMode & FITTING_MODE_APPLY_ANCHORS )) return;
	if ( !this->mAnchors.size ()) return;
	if ( !this->mViewport ) return;

	// reset the fitter
	this->mFitLoc.Init ( 0.0f, 0.0f, 0.0f );
	this->mFitScale = 1.0f;

	// grab the view transform
	USMatrix4x4 ident;
	ident.Ident ();
	USMatrix4x4 wndToWorld = this->mViewport->GetWndToWorldMtx ( ident );

	// grab the view rect in world space
	// TODO: take viewport offset into account
	USRect worldViewRect = this->mViewport->GetRect ();
	wndToWorld.Transform ( worldViewRect );
	worldViewRect.Bless ();
	
	// build the anchor rect (clipped to bounds)
	USRect anchorRect = this->GetAnchorRect ();
	
	// fit the view rect around the target rect while preserving aspect ratio
	USRect fitViewRect = worldViewRect;
	anchorRect.FitOutside ( fitViewRect );
	
	// get the fitting
	this->mFitScale = fitViewRect.Width () / worldViewRect.Width ();	
	fitViewRect.GetCenter ( this->mFitLoc );
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::UpdateTarget () {

	if ( !this->mViewport ) return;

	// reset the fitter
	this->mTargetLoc = this->mFitLoc;
	this->mTargetScale = this->mFitScale;

	// clamp to bounds
	if ( this->mFittingMode & FITTING_MODE_APPLY_BOUNDS ) {
	
		// grab the view transform
		USMatrix4x4 ident;
		ident.Ident ();
		USMatrix4x4 wndToWorld = this->mViewport->GetWndToWorldMtx ( ident );

		// grab the view rect in world space
		// TODO: take viewport offset into account
		USRect worldViewRect = this->mViewport->GetRect ();
		wndToWorld.Transform ( worldViewRect );
		worldViewRect.Bless ();
		
		// get the camera's target position and scale
		USAffine3D cameraMtx;
		float rot = this->mCamera ? this->mCamera->GetRot ().mZ : 0.0f;
		cameraMtx.ScRoTr ( this->mFitScale, this->mFitScale, 1.0f, 0.0f, 0.0f, rot * ( float )D2R, this->mFitLoc.mX, this->mFitLoc.mY, 0.0f );
		
		// get the camera rect
		USRect cameraRect = worldViewRect;
		cameraMtx.Transform ( cameraRect );
		cameraRect.Bless ();
		
		this->mBounds.ConstrainWithAspect ( cameraRect );
		
		// get the fitting
		this->mTargetScale = cameraRect.Width () / worldViewRect.Width ();	
		cameraRect.GetCenter ( this->mTargetLoc );
	}
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
	MOAINode::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAICameraFitter2D::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAction::RegisterLuaFuncs ( state );
	MOAINode::RegisterLuaFuncs ( state );
	
	state.SetField ( -1, "FITTING_MODE_SEEK_LOC", ( u32 )FITTING_MODE_SEEK_LOC );
	state.SetField ( -1, "FITTING_MODE_SEEK_SCALE", ( u32 )FITTING_MODE_SEEK_SCALE );
	state.SetField ( -1, "FITTING_MODE_APPLY_ANCHORS", ( u32 )FITTING_MODE_APPLY_ANCHORS );
	state.SetField ( -1, "FITTING_MODE_APPLY_BOUNDS", ( u32 )FITTING_MODE_APPLY_BOUNDS );
	state.SetField ( -1, "FITTING_MODE_DEFAULT", ( u32 )FITTING_MODE_DEFAULT );
	state.SetField ( -1, "FITTING_MODE_MASK", ( u32 )FITTING_MODE_MASK );
	
	luaL_Reg regTable [] = {
		{ "clearAnchors",		_clearAnchors },
		{ "clearFitMode",		_clearFitMode },
		{ "getFitDistance",		_getFitDistance },
		{ "getFitLoc",			_getFitLoc },
		{ "getFitMode",			_getFitMode },
		{ "getFitScale",		_getFitScale },
		{ "getTargetLoc",		_getTargetLoc },
		{ "getTargetScale",		_getTargetScale },
		{ "insertAnchor",		_insertAnchor },
		{ "removeAnchor",		_removeAnchor },
		{ "setBounds",			_setBounds },
		{ "setCamera",			_setCamera },
		{ "setDamper",			_setDamper },
		{ "setFitLoc",			_setFitLoc },
		{ "setFitMode",			_setFitMode },
		{ "setFitScale",		_setFitScale },
		{ "setMin",				_setMin },
		{ "setViewport",		_setViewport },
		{ "snapToTarget",		_snapToTarget },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

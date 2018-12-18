// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIDebugLines.h>
#include <moai-sim/MOAIDraw.h>
#include <moai-sim/MOAIGfxMgr.h>
#include <moai-sim/MOAIViewport.h>

#define DEFAULT_HFOV			60.0f
#define DEFAULT_NEAR_PLANE		1.0f
#define DEFAULT_FAR_PLANE		10000.0f

#define RETICLE_RADIUS			0.125f

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getFarPlane
	@text	Returns the camera's far plane.

	@in		MOAICamera self
	@out	number far
*/
int MOAICamera::_getFarPlane ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )
	lua_pushnumber ( state, self->mFarPlane );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getFieldOfView
	@text	Returns the camera's horizontal field of view.

	@in		MOAICamera self
	@out	number hfov
*/
int MOAICamera::_getFieldOfView ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )
	lua_pushnumber ( state, self->mFieldOfView );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getFloorMove
	@text	Project the given X, Y offset onto the floor (XY plane). Projected
			X, Y will be rotated to match camera's orientation. This is for
			implementing a truck along the floor plane relative to the
			camera's orientation.

	@in		MOAICamera self
	@in		number x
	@in		number y
	@out	number x
	@out	number y
*/
int MOAICamera::_getFloorMove ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )

	float x = state.GetValue < float >( 2, 0.0f );
	float y = state.GetValue < float >( 3, 0.0f );

	const ZLAffine3D& mtx = self->GetLocalToWorldMtx ();
	
	ZLVec3D v;
	ZLVec3D h;
	
	v = mtx.GetZAxis ();
	v.Scale ( -1.0f );
	v.mZ = 0.0f;
	
	if ( v.NormSafe () == 0.0f ) {
		v = mtx.GetYAxis ();
	}
	
	// TODO: double check this
	ZLVec2D r ( v.mX, v.mY );
	r.Rotate90Anticlockwise ();
	h.Init ( r.mX, r.mY, 0.0f );
	
	h.Scale ( x );
	v.Scale ( y );

	ZLVec3D m = h;
	m.Add ( v );

	lua_pushnumber ( state, m.mX );
	lua_pushnumber ( state, m.mY );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getFocalLength
	@text	Returns the camera's focal length given the width of
			the view plane.

	@in		MOAICamera self
	@in		number width
	@out	number length
*/
int MOAICamera::_getFocalLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "UN" )

	float width = state.GetValue < float >( 2, 0.0f );
	lua_pushnumber ( state, self->GetFocalLength ( width ));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getNearPlane
	@text	Returns the camera's near plane.

	@in		MOAICamera self
	@out	number near
*/
int MOAICamera::_getNearPlane ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )
	lua_pushnumber ( state, self->mNearPlane );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getViewVector
	@text	Get the camera's normalized view vector (i.e. the Z axis).

	@in		MOAICamera self
	@out	number xN
	@out	number yN
	@out	number zN
*/
int MOAICamera::_getViewVector ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )
	
	self->ForceUpdate ();
	ZLVec3D viewVec = self->GetViewVector ();
	
	state.Push ( viewVec );
	
	return 3;
}

//----------------------------------------------------------------//
/**	@lua	lookAt
	@text	Point the camera at a given point in space.

	@in		MOAICamera self
	@in		number x
	@in		number y
	@in		number z
	@out	nil
*/
int MOAICamera::_lookAt ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )

	float x		= state.GetValue < float >( 2, 0.0f );
	float y		= state.GetValue < float >( 3, 0.0f );
	float z		= state.GetValue < float >( 4, 0.0f );

	self->LookAt ( x, y, z );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	moveFieldOfView
	@text	Animate the camera's FOV.

	@in		MOAICamera self
	@in		number fov
	@in		number delay
	@out	nil
*/
int MOAICamera::_moveFieldOfView ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )
	
	float delay		= state.GetValue < float >( 3, 0.0f );
	
	if ( delay > 0.0f ) {
	
		u32 mode = state.GetValue < u32 >( 4, ZLInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForMove ( state, 2, self, 1, mode,
			MOAICameraAttr::Pack ( ATTR_FOV ), 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ( 0, false );
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mFieldOfView += state.GetValue < float >( 2, 0.0f );
	self->ScheduleUpdate ();

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	seekFieldOfView
	@text	Animate the camera's FOV.

	@in		MOAICamera self
	@in		number fov
	@in		number delay
	@out	nil
*/
int MOAICamera::_seekFieldOfView ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )

	float delay		= state.GetValue < float >( 3, 0.0f );
	
	if ( delay > 0.0f ) {

		u32 mode = state.GetValue < u32 >( 4, ZLInterpolate::kSmooth );
		
		MOAIEaseDriver* action = new MOAIEaseDriver ();
		
		action->ParseForSeek ( state, 2, self, 1, mode,
			MOAICameraAttr::Pack ( ATTR_FOV ), self->mFieldOfView, 0.0f
		);
		
		action->SetSpan ( delay );
		action->Start ( 0, false );
		action->PushLuaUserdata ( state );

		return 1;
	}
	
	self->mFieldOfView = state.GetValue < float >( 2, 0.0f );
	self->ScheduleUpdate ();
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setFarPlane
	@text	Sets the camera's far plane distance.

	@in		MOAICamera self
	@opt	number far			Default value is 10000.
	@out	nil
*/
int MOAICamera::_setFarPlane ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )
	self->mFarPlane = state.GetValue < float >( 2, DEFAULT_FAR_PLANE );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setFieldOfView
	@text	Sets the camera's horizontal field of view.

	@in		MOAICamera self
	@opt	number hfow			Default value is 60.
	@out	nil
*/
int MOAICamera::_setFieldOfView( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )
	self->mFieldOfView = state.GetValue < float >( 2, DEFAULT_HFOV );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setNearPlane
	@text	Sets the camera's near plane distance.

	@in		MOAICamera self
	@opt	number near			Default value is 1.
	@out	nil
*/
int MOAICamera::_setNearPlane ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )
	self->mNearPlane = state.GetValue < float >( 2, DEFAULT_NEAR_PLANE );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setOrtho
	@text	Sets orthographic mode.

	@in		MOAICamera self
	@opt	boolean ortho			Default value is true.
	@out	nil
*/
int MOAICamera::_setOrtho ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )
	self->mType = state.GetValue < bool >( 2, true ) ? CAMERA_TYPE_ORTHO : CAMERA_TYPE_3D;
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setType
	@text	Directly set the camera's type to one of CAMERA_TYPE_WINDOW, CAMERA_TYPE_ORTHO, CAMERA_TYPE_3D.

	@in		MOAICamera self
	@in		number type
	@out	nil
*/
int MOAICamera::_setType ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )
	self->mType = state.GetValue < u32 >( 2, CAMERA_TYPE_WINDOW );
	return 0;
}

//================================================================//
// MOAICamera
//================================================================//

//----------------------------------------------------------------//
void MOAICamera::DrawDebug () {

	MOAIDebugLinesMgr& debugLines = MOAIDebugLinesMgr::Get ();
	if ( !( debugLines.IsVisible () && debugLines.SelectStyleSet < MOAICamera >())) return;
	
	MOAIGfxState& gfxState = MOAIGfxMgr::Get ().mGfxState;
	
	MOAIDraw& draw = MOAIDraw::Get ();
	UNUSED ( draw ); // mystery warning in vs2008
	
	draw.Bind ();
	
	ZLRect viewRect = gfxState.GetViewRect ();
	//float aspect = viewRect.Width () / viewRect.Height ();
	
	ZLMatrix4x4 mtx = gfxState.GetMtx ( MOAIGfxState::CLIP_TO_DISPLAY_MTX );
	
	gfxState.SetVertexTransform ( mtx ); // draw in device space
	
	if ( debugLines.Bind ( DEBUG_DRAW_FRAME )) {
		draw.DrawRectOutline ( -1.0f, -1.0f, 1.0f, 1.0f );
	}
	
	mtx.m [ ZLMatrix4x4::C1_R1 ] *= viewRect.Width () / viewRect.Height ();
	gfxState.SetVertexTransform ( mtx );
	
	if ( debugLines.Bind ( DEBUG_DRAW_RETICLE )) {
		draw.DrawEllipseOutline ( 0.0f, 0.0f, RETICLE_RADIUS, RETICLE_RADIUS, 64 );
		draw.DrawLine ( -RETICLE_RADIUS, 0.0f, RETICLE_RADIUS, 0.0f );
		draw.DrawLine ( 0.0f, -RETICLE_RADIUS, 0.0f, RETICLE_RADIUS );
	}
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAICamera::GetBillboardMtx () const {

	ZLMatrix4x4 mtx ( this->GetLocalToWorldMtx ());
	mtx.m [ ZLMatrix4x4::C3_R0 ] = 0.0f;
	mtx.m [ ZLMatrix4x4::C3_R1 ] = 0.0f;
	mtx.m [ ZLMatrix4x4::C3_R2 ] = 0.0f;
	return mtx;
}

//----------------------------------------------------------------//
float MOAICamera::GetFocalLength ( float width ) const {

	if ( this->mType == CAMERA_TYPE_3D ) {
		float c = Cot ( this->mFieldOfView * 0.5f * ( float )D2R );
		return width * c * 0.5f;
	}
	return 0.0f;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAICamera::GetProjMtx ( const MOAIViewport& viewport ) const {
	
	ZLMatrix4x4 proj;
	ZLMatrix4x4 mtx;

	// rotate
	proj.RotateZ ( -viewport.mRotation * ( float )D2R );
	
	// project
	
	ZLVec2D viewScale = viewport.GetScale (); // TODO: bit confusing in the 3d; clarify
	
	if ( this->mUseProjectionMtx ) {
	
		mtx = this->mProjectionMtx;
	}
	else {
		
		switch ( this->mType ) {
		
			case CAMERA_TYPE_ORTHO: {
				
				float xs = ( 2.0f / viewport.Width ()) * viewScale.mX;
				float ys = ( 2.0f / viewport.Height ()) * viewScale.mY;
				
				mtx.Ortho ( xs, ys, this->mNearPlane, this->mFarPlane );
				break;
			}
			case CAMERA_TYPE_3D: {
				
				float xs = Tan (( this->mFieldOfView * ( float )D2R ) / 2.0f ) * this->mNearPlane;
				float ys = xs / viewport.GetAspect ();
				
				xs *= viewScale.mX;
				ys *= viewScale.mY;

				mtx.Frustum ( -xs, ys, xs, -ys, this->mNearPlane, this->mFarPlane );
				
				break;
			}
			case CAMERA_TYPE_WINDOW:
			default: {
				
				float xScale = ( 2.0f / viewport.Width ()) * viewScale.mX;
				float yScale = ( 2.0f / viewport.Height ()) * viewScale.mY;
				
				mtx.Scale ( xScale, yScale, -1.0 ); // Z must be non-zero to produce invertible projection matrix
			}
		}
	}
	
	proj.Append ( mtx );
	
	// offset
	mtx.Translate ( viewport.mOffset.mX, viewport.mOffset.mY, 0.0f );
	proj.Append ( mtx );
	
	return proj;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAICamera::GetViewMtx () const {

	return ZLMatrix4x4 ( this->GetWorldToLocalMtx ());
}

//----------------------------------------------------------------//
ZLVec3D MOAICamera::GetViewVector () const {

	ZLVec3D viewVec = this->GetLocalToWorldMtx ().GetZAxis ();
	viewVec.Norm ();
	viewVec.Scale ( -1.0f );
	return viewVec;
}

//----------------------------------------------------------------//
void MOAICamera::LookAt ( float x, float y, float z ) {

	static const ZLVec3D yAxis ( 0.0f, 1.0f, 0.0f );
	static const ZLVec3D zAxis ( 0.0f, 0.0f, -1.0f );

	this->ForceUpdate ();

	ZLVec3D local ( x, y, z );
	this->GetWorldToLocalMtx ().Transform ( local );
	
	ZLVec3D target;
	
	target.Init ( local.mX, 0.0f, local.mZ );
	target.Norm ();
	
	float yRot = ( float )( zAxis.Radians ( target ) * R2D * ( target.mX > 0.0f ? 1.0f : -1.0f )); // yaw
	
	target = local;
	target.Norm ();
	
	float xRot = ( float )( yAxis.Radians ( target ) * R2D ); // pitch
	
	
	ZLVec3D rot = this->GetRot ();
	this->SetRot ( rot.mX - ( xRot - 90.0f ), rot.mY - yRot, 0.0f );
}

//----------------------------------------------------------------//
MOAICamera::MOAICamera () :
	mUseProjectionMtx ( false ),
	mFieldOfView ( DEFAULT_HFOV ),
	mNearPlane ( DEFAULT_NEAR_PLANE ),
	mFarPlane ( DEFAULT_FAR_PLANE ),
	mType ( CAMERA_TYPE_WINDOW ) {

	RTTI_SINGLE ( MOAITransform )
	
	this->mProjectionMtx = ZLMatrix4x4::IDENT;
}

//----------------------------------------------------------------//
MOAICamera::~MOAICamera () {
}

//----------------------------------------------------------------//
void MOAICamera::RegisterLuaClass ( MOAILuaState& state ) {

	MOAITransform::RegisterLuaClass ( state );
	
	MOAIDebugLinesMgr::Get ().ReserveStyleSet < MOAICamera >( TOTAL_DEBUG_LINE_STYLES );
	
	state.SetField ( -1, "DEBUG_DRAW_CAMERA_MASTER",			MOAIDebugLinesMgr::Pack < MOAICamera >( (u32) -1 ));
	state.SetField ( -1, "DEBUG_DRAW_FRAME",					MOAIDebugLinesMgr::Pack < MOAICamera >( DEBUG_DRAW_FRAME ));
	state.SetField ( -1, "DEBUG_DRAW_RETICLE",					MOAIDebugLinesMgr::Pack < MOAICamera >( DEBUG_DRAW_RETICLE ));
	
	state.SetField ( -1, "ATTR_FOV",			MOAICameraAttr::Pack ( ATTR_FOV ));
	
	state.SetField ( -1, "CAMERA_TYPE_3D",		( u32 )CAMERA_TYPE_3D );
	state.SetField ( -1, "CAMERA_TYPE_ORTHO",	( u32 )CAMERA_TYPE_ORTHO );
	state.SetField ( -1, "CAMERA_TYPE_WINDOW",	( u32 )CAMERA_TYPE_WINDOW );
}

//----------------------------------------------------------------//
void MOAICamera::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAITransform::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "getFarPlane",		_getFarPlane },
		{ "getFieldOfView",		_getFieldOfView },
		{ "getFloorMove",		_getFloorMove },
		{ "getFocalLength",		_getFocalLength },
		{ "getNearPlane",		_getNearPlane },
		{ "getViewVector",		_getViewVector },
		{ "lookAt",				_lookAt },
		{ "moveFieldOfView",	_moveFieldOfView },
		{ "seekFieldOfView",	_seekFieldOfView },
		{ "setFarPlane",		_setFarPlane },
		{ "setFieldOfView",		_setFieldOfView },
		{ "setNearPlane",		_setNearPlane },
		{ "setOrtho",			_setOrtho },
		{ "setType",			_setType },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAICamera::SetProjMtx () {

	this->mProjectionMtx = ZLMatrix4x4::IDENT;
	this->mUseProjectionMtx = false;
}

//----------------------------------------------------------------//
void MOAICamera::SetProjMtx ( const ZLMatrix4x4& mtx ) {

	this->mProjectionMtx = mtx;
	this->mUseProjectionMtx = true;
}

//================================================================//
// ::implementation::
//================================================================//

//----------------------------------------------------------------//
bool MOAICamera::MOAINode_ApplyAttrOp ( u32 attrID, MOAIAttribute& attr, u32 op ) {

	if ( MOAICameraAttr::Check ( attrID )) {

		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_FOV:
				this->mFieldOfView = attr.Apply ( this->mFieldOfView, op, MOAIAttribute::ATTR_READ_WRITE );
				return true;
		}
	}
	return MOAITransform::MOAINode_ApplyAttrOp ( attrID, attr, op );
}

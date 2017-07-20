// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAISim.h>
#include <moai-sim/MOAIViewport.h>

#define DEFAULT_HFOV 60.0f
#define DEFAULT_NEAR_PLANE 1.0f
#define DEFAULT_FAR_PLANE 10000.0f

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
	r.Rotate90Clockwise ();
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
bool MOAICamera::ApplyAttrOp ( u32 attrID, MOAIAttrOp& attrOp, u32 op ) {

	if ( MOAICameraAttr::Check ( attrID )) {

		switch ( UNPACK_ATTR ( attrID )) {
			case ATTR_FOV:
				this->mFieldOfView = attrOp.Apply ( this->mFieldOfView, op, MOAIAttrOp::ATTR_READ_WRITE, MOAIAttrOp::ATTR_TYPE_FLOAT );
				return true;
		}
	}
	return MOAITransform::ApplyAttrOp ( attrID, attrOp, op );
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAICamera::GetBillboardMtx () const {

	ZLMatrix4x4 mtx;
	mtx.Init ( this->GetLocalToWorldMtx ());
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
	
	ZLVec2D viewScale = viewport.GetScale ();
	
	switch ( this->mType ) {
	
		case CAMERA_TYPE_ORTHO: {
			
			float xs = ( 2.0f / viewport.Width ()) * viewScale.mX;
			float ys = ( 2.0f / viewport.Height ()) * viewScale.mY;
			
			mtx.Ortho ( xs, ys, this->mNearPlane, this->mFarPlane );
			break;
		}
		case CAMERA_TYPE_3D: {
			
			float xs = Cot (( this->mFieldOfView * ( float )D2R ) / 2.0f );
			float ys = xs * viewport.GetAspect ();
			
			xs *= viewScale.mX;
			ys *= viewScale.mY;
			
			mtx.Perspective ( xs, ys, this->mNearPlane, this->mFarPlane );
			break;
		}
		case CAMERA_TYPE_WINDOW:
		default: {
			
			ZLRect rect = viewport.GetRect ();
			
			ZLVec2D viewScale = viewport.GetScale ();
			float xScale = ( 2.0f / rect.Width ()) * viewScale.mX;
			float yScale = ( 2.0f / rect.Height ()) * viewScale.mY;
			
			mtx.Scale ( xScale, yScale, -1.0f );
		}
	}
	
	proj.Append ( mtx );
	
	// offset
	mtx.Translate ( viewport.mOffset.mX, viewport.mOffset.mY, 0.0f );
	proj.Append ( mtx );
	
	return proj;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAICamera::GetProjMtxInv ( const MOAIViewport& viewport ) const {

	ZLMatrix4x4 mtx;
	mtx.Inverse ( this->GetProjMtx ( viewport ));
	return mtx;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAICamera::GetViewMtx () const {

	ZLMatrix4x4 mtx;
	mtx.Init ( this->GetWorldToLocalMtx ());
	return mtx;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAICamera::GetViewProjMtx ( const MOAIViewport& viewport ) const {

	ZLMatrix4x4 mtx = this->GetViewMtx ();
	mtx.Append ( this->GetProjMtx ( viewport ));
	return mtx;
}

//----------------------------------------------------------------//
ZLVec3D MOAICamera::GetViewVector () const {

	ZLVec3D viewVec = this->GetLocalToWorldMtx ().GetZAxis ();
	viewVec.Norm ();
	viewVec.Scale ( -1.0f );
	return viewVec;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAICamera::GetWndToWorldMtx ( const MOAIViewport& viewport ) const {

	ZLMatrix4x4 wndToWorld = this->GetWorldToWndMtx ( viewport );
	wndToWorld.Inverse ();
	return wndToWorld;
}

//----------------------------------------------------------------//
ZLMatrix4x4 MOAICamera::GetWorldToWndMtx ( const MOAIViewport& viewport ) const {
	
	ZLMatrix4x4 worldToWnd = this->GetViewMtx ();
	worldToWnd.Append ( this->GetProjMtx ( viewport ));
	worldToWnd.Append ( viewport.GetNormToWndMtx ());
	return worldToWnd;
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
	
	float yRot = zAxis.Radians ( target ) * R2D * ( target.mX > 0.0f ? 1.0f : -1.0f ); // yaw
	
	target = local;
	target.Norm ();
	
	float xRot = yAxis.Radians ( target ) * R2D; // pitch
	
	
	ZLVec3D rot = this->GetRot ();
	this->SetRot ( rot.mX - ( xRot - 90.0f ), rot.mY - yRot, 0.0f );
}

//----------------------------------------------------------------//
MOAICamera::MOAICamera () :
	mFieldOfView ( DEFAULT_HFOV ),
	mNearPlane ( DEFAULT_NEAR_PLANE ),
	mFarPlane ( DEFAULT_FAR_PLANE ),
	mType ( CAMERA_TYPE_3D ) {

	RTTI_SINGLE ( MOAITransform )
	
	//this->SetEulerOrder ( EULER_YXZ );
}

//----------------------------------------------------------------//
MOAICamera::~MOAICamera () {
}

//----------------------------------------------------------------//
void MOAICamera::RegisterLuaClass ( MOAILuaState& state ) {
	MOAITransform::RegisterLuaClass ( state );
	
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

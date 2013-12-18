// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAICamera.h>
#include <moai-sim/MOAIViewport.h>

#define DEFAULT_HFOV 60.0f
#define DEFAULT_NEAR_PLANE 1.0f
#define DEFAULT_FAR_PLANE 10000.0f

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getFarPlane
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
/**	@name	getFieldOfView
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
// TODO: doxygen
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
/**	@name	getFocalLength
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
/**	@name	getNearPlane
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
/**	@name	setFarPlane
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
/**	@name	setFieldOfView
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
/**	@name	setNearPlane
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
/**	@name	setOrtho
	@text	Sets orthographic mode.

	@in		MOAICamera self
	@opt	boolean ortho			Default value is true.
	@out	nil
*/
int MOAICamera::_setOrtho ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAICamera, "U" )
	self->mOrtho = state.GetValue < bool >( 2, true );
	return 0;
}

//================================================================//
// MOAICamera
//================================================================//

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

	if ( !this->mOrtho ) {
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
	
	if ( this->mOrtho ) {
		
		float xs = ( 2.0f / viewport.Width ()) * viewScale.mX;
		float ys = ( 2.0f / viewport.Height ()) * viewScale.mY;
		
		mtx.Ortho ( xs, ys, this->mNearPlane, this->mFarPlane );
	}
	else {
		
		float xs = Cot (( this->mFieldOfView * ( float )D2R ) / 2.0f );
		float ys = xs * viewport.GetAspect ();
		
		xs *= viewScale.mX;
		ys *= viewScale.mY;
		
		mtx.Perspective ( xs, ys, this->mNearPlane, this->mFarPlane );
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
MOAICamera::MOAICamera () :
	mFieldOfView ( DEFAULT_HFOV ),
	mNearPlane ( DEFAULT_NEAR_PLANE ),
	mFarPlane ( DEFAULT_FAR_PLANE ),
	mOrtho ( false ) {

	RTTI_SINGLE ( MOAITransform )
}

//----------------------------------------------------------------//
MOAICamera::~MOAICamera () {
}

//----------------------------------------------------------------//
void MOAICamera::RegisterLuaClass ( MOAILuaState& state ) {
	MOAITransform::RegisterLuaClass ( state );
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
		{ "setFarPlane",		_setFarPlane },
		{ "setFieldOfView",		_setFieldOfView },
		{ "setNearPlane",		_setNearPlane },
		{ "setOrtho",			_setOrtho },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

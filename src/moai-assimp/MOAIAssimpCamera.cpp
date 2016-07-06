// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-assimp/MOAIAssimpCamera.h>
#include <moai-assimp/MOAIAssimpUtil.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getAspect ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber(  state, self->mScene ? self->mAssimpCamera->mAspect : 0.0f );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getFarClipPlane ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber(  state, self->mScene ? self->mAssimpCamera->mClipPlaneFar : 0.0f );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getHorizontalFOV ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber(  state, self->mScene ? self->mAssimpCamera->mHorizontalFOV : 0.0f );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getLocationVector ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene ) {
		MOAIAssimpUtil::PushVector ( state, self->mAssimpCamera->mPosition );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getLookAtVector ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene ) {
		MOAIAssimpUtil::PushVector ( state, self->mAssimpCamera->mLookAt );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getMoaiCamera ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene && self->mMoaiCamera ) {
		self->mMoaiCamera->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getNearClipPlane ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber(  state , self->mScene ? self->mAssimpCamera->mClipPlaneNear : 0.0f );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getPitchAngle ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber ( state , self->mScene ? self->CalculatePitch ( self->mRotation) * R2D : 0.0f );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getPositionVector ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene ) {
		MOAIAssimpUtil::PushVector ( state, self->mAssimpCamera->mPosition );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getRollAngle ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber ( state , self->mScene ? self->CalculateRoll ( self->mRotation) * R2D : 0.0f );
	return 1;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getRotationQuaternion ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene ) {

		lua_newtable( state );
		lua_newtable( state );

		aiVector3D rotationAxis ( self->mRotation.x, self->mRotation.y, self->mRotation.z );

		MOAIAssimpUtil::PushVector ( state, rotationAxis );
		lua_setfield( state , -2 , "rotation_axis" );
		lua_pushnumber( state , self->mRotation.w );
		lua_setfield( state , -2 , "rotation_angle" );

		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getScalingVector ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene ) {
		MOAIAssimpUtil::PushVector ( state, self->mScaling );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getUpVector ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene ) {
		MOAIAssimpUtil::PushVector ( state, self->mAssimpCamera->mUp );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIAssimpCamera::_getYawAngle ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber ( state , self->mScene ? self->CalculateYaw ( self->mRotation) * R2D : 0.0f );
	return 1;
}

//================================================================//
// MOAIAssimpCamera
//================================================================//

//----------------------------------------------------------------//
float MOAIAssimpCamera::CalculatePitch(aiQuaternion quaternion) {
	return asin ( 2.0f * quaternion.z * quaternion.y + 2.0f * quaternion.x * quaternion. w );
}

//----------------------------------------------------------------//
float MOAIAssimpCamera::CalculateRoll ( aiQuaternion quaternion ) {
	float x2 = quaternion.x * quaternion.x;
	float z2 = quaternion.z * quaternion.z;
	return atan2 ( 2.0f * quaternion.z * quaternion.w - 2.0f * quaternion.y * quaternion.x, 1.0f- 2.0f * z2 - 2.0f * x2 );
}

//----------------------------------------------------------------//
float MOAIAssimpCamera::CalculateYaw ( aiQuaternion quaternion ) {
	float x2 = quaternion.x * quaternion.x;
	float y2 = quaternion.y * quaternion.y;
	return atan2 ( 2.0f * quaternion.y * quaternion.w - 2.0f * quaternion.z * quaternion.x , 1.0f - 2.0f * x2 - 2.0f * y2 );
}

//----------------------------------------------------------------//
MOAIAssimpCamera::MOAIAssimpCamera () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIAssimpSceneMember )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIAssimpCamera::~MOAIAssimpCamera () {
}

//----------------------------------------------------------------//
void MOAIAssimpCamera::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAssimpSceneMember::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIAssimpCamera::RegisterLuaFuncs ( MOAILuaState& state ) {

	MOAIAssimpSceneMember::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getAspect",				_getAspect },
		{ "getFarClipPlane",		_getFarClipPlane },
		{ "getNearClipPlane",		_getNearClipPlane },
		{ "getHorizontalFOV",		_getHorizontalFOV },
		{ "getLookAtVector",		_getLookAtVector },
		{ "getPositionVector",		_getPositionVector },
		{ "getUpVector",			_getUpVector },
		{ "getLocationVector",		_getLocationVector },
		{ "getScalingVector",		_getScalingVector },
		{ "getRotationQuaternion",	_getRotationQuaternion },
		{ "getPitchAngle",			_getPitchAngle },
		{ "getYawAngle",			_getYawAngle },
		{ "getRollAngle",			_getRollAngle },
		{ "getMoaiCamera",			_getMoaiCamera },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIAssimpCamera::SetCamera ( aiCamera *assimpCamera ) {

	this->mAssimpCamera = assimpCamera;

	aiMatrix4x4 transform = this->mNode->mTransformation;

	transform.Decompose ( this->mScaling, this->mRotation, this->mPosition );
	this->mMoaiCamera = new MOAICamera ();
	this->mMoaiCamera->SetLoc ( this->mPosition.x , this->mPosition.y , this->mPosition.z );
	this->mMoaiCamera->SetScl ( this->mScaling.x , this->mScaling.y , this->mScaling.z );
	this->mMoaiCamera->SetRot (
		( float )( this->CalculatePitch ( this->mRotation) * R2D ),
		( float )( this->CalculateYaw ( this->mRotation) * R2D ),
		( float )( this->CalculateRoll ( this->mRotation) * R2D )
	);
}

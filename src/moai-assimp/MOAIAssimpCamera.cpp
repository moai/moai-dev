// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-assimp/MOAIAssimpCamera.h>
#include <moai-assimp/MOAIAssimpUtil.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getAspect
 @text	Returns the aspect.
 
 @in	MOAIAssimpCamera self
 @out	number aspectRatio
 */
int MOAIAssimpCamera::_getAspect ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber(  state, self->mScene ? self->mAssimpCamera->mAspect : 0.0f );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getFarClipPlane
 @text	Returns the FarClipPlane
 
 @in	MOAIAssimpCamera self
 @out	number ClipPlaneFar
 */
int MOAIAssimpCamera::_getFarClipPlane ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber(  state, self->mScene ? self->mAssimpCamera->mClipPlaneFar : 0.0f );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getHorizontalFOV
 @text	Returns the HorizontalFOV
 
 @in	MOAIAssimpCamera self
 @out	number HorizontalFOV
 */
int MOAIAssimpCamera::_getHorizontalFOV ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber(  state, self->mScene ? self->mAssimpCamera->mHorizontalFOV : 0.0f );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getLocationVector
 @text	Returns the LocationVector
 
 @in	MOAIAssimpCamera self
 @out	table location(x,y,z)
 */
int MOAIAssimpCamera::_getLocationVector ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene ) {
		MOAIAssimpUtil::PushVector ( state, self->mAssimpCamera->mPosition );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getLookAtVector
 @text	Returns the LookAtVector
 
 @in	MOAIAssimpCamera self
 @out	table lookAt(x,y,z)
 */
int MOAIAssimpCamera::_getLookAtVector ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene ) {
		MOAIAssimpUtil::PushVector ( state, self->mAssimpCamera->mLookAt );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getMoaiCamera
 @text	Returns the Moai Camera
 
 @in	MOAIAssimpCamera self
 @out	MOAICamera camera
 */
int MOAIAssimpCamera::_getMoaiCamera ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene && self->mMoaiCamera ) {
		self->mMoaiCamera->PushLuaUserdata ( state );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getNearClipPlane
 @text	Returns the NearClipPlane
 
 @in	MOAIAssimpCamera self
 @out	number NearClipPlane
 */
int MOAIAssimpCamera::_getNearClipPlane ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber(  state , self->mScene ? self->mAssimpCamera->mClipPlaneNear : 0.0f );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getPitchAngle
 @text	Returns the PitchAngle
 
 @in	MOAIAssimpCamera self
 @out	number PitchAngle
 */
int MOAIAssimpCamera::_getPitchAngle ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber ( state , self->mScene ? self->CalculatePitch ( self->mRotation) * R2D : 0.0f );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getPositionVector
 @text	Returns the PositionVector
 
 @in	MOAIAssimpCamera self
 @out	table PitchAngle(x,y,z)
 */
int MOAIAssimpCamera::_getPositionVector ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene ) {
		MOAIAssimpUtil::PushVector ( state, self->mAssimpCamera->mPosition );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getRollAngle
 @text	Returns the RollAngle
 
 @in	MOAIAssimpCamera self
 @out	number RollAngle
 */
int MOAIAssimpCamera::_getRollAngle ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	lua_pushnumber ( state , self->mScene ? self->CalculateRoll ( self->mRotation) * R2D : 0.0f );
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getRotationQuaternion
 @text	Returns the RotationQuaternion
 
 @in	MOAIAssimpCamera self
 @out	table RotationQuaternion(x,y,z,w)
 */
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
/**	@lua	getScalingVector
 @text	Returns the ScalingVector
 
 @in	MOAIAssimpCamera self
 @out	table ScalingVector(x,y,z)
 */
int MOAIAssimpCamera::_getScalingVector ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene ) {
		MOAIAssimpUtil::PushVector ( state, self->mScaling );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getUpVector
 @text	Returns the UpVector
 
 @in	MOAIAssimpCamera self
 @out	table UpVector(x,y,z)
 */
int MOAIAssimpCamera::_getUpVector ( lua_State *L ) {
	MOAI_LUA_SETUP ( MOAIAssimpCamera, "U" )

	if ( self->mScene ) {
		MOAIAssimpUtil::PushVector ( state, self->mAssimpCamera->mUp );
		return 1;
	}
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getYawAngle
 @text	Returns the YawAngle
 
 @in	MOAIAssimpCamera self
 @out	number YawAngle
 */
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
	return (float) asin ( 2.0f * quaternion.z * quaternion.y + 2.0f * quaternion.x * quaternion. w );
}

//----------------------------------------------------------------//
float MOAIAssimpCamera::CalculateRoll ( aiQuaternion quaternion ) {
	float x2 = (float) quaternion.x * (float) quaternion.x;
	float z2 = (float) quaternion.z * (float) quaternion.z;
	return (float) atan2 ( 2.0f * quaternion.z * quaternion.w - 2.0f * quaternion.y * quaternion.x, 1.0f- 2.0f * z2 - 2.0f * x2 );
}

//----------------------------------------------------------------//
float MOAIAssimpCamera::CalculateYaw ( aiQuaternion quaternion ) {
	float x2 = (float) quaternion.x * (float) quaternion.x;
	float y2 = (float) quaternion.y * (float) quaternion.y;
	return (float) atan2 ( 2.0f * quaternion.y * quaternion.w - 2.0f * quaternion.z * quaternion.x , 1.0f - 2.0f * x2 - 2.0f * y2 );
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
	this->mMoaiCamera->SetLoc ((float) this->mPosition.x , (float) this->mPosition.y , (float) this->mPosition.z );
	this->mMoaiCamera->SetScl ((float) this->mScaling.x , (float) this->mScaling.y , (float) this->mScaling.z );
	this->mMoaiCamera->SetRot (
		( float )( this->CalculatePitch ( this->mRotation) * R2D ),
		( float )( this->CalculateYaw ( this->mRotation) * R2D ),
		( float )( this->CalculateRoll ( this->mRotation) * R2D )
	);
}

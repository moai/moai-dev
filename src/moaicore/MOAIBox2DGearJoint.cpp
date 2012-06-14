// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DGearJoint.h>
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAILogMessages.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getJointA
	@text	See Box2D documentation.
	
	@in		MOAIBox2DGearJoint self
	@out	MOAIBox2DJoint jointA
*/
int MOAIBox2DGearJoint::_getJointA ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DGearJoint, "U" )

	self->mJointA->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getJointB
	@text	See Box2D documentation.
	
	@in		MOAIBox2DGearJoint self
	@out	MOAIBox2DJoint jointB
*/
int MOAIBox2DGearJoint::_getJointB ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DGearJoint, "U" )

	self->mJointB->PushLuaUserdata ( state );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DGearJoint self
	@out	number ratio
*/
int MOAIBox2DGearJoint::_getRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DGearJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2GearJoint* joint = ( b2GearJoint* )self->mJoint;
	state.Push ( joint->GetRatio ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DGearJoint self
	@opt	number ratio		Default value is 0.
	@out	nil
*/
int MOAIBox2DGearJoint::_setRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DGearJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float ratio = state.GetValue < float >( 2, 0.0f );

	b2GearJoint* joint = ( b2GearJoint* )self->mJoint;
	joint->SetRatio ( ratio );
	
	return 0;
}

//================================================================//
// MOAIBox2DGearJoint
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DGearJoint::MOAIBox2DGearJoint () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DGearJoint::~MOAIBox2DGearJoint () {

	this->mJointA.Set ( *this, 0 );
	this->mJointB.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIBox2DGearJoint::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DGearJoint::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getJointA",				_getJointA },
		{ "getJointB",				_getJointB },
		{ "getRatio",				_getRatio },
		{ "setRatio",				_setRatio },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif

// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DBody.h>
#include <moai-box2d/MOAIBox2DGearJoint.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getJointA
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
/**	@lua	getJointB
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
/**	@lua	getRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DGearJoint self
	@out	number ratio
*/
int MOAIBox2DGearJoint::_getRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DGearJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2GearJoint* joint = ( b2GearJoint* )self->mJoint;
	state.Push ( joint->GetRatio ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DGearJoint self
	@opt	number ratio		Default value is 0.
	@out	nil
*/
int MOAIBox2DGearJoint::_setRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DGearJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
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
	
	MOAI_LUA_OBJECT_RTTI_BEGIN ( MOAIBox2DGearJoint )
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DGearJoint::~MOAIBox2DGearJoint () {

	this->mJointA.Set ( *this, 0 );
	this->mJointB.Set ( *this, 0 );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DGearJoint::MOAILuaObject_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIBox2DGearJoint::MOAILuaObject_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.DidVisit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getJointA",				_getJointA },
		{ "getJointB",				_getJointB },
		{ "getRatio",				_getRatio },
		{ "setRatio",				_setRatio },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

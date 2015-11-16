// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DBody.h>
#include <moai-box2d/MOAIBox2DWeldJoint.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getDampingRatio
	@text	See Box2D documentation.

	@in		MOAIBox2DDistanceJoint self
	@out	number 	dampingRatio
 */
int MOAIBox2DWeldJoint::_getDampingRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWeldJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2WeldJoint* joint = ( b2WeldJoint* )self->mJoint;
	state.Push ( joint->GetDampingRatio ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua    getFrequency
	@text   See Box2D documentation.

	@in		MOAIBox2DDistanceJoint 		self
	@out	number 	frequency			In Hz.
 */
int MOAIBox2DWeldJoint::_getFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWeldJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2WeldJoint* joint = ( b2WeldJoint* )self->mJoint;
	state.Push ( joint->GetFrequency ());

	return 1;
}

//----------------------------------------------------------------//
/**	@lua    setDampingRatio
	@text   See Box2D documentation.
	
	@in		MOAIBox2DDistanceJoint 		self
	@opt	number dampingRatio 		Default value is 0.
	@out	nil
 */
int MOAIBox2DWeldJoint::_setDampingRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWeldJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float dampingRatio = state.GetValue < float >( 2, 0.0f );

	b2WeldJoint* joint = ( b2WeldJoint* )self->mJoint;
	joint->SetDampingRatio ( dampingRatio );

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setFrequency
	@text	See Box2D documentation.
	   
	@in		MOAIBox2DDistanceJoint 	self
	@opt	number frequency		In Hz. Default value is 0.
	@out	nil
 */
int MOAIBox2DWeldJoint::_setFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWeldJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float frequency = state.GetValue < float >( 2, 0.0f );

	b2WeldJoint* joint = ( b2WeldJoint* )self->mJoint;
	joint->SetFrequency ( frequency );

	return 0;
}

//================================================================//
// MOAIBox2DWeldJoint
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DWeldJoint::MOAIBox2DWeldJoint () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DWeldJoint::~MOAIBox2DWeldJoint () {
}

//----------------------------------------------------------------//
void MOAIBox2DWeldJoint::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DWeldJoint::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getDampingRatio",		_getDampingRatio },
		{ "getFrequency",			_getFrequency },
		{ "setDampingRatio",		_setDampingRatio },
		{ "setFrequency",			_setFrequency },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

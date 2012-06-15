// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DRopeJoint.h>
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAISim.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

MOAIBox2DRopeJoint::MOAIBox2DRopeJoint () {
	
	RTTI_BEGIN
	RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
/**	@name	getLimitState
 @text	See Box2D documentation.
 
 @in		MOAIBox2DRopeJoint self
 @out	number limitState	one of the "LimitState" codes
 */
int MOAIBox2DRopeJoint::_getLimitState ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRopeJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2RopeJoint* joint = ( b2RopeJoint* )self->mJoint;
	state.Push ( joint->GetLimitState() );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMaxLength
 @text	See Box2D documentation.
 
 @in		MOAIBox2DRopeJoint self
 @out	number maxLength in units, converted from meters.
 */
int MOAIBox2DRopeJoint::_getMaxLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRopeJoint, "U" )
	
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2RopeJoint* joint = ( b2RopeJoint* )self->mJoint;
	state.Push ( joint->GetMaxLength() / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setMaxLength
 @text	See Box2D documentation.
 
 @in		MOAIBox2DRopeJoint self
 @in	number	maxLength in units, converted to meters. Default is 0.
 @out	nil
 */
int MOAIBox2DRopeJoint::_setMaxLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRopeJoint, "U" )
	
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	float length = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	b2RopeJoint* joint = ( b2RopeJoint* )self->mJoint;
	joint->SetMaxLength ( length );
	
	return 1;
}

//----------------------------------------------------------------//
MOAIBox2DRopeJoint::~MOAIBox2DRopeJoint () {
	
}

//----------------------------------------------------------------//
void MOAIBox2DRopeJoint::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DRopeJoint::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaFuncs ( state );
	luaL_Reg regTable [] = {
		{ "getLimitState",			_getLimitState },
		{ "getMaxLength",			_getMaxLength },
		{ "setMaxLength",			_setMaxLength },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif
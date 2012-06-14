// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DMouseJoint.h>
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAILogMessages.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getDampingRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@out	number dampingRatio
*/
int MOAIBox2DMouseJoint::_getDampingRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	state.Push ( joint->GetDampingRatio ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getFrequency
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@out	number frequency in Hz
*/
int MOAIBox2DMouseJoint::_getFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	state.Push ( joint->GetFrequency ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMaxForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@out	number maxForce in kg * units / s^2 converted from N [kg * m / s^2]
*/
int MOAIBox2DMouseJoint::_getMaxForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	state.Push ( joint->GetMaxForce () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getTarget
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@out	number x in units, world coordinates, converted from meters
	@out	number y in units, world coordinates, converted from meters
*/
int MOAIBox2DMouseJoint::_getTarget ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	
	b2Vec2 target = joint->GetTarget ();
	
	state.Push ( target.x / unitsToMeters );
	state.Push ( target.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	setDampingRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@opt	number dampingRatio		Default value is 0.
	@out	nil
*/
int MOAIBox2DMouseJoint::_setDampingRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float dampingRatio = state.GetValue < float >( 2, 0.0f );

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	joint->SetDampingRatio ( dampingRatio );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFrequency
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@opt	number frequency		in Hz. Default value is 0.
	@out	nil
*/
int MOAIBox2DMouseJoint::_setFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float frequency = state.GetValue < float >( 2, 0.0f );

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	joint->SetFrequency ( frequency );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMaxForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@opt	number maxForce		in kg * units / s^2 converted to N [kg * m / s^2]. Default value is 0.
	@out	nil
*/
int MOAIBox2DMouseJoint::_setMaxForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float maxForce = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	joint->SetMaxForce ( maxForce );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setTarget
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@opt	number x		in units, world coordinates, converted to meters. Default value is 0.
	@opt	number y		in units, world coordinates, converted to meters. Default value is 0.
	@out	nil
*/
int MOAIBox2DMouseJoint::_setTarget ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2Vec2 target;
	
	target.x = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;
	target.y = state.GetValue < float >( 3, 0.0f ) * unitsToMeters;

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	joint->SetTarget ( target );
	
	return 0;
}

//================================================================//
// MOAIBox2DMouseJoint
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DMouseJoint::MOAIBox2DMouseJoint () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DMouseJoint::~MOAIBox2DMouseJoint () {
}

//----------------------------------------------------------------//
void MOAIBox2DMouseJoint::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DMouseJoint::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getDampingRatio",		_getDampingRatio },
		{ "getFrequency",			_getFrequency },
		{ "getMaxForce",			_getMaxForce },
		{ "getTarget",				_getTarget },
		{ "setDampingRatio",		_setDampingRatio },
		{ "setFrequency",			_setFrequency },
		{ "setMaxForce",			_setMaxForce },
		{ "setTarget",				_setTarget },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif

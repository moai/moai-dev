// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DDistanceJoint.h>
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
	
	@in		MOAIBox2DDistanceJoint self
	@out	number dampingRatio
*/
int MOAIBox2DDistanceJoint::_getDampingRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DDistanceJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2DistanceJoint* joint = ( b2DistanceJoint* )self->mJoint;
	state.Push ( joint->GetDampingRatio ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getFrequency
	@text	See Box2D documentation.
	
	@in		MOAIBox2DDistanceJoint self
	@out	number frequency in Hz.
*/
int MOAIBox2DDistanceJoint::_getFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DDistanceJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2DistanceJoint* joint = ( b2DistanceJoint* )self->mJoint;
	state.Push ( joint->GetFrequency ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getLength
	@text	See Box2D documentation.
	
	@in		MOAIBox2DDistanceJoint self
	@out	number length in units, converted from meters.
*/
int MOAIBox2DDistanceJoint::_getLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DDistanceJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2DistanceJoint* joint = ( b2DistanceJoint* )self->mJoint;
	state.Push ( joint->GetLength () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setDampingRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DDistanceJoint self
	@opt	number dampingRatio		Default value is 0.
	@out	nil
*/
int MOAIBox2DDistanceJoint::_setDampingRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DDistanceJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float dampingRatio = state.GetValue < float >( 2, 0.0f );

	b2DistanceJoint* joint = ( b2DistanceJoint* )self->mJoint;
	joint->SetDampingRatio ( dampingRatio );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setFrequency
	@text	See Box2D documentation.
	
	@in		MOAIBox2DDistanceJoint self
	@opt	number frequency		In Hz. Default value is 0.
	@out	nil
*/
int MOAIBox2DDistanceJoint::_setFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DDistanceJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float frequency = state.GetValue < float >( 2, 0.0f );

	b2DistanceJoint* joint = ( b2DistanceJoint* )self->mJoint;
	joint->SetFrequency ( frequency );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLength
	@text	See Box2D documentation.
	
	@in		MOAIBox2DDistanceJoint self
	@opt	number length		in units, converted to meters. Default value is 0.
	@out	nil
*/
int MOAIBox2DDistanceJoint::_setLength ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DDistanceJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float length = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;

	b2DistanceJoint* joint = ( b2DistanceJoint* )self->mJoint;
	joint->SetLength ( length );
	
	return 0;
}

//================================================================//
// MOAIBox2DDistanceJoint
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DDistanceJoint::MOAIBox2DDistanceJoint () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DDistanceJoint::~MOAIBox2DDistanceJoint () {
}

//----------------------------------------------------------------//
void MOAIBox2DDistanceJoint::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DDistanceJoint::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getDampingRatio",		_getDampingRatio },
		{ "getFrequency",			_getFrequency },
		{ "getLength",				_getLength },
		{ "setDampingRatio",		_setDampingRatio },
		{ "setFrequency",			_setFrequency },
		{ "setLength",				_setLength },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif

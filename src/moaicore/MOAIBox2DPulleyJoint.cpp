// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DPulleyJoint.h>
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAILogMessages.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getGroundAnchorA
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPulleyJoint self
	@out	number x in units, world coordinates, converted from meters
	@out	number y in units, world coordinates, converted from meters
*/
int MOAIBox2DPulleyJoint::_getGroundAnchorA ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPulleyJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PulleyJoint* joint = ( b2PulleyJoint* )self->mJoint;
	
	b2Vec2 groundAnchorA = joint->GetGroundAnchorA ();
	
	state.Push ( groundAnchorA.x / unitsToMeters );
	state.Push ( groundAnchorA.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getGroundAnchorB
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPulleyJoint self
	@out	number x in units, world coordinates, converted from meters
	@out	number y in units, world coordinates, converted from meters
*/
int MOAIBox2DPulleyJoint::_getGroundAnchorB ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPulleyJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PulleyJoint* joint = ( b2PulleyJoint* )self->mJoint;
	
	b2Vec2 groundAnchorB = joint->GetGroundAnchorB ();
	
	state.Push ( groundAnchorB.x / unitsToMeters );
	state.Push ( groundAnchorB.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@name	getLength1
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPulleyJoint self
	@out	number length1 in units, converted from meters.
*/
int MOAIBox2DPulleyJoint::_getLength1 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPulleyJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PulleyJoint* joint = ( b2PulleyJoint* )self->mJoint;

	state.Push ( joint->GetLengthA () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getLength2
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPulleyJoint self
	@out	number length2 in units, converted from meters.
*/
int MOAIBox2DPulleyJoint::_getLength2 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPulleyJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PulleyJoint* joint = ( b2PulleyJoint* )self->mJoint;
	state.Push ( joint->GetLengthB () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPulleyJoint self
	@out	number ratio
*/
int MOAIBox2DPulleyJoint::_getRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPulleyJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PulleyJoint* joint = ( b2PulleyJoint* )self->mJoint;
	state.Push ( joint->GetRatio ());
	
	return 1;
}

//================================================================//
// MOAIBox2DPulleyJoint
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DPulleyJoint::MOAIBox2DPulleyJoint () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DPulleyJoint::~MOAIBox2DPulleyJoint () {
}

//----------------------------------------------------------------//
void MOAIBox2DPulleyJoint::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DPulleyJoint::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getGroundAnchorA",		_getGroundAnchorA },
		{ "getGroundAnchorB",		_getGroundAnchorB },
		{ "getLength1",				_getLength1 },
		{ "getLength2",				_getLength2 },
		{ "getRatio",				_getRatio },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif

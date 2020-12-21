// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DBody.h>
#include <moai-box2d/MOAIBox2DPulleyJoint.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getGroundAnchorA
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPulleyJoint self
	@out	number x					in units, world coordinates, converted from meters
	@out	number y					in units, world coordinates, converted from meters
*/
int MOAIBox2DPulleyJoint::_getGroundAnchorA ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPulleyJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PulleyJoint* joint = ( b2PulleyJoint* )self->mJoint;
	
	b2Vec2 groundAnchorA = joint->GetGroundAnchorA ();
	
	state.Push ( groundAnchorA.x / unitsToMeters );
	state.Push ( groundAnchorA.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getGroundAnchorB
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPulleyJoint self
	@out	number x					in units, world coordinates, converted from meters
	@out	number y					in units, world coordinates, converted from meters
*/
int MOAIBox2DPulleyJoint::_getGroundAnchorB ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPulleyJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PulleyJoint* joint = ( b2PulleyJoint* )self->mJoint;
	
	b2Vec2 groundAnchorB = joint->GetGroundAnchorB ();
	
	state.Push ( groundAnchorB.x / unitsToMeters );
	state.Push ( groundAnchorB.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	getLength1
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPulleyJoint self
	@out	number length1				in units, converted from meters.
*/
int MOAIBox2DPulleyJoint::_getLength1 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPulleyJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PulleyJoint* joint = ( b2PulleyJoint* )self->mJoint;

	state.Push ( joint->GetLengthA () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getLength2
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPulleyJoint self
	@out	number length2				in units, converted from meters.
*/
int MOAIBox2DPulleyJoint::_getLength2 ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPulleyJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PulleyJoint* joint = ( b2PulleyJoint* )self->mJoint;
	state.Push ( joint->GetLengthB () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPulleyJoint self
	@out	number ratio
*/
int MOAIBox2DPulleyJoint::_getRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPulleyJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
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
	
	RTTI_BEGIN ( MOAIBox2DPulleyJoint )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIBox2DPulleyJoint >)
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DPulleyJoint::~MOAIBox2DPulleyJoint () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DPulleyJoint::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIBox2DPulleyJoint::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

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

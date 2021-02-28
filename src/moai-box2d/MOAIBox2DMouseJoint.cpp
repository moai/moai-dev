// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DBody.h>
#include <moai-box2d/MOAIBox2DMouseJoint.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getDampingRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@out	number dampingRatio
*/
int MOAIBox2DMouseJoint::_getDampingRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	state.Push ( joint->GetDampingRatio ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getFrequency
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@out	number frequency			in Hz
*/
int MOAIBox2DMouseJoint::_getFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	state.Push ( joint->GetFrequency ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@out	number maxForce				in kg * units / s^2 converted from N [kg * m / s^2]
*/
int MOAIBox2DMouseJoint::_getMaxForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	state.Push ( joint->GetMaxForce () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getTarget
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@out	number x					in units, world coordinates, converted from meters
	@out	number y					in units, world coordinates, converted from meters
*/
int MOAIBox2DMouseJoint::_getTarget ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	
	b2Vec2 target = joint->GetTarget ();
	
	state.Push ( target.x / unitsToMeters );
	state.Push ( target.y / unitsToMeters );
	
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	setDampingRatio
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@opt	number dampingRatio		Default value is 0.
	@out	nil
*/
int MOAIBox2DMouseJoint::_setDampingRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float dampingRatio = state.GetValue < float >( 2, 0.0f );

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	joint->SetDampingRatio ( dampingRatio );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setFrequency
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@opt	number frequency		in Hz. Default value is 0.
	@out	nil
*/
int MOAIBox2DMouseJoint::_setFrequency ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float frequency = state.GetValue < float >( 2, 0.0f );

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	joint->SetFrequency ( frequency );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setMaxForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMouseJoint self
	@opt	number maxForce		in kg * units / s^2 converted to N [kg * m / s^2]. Default value is 0.
	@out	nil
*/
int MOAIBox2DMouseJoint::_setMaxForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMouseJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float maxForce = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;

	b2MouseJoint* joint = ( b2MouseJoint* )self->mJoint;
	joint->SetMaxForce ( maxForce );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setTarget
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
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
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
MOAIBox2DMouseJoint::MOAIBox2DMouseJoint ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIBox2DPrim ( context ),
	MOAIBox2DJoint ( context ) {
	
	RTTI_BEGIN ( MOAIBox2DMouseJoint )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIBox2DMouseJoint >)
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DMouseJoint::~MOAIBox2DMouseJoint () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DMouseJoint::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIBox2DMouseJoint::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

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

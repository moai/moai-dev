// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DBody.h>
#include <moai-box2d/MOAIBox2DFrictionJoint.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getMaxForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DFrictionJoint self
	@out	number maxForce					in kg * units / s^2, converted from N [kg * m / s^2].
*/
int MOAIBox2DFrictionJoint::_getMaxForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DFrictionJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2FrictionJoint* joint = ( b2FrictionJoint* )self->mJoint;
	state.Push ( joint->GetMaxForce () / unitsToMeters );

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMaxTorque
	@text	See Box2D documentation.
	
	@in		MOAIBox2DFrictionJoint self
	@out	number maxTorque		in (kg * units / s^2) * units, converted from N-m.
*/
int MOAIBox2DFrictionJoint::_getMaxTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DFrictionJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2FrictionJoint* joint = ( b2FrictionJoint* )self->mJoint;
	float unitsToMeters = self->GetUnitsToMeters();
	/* Convert to/from N-m (kg m / s^2) * m from/to (kg unit / s^2) * unit */
	state.Push ( joint->GetMaxTorque () / ( unitsToMeters * unitsToMeters ) );

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setMaxForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DFrictionJoint self
	@opt	number maxForce		in kg * units / s^2, converted to N [kg * m / s^2]. Default value is 0.
	@out	nil
*/
int MOAIBox2DFrictionJoint::_setMaxForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DFrictionJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float maxForce = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;

	b2FrictionJoint* joint = ( b2FrictionJoint* )self->mJoint;
	joint->SetMaxForce ( maxForce );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setMaxTorque
	@text	See Box2D documentation.
	
	@in		MOAIBox2DFrictionJoint self
	@opt	number maxTorque		in (kg * units / s^2) * units, converted to N-m. Default value is 0.
	@out	nil
*/
int MOAIBox2DFrictionJoint::_setMaxTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DFrictionJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float unitsToMeters = self->GetUnitsToMeters();
	/* Convert to/from N-m (kg m / s^2) * m from/to (kg unit / s^2) * unit */
	float maxTorque = state.GetValue < float >( 2, 0.0f ) * unitsToMeters * unitsToMeters;

	b2FrictionJoint* joint = ( b2FrictionJoint* )self->mJoint;
	joint->SetMaxTorque ( maxTorque );
	
	return 0;
}

//================================================================//
// MOAIBox2DFrictionJoint
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DFrictionJoint::MOAIBox2DFrictionJoint () {
	
	RTTI_BEGIN ( MOAIBox2DFrictionJoint )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIBox2DFrictionJoint >)
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DFrictionJoint::~MOAIBox2DFrictionJoint () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DFrictionJoint::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIBox2DFrictionJoint::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getMaxForce",			_getMaxForce },
		{ "getMaxTorque",			_getMaxTorque },
		{ "setMaxForce",			_setMaxForce },
		{ "setMaxTorque",			_setMaxTorque },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

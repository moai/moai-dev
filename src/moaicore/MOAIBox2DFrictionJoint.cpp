// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DFrictionJoint.h>
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAILogMessages.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getMaxForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DFrictionJoint self
	@out	number maxForce in kg * units / s^2, converted from N [kg * m / s^2].
*/
int MOAIBox2DFrictionJoint::_getMaxForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DFrictionJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2FrictionJoint* joint = ( b2FrictionJoint* )self->mJoint;
	state.Push ( joint->GetMaxForce () / unitsToMeters );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMaxTorque
	@text	See Box2D documentation.
	
	@in		MOAIBox2DFrictionJoint self
	@out	number maxTorque		in (kg * units / s^2) * units, converted from N-m.
*/
int MOAIBox2DFrictionJoint::_getMaxTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DFrictionJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2FrictionJoint* joint = ( b2FrictionJoint* )self->mJoint;
	float unitsToMeters = self->GetUnitsToMeters();
	/* Convert to/from N-m (kg m / s^2) * m from/to (kg unit / s^2) * unit */
	state.Push ( joint->GetMaxTorque () / ( unitsToMeters * unitsToMeters ) );

	return 1;
}

//----------------------------------------------------------------//
/**	@name	setMaxForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DFrictionJoint self
	@opt	number maxForce		in kg * units / s^2, converted to N [kg * m / s^2]. Default value is 0.
	@out	nil
*/
int MOAIBox2DFrictionJoint::_setMaxForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DFrictionJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float maxForce = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;

	b2FrictionJoint* joint = ( b2FrictionJoint* )self->mJoint;
	joint->SetMaxForce ( maxForce );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMaxTorque
	@text	See Box2D documentation.
	
	@in		MOAIBox2DFrictionJoint self
	@opt	number maxTorque		in (kg * units / s^2) * units, converted to N-m. Default value is 0.
	@out	nil
*/
int MOAIBox2DFrictionJoint::_setMaxTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DFrictionJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
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
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DFrictionJoint::~MOAIBox2DFrictionJoint () {
}

//----------------------------------------------------------------//
void MOAIBox2DFrictionJoint::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DFrictionJoint::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "_getMaxForce",			_getMaxForce },
		{ "_getMaxTorque",			_getMaxTorque },
		{ "_setMaxForce",			_setMaxForce },
		{ "_setMaxTorque",			_setMaxTorque },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif

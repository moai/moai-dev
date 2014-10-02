// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DBody.h>
#include <moai-box2d/MOAIBox2DMotorJoint.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@lua	setLinearOffset
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMotorJoint self
	@opt	number x	target linear offset, in frame A, in meters
	@opt	number y	target linear offset, in frame A, in meters
	@out	nil	
*/
int MOAIBox2DMotorJoint::_setLinearOffset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMotorJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float unitsToMeters = self->GetUnitsToMeters();

	float x = state.GetValue < float >( 2, 0.0f );
	float y = state.GetValue < float >( 3, 0.0f );

	b2MotorJoint* joint = ( b2MotorJoint* )self->mJoint;
	b2Vec2 offset( x * unitsToMeters, y * unitsToMeters );
	joint->SetLinearOffset( offset );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getLinearOffset
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMotorJoint self
	@out	number x	target linear offset, in frame A, in meters 
	@out	number y	target linear offset, in frame A, in meters
*/

int MOAIBox2DMotorJoint::_getLinearOffset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMotorJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float unitsToMeters = self->GetUnitsToMeters();

	b2MotorJoint* joint = ( b2MotorJoint* )self->mJoint;
	const b2Vec2& offset = joint->GetLinearOffset();
	
	state.Push ( offset.x / unitsToMeters );
	state.Push ( offset.y / unitsToMeters );

	return 2;
}

//----------------------------------------------------------------//
/**	@lua	setAngularOffset
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMotorJoint self
	@opt	number angle	in degrees
	@out	nil	
*/
int	MOAIBox2DMotorJoint::_setAngularOffset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMotorJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float angle = state.GetValue < float >( 2, 0.0f );

	b2MotorJoint* joint = ( b2MotorJoint* )self->mJoint;
	joint->SetAngularOffset( angle * ( float )D2R );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getAngularOffset
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMotorJoint self
	@out	number angle	in degrees, converted from radians
*/
int	MOAIBox2DMotorJoint::_getAngularOffset ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMotorJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2MotorJoint* joint = ( b2MotorJoint* )self->mJoint;
	state.Push ( joint->GetAngularOffset () * ( float )R2D );
	
	return 1;	
}

//----------------------------------------------------------------//
/**	@lua	setMaxForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMotorJoint self
	@opt	number force	maximum friction force in kg * units / s^2, converted to N [kg * m / s^2]
	@out	nil
*/
int	MOAIBox2DMotorJoint::_setMaxForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMotorJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float unitsToMeters = self->GetUnitsToMeters();
	float force = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;

	b2MotorJoint* joint = ( b2MotorJoint* )self->mJoint;
	joint->SetMaxForce( force );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	getMaxForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DMotorJoint self
	@out	number force	maximum friction force in kg * units / s^2, converted from N [kg * m / s^2].
*/
int	MOAIBox2DMotorJoint::_getMaxForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMotorJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float unitsToMeters = self->GetUnitsToMeters ();

	b2MotorJoint* joint = ( b2MotorJoint* )self->mJoint;
	state.Push ( joint->GetMaxForce() / unitsToMeters );
	
	return 1;	
}

//----------------------------------------------------------------//
/** @lua	setMaxTorque
    @text	See Box2D documentation.
    
	@in		MOAIBox2DMotorJoint self
    @opt	number	torque	maximum friction torque in kg * units / s^2 * units, converted to N-m [kg * m / s^2 * m].		
	@out	nil
*/
int	MOAIBox2DMotorJoint::_setMaxTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMotorJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float unitsToMeters = self->GetUnitsToMeters();
	/* Convert to/from N-m (kg m / s^2) * m from/to (kg unit / s^2) * unit */
	float torque = state.GetValue < float >( 2, 0.0f ) * unitsToMeters * unitsToMeters;

	b2MotorJoint* joint = ( b2MotorJoint* )self->mJoint;
	joint->SetMaxTorque( torque );

	return 0;
}
 
//----------------------------------------------------------------//
/** @lua	getMaxTorque
    @text	See Box2D documentation.
    
	@in		MOAIBox2DMotorJoint self
    @out	number	torque	maximum friction torque in (kg * units / s^2) * units, converted from N-m.		
*/
int MOAIBox2DMotorJoint::_getMaxTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMotorJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2MotorJoint* joint = ( b2MotorJoint* )self->mJoint;
	float unitsToMeters = self->GetUnitsToMeters();
	/* Convert to/from N-m (kg m / s^2) * m from/to (kg unit / s^2) * unit */
	state.Push ( joint->GetMaxTorque() / ( unitsToMeters * unitsToMeters ) );
	
	return 1;
}

//----------------------------------------------------------------//
/** @lua	setCorrectionFactor
    @text	See Box2D documentation.
    
	@in		MOAIBox2DMotorJoint self
    @opt	number	factor		position correction factor in the range [0,1]
	@out	nil
*/
int MOAIBox2DMotorJoint::_setCorrectionFactor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMotorJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float factor = state.GetValue < float >( 2, 0.0f );

	b2MotorJoint* joint = ( b2MotorJoint* )self->mJoint;
	joint->SetCorrectionFactor( factor );
		
	return 0;
}

//----------------------------------------------------------------//
/** @lua	getCorrectionFactor
    @text	See Box2D documentation.
    
	@in		MOAIBox2DMotorJoint self
    @out	number	factor		position correction factor in the range [0,1]
*/
int MOAIBox2DMotorJoint::_getCorrectionFactor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DMotorJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2MotorJoint* joint = ( b2MotorJoint* )self->mJoint;
	state.Push ( joint->GetCorrectionFactor() );
	
	return 1;	
}

//================================================================//
// MOAIBox2DRevoluteJoint
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DMotorJoint::MOAIBox2DMotorJoint () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DMotorJoint::~MOAIBox2DMotorJoint () {
}

//----------------------------------------------------------------//
void MOAIBox2DMotorJoint::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DMotorJoint::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "setLinearOffset", 		_setLinearOffset },
		{ "getLinearOffset", 		_getLinearOffset },
		{ "setAngularOffset", 		_setAngularOffset },
		{ "getAngularOffset", 		_getAngularOffset },
		{ "setMaxForce", 			_setMaxForce },
		{ "getMaxForce", 			_getMaxForce },
		{ "setMaxTorque", 			_setMaxTorque },
		{ "getMaxTorque", 			_getMaxTorque },
		{ "getMaxTorque", 			_setCorrectionFactor },
		{ "getCorrectionFactor", 	_getCorrectionFactor },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

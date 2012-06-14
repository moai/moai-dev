// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DWheelJoint.h>
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAISim.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//

//================================================================//
// MOAIBox2DWheelJoint
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DWheelJoint::MOAIBox2DWheelJoint () {
	
	RTTI_BEGIN
	RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
/**	@name	getJointTranslation
 @text	See Box2D documentation.
 
 @in		MOAIBox2DWheelJoint self
 @out	number jointTranslation in units, converted from meters
 */
int MOAIBox2DWheelJoint::_getJointTranslation ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	state.Push ( joint->GetJointTranslation () / unitsToMeters );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	getJointSpeed
 @text	See Box2D documentation.
 
 @in		MOAIBox2DWheelJoint self
 @out	number jointSpeed in units / s, converted from m/s
 */
int MOAIBox2DWheelJoint::_getJointSpeed ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	state.Push ( joint->GetJointSpeed () / unitsToMeters );
	
	return 1;
}
//----------------------------------------------------------------//
/**	@name	isMotorEnabled
 @text	See Box2D documentation.
 
 @in		MOAIBox2DWheelJoint self
 @out	boolean motorEnabled
 */
int MOAIBox2DWheelJoint::_isMotorEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	state.Push ( joint->IsMotorEnabled ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMotorSpeed
 @text	See Box2D documentation.
 
 @in		MOAIBox2DWheelJoint self
 @out	number motorSpeed in degrees/s, converted from radians/s
 */
int MOAIBox2DWheelJoint::_getMotorSpeed ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	state.Push ( joint->GetMotorSpeed () * ( float ) R2D );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getSpringDampingRatio
 @text	See Box2D documentation.
 
 @in		MOAIBox2DWheelJoint self
 @out	number	dampingRatio
 */
int MOAIBox2DWheelJoint::_getSpringDampingRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	state.Push ( joint->GetSpringDampingRatio() );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMaxMotorTorque
 @text	See Box2D documentation.
 
 @in		MOAIBox2DWheelJoint self
 @out	number maxMotorTorque		in (kg * units / s^2) * units, converted from N-m.
 */
int MOAIBox2DWheelJoint::_getMaxMotorTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	/* Convert from N-m (kg m / s^2) * m to (kg unit / s^2) * unit */
	state.Push ( joint->GetMaxMotorTorque () / ( unitsToMeters * unitsToMeters ) );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMotorTorque
 @text	See Box2D documentation.
 
 @in		MOAIBox2DWheelJoint self
 @out	number torque		in (kg * units / s^2) * units, converted from N-m.
 */
int MOAIBox2DWheelJoint::_getMotorTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;

	float step = ( float )( 1.0 / MOAISim::Get ().GetStep ());
	/* Convert from N-m (kg m / s^2) * m => (kg unit / s^2) * unit */
	state.Push ( joint->GetMotorTorque (step) / ( unitsToMeters * unitsToMeters ) );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getSpringFrequencyHz
 @text	See Box2D documentation.
 
 @in		MOAIBox2DWheelJoint self
 @out	number springFrequency in Hz
 */
int MOAIBox2DWheelJoint::_getSpringFrequencyHz ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	state.Push ( joint->GetSpringFrequencyHz() );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setMotor
	@text	See Box2D documentation.
			If speed is determined to be zero, the motor is disabled, unless forceEnable is set.

	@in		MOAIBox2DWheelJoint self
	@opt	number speed			in degrees/s, converted to radians/s. Default value is 0.
	@opt	number maxMotorTorque		in (kg * units / s^2) * units, converted from N-m. Default value is 0.
	@opt	boolean forceEnable		Default value is false.
	@out	nil
*/
int MOAIBox2DWheelJoint::_setMotor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	float unitsToMeters = self->GetUnitsToMeters ();
	float speed	= state.GetValue < float >( 2, 0.0f );
	float max	= state.GetValue < float >( 3, 0.0f );
	bool forceEnable = state.GetValue < bool >( 4, false );
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	joint->SetMotorSpeed ( speed * ( float )D2R );
	/* Convert from N-m (kg m / s^2) * m => (kg unit / s^2) * unit */
	joint->SetMaxMotorTorque ( max * unitsToMeters * unitsToMeters );
	joint->EnableMotor ( forceEnable ? true : ( speed != 0.0f ) );
	
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMotorSpeed
 @text	See Box2D documentation.
 
 @in	MOAIBox2DWheelJoint self
 @opt	number motorSpeed		in degrees/s, converted to radians/s. Default value is 0.

 */
int MOAIBox2DWheelJoint::_setMotorSpeed ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float speed = state.GetValue < float >( 2, 0.0f ) * ( float )D2R;

	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	joint->SetMotorSpeed ( speed );

	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMotorEnabled
 @text	See Box2D documentation.
 
 @in		MOAIBox2DWheelJoint self
 @opt	boolean enabled			Default value is 'true'
 @out	nil
 */
int MOAIBox2DWheelJoint::_setMotorEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	bool enabled = state.GetValue < bool >( 2, true );
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	joint->EnableMotor ( enabled );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMaxMotorTorque
 @text	See Box2D documentation.
 
 @in		MOAIBox2DWheelJoint self
 @opt	number maxMotorTorque		in (kg * units / s^2) * units, converted to N-m. Default value is 0.
 @out	nil
 */
int MOAIBox2DWheelJoint::_setMaxMotorTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	/* Convert to N-m (kg m / s^2) * m from (kg unit / s^2) * unit */
	float maxMotorTorque = state.GetValue < float >( 2, 0.0f ) * unitsToMeters * unitsToMeters;
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	joint->SetMaxMotorTorque ( maxMotorTorque );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSpringDampingRatio
 @text	See Box2D documentation.
 
 @in		MOAIBox2DWheelJoint self
 @opt	number dampingRatio		Default value is 0.
 @out	nil
 */
int MOAIBox2DWheelJoint::_setSpringDampingRatio ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	float dampingRatio = state.GetValue < float >( 2, 0.0f );
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	joint->SetSpringDampingRatio( dampingRatio );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setSpringFrequencyHz
 @text	See Box2D documentation.
 
 @in		MOAIBox2DWheelJoint self
 @opt	number springFrequencyHz		in Hz. Default value is 0.
 @out	nil
 */
int MOAIBox2DWheelJoint::_setSpringFrequencyHz ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DWheelJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	float springFrequencyHz = state.GetValue < float >( 2, 0.0f );
	
	b2WheelJoint* joint = ( b2WheelJoint* )self->mJoint;
	joint->SetSpringFrequencyHz( springFrequencyHz );
	
	return 0;
}

//----------------------------------------------------------------//
MOAIBox2DWheelJoint::~MOAIBox2DWheelJoint () {
}

//----------------------------------------------------------------//
void MOAIBox2DWheelJoint::RegisterLuaClass ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DWheelJoint::RegisterLuaFuncs ( MOAILuaState& state ) {
	MOAIBox2DJoint::RegisterLuaFuncs ( state );
	luaL_Reg regTable [] = {
		{ "getJointTranslation",		_getJointTranslation },
		{ "getJointSpeed",				_getJointSpeed },
		{ "isMotorEnabled",				_isMotorEnabled },
		{ "getMotorSpeed",				_getMotorSpeed },
		{ "getSpringDampingRatio",		_getSpringDampingRatio },
		{ "getMaxMotorTorque",			_getMaxMotorTorque },
		{ "getMotorTorque",				_getMotorTorque },
		{ "getSpringFrequencyHz",		_getSpringFrequencyHz },
		{ "setMotor",					_setMotor },
		{ "setMotorEnabled",			_setMotorEnabled },
		{ "setMotorSpeed",				_setMotorSpeed },
		{ "setMaxMotorTorque",			_setMaxMotorTorque },
		{ "setSpringDampingRatio",		_setSpringDampingRatio },
		{ "setSpringFrequencyHz",		_setSpringFrequencyHz },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif
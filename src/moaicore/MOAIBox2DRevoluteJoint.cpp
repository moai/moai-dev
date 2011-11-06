// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <Box2D/Box2D.h>
#include <moaicore/MOAIBox2DArbiter.h>
#include <moaicore/MOAIBox2DBody.h>
#include <moaicore/MOAIBox2DRevoluteJoint.h>
#include <moaicore/MOAIBox2DWorld.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAISim.h>

SUPPRESS_EMPTY_FILE_WARNING
#if USE_BOX2D

//================================================================//
// local
//================================================================//

//----------------------------------------------------------------//
/**	@name	getJointAngle
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@out	number angle
*/
int MOAIBox2DRevoluteJoint::_getJointAngle ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	state.Push ( joint->GetJointAngle () * ( float )R2D );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getJointSpeed
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@out	number jointSpeed
*/
int MOAIBox2DRevoluteJoint::_getJointSpeed ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	state.Push ( joint->GetJointSpeed () * ( float )R2D );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getLowerLimit
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@out	number lowerLimit
*/
int MOAIBox2DRevoluteJoint::_getLowerLimit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	state.Push ( joint->GetLowerLimit () * ( float )R2D );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMotorSpeed
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@out	number motorSpeed
*/
int MOAIBox2DRevoluteJoint::_getMotorSpeed ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	state.Push ( joint->GetMotorSpeed () * ( float )R2D );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getMotorTorque
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@out	number motorTorque
*/
int MOAIBox2DRevoluteJoint::_getMotorTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	float step = ( float )( 1.0 / MOAISim::Get ().GetStep ());
	state.Push ( joint->GetMotorTorque (step) * ( float )R2D );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getUpperLimit
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@out	number upperLimit
*/
int MOAIBox2DRevoluteJoint::_getUpperLimit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	state.Push ( joint->GetUpperLimit () * ( float )R2D );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isLimitEnabled
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@out	boolean limitEnabled
*/
int MOAIBox2DRevoluteJoint::_isLimitEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	state.Push ( joint->IsLimitEnabled ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	isMotorEnabled
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@out	boolean motorEnabled
*/
int MOAIBox2DRevoluteJoint::_isMotorEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	state.Push ( joint->IsMotorEnabled ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setLimit
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@opt	number lower			Default value is 0.
	@opt	number upper			Default value is 0.
	@out	nil
*/
int MOAIBox2DRevoluteJoint::_setLimit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	float lower	= state.GetValue < float >( 2, 0.0f );
	float upper	= state.GetValue < float >( 3, 0.0f );
	
	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	joint->SetLimits ( lower * ( float )D2R, upper * ( float )D2R );
	joint->EnableLimit ( true );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setLimitEnabled
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@opt	boolean enabled			Default value is 'true'
	@out	nil
*/
int MOAIBox2DRevoluteJoint::_setLimitEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	bool enabled = state.GetValue < bool >( 2, true );
	
	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	joint->EnableLimit ( enabled );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMaxMotorTorque
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@opt	number maxMotorTorque		Default value is 0.
	@out	nil
*/
int MOAIBox2DRevoluteJoint::_setMaxMotorTorque ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )

	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float maxMotorTorque = state.GetValue < float >( 1, 0.0f ) * ( float )D2R;

	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	joint->SetMaxMotorTorque ( maxMotorTorque );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMotor
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@opt	number speed			Default value is 0.
	@opt	number max				Default value is 0.
	@out	nil
*/
int MOAIBox2DRevoluteJoint::_setMotor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	float speed	= state.GetValue < float >( 2, 0.0f );
	float max	= state.GetValue < float >( 3, 0.0f );
	
	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	joint->SetMotorSpeed ( speed * ( float )D2R );
	joint->SetMaxMotorTorque ( max * ( float )D2R );
	joint->EnableMotor ( speed != 0.0f );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setMotorEnabled
	@text	See Box2D documentation.
	
	@in		MOAIBox2DRevoluteJoint self
	@opt	boolean enabled			Default value is 'true'
	@out	nil
*/
int MOAIBox2DRevoluteJoint::_setMotorEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DRevoluteJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILog ( state, MOAILogMessages::MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	bool enabled = state.GetValue < bool >( 2, true );
	
	b2RevoluteJoint* joint = ( b2RevoluteJoint* )self->mJoint;
	joint->EnableMotor ( enabled );
	
	return 0;
}

//================================================================//
// MOAIBox2DRevoluteJoint
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DRevoluteJoint::MOAIBox2DRevoluteJoint () {
	
	RTTI_BEGIN
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DRevoluteJoint::~MOAIBox2DRevoluteJoint () {
}

//----------------------------------------------------------------//
void MOAIBox2DRevoluteJoint::RegisterLuaClass ( USLuaState& state ) {
	MOAIBox2DJoint::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIBox2DRevoluteJoint::RegisterLuaFuncs ( USLuaState& state ) {
	MOAIBox2DJoint::RegisterLuaFuncs ( state );

	luaL_Reg regTable [] = {
		{ "getJointAngle",				_getJointAngle },
		{ "getJointSpeed",				_getJointSpeed },
		{ "getLowerLimit",				_getLowerLimit },
		{ "getMotorSpeed",				_getMotorSpeed },
		{ "getMotorTorque",				_getMotorTorque },
		{ "getUpperLimit",				_getUpperLimit },
		{ "isLimitEnabled",				_isLimitEnabled },
		{ "isMotorEnabled",				_isMotorEnabled },
		{ "setLimit",					_setLimit },
		{ "setLimitEnabled",			_setLimitEnabled },
		{ "setMaxMotorTorque",			_setMaxMotorTorque },
		{ "setMotor",					_setMotor },
		{ "setMotorEnabled",			_setMotorEnabled },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

#endif

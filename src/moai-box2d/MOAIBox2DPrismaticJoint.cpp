// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <Box2D/Box2D.h>
#include <moai-box2d/MOAIBox2DArbiter.h>
#include <moai-box2d/MOAIBox2DBody.h>
#include <moai-box2d/MOAIBox2DPrismaticJoint.h>
#include <moai-box2d/MOAIBox2DWorld.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getJointSpeed
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPrismaticJoint self
	@out	number jointSpeed				in units/s, converted from m/s
*/
int MOAIBox2DPrismaticJoint::_getJointSpeed ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	state.Push ( joint->GetJointSpeed () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getJointTranslation
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPrismaticJoint self
	@out	number jointTranslation			in units, converted from meters.
*/
int MOAIBox2DPrismaticJoint::_getJointTranslation ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	state.Push ( joint->GetJointTranslation () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getLowerLimit
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPrismaticJoint self
	@out	number lowerLimit				in units, converted from meters.
*/
int MOAIBox2DPrismaticJoint::_getLowerLimit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	state.Push ( joint->GetLowerLimit () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMotorForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPrismaticJoint self
	@out	number motorForce				in kg * units / s^2, converted from N [kg * m / s^2]
*/
int MOAIBox2DPrismaticJoint::_getMotorForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;

	float step = ( float )( 1.0 / self->Get < MOAIUpdateMgr >().GetStep ());
	state.Push ( joint->GetMotorForce (step) / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getMotorSpeed
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPrismaticJoint self
	@out	number motorSpeed				in units/s, converted from m/s
*/
int MOAIBox2DPrismaticJoint::_getMotorSpeed ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	state.Push ( joint->GetMotorSpeed () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	getUpperLimit
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPrismaticJoint self
	@out	number upperLimit				in units, converted from meters.
*/
int MOAIBox2DPrismaticJoint::_getUpperLimit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	state.Push ( joint->GetUpperLimit () / unitsToMeters );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	isLimitEnabled
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPrismaticJoint self
	@out	boolean limitEnabled
*/
int MOAIBox2DPrismaticJoint::_isLimitEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	state.Push ( joint->IsLimitEnabled ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	isMotorEnabled
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPrismaticJoint self
	@out	boolean motorEnabled
*/
int MOAIBox2DPrismaticJoint::_isMotorEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	state.Push ( joint->IsMotorEnabled ());
	
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setLimit
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPrismaticJoint self
	@opt	number lower			in units, converted to meters. Default value is 0.
	@opt	number upper			in units, converted to meters. Default value is 0.
	@out	nil
*/
int MOAIBox2DPrismaticJoint::_setLimit ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	float lower	= state.GetValue < float >( 2, 0.0f );
	float upper	= state.GetValue < float >( 3, 0.0f );
	
	float unitsToMeters = self->GetUnitsToMeters ();
	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	
	joint->SetLimits ( lower * unitsToMeters, upper * unitsToMeters );
	joint->EnableLimit ( true );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setLimitEnabled
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPrismaticJoint self
	@opt	boolean enabled			Default value is 'true'
	@out	nil
*/
int MOAIBox2DPrismaticJoint::_setLimitEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	bool enabled = state.GetValue < bool >( 2, true );
	
	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	joint->EnableLimit ( enabled );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setMaxMotorForce
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPrismaticJoint self
	@opt	number maxMotorForce		in kg * units / s^2, converted to N [kg * m / s^2]. Default value is 0.
	@out	nil
*/
int MOAIBox2DPrismaticJoint::_setMaxMotorForce ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )
	float unitsToMeters = self->GetUnitsToMeters ();

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float maxMotorForce = state.GetValue < float >( 2, 0.0f );

	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	joint->SetMaxMotorForce ( maxMotorForce * unitsToMeters );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setMotor
	@text	See Box2D documentation.
			If speed is determined to be zero, the motor is disabled, unless forceEnable is set.
	
	@in		MOAIBox2DPrismaticJoint self
	@opt	number speed			in units/s converted to m/s. Default value is 0.
	@opt	number maxForce			in kg * units / s^2, converted to N [kg * m / s^2]. Default value is 0.
	@opt	boolean forceEnable		Default value is false.
	@out	nil
*/
int MOAIBox2DPrismaticJoint::_setMotor ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	float speed	= state.GetValue < float >( 2, 0.0f );
	float max	= state.GetValue < float >( 3, 0.0f );
	bool forceEnable = state.GetValue < bool >( 4, false );
	
	float unitsToMeters = self->GetUnitsToMeters ();
	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	
	joint->SetMotorSpeed ( speed * unitsToMeters );
	joint->SetMaxMotorForce ( max * unitsToMeters );
	joint->EnableMotor ( forceEnable ? true : ( speed != 0.0f ) );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setMotorEnabled
	@text	See Box2D documentation.
	
	@in		MOAIBox2DPrismaticJoint self
	@opt	boolean enabled			Default value is 'true'
	@out	nil
*/
int MOAIBox2DPrismaticJoint::_setMotorEnabled ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )
	
	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}
	
	bool enabled = state.GetValue < bool >( 2, true );
	
	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	joint->EnableMotor ( enabled );
	
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setMotorSpeed
	@text	See Box2D documentation.

	@in		MOAIBox2DPrismaticJoint self
	@opt	number motorSpeed		in units/s, converted to m/s. Default value is 0.
	@out	nil

 */
int MOAIBox2DPrismaticJoint::_setMotorSpeed ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIBox2DPrismaticJoint, "U" )

	if ( !self->mJoint ) {
		MOAILogF ( state, ZLLog::LOG_ERROR, MOAISTRING_MOAIBox2DJoint_MissingInstance );
		return 0;
	}

	float unitsToMeters = self->GetUnitsToMeters ();
	float speed = state.GetValue < float >( 2, 0.0f ) * unitsToMeters;

	b2PrismaticJoint* joint = ( b2PrismaticJoint* )self->mJoint;
	joint->SetMotorSpeed ( speed );

	return 0;
}


//================================================================//
// MOAIBox2DPrismaticJoint
//================================================================//

//----------------------------------------------------------------//
MOAIBox2DPrismaticJoint::MOAIBox2DPrismaticJoint ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAIBox2DPrim ( context ),
	MOAIBox2DJoint ( context ) {
	
	RTTI_BEGIN ( MOAIBox2DPrismaticJoint)
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIBox2DPrismaticJoint >)
		RTTI_EXTEND ( MOAIBox2DJoint )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIBox2DPrismaticJoint::~MOAIBox2DPrismaticJoint () {
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIBox2DPrismaticJoint::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIBox2DPrismaticJoint::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getJointSpeed",				_getJointSpeed },
		{ "getJointTranslation",		_getJointTranslation },
		{ "getLowerLimit",				_getLowerLimit },
		{ "getMotorForce",				_getMotorForce },
		{ "getMotorSpeed",				_getMotorSpeed },
		{ "getUpperLimit",				_getUpperLimit },
		{ "isLimitEnabled",				_isLimitEnabled },
		{ "isMotorEnabled",				_isMotorEnabled },
		{ "setLimit",					_setLimit },
		{ "setLimitEnabled",			_setLimitEnabled },
		{ "setMaxMotorForce",			_setMaxMotorForce },
		{ "setMotor",					_setMotor },
		{ "setMotorSpeed",				_setMotorSpeed },
		{ "setMotorEnabled",			_setMotorEnabled },
		{ NULL, NULL }
	};
	
	luaL_register ( state, 0, regTable );
}

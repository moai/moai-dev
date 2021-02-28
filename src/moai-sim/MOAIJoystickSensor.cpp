// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIJoystickSensor.h>
#include <moai-sim/MOAIInputMgr.h>

//================================================================//
// MOAIJoystickEvent
//================================================================//
class MOAIJoystickEvent {
public:

	float	mX;
	float	mY;
};

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	getVector
	@text	Returns the joystick vector.

	@in		MOAIJoystickSensor self
	@out	number x
	@out	number y
*/
int MOAIJoystickSensor::_getVector ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIJoystickSensor, "U" )
	
	lua_pushnumber ( state, self->mX );
	lua_pushnumber ( state, self->mY );
	return 2;
}

//----------------------------------------------------------------//
/**	@lua	setCallback
	@text	Sets or clears the callback to be issued when the joystick vector changes.

	@in		MOAIJoystickSensor self
	@opt	function callback			Default value is nil.
	@out	nil
*/
int MOAIJoystickSensor::_setCallback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIJoystickSensor, "U" )
	
	self->mOnStick.SetRef ( state, 2 );
	
	return 0;
}

//================================================================//
// MOAIJoystickSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIJoystickSensor::EnqueueJoystickEvent ( MOAIInputMgr& inputMgr, ZLIndex deviceID, ZLIndex sensorID, float x, float y ) {

	if ( inputMgr.WriteEventHeader < MOAIJoystickSensor >( deviceID, sensorID )) {
		inputMgr.Write < float >( x );
		inputMgr.Write < float >( y );
	}
}

//----------------------------------------------------------------//
MOAIJoystickSensor::MOAIJoystickSensor ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	MOAISensor ( context ) {
	
	RTTI_BEGIN ( MOAIJoystickSensor )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIJoystickSensor >)
		RTTI_EXTEND ( MOAISensor )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIJoystickSensor::~MOAIJoystickSensor () {
}

//----------------------------------------------------------------//
void MOAIJoystickSensor::ParseEvent ( ZLStream& eventStream ) {

	this->mX = eventStream.Read < float >( 0.0f );
	this->mY = eventStream.Read < float >( 0.0f );
	
	if ( this->mOnStick ) {
		MOAIScopedLuaState state = this->mOnStick.GetSelf ();
		lua_pushnumber ( state, this->mX );
		lua_pushnumber ( state, this->mY );
		state.DebugCall ( 2, 0 );
	}
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIJoystickSensor::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;
}

//----------------------------------------------------------------//
void MOAIJoystickSensor::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getVector",		_getVector },
		{ "setCallback",	_setCallback },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

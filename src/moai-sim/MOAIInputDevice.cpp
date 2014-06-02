// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIButtonSensor.h>
#include <moai-sim/MOAICompassSensor.h>
#include <moai-sim/MOAIInputDevice.h>
#include <moai-sim/MOAIJoystickSensor.h>
#include <moai-sim/MOAIKeyboardSensor.h>
#include <moai-sim/MOAILocationSensor.h>
#include <moai-sim/MOAIMotionSensor.h>
#include <moai-sim/MOAIPointerSensor.h>
#include <moai-sim/MOAITouchSensor.h>
#include <moai-sim/MOAISensor.h>
#include <moai-sim/MOAIWheelSensor.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIInputDevice::_getExtendedName( lua_State* L )
{
	MOAI_LUA_SETUP ( MOAIInputDevice, "U" )

		lua_pushstring(state, self->mNameExtended.c_str());

	return 1;
}

//================================================================//
// MOAIInputDevice
//================================================================//

//----------------------------------------------------------------//
MOAISensor* MOAIInputDevice::GetSensor ( u8 sensorID ) {
	
	if ( sensorID < this->mSensors.Size ()) {
		return this->mSensors [ sensorID ];
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIInputDevice::MOAIInputDevice () :
	mIsActive ( true ) {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIInputDevice::~MOAIInputDevice () {

	for ( u32 i = 0; i < this->mSensors.Size (); ++i ) {
		this->LuaRelease ( this->mSensors [ i ]);
	}
}

//----------------------------------------------------------------//
void MOAIInputDevice::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "getExtendedName",	_getExtendedName },
		{ "new",					MOAILogMessages::_alertNewIsUnsupported },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIInputDevice::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIInputDevice::ReserveSensors ( u8 total ) {

	this->mSensors.Init ( total );
	this->mSensors.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIInputDevice::ResetSensors () {

	for ( u32 i = 0; i < this->mSensors.Size (); ++i ) {
		MOAISensor* sensor = this->mSensors [ i ];
		if ( sensor ) {
			sensor->Reset ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIInputDevice::SetExtendedName( cc8* nameExtended )
{
	this->mNameExtended = nameExtended;
}

//----------------------------------------------------------------//
void MOAIInputDevice::SetSensor ( u8 sensorID, cc8* name, u32 type ) {

	if ( !( sensorID < this->mSensors.Size ())) return;

	MOAISensor* sensor = 0;

	switch ( type ) {
		case MOAISensor::BUTTON: {
			sensor = new MOAIButtonSensor ();
			break;
		}
		case MOAISensor::COMPASS: {
			sensor = new MOAICompassSensor ();
			break;
		}
		case MOAISensor::JOYSTICK: {
			sensor = new MOAIJoystickSensor ();
			break;
		}
		case MOAISensor::KEYBOARD: {
			sensor = new MOAIKeyboardSensor ();
			break;
		}
		case MOAISensor::WHEEL: {
			sensor = new MOAIWheelSensor ();
			break;
		}
		case MOAISensor::LEVEL: {
			sensor = new MOAIMotionSensor ();
			break;
		}
		case MOAISensor::LOCATION: {
			sensor = new MOAILocationSensor ();
			break;
		}
		case MOAISensor::POINTER: {
			sensor = new MOAIPointerSensor ();
			break;
		}
		case MOAISensor::TOUCH: {
			sensor = new MOAITouchSensor ();
			break;
		}
	}
	
	if ( !sensor ) return;
	
	sensor->mType = type;
	sensor->mName = name;
	
	this->LuaRelease ( this->mSensors [ sensorID ]);
	
	this->mSensors [ sensorID ] = sensor;
	this->LuaRetain ( sensor );
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushLuaUserdata ( state );
	
	sensor->PushLuaUserdata ( state );
	lua_setfield ( state, -2, name );
}

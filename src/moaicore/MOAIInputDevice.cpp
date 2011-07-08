// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moaicore/MOAIButtonSensor.h>
#include <moaicore/MOAICompassSensor.h>
#include <moaicore/MOAIInputDevice.h>
#include <moaicore/MOAIJoystickSensor.h>
#include <moaicore/MOAIKeyboardSensor.h>
#include <moaicore/MOAILocationSensor.h>
#include <moaicore/MOAILogMessages.h>
#include <moaicore/MOAIMotionSensor.h>
#include <moaicore/MOAIPointerSensor.h>
#include <moaicore/MOAITouchSensor.h>
#include <moaicore/MOAISensor.h>

#define LUAVAR_SENSORS "sensors"

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
void MOAIInputDevice::HandleEvent ( u8 sensorID, USStream& eventStream ) {

	MOAISensor* sensor = this->GetSensor ( sensorID );
	if ( sensor ) {
		sensor->HandleEvent ( eventStream );
	}
}

//----------------------------------------------------------------//
MOAIInputDevice::MOAIInputDevice () :
	mIsActive ( true ) {
	
	RTTI_SINGLE ( USLuaObject )
}

//----------------------------------------------------------------//
MOAIInputDevice::~MOAIInputDevice () {
}

//----------------------------------------------------------------//
void MOAIInputDevice::RegisterLuaClass ( USLuaState& state ) {

	luaL_Reg regTable [] = {
		{ "new",					MOAILogMessages::_alertNewIsUnsupported },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIInputDevice::RegisterLuaFuncs ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIInputDevice::ReserveSensors ( u8 total ) {

	this->mSensors.Resize ( total );
}

//----------------------------------------------------------------//
void MOAIInputDevice::Reset () {

	for ( u32 i = 0; i < this->mSensors.Size (); ++i ) {
		MOAISensor* sensor = this->mSensors [ i ];
		if ( sensor ) {
			sensor->Reset ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIInputDevice::SetSensor ( u8 sensorID, cc8* name, u32 type ) {

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
	
	this->mSensors.Grow ( sensorID + 1 );
	this->mSensors [ sensorID ] = sensor;
	
	USLuaStateHandle state = USLuaRuntime::Get ().State ();
	this->PushLuaUserdata ( state );
	
	//if ( state.GetFieldWithType ( -1, LUAVAR_SENSORS, LUA_TTABLE )) {
		sensor->PushLuaUserdata ( state );
		lua_setfield ( state, -2, name );
	//}
}

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moaicore/MOAIJoystickSensor.h>
#include <moaicore/MOAILogMessages.h>

//================================================================//
// MOAIJoystickEvent
//================================================================//
class MOAIJoystickEvent {
public:

	float	mX;
	float	mY;
};

//================================================================//
// MOAIJoystickSensor
//================================================================//

//----------------------------------------------------------------//
void MOAIJoystickSensor::HandleEvent ( USStream& eventStream ) {
	UNUSED ( eventStream );
}

//----------------------------------------------------------------//
MOAIJoystickSensor::MOAIJoystickSensor () {

	RTTI_SINGLE ( MOAISensor )
}

//----------------------------------------------------------------//
MOAIJoystickSensor::~MOAIJoystickSensor () {
}

//----------------------------------------------------------------//
void MOAIJoystickSensor::RegisterLuaClass ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIJoystickSensor::RegisterLuaFuncs ( USLuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIJoystickSensor::WriteEvent ( USStream& eventStream, float x, float y ) {
	UNUSED ( eventStream );
	UNUSED ( x );
	UNUSED ( y );
}

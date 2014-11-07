// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIButtonSensor.h>
#include <moai-sim/MOAICompassSensor.h>
#include <moai-sim/MOAIInputDevice.h>
#include <moai-sim/MOAIInputMgr.h>
#include <moai-sim/MOAIJoystickSensor.h>
#include <moai-sim/MOAIKeyboardSensor.h>
#include <moai-sim/MOAILocationSensor.h>
#include <moai-sim/MOAIMotionSensor.h>
#include <moai-sim/MOAIPointerSensor.h>
#include <moai-sim/MOAISensor.h>
#include <moai-sim/MOAITouchSensor.h>
#include <moai-sim/MOAIWheelSensor.h>

#define LUAVAR_CONFIGURATION	"configuration"

//================================================================//
// MOAIInputMgr
//================================================================//

















//----------------------------------------------------------------//
MOAIInputMgr::MOAIInputMgr () {
	
	RTTI_SINGLE ( MOAIInputQueue )
}

//----------------------------------------------------------------//
MOAIInputMgr::~MOAIInputMgr () {
}

//----------------------------------------------------------------//
void MOAIInputMgr::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "deferEvents",				&MOAIInputQueue::_deferEvents < MOAIInputMgr > },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

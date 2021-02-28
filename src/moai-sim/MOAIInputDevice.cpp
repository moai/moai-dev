// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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
/**	@lua	getHardwareInfo
	@text	Get any hardware metadate string specified by the host.
	
	@in		MOAIInputDevice self
	@out	string hardwareInfo
*/
int MOAIInputDevice::_getHardwareInfo ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIInputDevice, "U" )
	
	lua_pushstring(state, self->mHardwareInfo.c_str ());
	return 1;
}

//================================================================//
// MOAIInputDevice
//================================================================//

//----------------------------------------------------------------//
void MOAIInputDevice::ClearSensorState () {

	for ( ZLIndex i = 0; i < this->mSensors.Size (); ++i ) {
		MOAISensor* sensor = this->mSensors [ i ];
		if ( sensor ) {
			sensor->ClearState ();
		}
	}
}

//----------------------------------------------------------------//
MOAISensor* MOAIInputDevice::GetSensor ( ZLIndex sensorID ) {
	
	if ( sensorID < this->mSensors.Size ()) {
		return this->mSensors [ sensorID ];
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIInputDevice::MOAIInputDevice ( ZLContext& context ) :
	ZLHasContext ( context ),
	MOAILuaObject ( context ),
	mIsActive ( true ) {
		
	RTTI_BEGIN ( MOAIInputDevice )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIInputDevice >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
}

//----------------------------------------------------------------//
MOAIInputDevice::~MOAIInputDevice () {

	for ( ZLIndex i = 0; i < this->mSensors.Size (); ++i ) {
		this->LuaRelease ( this->mSensors [ i ]);
	}
}

//----------------------------------------------------------------//
void MOAIInputDevice::ReserveSensors ( ZLSize total ) {

	this->mSensors.Init ( total );
	this->mSensors.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIInputDevice::ResetSensorState () {

	for ( ZLIndex i = 0; i < this->mSensors.Size (); ++i ) {
		MOAISensor* sensor = this->mSensors [ i ];
		if ( sensor ) {
			sensor->ResetState ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIInputDevice::SetHardwareInfo ( cc8* hardwareInfo ) {

	this->mHardwareInfo = hardwareInfo;
}

//----------------------------------------------------------------//
void MOAIInputDevice::SetSensor ( ZLIndex sensorID, cc8* name, MOAISensor* sensor ) {

	assert ( sensor );
	
	sensor->mName = name;
	
	this->LuaRelease ( this->mSensors [ sensorID ]);
	
	this->mSensors [ sensorID ] = sensor;
	this->LuaRetain ( sensor );
	
	MOAIScopedLuaState state = this->Get < MOAILuaRuntime >().State ();
	this->PushLuaUserdata ( state );
	
	sensor->PushLuaUserdata ( state );
	lua_setfield ( state, -2, name );
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIInputDevice::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	luaL_Reg regTable [] = {
		{ "getHardwareInfo",		_getHardwareInfo },
		{ "new",					MOAILuaObject::_alertNewIsUnsupported },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIInputDevice::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

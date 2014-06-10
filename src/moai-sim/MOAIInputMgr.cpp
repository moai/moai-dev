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
// MOAIInputMgr lua
//================================================================//

//----------------------------------------------------------------//
// TODO: doxygen
int MOAIInputMgr::_deferEvents ( lua_State* L ) {
	MOAILuaState state ( L );
	MOAIInputMgr::Get ().mDefer = state.GetValue < bool >( -1, false );
	return 0;
}

//================================================================//
// MOAIInputMgr
//================================================================//

//----------------------------------------------------------------//
bool MOAIInputMgr::CheckSensor ( u8 deviceID, u8 sensorID, u32 type ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device && device->mIsActive ) {
		MOAISensor* sensor = device->GetSensor ( sensorID );
		return ( sensor && ( sensor->mType == type ));
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueButtonEvent ( u8 deviceID, u8 sensorID, bool down ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::BUTTON )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAIButtonSensor::WriteEvent ( this->mEventQueue, down );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueCompassEvent ( u8 deviceID, u8 sensorID, float heading ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::COMPASS )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAICompassSensor::WriteEvent ( this->mEventQueue, heading );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueJoystickEvent( u8 deviceID, u8 sensorID, float x, float y ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::JOYSTICK )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAIJoystickSensor::WriteEvent ( this->mEventQueue, x, y );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueKeyboardEvent ( u8 deviceID, u8 sensorID, u32 keyID, bool down ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::KEYBOARD )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAIKeyboardSensor::WriteEvent ( this->mEventQueue, keyID, down );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueLevelEvent ( u8 deviceID, u8 sensorID, float x, float y, float z ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::LEVEL )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAIMotionSensor::WriteEvent ( this->mEventQueue, x, y, z );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueLocationEvent ( u8 deviceID, u8 sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::LOCATION )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAILocationSensor::WriteEvent ( this->mEventQueue, longitude, latitude, altitude, hAccuracy, vAccuracy, speed );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueuePointerEvent ( u8 deviceID, u8 sensorID, int x, int y ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::POINTER )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAIPointerSensor::WriteEvent ( this->mEventQueue, x, y );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueTouchEvent ( u8 deviceID, u8 sensorID, u32 touchID, bool down, float x, float y ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::TOUCH )) {
		this->WriteEventHeader ( deviceID, sensorID );
		float time = ( float )ZLDeviceTime::GetTimeInSeconds ();
		MOAITouchSensor::WriteEvent ( this->mEventQueue, touchID, down, x, y, time );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueTouchEventCancel ( u8 deviceID, u8 sensorID ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::TOUCH )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAITouchSensor::WriteEventCancel ( this->mEventQueue );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueWheelEvent ( u8 deviceID, u8 sensorID, float value ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::WHEEL )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAIWheelSensor::WriteEvent ( this->mEventQueue, value );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::FlushEvents ( double skip ) {

	this->mTimebase += skip;
	this->mEventQueue.DiscardAll ();
}

//----------------------------------------------------------------//
MOAIInputDevice* MOAIInputMgr::GetDevice ( u8 deviceID ) {

	if ( deviceID < this->mDevices.Size ()) {
		return this->mDevices [ deviceID ];
	}
	return 0;
}

//----------------------------------------------------------------//
MOAISensor* MOAIInputMgr::GetSensor ( u8 deviceID, u8 sensorID ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		return device->GetSensor ( sensorID );
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIInputMgr::MOAIInputMgr () :
	mTimebase ( 0 ),
	mTimestamp ( 0 ),
	mDefer ( false ) {
	
	RTTI_SINGLE ( MOAILuaObject )
	
	this->mEventQueue.SetChunkSize ( CHUNK_SIZE );
}

//----------------------------------------------------------------//
MOAIInputMgr::~MOAIInputMgr () {

	for ( u32 i = 0; i < this->mDevices.Size (); ++i ) {
		this->LuaRelease ( this->mDevices [ i ]);
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "deferEvents",				_deferEvents },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIInputMgr::ReserveDevices ( u8 total ) {

	this->mDevices.Init ( total );
	this->mDevices.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIInputMgr::ReserveSensors ( u8 deviceID, u8 total ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->ReserveSensors ( total );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::ResetSensors () {

	for ( u32 i = 0; i < this->mDevices.Size (); ++i ) {
		MOAIInputDevice* device = this->mDevices [ i ];
		if ( device ) {
			device->ResetSensors ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::SetConfigurationName ( cc8* name ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushLuaClassTable ( state );
	
	state.SetField ( -1, LUAVAR_CONFIGURATION, name );
}

//----------------------------------------------------------------//
void MOAIInputMgr::SetDevice ( u8 deviceID, cc8* name ) {

	if ( !( deviceID < this->mDevices.Size ())) return;

	MOAIInputDevice* device = new MOAIInputDevice ();
	device->SetName ( name );
	
	this->LuaRelease ( this->mDevices [ deviceID ]);
	
	this->mDevices [ deviceID ] = device;
	this->LuaRetain ( device );
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushLuaClassTable ( state );
	
	device->PushLuaUserdata ( state );
	lua_setfield ( state, -2, name );
}

//----------------------------------------------------------------//
void MOAIInputMgr::SetDeviceActive ( u8 deviceID, bool active ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->SetActive ( active );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::SetDeviceHardwareInfo ( u8 deviceID, cc8* hardwareInfo ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->SetHardwareInfo ( hardwareInfo );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::SetSensor ( u8 deviceID, u8 sensorID, cc8* name, u32 type ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->SetSensor ( sensorID, name, type );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::Update ( double timestep ) {

	// reset the input sensors
	this->ResetSensors ();

	size_t cursor = 0;

	if ( !this->mDefer ) {
		
		// rewind the event queue
		this->mEventQueue.Seek ( 0, SEEK_SET );
		
		bool first = true;
		double timebase = 0;
		
		// parse events until we run out or hit an event after the current sim time
		while ( this->mEventQueue.GetCursor () < this->mEventQueue.GetLength ()) {
			
			u8 deviceID			= this->mEventQueue.Read < u8 >( 0 );
			u8 sensorID			= this->mEventQueue.Read < u8 >( 0 );
			double timestamp	= this->mEventQueue.Read < double >( 0 );
			
			if ( first ) {
				timebase = timestamp;
				first = false;
			}
			
			if ( timestep < ( timestamp - timebase )) break;
			
			MOAISensor* sensor = this->GetSensor ( deviceID, sensorID );
			assert ( sensor );
			sensor->mTimestamp = timestamp;
			sensor->ParseEvent ( this->mEventQueue );
			
			cursor = this->mEventQueue.GetCursor ();
		}
		
		// discard processed events
		this->mEventQueue.DiscardFront ( cursor );
		
		// back to the end of the queue
		this->mEventQueue.Seek ( this->mEventQueue.GetLength (), SEEK_SET );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::WriteEventHeader ( u8 deviceID, u8 sensorID ) {

	this->mEventQueue.Write < u8 >(( u8 )deviceID );
	this->mEventQueue.Write < u8 >(( u8 )sensorID );
	this->mEventQueue.Write < double >( this->mTimestamp - this->mTimebase );
}

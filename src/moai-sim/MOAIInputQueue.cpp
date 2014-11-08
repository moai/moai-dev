// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIButtonSensor.h>
#include <moai-sim/MOAICompassSensor.h>
#include <moai-sim/MOAIInputDevice.h>
#include <moai-sim/MOAIInputQueue.h>
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
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIInputQueue::_deferEvents ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIInputQueue, "U" )

	bool defer = state.GetValue < bool >( 2, false );
	self->DeferEvents ( defer );

	return 0;
}

//================================================================//
// MOAIInputQueue
//================================================================//

//----------------------------------------------------------------//
bool MOAIInputQueue::CheckSensor ( u8 deviceID, u8 sensorID, u32 type ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device && device->mIsActive ) {
		MOAISensor* sensor = device->GetSensor ( sensorID );
		return ( sensor && ( sensor->mType == type ));
	}
	return false;
}

//----------------------------------------------------------------//
void MOAIInputQueue::DeferEvents ( bool defer ) {

	this->mDefer = defer;
}

//----------------------------------------------------------------//
void MOAIInputQueue::FlushEvents ( double skip ) {

	this->mTimebase += skip;
	this->DiscardAll ();
}

//----------------------------------------------------------------//
MOAIInputDevice* MOAIInputQueue::GetDevice ( u8 deviceID ) {

	if ( deviceID < this->mDevices.Size ()) {
		return this->mDevices [ deviceID ];
	}
	return 0;
}

//----------------------------------------------------------------//
MOAISensor* MOAIInputQueue::GetSensor ( u8 deviceID, u8 sensorID ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		return device->GetSensor ( sensorID );
	}
	return 0;
}

//----------------------------------------------------------------//
bool MOAIInputQueue::IsDone () {
	return false;
}

//----------------------------------------------------------------//
MOAIInputQueue::MOAIInputQueue () :
	mTimebase ( 0 ),
	mTimestamp ( 0 ),
	mDefer ( false ) {
	
	RTTI_SINGLE ( MOAIAction )
	
	this->SetChunkSize ( CHUNK_SIZE );
}

//----------------------------------------------------------------//
MOAIInputQueue::~MOAIInputQueue () {

	for ( u32 i = 0; i < this->mDevices.Size (); ++i ) {
		this->LuaRelease ( this->mDevices [ i ]);
	}
}

//----------------------------------------------------------------//
void MOAIInputQueue::OnUpdate ( double timestep ) {

	// reset the input sensors
	this->ResetSensors ();

	size_t cursor = 0;

	if ( !this->mDefer ) {
		
		// rewind the event queue
		this->Seek ( 0, SEEK_SET );
		
		bool first = true;
		double timebase = 0;
		
		// parse events until we run out or hit an event after the current sim time
		while ( this->GetCursor () < this->GetLength ()) {
			
			u8 deviceID			= this->Read < u8 >( 0 );
			u8 sensorID			= this->Read < u8 >( 0 );
			double timestamp	= this->Read < double >( 0 );
			
			if ( first ) {
				timebase = timestamp;
				first = false;
			}
			
			if ( timestep < ( timestamp - timebase )) break;
			
			MOAISensor* sensor = this->GetSensor ( deviceID, sensorID );
			assert ( sensor );
			sensor->mTimestamp = timestamp;
			sensor->ParseEvent ( *this );
			
			cursor = this->GetCursor ();
		}
		
		// discard processed events
		this->DiscardFront ( cursor );
		
		// back to the end of the queue
		this->Seek ( this->GetLength (), SEEK_SET );
	}
}

//----------------------------------------------------------------//
void MOAIInputQueue::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIInputQueue::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	luaL_Reg regTable [] = {
		{ "deferEvents",		_deferEvents },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIInputQueue::ReserveDevices ( u8 total ) {

	this->mDevices.Init ( total );
	this->mDevices.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIInputQueue::ReserveSensors ( u8 deviceID, u8 total ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->ReserveSensors ( total );
	}
}

//----------------------------------------------------------------//
void MOAIInputQueue::ResetSensors () {

	for ( u32 i = 0; i < this->mDevices.Size (); ++i ) {
		MOAIInputDevice* device = this->mDevices [ i ];
		if ( device ) {
			device->ResetSensors ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIInputQueue::SetConfigurationName ( cc8* name ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushLuaClassTable ( state );
	
	state.SetField ( -1, LUAVAR_CONFIGURATION, name );
}

//----------------------------------------------------------------//
void MOAIInputQueue::SetDevice ( u8 deviceID, cc8* name ) {

	if ( !( deviceID < this->mDevices.Size ())) return;

	MOAIInputDevice* device = new MOAIInputDevice ();
	device->SetName ( name );
	
	this->LuaRelease ( this->mDevices [ deviceID ]);
	
	this->mDevices [ deviceID ] = device;
	this->LuaRetain ( device );
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushLuaUserdata ( state );
	
	device->PushLuaUserdata ( state );
	lua_setfield ( state, -2, name );
}

//----------------------------------------------------------------//
void MOAIInputQueue::SetDeviceActive ( u8 deviceID, bool active ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->SetActive ( active );
	}
}

//----------------------------------------------------------------//
void MOAIInputQueue::SetDeviceHardwareInfo ( u8 deviceID, cc8* hardwareInfo ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->SetHardwareInfo ( hardwareInfo );
	}
}

//----------------------------------------------------------------//
bool MOAIInputQueue::WriteEventHeader ( u8 deviceID, u8 sensorID, u32 type ) {

	if ( this->CheckSensor ( deviceID, sensorID, type )) {
		this->Write < u8 >(( u8 )deviceID );
		this->Write < u8 >(( u8 )sensorID );
		this->Write < double >( this->mTimestamp - this->mTimebase );
		return true;
	}
	return false;
}

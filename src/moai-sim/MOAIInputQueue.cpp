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
int MOAIInputQueue::_autoTimestamp ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIInputQueue, "U" )
	
	self->mAutoTimestamp = state.GetValue < bool >( 2, true );
	if ( self->mAutoTimestamp ) {
		self->mTimebase = ZLDeviceTime::GetTimeInSeconds ();
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIInputQueue::_deferEvents ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIInputQueue, "U" )

	bool defer = state.GetValue < bool >( 2, false );
	self->DeferEvents ( defer );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIInputQueue::_discardEvents ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIInputQueue, "U" )
	
	self->DiscardAll ();
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIInputQueue::_playback ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIInputQueue, "U" )
	
	self->mPlayback = state.GetValue < bool >( 2, true );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIInputQueue::_setAutosuspend ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIInputQueue, "U" )
	
	self->mAutosuspend = state.GetValue < double >( 2, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIInputQueue::_setRecorder ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIInputQueue, "U" )
	
	self->mRecorder.Set ( *self, state.GetLuaObject < MOAIStream >( 2, true ));
	
	return 0;

}

//----------------------------------------------------------------//
int MOAIInputQueue::_suspendEvents ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIInputQueue, "U" )
	
	self->SuspendEvents ( state.GetValue ( 2, false ));
	
	return 0;
}

//================================================================//
// MOAIInputQueue
//================================================================//

//----------------------------------------------------------------//
bool MOAIInputQueue::CanWrite () {

	if (( this->mAutosuspend > 0 ) && ( this->mAutosuspend <= ( ZLDeviceTime::GetTimeInSeconds () - this->mLastUpdate ))) {
		this->DiscardAll ();
		this->mAutosuspended = true;
	}
	return !( this->mPlayback || this->mSuspended || this->mAutosuspended );
}

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
	mAutoTimestamp ( false ),
	mDefer ( false ),
	mSuspended ( false ),
	mAutosuspend ( 0 ),
	mAutosuspended ( false ),
	mLastUpdate ( 0 ),
	mPlayback ( false ) {
	
	RTTI_SINGLE ( MOAIAction )
	
	this->SetChunkSize ( CHUNK_SIZE );
}

//----------------------------------------------------------------//
MOAIInputQueue::~MOAIInputQueue () {

	for ( u32 i = 0; i < this->mDevices.Size (); ++i ) {
		this->LuaRelease ( this->mDevices [ i ]);
	}
	this->mRecorder.Set ( *this, 0 );
}

//----------------------------------------------------------------//
void MOAIInputQueue::OnUpdate ( double timestep ) {

	ZLStream* eventStream = this;

	if ( this->mPlayback ) {
		if ( this->mRecorder ) {
			size_t cursor = this->ParseEvents ( *this->mRecorder, timestep );
			this->mRecorder->Seek ( cursor, SEEK_SET );
		}
	}
	else {

		this->mLastUpdate = ZLDeviceTime::GetTimeInSeconds ();
		this->mAutosuspended = false;

		// reset the input sensors
		this->ResetSensors ();

		if ( !this->mDefer ) {
			
			// rewind the event queue
			this->Seek ( 0, SEEK_SET );
			
			size_t cursor = this->ParseEvents ( *this, timestep );
			
			// record the processed events
			this->Record ( cursor );
			
			// discard processed events
			this->DiscardFront ( cursor );
			
			// back to the end of the queue
			this->Seek ( this->GetLength (), SEEK_SET );
		}
	}
}

//----------------------------------------------------------------//
size_t MOAIInputQueue::ParseEvents ( ZLStream& stream, double timestep ) {

	bool first = true;
	double timebase = 0;
	size_t cursor = stream.GetCursor ();
	
	// parse events until we run out or hit an event after the current sim time
	while ( stream.GetCursor () < stream.GetLength ()) {
		
		u8 deviceID			= stream.Read < u8 >( 0 );
		u8 sensorID			= stream.Read < u8 >( 0 );
		double timestamp	= stream.Read < double >( 0 );
		
		if ( first ) {
			timebase = timestamp;
			first = false;
		}
		
		if ( timestep < ( timestamp - timebase )) break;
		
		MOAISensor* sensor = this->GetSensor ( deviceID, sensorID );
		assert ( sensor );
		sensor->mTimestamp = timestamp;
		sensor->ParseEvent ( stream );
		
		cursor = stream.GetCursor ();
	}
	return cursor;
}

//----------------------------------------------------------------//
void MOAIInputQueue::Record ( size_t size ) {

	if ( this->mRecorder && size ) {
		this->Seek ( 0, SEEK_SET );
		this->mRecorder->WriteStream ( *this, size );
		this->Seek ( size, SEEK_SET );
	}
}

//----------------------------------------------------------------//
void MOAIInputQueue::RegisterLuaClass ( MOAILuaState& state ) {

	MOAIAction::RegisterLuaClass ( state );
}

//----------------------------------------------------------------//
void MOAIInputQueue::RegisterLuaFuncs ( MOAILuaState& state ) {
	
	MOAIAction::RegisterLuaFuncs ( state );
	
	luaL_Reg regTable [] = {
		{ "autoTimestamp",		_autoTimestamp },
		{ "deferEvents",		_deferEvents },
		{ "discardEvents",		_discardEvents },
		{ "playback",			_playback },
		{ "setAutosuspend",		_setAutosuspend },
		{ "setRecorder",		_setRecorder },
		{ "suspendEvents",		_suspendEvents },
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
void MOAIInputQueue::SetAutosuspend ( double autosuspend ) {

	this->mAutosuspend = autosuspend >= 0 ? autosuspend : 0;
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
void MOAIInputQueue::SuspendEvents ( bool suspend ) {

	if ( suspend ) {
		this->DiscardAll ();
	}
	this->mSuspended = suspend;
}

//----------------------------------------------------------------//
bool MOAIInputQueue::WriteEventHeader ( u8 deviceID, u8 sensorID, u32 type ) {

	if ( this->CanWrite () && this->CheckSensor ( deviceID, sensorID, type )) {

		this->Write < u8 >(( u8 )deviceID );
		this->Write < u8 >(( u8 )sensorID );
		
		double timestamp = this->mAutoTimestamp ? ZLDeviceTime::GetTimeInSeconds () : this->mTimestamp;
		this->Write < double >( timestamp - this->mTimebase );
		
		return true;
	}
	return false;
}

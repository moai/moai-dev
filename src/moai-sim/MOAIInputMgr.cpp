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
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIInputMgr::_autoTimestamp ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIInputMgr, "" )
	
	self->SetAutotimestamp ( state.GetValue < bool >( 1, true ));
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIInputMgr::_deferEvents ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIInputMgr, "" )

	bool defer = state.GetValue < bool >( 1, false );
	self->DeferEvents ( defer );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIInputMgr::_discardEvents ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIInputMgr, "" )
	
	self->DiscardAll ();
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIInputMgr::_playback ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIInputMgr, "" )
	
	self->mPlayback = state.GetValue < bool >( 1, true );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIInputMgr::_setAutosuspend ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIInputMgr, "" )
	
	self->mAutosuspend = state.GetValue < double >( 1, 0 );
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIInputMgr::_setRecorder ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIInputMgr, "" )
	
	self->mRecorder.Set ( *self, state.GetLuaObject < MOAIStream >( 1, true ));
	
	return 0;

}

//----------------------------------------------------------------//
int MOAIInputMgr::_suspendEvents ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIInputMgr, "" )
	
	self->SuspendEvents ( state.GetValue ( 1, false ));
	
	return 0;
}

//================================================================//
// MOAIInputMgr
//================================================================//

//----------------------------------------------------------------//
u8 MOAIInputMgr::AddDevice ( cc8* name  ) {

	u8 id = this->mDevices.GetTop ();
	
	this->mDevices.Push ();
	this->SetDevice ( id, name );
	return id;
}

//----------------------------------------------------------------//
bool MOAIInputMgr::CanWrite () {

	if (( this->mAutosuspend > 0 ) && ( this->mAutosuspend <= ( ZLDeviceTime::GetTimeInSeconds () - this->mLastUpdate ))) {
		this->DiscardAll ();
		this->mAutosuspended = true;
	}
	return !( this->mPlayback || this->mSuspended || this->mAutosuspended );
}

//----------------------------------------------------------------//
void MOAIInputMgr::ClearSensorState () {

	for ( u32 i = 0; i < this->mDevices.Size (); ++i ) {
		MOAIInputDevice* device = this->mDevices [ i ];
		if ( device ) {
			device->ClearSensorState ();
		}
	}
}

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
void MOAIInputMgr::DeferEvents ( bool defer ) {

	this->mDefer = defer;
}

//----------------------------------------------------------------//
void MOAIInputMgr::FlushEvents ( double skip ) {

	this->mTimebase += skip;
	this->DiscardAll ();
	this->ClearSensorState ();
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
bool MOAIInputMgr::IsDone () {
	return false;
}

//----------------------------------------------------------------//
MOAIInputMgr::MOAIInputMgr () :
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
MOAIInputMgr::~MOAIInputMgr () {

	if ( this->mRecorder ) {
		this->mRecorder->Flush ();
	}

	for ( u32 i = 0; i < this->mDevices.Size (); ++i ) {
		this->LuaRelease ( this->mDevices [ i ]);
	}
	this->mRecorder.Set ( *this, 0 );
}

//----------------------------------------------------------------//
size_t MOAIInputMgr::ParseEvents ( ZLStream& stream, double timestep ) {

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
void MOAIInputMgr::Record ( size_t size ) {

	if ( this->mRecorder && size ) {
		this->Seek ( 0, SEEK_SET );
		this->mRecorder->WriteStream ( *this, size );
		this->Seek ( size, SEEK_SET );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::RegisterLuaClass ( MOAILuaState& state ) {

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
void MOAIInputMgr::RegisterLuaFuncs ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIInputMgr::ReserveDevices ( u8 total ) {

	this->mDevices.SetTop ( total, 0 );
}

//----------------------------------------------------------------//
void MOAIInputMgr::ReserveSensors ( u8 deviceID, u8 total ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->ReserveSensors ( total );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::ResetSensorState () {

	for ( u32 i = 0; i < this->mDevices.Size (); ++i ) {
		MOAIInputDevice* device = this->mDevices [ i ];
		if ( device ) {
			device->ResetSensorState ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::SetAutosuspend ( double autosuspend ) {

	this->mAutosuspend = autosuspend >= 0 ? autosuspend : 0;
}

//----------------------------------------------------------------//
void MOAIInputMgr::SetAutotimestamp ( bool autotimestamp ) {

	this->mAutoTimestamp = autotimestamp;
	if ( this->mAutoTimestamp ) {
		this->mTimebase = ZLDeviceTime::GetTimeInSeconds ();
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

	this->LuaRelease ( this->mDevices [ deviceID ]);
	
	MOAIInputDevice* device = new MOAIInputDevice ();
	this->mDevices [ deviceID ] = device;
	this->LuaRetain ( device );
	
	if ( name ) {
	
		device->SetName ( name );
	
		MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
		this->PushMemberTable ( state );
		device->PushLuaUserdata ( state );
		lua_setfield ( state, -2, name );
	}
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
void MOAIInputMgr::SuspendEvents ( bool suspend ) {

	if ( suspend ) {
		this->DiscardAll ();
	}
	this->mSuspended = suspend;
}

//----------------------------------------------------------------//
void MOAIInputMgr::Update ( double timestep ) {

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
		this->ResetSensorState ();

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
bool MOAIInputMgr::WriteEventHeader ( u8 deviceID, u8 sensorID, u32 type ) {

	if ( this->CanWrite () && this->CheckSensor ( deviceID, sensorID, type )) {

		this->Write < u8 >(( u8 )deviceID );
		this->Write < u8 >(( u8 )sensorID );
		
		double timestamp = this->mAutoTimestamp ? ZLDeviceTime::GetTimeInSeconds () : this->mTimestamp;
		
		this->Write < double >( timestamp - this->mTimebase );
		
		return true;
	}
	return false;
}

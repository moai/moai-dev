// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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
int MOAIInputMgr::_setEventCallback ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIInputMgr, "" )
	
	self->mEventCallback.SetRef ( state, 1 );
	
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
ZLIndex MOAIInputMgr::AddDevice ( cc8* name ) {

	ZLSize top = this->mDevices.GetTop ();
	assert ( top < MAX_DEVICES );
	
	ZLIndex deviceID = top;
	
	this->mDevices.Push ();
	this->SetDevice ( deviceID, name );
	return deviceID;
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

	for ( ZLIndex i = 0; i < this->mDevices.Size (); ++i ) {
		MOAIInputDevice* device = this->mDevices [ i ];
		if ( device ) {
			device->ClearSensorState ();
		}
	}
}

//----------------------------------------------------------------//
bool MOAIInputMgr::CheckSensor ( ZLIndex deviceID, ZLIndex sensorID, u32 type ) {

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
MOAIInputDevice* MOAIInputMgr::GetDevice ( ZLIndex deviceID ) {

	if ( deviceID < this->mDevices.Size ()) {
		return this->mDevices [ deviceID ];
	}
	return 0;
}

//----------------------------------------------------------------//
MOAISensor* MOAIInputMgr::GetSensor ( ZLIndex deviceID, ZLIndex sensorID ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		return device->GetSensor ( sensorID );
	}
	return 0;
}

//----------------------------------------------------------------//
bool MOAIInputMgr::HasEvents () {

	// TODO: this ignores the case where there are pending events that would not be processed until after the current frame
	return this->GetCursor () > 0;
}

//----------------------------------------------------------------//
void MOAIInputMgr::InvokeCallback ( u32 event, double timestamp ) {

	if ( this->mEventCallback ) {
		
		MOAIScopedLuaState state = this->mEventCallback.GetSelf ();
		lua_pushnumber ( state, event );
		lua_pushnumber ( state, timestamp );
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
//bool MOAIInputMgr::IsDone () {
//	return false;
//}

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
		
	RTTI_BEGIN ( MOAIInputMgr )
		RTTI_VISITOR ( MOAIAbstractLuaRegistrationVisitor, MOAILuaRegistrationVisitor < MOAIInputMgr >)
		RTTI_EXTEND ( MOAILuaObject )
	RTTI_END
	
	this->SetChunkSize ( CHUNK_SIZE );
}

//----------------------------------------------------------------//
MOAIInputMgr::~MOAIInputMgr () {

	if ( this->mRecorder ) {
		this->mRecorder->Flush ();
	}

	for ( ZLIndex i = 0; i < this->mDevices.Size (); ++i ) {
		this->LuaRelease ( this->mDevices [ i ]);
	}
	this->mRecorder.Set ( *this, 0 );
}

//----------------------------------------------------------------//
size_t MOAIInputMgr::ParseEvents ( ZLStream& stream, double timestep ) {
	UNUSED ( timestep ); // TODO: fix this

	bool first = true;
	//double timebase = 0;
	size_t cursor = stream.GetCursor ();
	
	// parse events until we run out or hit an event after the current sim time
	while ( stream.GetCursor () < stream.GetLength ()) {
		
		ZLIndex deviceID	= stream.Read < u8 >( 0 );
		ZLIndex sensorID	= stream.Read < u8 >( 0 );
		double timestamp	= stream.Read < double >( 0 );
		
		if ( first ) {
			//timebase = timestamp;
			first = false;
		}
		
		//if ( timestep < ( timestamp - timebase )) break; // TODO: come back to this
		
		MOAISensor* sensor = this->GetSensor ( deviceID, sensorID );
		assert ( sensor );
		sensor->mTimestamp = timestamp;
		sensor->ParseEvent ( stream );
		
		this->InvokeCallback ( INPUT_EVENT, timestamp );
		
		cursor = stream.GetCursor ();
	}
	
	this->InvokeCallback ( FINISHED_UPDATE, ZLDeviceTime::GetTimeInSeconds () - this->mTimebase );
	
	return cursor;
}

//----------------------------------------------------------------//
void MOAIInputMgr::Record ( size_t size ) {

	if ( this->mRecorder && size ) {
		this->SetCursor ( 0 );
		this->mRecorder->WriteStream ( *this, size );
		this->SetCursor ( size );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::ReserveDevices ( ZLSize total ) {

	assert ( total < MAX_DEVICES );

	this->mDevices.SetTop ( total, 0 );
}

//----------------------------------------------------------------//
void MOAIInputMgr::ReserveSensors ( ZLIndex deviceID, ZLSize total ) {

	assert ( total < MAX_SENSORS );

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->ReserveSensors ( total );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::ResetSensorState () {

	for ( ZLIndex i = 0; i < this->mDevices.Size (); ++i ) {
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
void MOAIInputMgr::SetDevice ( ZLIndex deviceID, cc8* name ) {

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
void MOAIInputMgr::SetDeviceActive ( ZLIndex deviceID, bool active ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->SetActive ( active );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::SetDeviceHardwareInfo ( ZLIndex deviceID, cc8* hardwareInfo ) {

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

	//ZLStream* eventStream = this;

	if ( this->mPlayback ) {
		if ( this->mRecorder ) {
			size_t cursor = this->ParseEvents ( *this->mRecorder, timestep );
			this->mRecorder->SetCursor ( cursor );
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
			this->SetCursor ( this->GetLength ());
		}
	}
}

//----------------------------------------------------------------//
bool MOAIInputMgr::WriteEventHeader ( ZLIndex deviceID, ZLIndex sensorID, u32 type ) {

	if ( this->CanWrite () && this->CheckSensor ( deviceID, sensorID, type )) {

		this->Write < u8 >(( u8 )deviceID );
		this->Write < u8 >(( u8 )sensorID );
		
		double timestamp = this->mAutoTimestamp ? ZLDeviceTime::GetTimeInSeconds () : this->mTimestamp;
		
		this->Write < double >( timestamp - this->mTimebase );
		
		return true;
	}
	return false;
}

//================================================================//
// virtual
//================================================================//

//----------------------------------------------------------------//
void MOAIInputMgr::_RegisterLuaClass ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	if ( history.Visit ( *this )) return;

	state.SetField ( -1, "INPUT_EVENT",			( u32 )INPUT_EVENT );
	state.SetField ( -1, "FINISHED_UPDATE",		( u32 )FINISHED_UPDATE );


	luaL_Reg regTable [] = {
		{ "autoTimestamp",		_autoTimestamp },
		{ "deferEvents",		_deferEvents },
		{ "discardEvents",		_discardEvents },
		{ "playback",			_playback },
		{ "setAutosuspend",		_setAutosuspend },
		{ "setEventCallback",	_setEventCallback },
		{ "setRecorder",		_setRecorder },
		{ "suspendEvents",		_suspendEvents },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIInputMgr::_RegisterLuaFuncs ( RTTIVisitorHistory& history, MOAILuaState& state ) {
	UNUSED ( state );
	if ( history.Visit ( *this )) return;
}

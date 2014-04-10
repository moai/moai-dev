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
// MOAIInputContext lua
//================================================================//

//----------------------------------------------------------------//
int MOAIInputContext::_resetSensors ( lua_State* L ) {
	MOAI_LUA_SETUP ( MOAIInputContext, "U" )
	
	self->ResetSensors ();
	
	return 0;
}

//================================================================//
// MOAIInputContext
//================================================================//

//----------------------------------------------------------------//
bool MOAIInputContext::CheckSensor ( u8 deviceID, u8 sensorID, u32 type ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device && device->mIsActive ) {
		MOAISensor* sensor = device->GetSensor ( sensorID );
		return ( sensor && ( sensor->mType == type ));
	}
	return false;
}

//----------------------------------------------------------------//
MOAIInputDevice* MOAIInputContext::GetDevice ( u8 deviceID ) {

	if ( deviceID < this->mDevices.Size ()) {
		return this->mDevices [ deviceID ];
	}
	return 0;
}

//----------------------------------------------------------------//
MOAISensor* MOAIInputContext::GetSensor ( u8 deviceID, u8 sensorID ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		return device->GetSensor ( sensorID );
	}
	return 0;
}

//----------------------------------------------------------------//
MOAIInputContext::MOAIInputContext () {
	
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIInputContext::~MOAIInputContext () {

	for ( u32 i = 0; i < this->mDevices.Size (); ++i ) {
		this->LuaRelease ( this->mDevices [ i ]);
	}
}

//----------------------------------------------------------------//
void MOAIInputContext::ParseEvent ( ZLStream& stream, bool reset ) {

	u8 deviceID			= stream.Read < u8 >( 0 );
	u8 sensorID			= stream.Read < u8 >( 0 );
	double timestamp	= stream.Read < double >( 0 );
		
	MOAISensor* sensor = this->GetSensor ( deviceID, sensorID );
	assert ( sensor );
	
	if ( reset ) {
		sensor->Reset ();
	}
	sensor->mTimestamp = timestamp;
	sensor->ParseEvent ( stream );
}

//----------------------------------------------------------------//
void MOAIInputContext::RegisterLuaClass ( MOAILuaState& state ) {
	UNUSED ( state );
}

//----------------------------------------------------------------//
void MOAIInputContext::RegisterLuaFuncs ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "resetSensors",			_resetSensors },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIInputContext::ReserveDevices ( u8 total ) {

	this->mDevices.Init ( total );
	this->mDevices.Fill ( 0 );
}

//----------------------------------------------------------------//
void MOAIInputContext::ReserveSensors ( u8 deviceID, u8 total ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->ReserveSensors ( total );
	}
}

//----------------------------------------------------------------//
void MOAIInputContext::ResetSensors () {

	for ( u32 i = 0; i < this->mDevices.Size (); ++i ) {
		MOAIInputDevice* device = this->mDevices [ i ];
		if ( device ) {
			device->ResetSensors ();
		}
	}
}

//----------------------------------------------------------------//
void MOAIInputContext::SetDevice ( u8 deviceID, cc8* name ) {

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
void MOAIInputContext::SetDeviceActive ( u8 deviceID, bool active ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->SetActive ( active );
	}
}

//----------------------------------------------------------------//
void MOAIInputContext::SetSensor ( u8 deviceID, u8 sensorID, cc8* name, u32 type ) {

	MOAIInputDevice* device = this->GetDevice ( deviceID );
	if ( device ) {
		device->SetSensor ( sensorID, name, type );
	}
}

//================================================================//
// MOAIInputMgr lua
//================================================================//

//----------------------------------------------------------------//
int MOAIInputMgr::_enableAutoUpdate ( lua_State* L ) {
	MOAILuaState state ( L );
	
	MOAIInputMgr::Get ().mAutoUpdate = state.GetValue < bool >( 1, false );
	return 0;
}

//----------------------------------------------------------------//
int MOAIInputMgr::_more ( lua_State* L ) {
	MOAILuaState state ( L );
	
	state.Push ( MOAIInputMgr::Get ().More ());
	return 1;
}

//----------------------------------------------------------------//
int MOAIInputMgr::_parseEvent ( lua_State* L ) {
	MOAILuaState state ( L );
	
	MOAIInputMgr& inputMgr = MOAIInputMgr::Get ();
	
	MOAIInputContext* context	= state.GetLuaObject < MOAIInputContext >( 1, true );
	bool reset					= state.GetValue < bool >( 2, true );
	
	context = context ? context : &inputMgr;
	
	state.Push ( inputMgr.ParseEvent ( *context, reset ));
	return 1;
}

//----------------------------------------------------------------//
int MOAIInputMgr::_rewind ( lua_State* L ) {
	UNUSED ( L );

	MOAIInputMgr::Get ().Rewind ();
	return;
}

//================================================================//
// MOAIInputMgr
//================================================================//

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueButtonEvent ( u8 deviceID, u8 sensorID, bool down ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::BUTTON )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAIButtonSensor::WriteEvent ( *this->mEvents, down );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueCompassEvent ( u8 deviceID, u8 sensorID, float heading ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::COMPASS )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAICompassSensor::WriteEvent ( *this->mEvents, heading );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueKeyboardEvent ( u8 deviceID, u8 sensorID, u32 keyID, bool down ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::KEYBOARD )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAIKeyboardSensor::WriteEvent ( *this->mEvents, keyID, down );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueLevelEvent ( u8 deviceID, u8 sensorID, float x, float y, float z ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::LEVEL )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAIMotionSensor::WriteEvent ( *this->mEvents, x, y, z );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueLocationEvent ( u8 deviceID, u8 sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::LOCATION )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAILocationSensor::WriteEvent ( *this->mEvents, longitude, latitude, altitude, hAccuracy, vAccuracy, speed );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueuePointerEvent ( u8 deviceID, u8 sensorID, int x, int y ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::POINTER )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAIPointerSensor::WriteEvent ( *this->mEvents, x, y );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueTouchEvent ( u8 deviceID, u8 sensorID, u32 touchID, bool down, float x, float y ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::TOUCH )) {
		this->WriteEventHeader ( deviceID, sensorID );
		float time = ( float )ZLDeviceTime::GetTimeInSeconds ();
		MOAITouchSensor::WriteEvent ( *this->mEvents, touchID, down, x, y, time );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueTouchEventCancel ( u8 deviceID, u8 sensorID ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::TOUCH )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAITouchSensor::WriteEventCancel ( *this->mEvents );
	}
}

//----------------------------------------------------------------//
void MOAIInputMgr::EnqueueWheelEvent ( u8 deviceID, u8 sensorID, float value ) {

	if ( this->CheckSensor ( deviceID, sensorID, MOAISensor::WHEEL )) {
		this->WriteEventHeader ( deviceID, sensorID );
		MOAIWheelSensor::WriteEvent ( *this->mEvents, value );
	}
}

//----------------------------------------------------------------//
MOAIInputMgr::MOAIInputMgr () :
	mTimestamp ( 0 ),
	mAutoUpdate ( true ) ,
	mEvents ( 0 ),
	mCachedEvents ( 0 ),
	mCachedEventsTop ( 0 ) {
	
	RTTI_SINGLE ( MOAILuaObject )
	
	this->mStreamA.SetChunkSize ( 512 );
	this->mStreamB.SetChunkSize ( 512 );
	
	this->mEvents = &this->mStreamA;
	this->mCachedEvents = &this->mStreamB;
}

//----------------------------------------------------------------//
MOAIInputMgr::~MOAIInputMgr () {
}

//----------------------------------------------------------------//
bool MOAIInputMgr::More () {

	return this->mCachedEvents->GetCursor () < this->mCachedEventsTop;
}

//----------------------------------------------------------------//
bool MOAIInputMgr::ParseEvent ( MOAIInputContext& context, bool reset ) {

	if ( this->More ()) {
		context.ParseEvent ( *this->mCachedEvents, reset );
	}
	return this->More ();
}

//----------------------------------------------------------------//
void MOAIInputMgr::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "enableAutoUpdate",		_enableAutoUpdate },
		{ "more",					_more },
		{ "parseEvent",				_parseEvent },
		{ "rewind",					_rewind },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIInputMgr::Rewind () {

	this->mCachedEvents->Seek ( 0, SEEK_SET );
}

//----------------------------------------------------------------//
void MOAIInputMgr::SetConfigurationName ( cc8* name ) {

	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	this->PushLuaClassTable ( state );
	
	state.SetField ( -1, LUAVAR_CONFIGURATION, name );
}

//----------------------------------------------------------------//
void MOAIInputMgr::SetTimestamp ( double timestamp ) {

	this->mTimestamp = timestamp;
}

//----------------------------------------------------------------//
void MOAIInputMgr::Update () {

	// toggle streams
	if ( this->mEvents == &this->mStreamA ) {
		this->mCachedEvents = &this->mStreamA;
		this->mEvents = &this->mStreamB;
	}
	else {
		this->mCachedEvents = &this->mStreamB;
		this->mEvents = &this->mStreamA;
	}

	// rewind events queue
	this->mEvents->Seek ( 0, SEEK_SET );

	// get length of cached event queue and rewind
	this->mCachedEventsTop = this->mCachedEvents->GetCursor ();
	this->Rewind ();

	// if autoupdate, feed all events to the default context
	if ( this->mAutoUpdate ) {
		this->ResetSensors ();
		while ( this->ParseEvent ( *this, false ));
	}
	
	// rewind cached event queue again to get ready for event reading during the frame
	this->Rewind ();
}

//----------------------------------------------------------------//
void MOAIInputMgr::WriteEventHeader ( u8 deviceID, u8 sensorID ) {

	this->mEvents->Write < u8 >(( u8 )deviceID );
	this->mEvents->Write < u8 >(( u8 )sensorID );
	this->mEvents->Write < double >(( double )this->mTimestamp );
}

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-ios-motion/MOAIMotionMgrIOS.h>
#include <moai-sim/MOAIVectorSensor.h>


#import <CoreLocation/CoreLocation.h>
#import <CoreMotion/CoreMotion.h>

namespace MOAILocationSensorID {
	enum {
		COMPASS,
		LOCATION,
		TOTAL,
	};
}

namespace MOAIRawMotionSensorID {
	enum {
		ACCELEROMETER,
		GYRO,
		MAGNETOMETER,
		TOTAL,
	};
}

namespace MOAIDeviceMotionSensorID {
	enum {
		ACCELEROMETER,
		GRAVITY,
		GYRO,
		MAGNETOMETER,
		TOTAL,
	};
}

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int	MOAIMotionMgrIOS::_init ( lua_State* L ) {
	UNUSED ( L );
	MOAIMotionMgrIOS::Get ().Init ();
	return 0;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_isAccelerometerAvailable ( lua_State* L ) {
	MOAILuaState state ( L );
	
	CMMotionManager* motionMgr = MOAIMotionMgrIOS::Get ().mMotionMgr;
	if ( !motionMgr ) return 0;
	
	state.Push ([ motionMgr isAccelerometerAvailable ]);
	state.Push ([ motionMgr isAccelerometerActive ]);
	
	return 2;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_isDeviceMotionAvailable ( lua_State* L ) {
	MOAILuaState state ( L );
	
	CMMotionManager* motionMgr = MOAIMotionMgrIOS::Get ().mMotionMgr;
	if ( !motionMgr ) return 0;
	
	state.Push ([ motionMgr isDeviceMotionAvailable ]);
	state.Push ([ motionMgr isDeviceMotionActive ]);
	
	return 2;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_isGyroAvailable ( lua_State* L ) {
	MOAILuaState state ( L );
	
	CMMotionManager* motionMgr = MOAIMotionMgrIOS::Get ().mMotionMgr;
	if ( !motionMgr ) return 0;
	
	state.Push ([ motionMgr isGyroAvailable ]);
	state.Push ([ motionMgr isGyroActive ]);
	
	return 2;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_isHeadingAvailable ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMotionMgrIOS, "" );
	
	if ( !self->mLocationMgr ) return 0;
	
	state.Push ([ CLLocationManager headingAvailable ]);
	state.Push ( self->mIsHeadingActive );
	
	return 2;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_isMagnetometerAvailable ( lua_State* L ) {
	MOAILuaState state ( L );
	
	CMMotionManager* motionMgr = MOAIMotionMgrIOS::Get ().mMotionMgr;
	if ( !motionMgr ) return 0;
	
	state.Push ([ motionMgr isMagnetometerAvailable ]);
	state.Push ([ motionMgr isMagnetometerActive ]);
	
	return 2;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_setAccelerometerActive ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMotionMgrIOS, "" )
	
	CMMotionManager* motionMgr = self->mMotionMgr;
	if ( !motionMgr ) return 0;
	
	bool active = state.GetValue < bool >( 1, true );
	bool isActive = [ motionMgr isAccelerometerActive ];
	
	if ( active != isActive ) {
		if ( active ) {
			[ motionMgr startAccelerometerUpdates ];
		}
		else {
			[ motionMgr stopAccelerometerUpdates ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_setDeviceMotionActive ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMotionMgrIOS, "" )
	
	CMMotionManager* motionMgr = self->mMotionMgr;
	if ( !motionMgr ) return 0;
	
	bool active = state.GetValue < bool >( 1, true );
	u32 referenceFrame = state.GetValue < u32 >( 2, ( u32 )[ motionMgr attitudeReferenceFrame ]);
	
	bool isActive = [ motionMgr isDeviceMotionActive ];
	
	if ( active != isActive ) {
		if ( active ) {
			[ motionMgr startDeviceMotionUpdatesUsingReferenceFrame:( CMAttitudeReferenceFrame )referenceFrame ];
		}
		else {
			[ motionMgr stopDeviceMotionUpdates ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_setGyroActive ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMotionMgrIOS, "" )
	
	CMMotionManager* motionMgr = self->mMotionMgr;
	if ( !motionMgr ) return 0;
	
	bool active = state.GetValue < bool >( 1, true );
	bool isActive = [ motionMgr isGyroActive ];
	
	if ( active != isActive ) {
		if ( active ) {
			[ motionMgr startGyroUpdates ];
		}
		else {
			[ motionMgr stopGyroUpdates ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_setHeadingActive ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMotionMgrIOS, "" )
	
	if ( ![ CLLocationManager headingAvailable ]) return 0;
	
	CLLocationManager* locationMgr = self->mLocationMgr;
	if ( !locationMgr ) return 0;
	
	bool active = state.GetValue < bool >( 1, true );
	
	if ( active != self->mIsHeadingActive ) {
	
		self->mIsHeadingActive = active;
		
		if ( self->mIsHeadingActive ) {
			[ locationMgr startUpdatingHeading ];
		}
		else {
			[ locationMgr stopUpdatingHeading ];
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_setMagnetometerActive ( lua_State* L ) {
	MOAI_LUA_SETUP_SINGLE ( MOAIMotionMgrIOS, "" )
	
	CMMotionManager* motionMgr = self->mMotionMgr;
	if ( !motionMgr ) return 0;
	
	bool active = state.GetValue < bool >( 1, true );
	bool isActive = [ motionMgr isMagnetometerActive ];
	
	if ( active != isActive ) {
		if ( active ) {
			[ motionMgr startMagnetometerUpdates ];
		}
		else {
			[ motionMgr stopMagnetometerUpdates ];
		}
	}
	return 0;
}

//================================================================//
// MOAIMotionMgrIOS
//================================================================//

//----------------------------------------------------------------//
void MOAIMotionMgrIOS::Init () {

	if ( this->mMotionMgr != nil ) return;

	this->mLocationMgr		= [[ CLLocationManager alloc ] init ];
	this->mMotionMgr		= [[ CMMotionManager alloc ] init ];
	this->mOperationQueue	= [[ NSOperationQueue alloc ] init ];
	
	MOAIInputMgr& inputMgr = MOAIInputMgr::Get ();
	
	this->mLocationDeviceID = inputMgr.AddDevice ( 0 );
	
	inputMgr.ReserveSensors ( this->mLocationDeviceID, MOAIRawMotionSensorID::TOTAL );
	inputMgr.SetSensor < MOAICompassSensor >( this->mLocationDeviceID, MOAILocationSensorID::COMPASS, "compass" );
	inputMgr.SetSensor < MOAILocationSensor >( this->mLocationDeviceID, MOAILocationSensorID::LOCATION, "location" );

	this->mRawInputDeviceID = inputMgr.AddDevice ( 0 );
	
	inputMgr.ReserveSensors ( this->mRawInputDeviceID, MOAIRawMotionSensorID::TOTAL );
	inputMgr.SetSensor < MOAIVectorSensor >( this->mRawInputDeviceID, MOAIRawMotionSensorID::ACCELEROMETER, "accelerometer" );
	inputMgr.SetSensor < MOAIVectorSensor >( this->mRawInputDeviceID, MOAIRawMotionSensorID::GYRO, "gyro" );
	inputMgr.SetSensor < MOAIVectorSensor >( this->mRawInputDeviceID, MOAIRawMotionSensorID::MAGNETOMETER, "magnetometer" );
	
	this->mInputDeviceID = inputMgr.AddDevice ( 0 );
	
	inputMgr.ReserveSensors ( this->mInputDeviceID, MOAIDeviceMotionSensorID::TOTAL );
	inputMgr.SetSensor < MOAIVectorSensor >( this->mInputDeviceID, MOAIDeviceMotionSensorID::ACCELEROMETER, "accelerometer" );
	inputMgr.SetSensor < MOAIVectorSensor >( this->mInputDeviceID, MOAIDeviceMotionSensorID::GRAVITY, "gravity" );
	inputMgr.SetSensor < MOAIVectorSensor >( this->mInputDeviceID, MOAIDeviceMotionSensorID::GYRO, "gyro" );
	inputMgr.SetSensor < MOAIVectorSensor >( this->mInputDeviceID, MOAIDeviceMotionSensorID::MAGNETOMETER, "magnetometer" );
	
	MOAIScopedLuaState state = MOAILuaRuntime::Get ().State ();
	
	this->PushMemberTable ( state );
	
	state.Push ( inputMgr.GetDevice ( this->mLocationDeviceID ));
	lua_setfield ( state, -2, "location" );
	
	state.Push ( inputMgr.GetDevice ( this->mRawInputDeviceID ));
	lua_setfield ( state, -2, "rawMotion" );
	
	state.Push ( inputMgr.GetDevice ( this->mInputDeviceID ));
	lua_setfield ( state, -2, "deviceMotion" );
	
	this->Start ( MOAISim::Get ().GetActionTree (), true );
}

//----------------------------------------------------------------//
bool MOAIMotionMgrIOS::IsDone () {

	return false;
}

//----------------------------------------------------------------//
MOAIMotionMgrIOS::MOAIMotionMgrIOS () :
	mIsHeadingActive ( false ),
	//mIsLocationActive ( false ),
	mInputDeviceID ( 0 ),
	mRawInputDeviceID ( 0 ),
	mLocationMgr ( 0 ),
	mMotionMgr ( 0 ),
	mOperationQueue ( 0 ) {

	RTTI_SINGLE ( MOAIAction )
}

//----------------------------------------------------------------//
MOAIMotionMgrIOS::~MOAIMotionMgrIOS () {

	if ( this->mLocationMgr ) {
		
		[ this->mLocationMgr stopUpdatingHeading ];
		[ this->mLocationMgr stopUpdatingLocation ];
		
		[ this->mLocationMgr release ];
	}

	if ( this->mMotionMgr ) {
		
		[ this->mMotionMgr stopAccelerometerUpdates ];
		[ this->mMotionMgr stopDeviceMotionUpdates ];
		[ this->mMotionMgr stopGyroUpdates ];
		[ this->mMotionMgr stopMagnetometerUpdates ];
		
		[ this->mMotionMgr release ];
	}
	
	if ( this->mOperationQueue ) {
		[ this->mOperationQueue release ];
	}
}

//----------------------------------------------------------------//
void MOAIMotionMgrIOS::OnUpdate ( double step ) {
	UNUSED ( step );
	
	// poll all the active services and dispatch the events
	
	if ( this->mLocationMgr ) {
	
		if ( this->mIsHeadingActive && [ CLLocationManager headingAvailable ]) {
			
			CLHeading* heading = [ this->mLocationMgr heading ];
			if ( heading ) {
				MOAICompassSensor::EnqueueCompassEvent (
					this->mLocationDeviceID,
					MOAILocationSensorID::COMPASS,
					( float )[ heading trueHeading ]
				);
			}
		}
	}
	
	if ( this->mMotionMgr ) {
	
		if ([ this->mMotionMgr isAccelerometerActive ]) {
		
			CMAccelerometerData* accelerometerData = [ this->mMotionMgr accelerometerData ];
			if ( accelerometerData ) {
			
				const CMAcceleration& acceleration = [ accelerometerData acceleration ];
				MOAIVectorSensor::EnqueueVectorEvent (
					this->mRawInputDeviceID,
					MOAIRawMotionSensorID::ACCELEROMETER,
					acceleration.x,
					acceleration.y,
					acceleration.z
				);
			}
		}
		
		if ([ this->mMotionMgr isDeviceMotionActive ]) {
		
			CMDeviceMotion* deviceMotion = [ this->mMotionMgr deviceMotion ];
			if ( deviceMotion ) {
				
				const CMRotationRate& rotationRate		= [ deviceMotion rotationRate ];
				const CMAcceleration& userAcceleration	= [ deviceMotion userAcceleration ];
				const CMAcceleration& gravity			= [ deviceMotion gravity ];
				
				const CMCalibratedMagneticField& calibratedMagneticField = [ deviceMotion magneticField ];
				const CMMagneticField& magneticField = calibratedMagneticField.field;
				
				// TODO: consolidate these once custom event handlers are implemented
				MOAIVectorSensor::EnqueueVectorEvent (
					this->mInputDeviceID,
					MOAIDeviceMotionSensorID::GYRO,
					rotationRate.x,
					rotationRate.y,
					rotationRate.z
				);
				
				MOAIVectorSensor::EnqueueVectorEvent (
					this->mInputDeviceID,
					MOAIDeviceMotionSensorID::ACCELEROMETER,
					userAcceleration.x,
					userAcceleration.y,
					userAcceleration.z
				);
				
				MOAIVectorSensor::EnqueueVectorEvent (
					this->mInputDeviceID,
					MOAIDeviceMotionSensorID::GRAVITY,
					gravity.x,
					gravity.y,
					gravity.z
				);
				
				MOAIVectorSensor::EnqueueVectorEvent (
					this->mInputDeviceID,
					MOAIDeviceMotionSensorID::MAGNETOMETER,
					magneticField.x,
					magneticField.y,
					magneticField.z
				);
			}
		}
		
		if ([ this->mMotionMgr isGyroActive ]) {
		
			CMGyroData* gyroData = [ this->mMotionMgr gyroData ];
			if ( gyroData ) {
			
				const CMRotationRate& rotationRate = [ gyroData rotationRate ];
				MOAIVectorSensor::EnqueueVectorEvent (
					this->mRawInputDeviceID,
					MOAIRawMotionSensorID::GYRO,
					rotationRate.x,
					rotationRate.y,
					rotationRate.z
				);
			}
		}
		
		if ([ this->mMotionMgr isMagnetometerActive ]) {
		
			CMMagnetometerData* magnetometerData = [ this->mMotionMgr magnetometerData ];
			if ( magnetometerData ) {
			
				const CMMagneticField& magneticField = [ magnetometerData magneticField ];
				MOAIVectorSensor::EnqueueVectorEvent (
					this->mRawInputDeviceID,
					MOAIRawMotionSensorID::MAGNETOMETER,
					magneticField.x,
					magneticField.y,
					magneticField.z
				);
			}
		}
	}
}

//----------------------------------------------------------------//
void MOAIMotionMgrIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	
	NSUInteger availableFrames = [ CMMotionManager availableAttitudeReferenceFrames ];
	
	if ( availableFrames & CMAttitudeReferenceFrameXArbitraryZVertical ) {
		state.SetField ( -1, "XARBITRARY_ZVERTICAL",			( u32 )CMAttitudeReferenceFrameXArbitraryZVertical );
	}
	
	if ( availableFrames & CMAttitudeReferenceFrameXArbitraryCorrectedZVertical ) {
		state.SetField ( -1, "XARBITRARY_CORRECTEDZVERTICAL",	( u32 )CMAttitudeReferenceFrameXArbitraryCorrectedZVertical );
	}
	
	if ( availableFrames & CMAttitudeReferenceFrameXMagneticNorthZVertical ) {
		state.SetField ( -1, "XMAGNETICNORTH_ZVERTICAL",		( u32 )CMAttitudeReferenceFrameXMagneticNorthZVertical );
	}
	
	if ( availableFrames & CMAttitudeReferenceFrameXTrueNorthZVertical ) {
		state.SetField ( -1, "XTRUENORTH_ZVERTICAL",			( u32 )CMAttitudeReferenceFrameXTrueNorthZVertical );
	}
	
	luaL_Reg regTable [] = {
		{ "init",								_init },
		{ "isAccelerometerAvailable",			_isAccelerometerAvailable },
		{ "isDeviceMotionAvailable",			_isDeviceMotionAvailable },
		{ "isGyroAvailable",					_isGyroAvailable }, // Mmmmmm. Gyros!
		{ "isHeadingAvailable",					_isHeadingAvailable },
		{ "isMagnetometerAvailable",			_isMagnetometerAvailable },
		{ "setAccelerometerActive",				_setAccelerometerActive },
		{ "setGyroActive",						_setGyroActive },
		{ "setDeviceMotionActive",				_setDeviceMotionActive },
		{ "setHeadingActive",					_setHeadingActive },
		{ "setMagnetometerActive",				_setMagnetometerActive },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

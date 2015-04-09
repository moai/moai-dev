// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <moai-sim/MOAIVectorSensor.h>
#include <moai-ios-motion/MOAIMotionMgrIOS.h>

#import <CoreMotion/CoreMotion.h>

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
	MOAILuaState state ( L );
	
	MOAIMotionMgrIOS& moaiMotionMgr = MOAIMotionMgrIOS::Get ();
	CMMotionManager* motionMgr = MOAIMotionMgrIOS::Get ().mMotionMgr;
	NSOperationQueue* queue = MOAIMotionMgrIOS::Get ().mOperationQueue;
	
	if ( !motionMgr ) return 0;
	
	bool active = state.GetValue < bool >( 1, false );
	double interval = state.GetValue < double >( 2, 0.0 );
	
	bool isActive = [ motionMgr isAccelerometerActive ];
	
	if ( active != isActive ) {
		if ( active ) {
		
			[ motionMgr setAccelerometerUpdateInterval:( NSTimeInterval )interval ];
			[ motionMgr startAccelerometerUpdatesToQueue:queue withHandler:^( CMAccelerometerData* accelerometerData, NSError* error ) {
				
				if ( error ) {
					[ motionMgr stopAccelerometerUpdates ];
					// TODO: log error
				}
				else {
					dispatch_async ( dispatch_get_main_queue (), ^( void ) {
					
						if ( MOAIMotionMgrIOS::IsValid ()) {
							const CMAcceleration& acceleration = [ accelerometerData acceleration ];
							MOAIVectorSensor::EnqueueVectorEvent (
								moaiMotionMgr.mRawInputDeviceID,
								MOAIRawMotionSensorID::ACCELEROMETER,
								acceleration.x,
								acceleration.y,
								acceleration.z
							);
						}
					});
				}
			}];
		}
		else {
			[ motionMgr stopAccelerometerUpdates ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_setAccelerometerUpdateInterval ( lua_State* L ) {
	MOAILuaState state ( L );
	
	CMMotionManager* motionMgr = MOAIMotionMgrIOS::Get ().mMotionMgr;
	if ( !motionMgr ) return 0;
	
	double interval = state.GetValue < double >( 1, 0.0 );
	[ motionMgr setAccelerometerUpdateInterval:( NSTimeInterval )interval ];
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_setDeviceMotionActive ( lua_State* L ) {
	MOAILuaState state ( L );
	
	MOAIMotionMgrIOS& moaiMotionMgr = MOAIMotionMgrIOS::Get ();
	CMMotionManager* motionMgr = MOAIMotionMgrIOS::Get ().mMotionMgr;
	NSOperationQueue* queue = MOAIMotionMgrIOS::Get ().mOperationQueue;
	
	if ( !motionMgr ) return 0;
	
	bool active = state.GetValue < bool >( 1, false );
	double interval = state.GetValue < double >( 2, 0.0 );
	u32 referenceFrame = state.GetValue < u32 >( 3, ( u32 )[ motionMgr attitudeReferenceFrame ]);
	
	bool isActive = [ motionMgr isDeviceMotionActive ];
	
	if ( active != isActive ) {
		if ( active ) {
		
			[ motionMgr setDeviceMotionUpdateInterval:( NSTimeInterval )interval ];
			[ motionMgr
				startDeviceMotionUpdatesUsingReferenceFrame:( CMAttitudeReferenceFrame )referenceFrame
				toQueue:queue withHandler:^( CMDeviceMotion* deviceMotion,
				NSError* error
			) {
				
				if ( error ) {
					[ motionMgr stopDeviceMotionUpdates ];
					// TODO: log error
				}
				else {
					dispatch_async ( dispatch_get_main_queue (), ^( void ) {
					
						if ( MOAIMotionMgrIOS::IsValid ()) {
							
							const CMRotationRate& rotationRate		= [ deviceMotion rotationRate ];
							const CMAcceleration& userAcceleration	= [ deviceMotion userAcceleration ];
							const CMAcceleration& gravity			= [ deviceMotion gravity ];
							
							const CMCalibratedMagneticField& calibratedMagneticField = [ deviceMotion magneticField ];
							const CMMagneticField& magneticField = calibratedMagneticField.field;
							
							// TODO: consolidate these once custom event handlers are implemented
							MOAIVectorSensor::EnqueueVectorEvent (
								moaiMotionMgr.mInputDeviceID,
								MOAIDeviceMotionSensorID::GYRO,
								rotationRate.x,
								rotationRate.y,
								rotationRate.z
							);
							
							MOAIVectorSensor::EnqueueVectorEvent (
								moaiMotionMgr.mInputDeviceID,
								MOAIDeviceMotionSensorID::ACCELEROMETER,
								userAcceleration.x,
								userAcceleration.y,
								userAcceleration.z
							);
							
							MOAIVectorSensor::EnqueueVectorEvent (
								moaiMotionMgr.mInputDeviceID,
								MOAIDeviceMotionSensorID::GRAVITY,
								gravity.x,
								gravity.y,
								gravity.z
							);
							
							MOAIVectorSensor::EnqueueVectorEvent (
								moaiMotionMgr.mInputDeviceID,
								MOAIDeviceMotionSensorID::MAGNETOMETER,
								magneticField.x,
								magneticField.y,
								magneticField.z
							);
						}
					});
				}
			}];
		}
		else {
			[ motionMgr stopDeviceMotionUpdates ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_setDeviceMotionUpdateInterval ( lua_State* L ) {
	MOAILuaState state ( L );
	
	CMMotionManager* motionMgr = MOAIMotionMgrIOS::Get ().mMotionMgr;
	if ( !motionMgr ) return 0;
	
	double interval = state.GetValue < double >( 1, 0.0 );
	[ motionMgr setDeviceMotionUpdateInterval:( NSTimeInterval )interval ];
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_setGyroActive ( lua_State* L ) {
	MOAILuaState state ( L );
	
	MOAIMotionMgrIOS& moaiMotionMgr = MOAIMotionMgrIOS::Get ();
	CMMotionManager* motionMgr = MOAIMotionMgrIOS::Get ().mMotionMgr;
	NSOperationQueue* queue = MOAIMotionMgrIOS::Get ().mOperationQueue;
	
	if ( !motionMgr ) return 0;
	
	bool active = state.GetValue < bool >( 1, false );
	double interval = state.GetValue < double >( 2, 0.0 );
	
	bool isActive = [ motionMgr isGyroActive ];
	
	if ( active != isActive ) {
		if ( active ) {
		
			[ motionMgr setGyroUpdateInterval:( NSTimeInterval )interval ];
			[ motionMgr startGyroUpdatesToQueue:queue withHandler:^( CMGyroData* gyroData, NSError* error ) {
				
				if ( error ) {
					[ motionMgr stopGyroUpdates ];
					// TODO: log error
				}
				else {
					dispatch_async ( dispatch_get_main_queue (), ^( void ) {
					
						if ( MOAIMotionMgrIOS::IsValid ()) {
							const CMRotationRate& rotationRate = [ gyroData rotationRate ];
							MOAIVectorSensor::EnqueueVectorEvent (
								moaiMotionMgr.mRawInputDeviceID,
								MOAIRawMotionSensorID::GYRO,
								rotationRate.x,
								rotationRate.y,
								rotationRate.z
							);
						}
					});
				}
			}];
		}
		else {
			[ motionMgr stopGyroUpdates ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_setGyroUpdateInterval ( lua_State* L ) {
	MOAILuaState state ( L );
	
	CMMotionManager* motionMgr = MOAIMotionMgrIOS::Get ().mMotionMgr;
	if ( !motionMgr ) return 0;
	
	double interval = state.GetValue < double >( 1, 0.0 );
	[ motionMgr setGyroUpdateInterval:( NSTimeInterval )interval ];
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_setMagnetometerActive ( lua_State* L ) {
	MOAILuaState state ( L );
	
	MOAIMotionMgrIOS& moaiMotionMgr = MOAIMotionMgrIOS::Get ();
	CMMotionManager* motionMgr = MOAIMotionMgrIOS::Get ().mMotionMgr;
	NSOperationQueue* queue = MOAIMotionMgrIOS::Get ().mOperationQueue;
	
	if ( !motionMgr ) return 0;
	
	bool active = state.GetValue < bool >( 1, false );
	double interval = state.GetValue < double >( 2, 0.0 );
	
	bool isActive = [ motionMgr isMagnetometerActive ];
	
	if ( active != isActive ) {
		if ( active ) {
			[ motionMgr startMagnetometerUpdatesToQueue:queue withHandler:^( CMMagnetometerData* magnetometerData, NSError* error ) {
				
				if ( error ) {
				
					[ motionMgr setMagnetometerUpdateInterval:( NSTimeInterval )interval ];
					[ motionMgr stopMagnetometerUpdates ];
					// TODO: log error
				}
				else {
					dispatch_async ( dispatch_get_main_queue (), ^( void ) {
					
						if ( MOAIMotionMgrIOS::IsValid ()) {
							
							const CMMagneticField& magneticField = [ magnetometerData magneticField ];
							MOAIVectorSensor::EnqueueVectorEvent (
								moaiMotionMgr.mRawInputDeviceID,
								MOAIRawMotionSensorID::MAGNETOMETER,
								magneticField.x,
								magneticField.y,
								magneticField.z
							);
						}
					});
				}
			}];
		}
		else {
			[ motionMgr stopMagnetometerUpdates ];
		}
	}
	return 0;
}

//----------------------------------------------------------------//
int MOAIMotionMgrIOS::_setMagnetometerUpdateInterval ( lua_State* L ) {
	MOAILuaState state ( L );
	
	CMMotionManager* motionMgr = MOAIMotionMgrIOS::Get ().mMotionMgr;
	if ( !motionMgr ) return 0;
	
	double interval = state.GetValue < double >( 1, 0.0 );
	[ motionMgr setMagnetometerUpdateInterval:( NSTimeInterval )interval ];
	
	return 0;
}

//================================================================//
// MOAIMotionMgrIOS
//================================================================//

//----------------------------------------------------------------//
void MOAIMotionMgrIOS::Init () {

	if ( this->mMotionMgr != nil ) return;

	this->mMotionMgr = [[ CMMotionManager alloc ] init ];
	this->mOperationQueue = [[ NSOperationQueue alloc ] init ];
	
	MOAIInputMgr& inputMgr = MOAIInputMgr::Get ();
	
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
	
	state.Push ( inputMgr.GetDevice ( this->mRawInputDeviceID ));
	lua_setfield ( state, -2, "rawMotion" );
	
	state.Push ( inputMgr.GetDevice ( this->mInputDeviceID ));
	lua_setfield ( state, -2, "deviceMotion" );
}

//----------------------------------------------------------------//
MOAIMotionMgrIOS::MOAIMotionMgrIOS () :
	mInputDeviceID ( 0 ),
	mRawInputDeviceID ( 0 ),
	mMotionMgr ( 0 ),
	mOperationQueue ( 0 ) {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIMotionMgrIOS::~MOAIMotionMgrIOS () {

	if ( this->mMotionMgr ) {
		
		[ this->mMotionMgr stopAccelerometerUpdates ];
		[ this->mMotionMgr stopDeviceMotionUpdates ];
		[ this->mMotionMgr stopGyroUpdates ];
		
		[ this->mMotionMgr release ];
	}
	
	if ( this->mOperationQueue ) {
		[ this->mOperationQueue release ];
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
		{ "isGyroAvailable",					_isGyroAvailable }, // Mmmmmm. Gyro!
		{ "isMagnetometerAvailable",			_isMagnetometerAvailable },
		{ "setAccelerometerActive",				_setAccelerometerActive },
		{ "setAccelerometerUpdateInterval",		_setAccelerometerUpdateInterval },
		{ "setGyroActive",						_setGyroActive },
		{ "setGyroUpdateInterval",				_setGyroUpdateInterval },
		{ "setDeviceMotionActive",				_setDeviceMotionActive },
		{ "setDeviceMotionUpdateInterval",		_setDeviceMotionUpdateInterval },
		{ "setMagnetometerActive",				_setMagnetometerActive },
		{ "setMagnetometerUpdateInterval",		_setMagnetometerUpdateInterval },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

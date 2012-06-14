// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIINPUTMGR_H
#define MOAIINPUTMGR_H

#include <moaicore/MOAILua.h>

class MOAIInputDevice;
class MOAISensor;

//================================================================//
// MOAIInputMgr
//================================================================//
/**	@name	MOAIInputMgr
	@text	Input device class. Has no public methods.
*/
class MOAIInputMgr :
	public MOAIGlobalClass < MOAIInputMgr, MOAILuaObject > {
private:

	USMemStream	mInput;

	USLeanArray < MOAIInputDevice* > mDevices;

	//----------------------------------------------------------------//
	bool				CheckSensor					( u8 deviceID, u8 sensorID, u32 type );
	MOAIInputDevice*	GetDevice					( u8 deviceID );
	MOAISensor*			GetSensor					( u8 deviceID, u8 sensorID );
	void				Reset						();
	void				WriteEventHeader			( u8 deviceID, u8 sensorID, u32 type );

public:

	DECL_LUA_SINGLETON ( MOAIInputMgr )

	//----------------------------------------------------------------//
	void				EnqueueButtonEvent			( u8 deviceID, u8 sensorID, bool down );
	void				EnqueueCompassEvent			( u8 deviceID, u8 sensorID, float heading );
	void				EnqueueKeyboardEvent		( u8 deviceID, u8 sensorID, u32 keyID, bool down );
	void				EnqueueLevelEvent			( u8 deviceID, u8 sensorID, float x, float y, float z );
	void				EnqueueLocationEvent		( u8 deviceID, u8 sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed );
	void				EnqueuePointerEvent			( u8 deviceID, u8 sensorID, int x, int y );
	void				EnqueueTouchEvent			( u8 deviceID, u8 sensorID, u32 touchID, bool down, float x, float y );
	void				EnqueueTouchEventCancel		( u8 deviceID, u8 sensorID );
	void				EnqueueWheelEvent			( u8 deviceID, u8 sensorID, float value );

						MOAIInputMgr				();
						~MOAIInputMgr				();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				ReserveDevices				( u8 total );
	void				ReserveSensors				( u8 deviceID, u8 total );
	void				SetConfigurationName		( cc8* name );
	void				SetDevice					( u8 deviceID, cc8* name );
	void				SetDeviceActive				( u8 deviceID, bool active );
	void				SetSensor					( u8 deviceID, u8 sensorID, cc8* name, u32 type );
	void				Update						();
};

#endif

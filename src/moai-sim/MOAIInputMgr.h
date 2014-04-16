// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIINPUTMGR_H
#define MOAIINPUTMGR_H

class MOAIInputDevice;
class MOAISensor;

//================================================================//
// MOAIInputContext
//================================================================//
// TODO: doxygen
class MOAIInputContext :
	public virtual MOAILuaObject {
protected:

	ZLLeanArray < MOAIInputDevice* > mDevices;

public:

	DECL_LUA_FACTORY ( MOAIInputContext )

	//----------------------------------------------------------------//
	bool				CheckSensor					( u8 deviceID, u8 sensorID, u32 type );
	MOAIInputDevice*	GetDevice					( u8 deviceID );
	MOAISensor*			GetSensor					( u8 deviceID, u8 sensorID );
						MOAIInputContext			();
						~MOAIInputContext			();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveDevices				( u8 total );
	void				ReserveSensors				( u8 deviceID, u8 total );
	void				ResetSensors				();
	void				SetDevice					( u8 deviceID, cc8* name );
	void				SetDeviceActive				( u8 deviceID, bool active );
	void				SetSensor					( u8 deviceID, u8 sensorID, cc8* name, u32 type );
};

//================================================================//
// MOAIInputMgr
//================================================================//
/**	@name	MOAIInputMgr
	@text	Input device class.
*/
class MOAIInputMgr :
	public MOAIGlobalClass < MOAIInputMgr, MOAIInputContext > {
private:

	static const size_t CHUNK_SIZE = 256;

	double	mTimebase;		// used to position timestamps against sim timeline
	double	mTimestamp;		// timestamp for next event
	bool	mDefer;

	// queue for cached events to be processed by runtime
	ZLMemStream			mEventQueue;

	//----------------------------------------------------------------//
	void				WriteEventHeader			( u8 deviceID, u8 sensorID );

	//----------------------------------------------------------------//
	static int			_deferEvents				( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIInputMgr )

	SET ( double, Timebase, mTimebase )
	SET ( double, Timestamp, mTimestamp )

	//----------------------------------------------------------------//
	void				EnqueueButtonEvent			( u8 deviceID, u8 sensorID, bool down );
	void				EnqueueCompassEvent			( u8 deviceID, u8 sensorID, float heading );
	void				EnqueueJoystickEvent			( u8 deviceID, u8 sensorID, float x, float y );
	void				EnqueueKeyboardEvent		( u8 deviceID, u8 sensorID, u32 keyID, bool down );
	void				EnqueueLevelEvent			( u8 deviceID, u8 sensorID, float x, float y, float z );
	void				EnqueueLocationEvent		( u8 deviceID, u8 sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed );
	void				EnqueuePointerEvent			( u8 deviceID, u8 sensorID, int x, int y );
	void				EnqueueTouchEvent			( u8 deviceID, u8 sensorID, u32 touchID, bool down, float x, float y );
	void				EnqueueTouchEventCancel		( u8 deviceID, u8 sensorID );
	void				EnqueueWheelEvent			( u8 deviceID, u8 sensorID, float value );
	void				FlushEvents					( double skip );
						MOAIInputMgr				();
						~MOAIInputMgr				();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				SetConfigurationName		( cc8* name );
	void				SetDeviceExtendedName		( u8 deviceID, cc8* nameExtended );
	void				Update						( double timestep );
};

#endif

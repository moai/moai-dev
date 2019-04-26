// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIINPUTMGR_H
#define MOAIINPUTMGR_H

#include <moai-sim/MOAIAction.h>
#include <moai-sim/MOAIInputDevice.h>
#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAIInputMgr
//================================================================//
/**	@lua	MOAIInputMgr
	@text	Base class for input streams and device sets.
*/
class MOAIInputMgr :
	public ZLContextClass < MOAIInputMgr >,
	public virtual MOAILuaObject,
	public virtual ZLMemStream {
private:

	static const ZLSize CHUNK_SIZE		= 256;

	static const ZLSize MAX_DEVICES		= 0xff;
	static const ZLSize MAX_SENSORS		= 0xff;

	enum {
		INPUT_EVENT,
		FINISHED_UPDATE,
	};

	ZLLeanStack < MOAIInputDevice*, 8 > mDevices;

	double	mTimebase;				// used to position timestamps against sim timeline
	double	mTimestamp;				// timestamp for next event
	
	bool	mAutoTimestamp;
	
	bool	mDefer;
	bool	mSuspended;
	
	double	mAutosuspend;	// if > 0, use as a timeout to susped input
	bool	mAutosuspended;

	double	mLastUpdate;	// last update in *device* time

	MOAILuaSharedPtr < MOAIStream > mRecorder;
	bool mPlayback;
	
	MOAILuaStrongRef	mEventCallback;

	//----------------------------------------------------------------//
	static int			_autoTimestamp				( lua_State* L );
	static int			_deferEvents				( lua_State* L );
	static int			_discardEvents				( lua_State* L );
	static int			_playback					( lua_State* L );
	static int			_setAutosuspend				( lua_State* L );
	static int			_setEventCallback			( lua_State* L );
	static int			_setRecorder				( lua_State* L );
	static int			_suspendEvents				( lua_State* L );

	//----------------------------------------------------------------//
	bool				CanWrite					();
	bool				CheckSensor					( ZLIndex deviceID, ZLIndex sensorID, u32 type );
	void				InvokeCallback				( u32 event, double timestamp );
	size_t				ParseEvents					( ZLStream& stream, double timestep );
	void				Record						( size_t size );
	bool				WriteEventHeader			( ZLIndex deviceID, ZLIndex sensorID, u32 type );

public:

	DECL_LUA_SINGLETON ( MOAIInputMgr )

	SET ( double, Timebase, mTimebase )
	SET ( double, Timestamp, mTimestamp )

	//----------------------------------------------------------------//
	ZLIndex				AddDevice					( cc8* name );
	void				ClearSensorState			();
	void				DeferEvents					( bool defer );
	void				FlushEvents					( double skip );
	MOAIInputDevice*	GetDevice					( ZLIndex deviceID );
	MOAISensor*			GetSensor					( ZLIndex deviceID, ZLIndex sensorID );
	bool				HasEvents					();
	//bool				IsDone						();
						MOAIInputMgr				();
						~MOAIInputMgr				();
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				ReserveDevices				( ZLSize total );
	void				ReserveSensors				( ZLIndex deviceID, ZLSize total );
	void				ResetSensorState			();
	void				SetAutosuspend				( double autosuspend );
	void				SetAutotimestamp			( bool autotimestamp );
	void				SetConfigurationName		( cc8* name );
	void				SetDevice					( ZLIndex deviceID, cc8* name ); // back compat
	void				SetDeviceActive				( ZLIndex deviceID, bool active );
	void				SetDeviceHardwareInfo		( ZLIndex deviceID, cc8* hardwareInfo );
	void				SuspendEvents				( bool suspend );
	void				Update						( double timestep );

	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetSensor ( ZLIndex deviceID, ZLIndex sensorID, cc8* name ) {
	
		MOAIInputDevice* device = this->GetDevice ( deviceID );
		if ( device ) {
			if ( sensorID < device->mSensors.Size ()) {
				MOAISensor* sensor = new TYPE;
				sensor->SetType (( u32 )ZLType::RawID < TYPE >());
				device->SetSensor ( sensorID, name, sensor );
			}
		}
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool WriteEventHeader ( ZLIndex deviceID, ZLIndex sensorID ) {
		return this->WriteEventHeader ( deviceID, sensorID, ( u32 )ZLType::RawID < TYPE >()); // TODO: overflow
	}
};

#endif

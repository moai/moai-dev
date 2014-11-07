// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIINPUTQUEUE_H
#define MOAIINPUTQUEUE_H

#include <moai-sim/MOAIInputDevice.h>
#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAIInputQueue
//================================================================//
/**	@lua	MOAIInputQueue
	@text	Base class for input streams and device sets.
*/
class MOAIInputQueue :
	public virtual MOAILuaObject,
	public virtual ZLMemStream {
private:

	static const size_t CHUNK_SIZE = 256;

	ZLLeanArray < MOAIInputDevice* > mDevices;

	double	mTimebase;		// used to position timestamps against sim timeline
	double	mTimestamp;		// timestamp for next event
	
	bool	mDefer;

	//----------------------------------------------------------------//
	bool				CheckSensor					( u8 deviceID, u8 sensorID, u32 type );
	bool				WriteEventHeader			( u8 deviceID, u8 sensorID, u32 type );

protected:

	//----------------------------------------------------------------//
	template < typename TYPE >
	static int _deferEvents ( lua_State* L ) {
		
		MOAILuaState state ( L );
		bool defer = state.GetValue < bool >( 1, false );
		
		TYPE& global = TYPE::Get ();
		global.DeferEvents ( defer );

		return 0;
	}

public:

	SET ( double, Timebase, mTimebase )
	SET ( double, Timestamp, mTimestamp )

	//----------------------------------------------------------------//
	void				DeferEvents					( bool defer );
	void				FlushEvents					( double skip );
	MOAIInputDevice*	GetDevice					( u8 deviceID );
	MOAISensor*			GetSensor					( u8 deviceID, u8 sensorID );
						MOAIInputQueue				();
						~MOAIInputQueue				();
	void				ReserveDevices				( u8 total );
	void				ReserveSensors				( u8 deviceID, u8 total );
	void				ResetSensors				();
	void				SetConfigurationName		( cc8* name );
	void				SetDevice					( u8 deviceID, cc8* name );
	void				SetDeviceActive				( u8 deviceID, bool active );
	void				SetDeviceHardwareInfo		( u8 deviceID, cc8* hardwareInfo );
	void				Update						( double timestep );

	//----------------------------------------------------------------//
	template < typename TYPE >
	void SetSensor ( u8 deviceID, u8 sensorID, cc8* name ) {
	
		MOAIInputDevice* device = this->GetDevice ( deviceID );
		if ( device ) {
			if ( sensorID < device->mSensors.Size ()) {
				MOAISensor* sensor = new TYPE;
				sensor->SetType ( ZLTypeID < TYPE >::GetID ());
				device->SetSensor ( sensorID, name, sensor );
			}
		}
	}
	
	//----------------------------------------------------------------//
	template < typename TYPE >
	bool WriteEventHeader ( u8 deviceID, u8 sensorID ) {
		return this->WriteEventHeader ( deviceID, sensorID, ZLTypeID < TYPE >::GetID ());
	}
};

#endif

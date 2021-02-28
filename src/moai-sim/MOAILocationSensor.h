// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILOCATIONSENSOR_H
#define MOAILOCATIONSENSOR_H

#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAILocationSensor
//================================================================//
/**	@lua	MOAILocationSensor
	@text	Location services sensor.
*/
class MOAILocationSensor :
	public MOAISensor {
private:

	double mLongitude;
	double mLatitude;
	double mAltitude;
	
	float mHAccuracy;
	float mVAccuracy;

	float mSpeed;

	MOAILuaStrongRef mCallback;

	//----------------------------------------------------------------//
	static int			_getLocation			( lua_State* L );
	static int			_setCallback			( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	DECL_LUA_FACTORY ( MOAILocationSensor )

	//----------------------------------------------------------------//
	static void			EnqueueLocationEvent	( MOAIInputMgr& inputMgr, ZLIndex deviceID, ZLIndex sensorID, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed );
						MOAILocationSensor		( ZLContext& context );
						~MOAILocationSensor		();
	void				ParseEvent				( ZLStream& eventStream );
	
};

#endif

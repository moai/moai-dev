// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILOCATIONSENSOR_H
#define MOAILOCATIONSENSOR_H

#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAILocationSensor
//================================================================//
/**	@name	MOAILocationSensor
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
	static int	_getLocation	( lua_State* L );
	static int	_setCallback	( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAILocationSensor )

	//----------------------------------------------------------------//
	void			HandleEvent				( ZLStream& eventStream );
					MOAILocationSensor		();
					~MOAILocationSensor		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	static void		WriteEvent				( ZLStream& eventStream, double longitude, double latitude, double altitude, float hAccuracy, float vAccuracy, float speed );
};

#endif

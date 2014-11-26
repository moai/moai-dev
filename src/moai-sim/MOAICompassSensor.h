// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICOMPASSSENSOR_H
#define MOAICOMPASSSENSOR_H

#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAICompassSensor
//================================================================//
/**	@lua	MOAICompassSensor
	@text	Device heading sensor.
*/
class MOAICompassSensor :
	public MOAISensor {
private:

	float mHeading;

	MOAILuaStrongRef mCallback;

	//----------------------------------------------------------------//
	static int	_getHeading		( lua_State* L );
	static int	_setCallback	( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAICompassSensor )

	//----------------------------------------------------------------//
	static void			EnqueueCompassEvent		( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, float heading );
						MOAICompassSensor		();
						~MOAICompassSensor		();
	void				ParseEvent				( ZLStream& eventStream );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
};

#endif

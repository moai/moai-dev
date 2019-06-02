// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
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
	static int			_getHeading				( lua_State* L );
	static int			_setCallback			( lua_State* L );

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );

public:

	DECL_LUA_FACTORY ( MOAICompassSensor )

	//----------------------------------------------------------------//
	static void			EnqueueCompassEvent		( ZLIndex deviceID, ZLIndex sensorID, float heading );
						MOAICompassSensor		();
						~MOAICompassSensor		();
	void				ParseEvent				( ZLStream& eventStream );
	
};

#endif

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIWHEELSENSOR_H
#define MOAIWHEELSENSOR_H

#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAIWheelSensor
//================================================================//
/**	@lua	MOAIWheelSensor
	@text	Hardware wheel sensor.
*/
class MOAIWheelSensor :
	public MOAISensor {
private:

	float mValue;
	float mDelta;

	MOAILuaStrongRef mCallback;

	//----------------------------------------------------------------//
	static int	_getValue		( lua_State* L );
	static int	_getDelta		( lua_State* L );
	static int	_setCallback	( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIWheelSensor )

	//----------------------------------------------------------------//
	static void			EnqueueWheelEvent		( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, float value );
						MOAIWheelSensor			();
						~MOAIWheelSensor		();
	void				ParseEvent				( ZLStream& eventStream );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
	void				Reset					();
};

#endif
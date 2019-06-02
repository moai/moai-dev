// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIJOYSTICKSENSOR_H
#define MOAIJOYSTICKSENSOR_H

#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAIJoystickSensor
//================================================================//
/**	@lua	MOAIJoystickSensor
	@text	Analog and digital joystick sensor.
*/
class MOAIJoystickSensor :
	public MOAISensor {
private:

	float mX;
	float mY;
	
	MOAILuaStrongRef	mOnStick;

	//----------------------------------------------------------------//
	static int		_getVector				( lua_State* L );
	static int		_setCallback			( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIJoystickSensor )

	//----------------------------------------------------------------//
	static void			EnqueueJoystickEvent	( ZLIndex deviceID, ZLIndex sensorID, float x, float y );
						MOAIJoystickSensor		();
						~MOAIJoystickSensor		();
	void				ParseEvent				( ZLStream& eventStream );
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
};

#endif

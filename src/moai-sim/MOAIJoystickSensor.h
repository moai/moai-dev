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
	static int			_getVector				( lua_State* L );
	static int			_setCallback			( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	DECL_LUA_FACTORY ( MOAIJoystickSensor )

	//----------------------------------------------------------------//
	static void			EnqueueJoystickEvent	( ZLIndex deviceID, ZLIndex sensorID, float x, float y );
						MOAIJoystickSensor		();
						~MOAIJoystickSensor		();
	void				ParseEvent				( ZLStream& eventStream );
};

#endif

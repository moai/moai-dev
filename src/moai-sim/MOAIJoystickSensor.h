// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
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
	// WHICH JOYSTICK IS IT?
	int		mWhich;
	// 1-BUTTON_DOWN, 2-BUTTON_UP, 3-AXIS, 4-NUM_JOYS, 5-JOY_ADD, 6-JOY_REMOVE, 7-NAME, 8-NUM_AXIS, 9-NUM_BUTTONS, 10-NUM_BALLS
	int		mEvent; 
	
	MOAILuaStrongRef	mOnStick;

	//----------------------------------------------------------------//
	static int		_getVector				( lua_State* L );
	static int		_setCallback			( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIJoystickSensor )

	//----------------------------------------------------------------//
	static void			EnqueueJoystickEvent	( u8 deviceID, u8 sensorID, float x, float y, int which, int event );
						MOAIJoystickSensor		();
						~MOAIJoystickSensor		();
	void				ParseEvent				( ZLStream& eventStream );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
};

#endif

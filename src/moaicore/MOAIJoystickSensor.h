// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIJOYSTICKSENSOR_H
#define MOAIJOYSTICKSENSOR_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAISensor.h>

//================================================================//
// MOAIJoystickSensor
//================================================================//
/**	@name	MOAIJoystickSensor
	@text	Analog and digital joystick sensor.
*/
class MOAIJoystickSensor :
	public MOAISensor {
private:

	float mX;
	float mY;
	
	MOAILuaRef	mOnStick;

	//----------------------------------------------------------------//
	static int		_getVector				( lua_State* L );
	static int		_setCallback			( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIJoystickSensor )

	//----------------------------------------------------------------//
	void			HandleEvent				( USStream& eventStream );
					MOAIJoystickSensor		();
					~MOAIJoystickSensor		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	static void		WriteEvent				( USStream& eventStream, float x, float y );
};

#endif

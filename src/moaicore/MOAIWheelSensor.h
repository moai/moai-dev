// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIWHEELSENSOR_H
#define MOAIWHEELSENSOR_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAISensor.h>

//================================================================//
// MOAIWheelSensor
//================================================================//
/**	@name	MOAIWheelSensor
	@text	Hardware wheel sensor.
*/
class MOAIWheelSensor :
	public MOAISensor {
private:

	float mValue;
	float mDelta;

	MOAILuaRef mCallback;

	//----------------------------------------------------------------//
	static int	_getValue		( lua_State* L );
	static int	_getDelta		( lua_State* L );
	static int	_setCallback	( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIWheelSensor )

	//----------------------------------------------------------------//
	void			HandleEvent				( USStream& eventStream );
					MOAIWheelSensor			();
					~MOAIWheelSensor		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );

	void			Reset					();

	static void		WriteEvent				( USStream& eventStream, float heading );
};

#endif
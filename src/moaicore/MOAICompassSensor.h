// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICOMPASSSENSOR_H
#define MOAICOMPASSSENSOR_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAISensor.h>

//================================================================//
// MOAICompassSensor
//================================================================//
/**	@name	MOAICompassSensor
	@text	Device heading sensor.
*/
class MOAICompassSensor :
	public MOAISensor {
private:

	float mHeading;

	MOAILuaRef mCallback;

	//----------------------------------------------------------------//
	static int	_getHeading		( lua_State* L );
	static int	_setCallback	( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAICompassSensor )

	//----------------------------------------------------------------//
	void			HandleEvent				( USStream& eventStream );
					MOAICompassSensor		();
					~MOAICompassSensor		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	static void		WriteEvent				( USStream& eventStream, float heading );
};

#endif

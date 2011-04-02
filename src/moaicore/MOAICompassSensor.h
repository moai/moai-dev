// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAICOMPASSSENSOR_H
#define MOAICOMPASSSENSOR_H

#include <moaicore/MOAISensor.h>

//================================================================//
// MOAICompassSensor
//================================================================//
/**	@brief	Device heading sensor.
*/
class MOAICompassSensor :
	public MOAISensor {
private:

	float mHeading;

	USLuaRef mCallback;

	//----------------------------------------------------------------//
	static int	_getHeading		( lua_State* L );
	static int	_setCallback	( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAICompassSensor )

	//----------------------------------------------------------------//
	void			HandleEvent				( USStream& eventStream );
					MOAICompassSensor		();
					~MOAICompassSensor		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	static void		WriteEvent				( USStream& eventStream, float heading );
};

#endif

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAILEVELSENSOR_H
#define MOAILEVELSENSOR_H

#include <moaicore/MOAISensor.h>

//================================================================//
// MOAILevelSensor
//================================================================//
/**	@name	MOAILevelSensor
	@text	Gravity/acceleration sensor.
*/
class MOAILevelSensor :
	public MOAISensor {
private:

	float	mX;
	float	mY;
	float	mZ;

	USLuaRef mCallback;

	//----------------------------------------------------------------//
	static int	_getLevel		( lua_State* L );
	static int	_setCallback	( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAILevelSensor )

	//----------------------------------------------------------------//
	void			HandleEvent				( USStream& eventStream );
					MOAILevelSensor			();
					~MOAILevelSensor		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	static void		WriteEvent				( USStream& eventStream, float x, float y, float z );
};

#endif

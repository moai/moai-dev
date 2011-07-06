// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMOTIONSENSOR_H
#define MOAIMOTIONSENSOR_H

#include <moaicore/MOAISensor.h>

//================================================================//
// MOAIMotionSensor
//================================================================//
/**	@name	MOAIMotionSensor
	@text	Gravity/acceleration sensor.
*/
class MOAIMotionSensor :
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

	DECL_LUA_FACTORY ( MOAIMotionSensor )

	//----------------------------------------------------------------//
	void			HandleEvent				( USStream& eventStream );
					MOAIMotionSensor			();
					~MOAIMotionSensor		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	static void		WriteEvent				( USStream& eventStream, float x, float y, float z );
};

#endif

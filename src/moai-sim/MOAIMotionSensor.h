// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIMOTIONSENSOR_H
#define MOAIMOTIONSENSOR_H

#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAIMotionSensor
//================================================================//
/**	@lua	MOAIMotionSensor
	@text	Gravity/acceleration sensor.
*/
class MOAIMotionSensor :
	public MOAISensor {
private:

	float	mX;
	float	mY;
	float	mZ;

	MOAILuaStrongRef mCallback;

	//----------------------------------------------------------------//
	static int			_getLevel				( lua_State* L );
	static int			_setCallback			( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	DECL_LUA_FACTORY ( MOAIMotionSensor )

	//----------------------------------------------------------------//
	static void			EnqueueLevelEvent		( ZLIndex deviceID, ZLIndex sensorID, float x, float y, float z );
						MOAIMotionSensor		();
						~MOAIMotionSensor		();
	void				ParseEvent				( ZLStream& eventStream );
};

#endif

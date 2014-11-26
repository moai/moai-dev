// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIVECTORSENSOR_H
#define MOAIVECTORSENSOR_H

#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAIVectorSensor
//================================================================//
/**	@name	MOAIVectorSensor
	@text	General purpose 3-tuple
*/
class MOAIVectorSensor :
	public MOAISensor {
private:

	float	mX;
	float	mY;
	float	mZ;

	MOAILuaStrongRef mCallback;

	//----------------------------------------------------------------//
	static int		_getVector				( lua_State* L );
	static int		_setCallback			( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIVectorSensor )

	//----------------------------------------------------------------//
	static void			EnqueueVectorEvent		( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, float x, float y, float z );
						MOAIVectorSensor		();
						~MOAIVectorSensor		();
	void				ParseEvent				( ZLStream& eventStream );
	void				RegisterLuaClass		( MOAILuaState& state );
	void				RegisterLuaFuncs		( MOAILuaState& state );
};

#endif
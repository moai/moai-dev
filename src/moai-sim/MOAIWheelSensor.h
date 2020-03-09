// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIWHEELSENSOR_H
#define MOAIWHEELSENSOR_H

#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAIWheelSensor
//================================================================//
/**	@lua	MOAIWheelSensor
	@text	Hardware wheel sensor.
*/
class MOAIWheelSensor :
	public MOAISensor {
private:

	float mValue;
	float mDelta;

	MOAILuaStrongRef mCallback;

	//----------------------------------------------------------------//
	static int			_getValue				( lua_State* L );
	static int			_getDelta				( lua_State* L );
	static int			_setCallback			( lua_State* L );

	//----------------------------------------------------------------//
	void				MOAILuaObject_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	DECL_LUA_FACTORY ( MOAIWheelSensor )

	//----------------------------------------------------------------//
	static void			EnqueueWheelEvent		( ZLIndex deviceID, ZLIndex sensorID, float value );
						MOAIWheelSensor			();
						~MOAIWheelSensor		();
	void				ParseEvent				( ZLStream& eventStream );
	void				ResetState				();
};

#endif

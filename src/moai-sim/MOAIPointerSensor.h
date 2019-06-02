// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPOINTERSENSOR_H
#define MOAIPOINTERSENSOR_H

#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAIPointerSensor
//================================================================//
/**	@lua	MOAIPointerSensor
	@text	Pointer sensor.
*/
class MOAIPointerSensor :
	public MOAISensor {
private:

	int mX;
	int mY;

	MOAILuaStrongRef	mOnMove;

	//----------------------------------------------------------------//
	static int		_getLoc					( lua_State* L );
	static int		_setCallback			( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIPointerSensor )

	//----------------------------------------------------------------//
	static void			EnqueuePointerEvent		( ZLIndex deviceID, ZLIndex sensorID, int x, int y );
						MOAIPointerSensor		();
						~MOAIPointerSensor		();
	void				ParseEvent				( ZLStream& eventStream );
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
};

#endif

// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIVECTORSENSOR_H
#define MOAIVECTORSENSOR_H

#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAIVectorSensor
//================================================================//
/**	@lua	MOAIVectorSensor
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
	static int			_getVector				( lua_State* L );
	static int			_setCallback			( lua_State* L );

	//----------------------------------------------------------------//
	void				_RegisterLuaClass		( RTTIVisitorHistory& history, MOAILuaState& state );
	void				_RegisterLuaFuncs		( RTTIVisitorHistory& history, MOAILuaState& state );

public:

	DECL_LUA_FACTORY ( MOAIVectorSensor )

	//----------------------------------------------------------------//
	static void			EnqueueVectorEvent		( MOAIInputMgr& inputMgr, ZLIndex deviceID, ZLIndex sensorID, float x, float y, float z );
						MOAIVectorSensor		( ZLContext& context );
						~MOAIVectorSensor		();
	void				ParseEvent				( ZLStream& eventStream );
};

#endif

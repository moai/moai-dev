// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIPOINTERSENSOR_H
#define MOAIPOINTERSENSOR_H

#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAIPointerSensor
//================================================================//
/**	@name	MOAIPointerSensor
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
	void			HandleEvent				( ZLStream& eventStream );
					MOAIPointerSensor		();
					~MOAIPointerSensor		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	static void		WriteEvent				( ZLStream& eventStream, int x, int y );
};

#endif

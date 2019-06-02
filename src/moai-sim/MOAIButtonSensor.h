// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBUTTONSETSENSOR_H
#define MOAIBUTTONSETSENSOR_H

#include <moai-sim/MOAISensor.h>

//================================================================//
// MOAIButtonSensor
//================================================================//
/**	@lua	MOAIButtonSensor
	@text	Button sensor.
*/
class MOAIButtonSensor :
	public MOAISensor {
private:

	enum {
		IS_DOWN		= 1 << 0x00,
		DOWN		= 1 << 0x01,
		UP			= 1 << 0x02,
	};

	u32	mState;

	MOAILuaStrongRef	mOnButton;

	//----------------------------------------------------------------//
	static int		_down				( lua_State* L );
	static int		_isDown				( lua_State* L );
	static int		_isUp				( lua_State* L );
	static int		_up					( lua_State* L );
	static int		_setCallback		( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIButtonSensor )

	//----------------------------------------------------------------//
	bool				ButtonDown				();
	bool				ButtonIsDown			();
	bool				ButtonIsUp				();
	bool				ButtonUp				();
	void				ClearState				();
	static void			EnqueueButtonEvent		( ZLIndex deviceID, ZLIndex sensorID, bool down );
						MOAIButtonSensor		();
						~MOAIButtonSensor		();
	void				ParseEvent				( ZLStream& eventStream );
	void				MOAILuaObject_RegisterLuaClass		( MOAIComposer& composer, MOAILuaState& state );
	void				MOAILuaObject_RegisterLuaFuncs		( MOAIComposer& composer, MOAILuaState& state );
	void				ResetState				();
};

#endif

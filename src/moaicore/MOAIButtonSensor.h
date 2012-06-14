// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIBUTTONSETSENSOR_H
#define MOAIBUTTONSETSENSOR_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAISensor.h>

//================================================================//
// MOAIButtonSensor
//================================================================//
/**	@name	MOAIButtonSensor
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

	MOAILuaRef	mOnButton;

	//----------------------------------------------------------------//
	static int		_down				( lua_State* L );
	static int		_isDown				( lua_State* L );
	static int		_isUp				( lua_State* L );
	static int		_up					( lua_State* L );
	static int		_setCallback		( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIButtonSensor )

	//----------------------------------------------------------------//
	bool			ButtonDown				();
	bool			ButtonIsDown			();
	bool			ButtonIsUp				();
	bool			ButtonUp				();
	void			HandleEvent				( USStream& eventStream );
					MOAIButtonSensor		();
					~MOAIButtonSensor		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Reset					();
	static void		WriteEvent				( USStream& eventStream, bool down );
};

#endif

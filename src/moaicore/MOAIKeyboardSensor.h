// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIKEYBOARDSENSOR_H
#define MOAIKEYBOARDSENSOR_H

#include <moaicore/MOAILua.h>
#include <moaicore/MOAISensor.h>

//================================================================//
// MOAIKeyCodes
//================================================================//
namespace MOAIKeyCodes {
	enum {
		SHIFT	= 256,
		CONTROL,
		ALT,
		TOTAL,
	};
};

//================================================================//
// MOAIKeyboardSensor
//================================================================//
/**	@name	MOAIKeyboardSensor
	@text	Hardware keyboard sensor.
*/
class MOAIKeyboardSensor :
	public MOAISensor {
private:

	enum {
		IS_DOWN		= 1 << 0x00,
		DOWN		= 1 << 0x01,
		UP			= 1 << 0x02,
	};

	u32 mState [ MOAIKeyCodes::TOTAL ];

	u32 mClearCount;
	u32 mClearQueue [ MOAIKeyCodes::TOTAL ];
	
	MOAILuaRef		mOnKey;

	//----------------------------------------------------------------//
	static int		_keyDown				( lua_State* L );
	static int		_keyIsDown				( lua_State* L );
	static int		_keyIsUp				( lua_State* L );
	static int		_keyUp					( lua_State* L );
	static int		_setCallback			( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIKeyboardSensor )

	//----------------------------------------------------------------//
	void			HandleEvent				( USStream& eventStream );
	bool			KeyDown					( u32 keyID );
	bool			KeyIsDown				( u32 keyID );
	bool			KeyIsUp					( u32 keyID );
	bool			KeyUp					( u32 keyID );
					MOAIKeyboardSensor		();
					~MOAIKeyboardSensor		();
	void			RegisterLuaClass		( MOAILuaState& state );
	void			RegisterLuaFuncs		( MOAILuaState& state );
	void			Reset					();
	static void		WriteEvent				( USStream& eventStream, u32 key, bool down );
};

#endif

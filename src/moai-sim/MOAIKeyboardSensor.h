// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIKEYBOARDSENSOR_H
#define MOAIKEYBOARDSENSOR_H

#include <moai-sim/MOAISensor.h>
#include <moai-sim/MOAIKeyCode.h>
#include <moai-sim/MOAIKeyCodeEnum.h>

//================================================================//
// MOAIKeyboardSensor
//================================================================//
/**	@lua	MOAIKeyboardSensor
	@text	Hardware keyboard sensor.


*/
class MOAIKeyboardSensor :
	public MOAISensor {
private:

	enum {
		IS_DOWN		= 1 << 0x00,	// The key is currently pressed
		DOWN		= 1 << 0x01,	// The key was pressed during the last iteration
		UP			= 1 << 0x02,	// The key was released during the last iteration
	};

	u32 mState [ MOAI_KEY_TOTAL ];

	u32 mClearCount;
	u32 mClearQueue [ MOAI_KEY_TOTAL ];	// The keys whose DOWN or UP flags are set
	
	MOAILuaStrongRef		mOnKey;
	MOAILuaStrongRef		mOnChar;
	MOAILuaStrongRef		mOnEdit;

	//----------------------------------------------------------------//
	static int		_keyDown				( lua_State* L );
	static int		_keyIsDown				( lua_State* L );
	static int		_keyIsUp				( lua_State* L );
	static int		_keyUp					( lua_State* L );
	static int		_setCallback			( lua_State* L );
	static int		_setCharCallback		( lua_State* L );
	static int		_setEditCallback		( lua_State* L );
	static int		_setKeyCallback			( lua_State* L );

public:

	DECL_LUA_FACTORY ( MOAIKeyboardSensor )

	//----------------------------------------------------------------//
	static void			EnqueueKeyboardCharEvent	( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, u32 unicodeChar );
	static void			EnqueueKeyboardEditEvent	( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, char const* text, u32 start, u32 editLength, u32 maxLength );
	static void			EnqueueKeyboardKeyEvent		( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, u32 keyID, bool down );
	static void			EnqueueKeyboardTextEvent	( MOAIInputQueue& queue, u8 deviceID, u8 sensorID, cc8* text );
	static int			CheckKeys					( lua_State* L, bool ( MOAIKeyboardSensor::*predicate )( u32 keyCode ));
	bool				KeyDown						( u32 keyID );
	bool				KeyIsDown					( u32 keyID );
	bool				KeyIsUp						( u32 keyID );
	bool				KeyUp						( u32 keyID );
						MOAIKeyboardSensor			();
						~MOAIKeyboardSensor			();
	void				ParseEvent					( ZLStream& eventStream );
	void				RegisterLuaClass			( MOAILuaState& state );
	void				RegisterLuaFuncs			( MOAILuaState& state );
	void				Reset						();
};

#endif

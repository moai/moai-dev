// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAITOUCHSENSOR_H
#define MOAITOUCHSENSOR_H

#include <moaicore/MOAISensor.h>

//================================================================//
// MOAITouch
//================================================================//
class MOAITouch {
private:

	friend class MOAITouchSensor;
	
	u32		mState;
	
	float	mX;
	float	mY;
	
	u32		mTouchID;
	u32		mTapCount;
};

//================================================================//
// MOAITouchSensor
//================================================================//
/**	@brief Multitouch sensor.
*/
class MOAITouchSensor :
	public MOAISensor {
private:

	enum {
		IS_DOWN		= 1 << 0x00,
		DOWN		= 1 << 0x01,
		UP			= 1 << 0x02,
	};

	static const u32 MAX_TOUCHES		= 16;
	static const u32 UNKNOWN_TOUCH		= 0xffffffff;

	MOAITouch	mTouches [ MAX_TOUCHES ];
	u32			mAllocStack [ MAX_TOUCHES ];
	u32			mActiveStack [ MAX_TOUCHES ];
	u32			mTop;

	USLuaRef	mCallback;

	//----------------------------------------------------------------//
	static int		_down					( lua_State* L );
	static int		_getActiveTouches		( lua_State* L );
	static int		_getTouch				( lua_State* L );
	static int		_hasTouches				( lua_State* L );
	static int		_isDown					( lua_State* L );
	static int		_setCallback			( lua_State* L );
	static int		_up						( lua_State* L );

	//----------------------------------------------------------------//
	u32				AddTouch				();
	void			Clear					();
	u32				FindTouch				( u32 touchID );
	void			PrintStacks				();

public:

	enum {
		TOUCH_DOWN,
		TOUCH_MOVE,
		TOUCH_UP,
		TOUCH_CANCEL,
	};

	DECL_LUA_DATA ( MOAITouchSensor )

	//----------------------------------------------------------------//
	void			HandleEvent				( USStream& eventStream );
					MOAITouchSensor			();
					~MOAITouchSensor		();
	void			RegisterLuaClass		( USLuaState& state );
	void			RegisterLuaFuncs		( USLuaState& state );
	void			Reset					();
	STLString		ToString				();
	static void		WriteEvent				( USStream& eventStream, u32 touchID, bool down, float x, float y, u32 tapCount );
	static void		WriteEventCancel		( USStream& eventStream );
};

#endif

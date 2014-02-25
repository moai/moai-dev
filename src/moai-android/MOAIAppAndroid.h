// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPPANDROID_H
#define	MOAIAPPANDROID_H

#include <moai-core/headers.h>

//================================================================//
// MOAIAppAndroid
//================================================================//
/**	@name	MOAIAppAndroid
	@text	Wrapper for base application class on Android devices.
			Exposed to Lua via MOAIApp on all mobile platforms.

	@const	SESSION_START			Event code indicating the beginning of an app session.
	@const	SESSION_END				Event code indicating the end of an app sessions.
	@const	BACK_BUTTON_PRESSED		Event code indicating that the physical device back button was pressed.
*/
class MOAIAppAndroid :
	public MOAIGlobalClass < MOAIAppAndroid, MOAIGlobalEventSource > {
private:

	enum {
		ACTIVITY_ON_CREATE,
		ACTIVITY_ON_DESTROY,
		ACTIVITY_ON_START,
		ACTIVITY_ON_STOP,
		ACTIVITY_ON_PAUSE,
		ACTIVITY_ON_RESUME,
		ACTIVITY_ON_RESTART,
		BACK_BUTTON_PRESSED,
	};

	//----------------------------------------------------------------//
	static int	_exitGame			( lua_State* L );
	static int	_getUTCTime			( lua_State* L );
	static int 	_getStatusBarHeight ( lua_State* L );
	static int	_sendMail			( lua_State* L );
	static int	_share				( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIAppAndroid )

	//----------------------------------------------------------------//
			MOAIAppAndroid				();
			~MOAIAppAndroid				();
	void	RegisterLuaClass			( MOAILuaState& state );
};

#endif

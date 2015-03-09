// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPPANDROID_H
#define	MOAIAPPANDROID_H

#include <moai-sim/host.h>
#include <moai-core/headers.h>
#include <moai-android/JniUtils.h>

#include <pthread.h>

//================================================================//
// MOAIAppAndroid
//================================================================//
/**	@lua	MOAIAppAndroid
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
		APP_OPENED_FROM_URL,
		BACK_BUTTON_PRESSED,
		EVENT_PICTURE_TAKEN,
	};

	//----------------------------------------------------------------//
	static int	_getPictureCode			( lua_State* L );
	static int	_getPicturePath			( lua_State* L );
	static int	_getUTCTime				( lua_State* L );
	static int 	_getStatusBarHeight 	( lua_State* L );
	static int	_openURL				( lua_State* L );
	static int	_sendMail				( lua_State* L );
	static int	_share					( lua_State* L );
	static int	_takePicture			( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIAppAndroid )

	//----------------------------------------------------------------//
	void	AppOpenedFromURL			( jstring url );
			MOAIAppAndroid				();
			~MOAIAppAndroid				();
	void	NotifyPictureTaken			();
	void	PushPictureCode				( MOAILuaState& state );
	void	PushPictureData				( MOAILuaState& state );
	void	PushPicturePath				( MOAILuaState& state );
	void	RegisterLuaClass			( MOAILuaState& state );
};

#endif

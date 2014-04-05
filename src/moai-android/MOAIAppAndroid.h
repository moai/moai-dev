// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPPANDROID_H
#define	MOAIAPPANDROID_H

#include <moai-sim/host.h>
#include <moai-core/headers.h>

#include <pthread.h>

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
	public MOAIGlobalClass < MOAIAppAndroid, MOAILuaObject > {
private:

	enum {
		APP_OPENED_FROM_URL,
		SESSION_START,
		SESSION_END,
		BACK_BUTTON_PRESSED,
		EVENT_PICTURE_TAKEN,
		TOTAL,
	};

	MOAILuaStrongRef	mListeners [ TOTAL ];

	//----------------------------------------------------------------//
	static int	_exitGame			( lua_State* L );
	static int	_getUTCTime			( lua_State* L );
	static int 	_getStatusBarHeight ( lua_State* L );
	static int	_sendMail			( lua_State* L );
	static int	_setListener		( lua_State* L );
	static int	_share				( lua_State* L );


	// ## CAMERA SUPPORT
	static int _takePicture		( lua_State* L );
	static int _getPictureCode	( lua_State* L );
	static int _getPicturePath	( lua_State* L );

	// ## /CAMERA SUPPORT

public:

	DECL_LUA_SINGLETON ( MOAIAppAndroid )

			MOAIAppAndroid				();
			~MOAIAppAndroid				();
	bool	NotifyBackButtonPressed		();
	void	AppOpenedFromURL		( jstring url );
	void	NotifyDidStartSession		( bool resumed );
	void	NotifyWillEndSession		();

//	void	NotifyPictureTaken			( int code, cc8* path );
	void	NotifyPictureTaken			();
	void	PushPictureData( MOAILuaState& state );
	void	PushPictureCode( MOAILuaState& state );
	void	PushPicturePath( MOAILuaState& state );

	void	RegisterLuaClass			( MOAILuaState& state );


};

#endif

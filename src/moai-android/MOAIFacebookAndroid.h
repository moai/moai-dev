// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIFACEBOOKANDROID_H
#define	MOAIFACEBOOKANDROID_H

#ifndef DISABLE_FACEBOOK

#include <moai-core/headers.h>

//================================================================//
// MOAIFacebookAndroid
//================================================================//
/**	@name	MOAIFacebookAndroid
	@text	Wrapper for Facebook integration on Android devices.
			Facebook provides social integration for sharing on
			www.facebook.com. Exposed to Lua via MOAIFacebook on
			all mobile platforms.

	@const	DIALOG_DID_COMPLETE			Event code for a successfully completed Facebook dialog.
	@const	DIALOG_DID_NOT_COMPLETE		Event code for a failed (or canceled) Facebook dialog.
	@const	SESSION_DID_LOGIN			Event code for a successfully completed Facebook login.
	@const	SESSION_DID_NOT_LOGIN		Event code for a failed (or canceled) Facebook login.
	@const	REQUEST_RESPONSE			Event code for graph request responses.
	@const	REQUEST_RESPONSE_FAILED		Event code for failed graph request responses.
*/
class MOAIFacebookAndroid :
	public MOAIGlobalClass < MOAIFacebookAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_extendToken	( lua_State* L );
	static int	_getExpirationDate	( lua_State* L );
	static int	_getToken		( lua_State* L );
	static int	_graphRequest	( lua_State* L );
	static int	_init			( lua_State* L );
	static int	_login			( lua_State* L );
	static int	_logout			( lua_State* L );
	static int	_postToFeed		( lua_State* L );
	static int	_sendRequest	( lua_State* L );
	static int	_sessionValid	( lua_State* L );
	static int	_setListener 	( lua_State* L );
	static int	_setExpirationDate	( lua_State* L );
	static int	_setToken	 	( lua_State* L );

public:

	DECL_LUA_SINGLETON ( MOAIFacebookAndroid );

	enum {
		DIALOG_DID_COMPLETE,
		DIALOG_DID_NOT_COMPLETE,
		SESSION_DID_LOGIN,
		SESSION_DID_NOT_LOGIN,
        REQUEST_RESPONSE,
		REQUEST_RESPONSE_FAILED,
		TOTAL,
	};

	enum {
        DIALOG_RESULT_SUCCESS,
        DIALOG_RESULT_CANCEL,
        DIALOG_RESULT_ERROR,
	};

	MOAILuaRef		mListeners [ TOTAL ];

			MOAIFacebookAndroid		();
			~MOAIFacebookAndroid	();
	void 	NotifyLoginComplete		( int code );
	void 	NotifyDialogComplete	( int code );
	void 	NotifyRequestComplete	( cc8* result );
	void 	NotifyRequestFailed	    ();
	void	RegisterLuaClass		( MOAILuaState& state );
};

#endif  //DISABLE_FACEBOOK

#endif  //MOAIFACEBOOK_H

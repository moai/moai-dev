// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAITWITTERANDROID_H
#define	MOAITWITTERANDROID_H

#include <moai-core/headers.h>

//================================================================//
// MOAITwitterAndroid
//================================================================//
/**	@lua	MOAITwitterAndroid
	@text	Wrapper for Twitter integration on Android devices.
			Twitter provides social integration for sharing on
			www.twitter.com.

	@const	TWEET_SUCCESSFUL			Event indicating a successful Tweet..
	@const	TWEET_CANCELLED		        Event indicating an unsuccessful Tweet.

    @const	SESSION_DID_LOGIN			Event indicating a successfully completed Twitter login.
	@const	SESSION_DID_NOT_LOGIN		Event indicating an unsuccessful (or canceled) Twitter login.
*/
class MOAITwitterAndroid :
	public MOAIGlobalClass < MOAITwitterAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_init			( lua_State* L );
	static int	_login			( lua_State* L );
	static int	_isLoggedIn		( lua_State* L );
	static int	_setAccessToken	( lua_State* L );
	static int	_sendTweet  	( lua_State* L );
	static int	_setListener 	( lua_State* L );

    enum {
        RESULT_SUCCESS,
        RESULT_ERROR,
	};

public:

	DECL_LUA_SINGLETON ( MOAITwitterAndroid );

	enum {
		TWEET_SUCCESSFUL,
		TWEET_CANCELLED,

        SESSION_DID_LOGIN,
		SESSION_DID_NOT_LOGIN,

		TOTAL,
	};

	MOAILuaStrongRef		mListeners [ TOTAL ];

			MOAITwitterAndroid		    ();
			~MOAITwitterAndroid	        ();
	void 	NotifyLoginComplete		    ( int code, cc8* token, cc8* tokenSecret);
	void 	NotifyTweetComplete	        ( int code );
	void	RegisterLuaClass		    ( MOAILuaState& state );
};

#endif  //MOAITWITTER_H

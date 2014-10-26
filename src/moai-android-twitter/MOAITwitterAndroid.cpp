// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android-twitter/MOAITwitterAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	init
	@text	Initialize Twitter.

	@in		string	consumerKey		    OAuth consumer key
	@in		string	consumerSecret	    OAuth consumer secret
	@in		string	callbackUrl			Available in Twitter developer settings.
	@out 	nil
*/
int MOAITwitterAndroid::_init ( lua_State* L ) {

	MOAILuaState state ( L );

	cc8* consumerKey = lua_tostring ( state, 1 );
	cc8* consumerSecret = lua_tostring ( state, 2 );
	cc8* callbackUrl = lua_tostring ( state, 3 );

	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( consumerKey , jconsumerKey );
	JNI_GET_JSTRING ( consumerSecret , jconsumerSecret );
	JNI_GET_JSTRING ( callbackUrl , jcallbackUrl );

	jclass twitter = env->FindClass ( "com/ziplinegames/moai/MoaiTwitter" );
    if ( twitter == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITwitterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTwitter" );
    } else {

    	jmethodID init = env->GetStaticMethodID ( twitter, "init", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V" );
   		if ( init == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITwitterAndroid: Unable to find static java method %s", "init" );
		} else {

			env->CallStaticVoidMethod ( twitter, init, jconsumerKey, jconsumerSecret, jcallbackUrl);
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	login
	@text	Prompt the user to login to Twitter.

	@out 	nil
*/
int MOAITwitterAndroid::_login ( lua_State *L ) {

	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass twitter = env->FindClass ( "com/ziplinegames/moai/MoaiTwitter" );
    if ( twitter == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITwitterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTwitter" );
    } else {

    	jmethodID login = env->GetStaticMethodID ( twitter, "login", "()V" );
    	if ( login == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITwitterAndroid: Unable to find static java method %s", "login" );
    	} else {

			env->CallStaticVoidMethod ( twitter, login);
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	isLoggedIn
	@text   Determine if twitter is currently authorized.

	@out    boolean isLoggedIn		True if logged in, false otherwise.
*/
int MOAITwitterAndroid::_isLoggedIn ( lua_State *L ) {

	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass twitter = env->FindClass ( "com/ziplinegames/moai/MoaiTwitter" );
    if ( twitter == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITwitterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTwitter" );
    } else {

    	jmethodID isLoggedIn = env->GetStaticMethodID ( twitter, "isLoggedIn", "()Z" );
    	if ( isLoggedIn  == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITwitterAndroid: Unable to find static java method %s", "isLoggedIn" );
    	} else {

			jboolean isUserLoggedIn = env->CallStaticBooleanMethod ( twitter, isLoggedIn );
            lua_pushboolean( state, isUserLoggedIn );
		}
	}

	return 1;
}

//----------------------------------------------------------------//
/**	@lua	setAccessToken
	@text	Set the access token that authenticates the user.

	@in		string	token		        OAuth token
	@in		string	tokenSecret	        OAuth token secret
	@out 	nil
*/
int MOAITwitterAndroid::_setAccessToken ( lua_State* L ) {

	MOAILuaState state ( L );

	cc8* token = lua_tostring ( state, 1 );
	cc8* tokenSecret = lua_tostring ( state, 2 );

	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( token , jtoken );
	JNI_GET_JSTRING ( tokenSecret , jtokenSecret );

	jclass twitter = env->FindClass ( "com/ziplinegames/moai/MoaiTwitter" );
    if ( twitter == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITwitterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTwitter" );
    } else {

    	jmethodID setAccessToken = env->GetStaticMethodID ( twitter, "setAccessToken", "(Ljava/lang/String;Ljava/lang/String;)V" );
   		if ( setAccessToken == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITwitterAndroid: Unable to find static java method %s", "setAccessToken" );
		} else {

			env->CallStaticVoidMethod ( twitter, setAccessToken, jtoken, jtokenSecret );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	sendTweet
	@text   Tweet the provided text

	@opt	string	text		 The text for the tweet.
	@out 	nil
*/
int MOAITwitterAndroid::_sendTweet ( lua_State* L ) {

	MOAILuaState state ( L );

	cc8* text = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( text, jtext );

	jclass twitter = env->FindClass ( "com/ziplinegames/moai/MoaiTwitter" );
    if ( twitter == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAITwitterAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiTwitter" );
    } else {

    	jmethodID sendTweet = env->GetStaticMethodID ( twitter, "sendTweet", "(Ljava/lang/String;)V" );
   		if ( sendTweet == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAITwitterAndroid: Unable to find static java method %s", "sendTweet" );
		} else {

			env->CallStaticVoidMethod ( twitter, sendTweet, jtext );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
int MOAITwitterAndroid::_setListener ( lua_State* L ) {

	MOAILuaState state ( L );

	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {

		MOAITwitterAndroid::Get ().mListeners [ idx ].SetRef ( state, 2 );
	}

	return 0;
}

//================================================================//
// MOAITwitterAndroid
//================================================================//

//----------------------------------------------------------------//
MOAITwitterAndroid::MOAITwitterAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAITwitterAndroid::~MOAITwitterAndroid () {

}

//----------------------------------------------------------------//
void MOAITwitterAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "TWEET_SUCCESSFUL",		( u32 ) TWEET_SUCCESSFUL );
	state.SetField ( -1, "TWEET_CANCELLED",	        ( u32 ) TWEET_CANCELLED );

	state.SetField ( -1, "SESSION_DID_LOGIN",		( u32 ) SESSION_DID_LOGIN );
	state.SetField ( -1, "SESSION_DID_NOT_LOGIN",	( u32 ) SESSION_DID_NOT_LOGIN );

	luaL_Reg regTable [] = {
		{ "init",			_init },
		{ "login",			_login },
		{ "isLoggedIn",		_isLoggedIn },
		{ "setAccessToken",	_setAccessToken },
		{ "sendTweet",	    _sendTweet },
		{ "setListener",	_setListener },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAITwitterAndroid::NotifyLoginComplete ( int code, cc8* token, cc8* tokenSecret ) {

	MOAILuaRef& callback = this->mListeners [ SESSION_DID_NOT_LOGIN ];
	if ( code == RESULT_SUCCESS ) {

		callback = this->mListeners [ SESSION_DID_LOGIN ];
	}

	if ( callback ) {

		MOAIScopedLuaState state = callback.GetSelf ();

        lua_pushstring ( state, token );
		lua_pushstring ( state, tokenSecret );

		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAITwitterAndroid::NotifyTweetComplete ( int code ) {

	MOAILuaRef& callback = this->mListeners [ TWEET_CANCELLED ];
	if ( code == RESULT_SUCCESS ) {

		callback = this->mListeners [ TWEET_SUCCESSFUL ];
	}

	if ( callback ) {

		MOAIScopedLuaState state = callback.GetSelf ();

		state.DebugCall ( 0, 0 );
	}
}

//================================================================//
// Twitter JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiTwitter_AKUNotifyTwitterLoginComplete ( JNIEnv* env, jclass obj,
                                                                    jint code, jstring jtoken, jstring jtokenSecret ) {

	JNI_GET_CSTRING ( jtoken, token );
	JNI_GET_CSTRING ( jtokenSecret, tokenSecret );

	MOAITwitterAndroid::Get ().NotifyLoginComplete ( code, token, tokenSecret );

	JNI_RELEASE_CSTRING ( jtoken, token );
	JNI_RELEASE_CSTRING ( jtokenSecret, tokenSecret );
}

//----------------------------------------------------------------//
extern "C" JNIEXPORT void JNICALL Java_com_ziplinegames_moai_MoaiTwitter_AKUNotifyTwitterTweetComplete ( JNIEnv* env, jclass obj, jint code ) {

	MOAITwitterAndroid::Get ().NotifyTweetComplete ( code );
}

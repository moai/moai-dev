// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android/JniUtils.h>
#include <moai-android/MOAIBrowserAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	canOpenURL
	@text	Return true if the device has an app installed that can open the URL.

	@in		string	url				The URL to check.
	@out 	boolean
*/
int MOAIBrowserAndroid::_canOpenURL ( lua_State* L ) {

	MOAILuaState state ( L );

	cc8* url = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	JNI_GET_JSTRING ( url, jurl );

	jclass moaiBrowser = env->FindClass ( "com/ziplinegames/moai/MoaiBrowser" );
    if ( moaiBrowser == NULL ) {
		ZLLog::Print ( "MOAIBrowserAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiBrowser" );
    } else {
    	jmethodID canOpenURL = env->GetStaticMethodID ( moaiBrowser, "canOpenURL", "(Ljava/lang/String;)Z" );
    	if ( canOpenURL == NULL ) {
			ZLLog::Print ( "MOAIBrowserAndroid: Unable to find static java method %s", "canOpenURL" );
    	} else {
			jboolean jsuccess = ( jboolean )env->CallStaticBooleanMethod ( moaiBrowser, canOpenURL, jurl );
			lua_pushboolean( state, jsuccess );
			return 1;
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	openURL
	@text	Open the given URL in the device browser.

	@in		string	url				The URL to open.
	@out 	nil
*/
int MOAIBrowserAndroid::_openURL ( lua_State* L ) {

	MOAILuaState state ( L );

	cc8* url = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( url, jurl );

	jclass moaiBrowser = env->FindClass ( "com/ziplinegames/moai/MoaiBrowser" );
    if ( moaiBrowser == NULL ) {

		ZLLog::Print ( "MOAIBrowserAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiBrowser" );
    } else {

    	jmethodID openURL = env->GetStaticMethodID ( moaiBrowser, "openURL", "(Ljava/lang/String;)V" );
    	if ( openURL == NULL ) {

			ZLLog::Print ( "MOAIBrowserAndroid: Unable to find static java method %s", "openURL" );
    	} else {

			env->CallStaticVoidMethod ( moaiBrowser, openURL, jurl );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	openURLWithParams
	@text	Open the native device web browser at the specified URL
			with the specified list of query string parameters.

	@in		string url
	@in		table params
	@out	nil
*/
int MOAIBrowserAndroid::_openURLWithParams ( lua_State* L ) {

	MOAILuaState state ( L );

	cc8* url = lua_tostring ( state, 1 );

    jobject params;
    if ( state.IsType ( 2, LUA_TTABLE ) ) {
        params = JniUtils::bundleFromLua( L, 2 );
    }

	if ( url == NULL || params == NULL ) return 0;

    JNI_GET_ENV ( jvm, env );
	JNI_GET_JSTRING ( url, jurl );

	jclass moaiBrowser = env->FindClass ( "com/ziplinegames/moai/MoaiBrowser" );
    if ( moaiBrowser == NULL ) {
		ZLLog::Print ( "MOAIBrowserAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiBrowser" );
    } else {
    	jmethodID openURL = env->GetStaticMethodID ( moaiBrowser, "openURLWithParams", "(Ljava/lang/String;Landroid/os/Bundle;)V" );
    	if ( openURL == NULL ) {
			ZLLog::Print ( "MOAIBrowserAndroid: Unable to find static java method %s", "openURLWithParams" );
    	} else {
			env->CallStaticVoidMethod ( moaiBrowser, openURL, jurl, params );
		}
	}

	return 0;
}

//================================================================//
// MOAIBrowserAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIBrowserAndroid::MOAIBrowserAndroid () {
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIBrowserAndroid::~MOAIBrowserAndroid () {}

//----------------------------------------------------------------//
void MOAIBrowserAndroid::RegisterLuaClass ( MOAILuaState& state ) {
	luaL_Reg regTable [] = {
		{ "canOpenURL",				_canOpenURL },
		{ "openURL",				_openURL },
		{ "openURLWithParams",		_openURLWithParams },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

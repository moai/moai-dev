// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/JniUtils.h>
#include <moai-android/MOAIBrowserAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	canOpenURL
	@text	Return true if the device has an app installed that can open the URL.

	@in		string	url				The URL to check.
	@out 	boolean
*/
int MOAIBrowserAndroid::_canOpenURL ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIBrowserAndroid, "" )

    ZLLogF ( ZLLog::CONSOLE, "MOAIBrowserAndroid: _canOpenURL");

    JNI_GET_ENV ( jvm, env );

    jclass browser = env->FindClass ( "com/moaisdk/core/MoaiBrowser" );
    jmethodID canOpenUrl = env->GetStaticMethodID ( browser, "canOpenURL", "(Ljava/lang/String;)Z" );

	MOAIJString jurl = self->GetJString ( lua_tostring ( state, 1 ));
	lua_pushboolean( state, self->CallStaticBooleanMethod ( canOpenUrl, ( jstring )jurl ));
	return 1;
}

//----------------------------------------------------------------//
/**	@lua	openURL
	@text	Open the given URL in the device browser.

	@in		string	url				The URL to open.
	@out 	nil
*/
int MOAIBrowserAndroid::_openURL ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIBrowserAndroid, "" )

	ZLLogF ( ZLLog::CONSOLE, "MOAIBrowserAndroid: _openURL");

    JNI_GET_ENV ( jvm, env );

    jclass browser = env->FindClass ( "com/moaisdk/core/MoaiBrowser" );
    jmethodID openUrl = env->GetStaticMethodID ( browser, "openURL", "(Ljava/lang/String;)V" );

	MOAIJString jurl = self->GetJString ( lua_tostring ( state, 1 ));
	self->CallStaticVoidMethod ( openUrl, ( jstring )jurl );
	return 0;
}

//----------------------------------------------------------------//
/**	@lua	openURLWithParams
	@text	Open the native device web browser at the specified URL
			with the specified list of query string parameters.

	@in		string url
	@in		table params
	@out	nil
*/
int MOAIBrowserAndroid::_openURLWithParams ( lua_State* L ) {
	MOAI_JAVA_LUA_SETUP ( MOAIBrowserAndroid, "" )

	ZLLogF ( ZLLog::CONSOLE, "MOAIBrowserAndroid: _openURLWithParams");

	cc8* url = lua_tostring ( state, 1 );

    jobject params;
    if ( state.IsType ( 2, LUA_TTABLE ) ) {
        params = self->BundleFromLua( L, 2 );
    }

	if ( url == NULL || params == NULL ) return 0;

	JNI_GET_ENV ( jvm, env );

	jclass browser = env->FindClass ( "com/moaisdk/core/MoaiBrowser" );
	jmethodID openURLWithParams = env->GetStaticMethodID ( browser, "openURLWithParams", "(Ljava/lang/String;Landroid/os/Bundle;)V" );

	MOAIJString jurl = self->GetJString ( url );
	self->CallStaticVoidMethod ( openURLWithParams, ( jstring )jurl, params );

	return 0;
}

//================================================================//
// MOAIBrowserAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIBrowserAndroid::MOAIBrowserAndroid () {
	RTTI_SINGLE ( MOAILuaObject )
	this->SetClass ( "com/moaisdk/core/MoaiBrowser" );
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

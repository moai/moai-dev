// Copyright (c) 2010-2013 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android/MOAIBrowserAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

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

	jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		ZLLog::Print ( "MOAIBrowserAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID openURL = env->GetStaticMethodID ( moai, "openURL", "(Ljava/lang/String;)V" );
    	if ( openURL == NULL ) {

			ZLLog::Print ( "MOAIBrowserAndroid: Unable to find static java method %s", "openURL" );
    	} else {

			env->CallStaticVoidMethod ( moai, openURL, jurl );
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
		{ "openURL",				_openURL },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

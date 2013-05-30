// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_PLAYSERVICES

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIGooglePlayServicesAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	connect
	@text	Connects to the Google Play Game Services

	@out	nil
*/
int MOAIGooglePlayServicesAndroid::_connect ( lua_State* L ) {

	JNI_GET_ENV ( jvm, env );
	MOAILuaState state ( L );

	jclass playserv = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePlayServices" );
    if ( playserv == NULL ) {

		USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePlayServices" );
    } else {

    	jmethodID connect = env->GetStaticMethodID ( playserv, "connect", "()Z" );
   		if ( connect == NULL ) {

			USLog::Print ( "MOAIGooglePlayServicesAndroid: Unable to find static java method %s", "connect" );
		} else {

			bool success = ( bool ) env->CallStaticBooleanMethod ( playserv, connect );

			lua_pushboolean ( state, success );
			return 1;
		}
	}

	lua_pushboolean ( state, false );
	return 1;
}


//================================================================//
// MOAIGooglePlayServicesAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIGooglePlayServicesAndroid::MOAIGooglePlayServicesAndroid () {

	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIGooglePlayServicesAndroid::~MOAIGooglePlayServicesAndroid () {

}

//----------------------------------------------------------------//
void MOAIGooglePlayServicesAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "connect", 				_connect },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

#endif
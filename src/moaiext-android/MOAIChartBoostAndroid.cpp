// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_CHARTBOOST

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIChartBoostAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIChartBoostAndroid::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );
	cc8* signature = lua_tostring ( state, 2 );

	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( identifier, jidentifier );
	JNI_GET_JSTRING ( signature, jsignature );

	jclass chartboost = env->FindClass ( "com/ziplinegames/moai/MoaiChartBoost" );
    if ( chartboost == NULL ) {

		USLog::Print ( "MOAIChartBoostAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiChartBoost" );
    } else {

    	jmethodID init = env->GetStaticMethodID ( chartboost, "init", "(Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( init == NULL ) {

			USLog::Print ( "MOAIChartBoostAndroid: Unable to find static java method %s", "init" );
    	} else {

			env->CallStaticVoidMethod ( chartboost, init, jidentifier, jsignature );				
		}
	}
			
	return 0;
}

//----------------------------------------------------------------//
int MOAIChartBoostAndroid::_install ( lua_State* L ) {
	
	MOAILuaState state ( L );

	JNI_GET_ENV ( jvm, env );

	jclass chartboost = env->FindClass ( "com/ziplinegames/moai/MoaiChartBoost" );
    if ( chartboost == NULL ) {

		USLog::Print ( "MOAIChartBoostAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiChartBoost" );
    } else {

    	jmethodID install = env->GetStaticMethodID ( chartboost, "install", "()V" );
    	if ( install == NULL ) {

			USLog::Print ( "MOAIChartBoostAndroid: Unable to find static java method %s", "install" );
    	} else {

			env->CallStaticVoidMethod ( chartboost, install );				
		}
	}
			
	return 0;
}

//----------------------------------------------------------------//
int MOAIChartBoostAndroid::_showInterstitial ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* location = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( location, jlocation );

	jclass chartboost = env->FindClass ( "com/ziplinegames/moai/MoaiChartBoost" );
    if ( chartboost == NULL ) {

		USLog::Print ( "MOAIChartBoostAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiChartBoost" );
    } else {

    	jmethodID showInterstitial = env->GetStaticMethodID ( chartboost, "showInterstitial", "(Ljava/lang/String;)V" );
    	if ( showInterstitial == NULL ) {

			USLog::Print ( "MOAIChartBoostAndroid: Unable to find static java method %s", "showInterstitial" );
    	} else {

			env->CallStaticVoidMethod ( chartboost, showInterstitial, jlocation );				
		}
	}
			
	return 0;
}

//================================================================//
// MOAIChartBoostAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIChartBoostAndroid::MOAIChartBoostAndroid () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAIChartBoostAndroid::~MOAIChartBoostAndroid () {

}

//----------------------------------------------------------------//
void MOAIChartBoostAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "init",				_init },
		{ "install",			_install },
		{ "showInterstitial",	_showInterstitial },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

#endif
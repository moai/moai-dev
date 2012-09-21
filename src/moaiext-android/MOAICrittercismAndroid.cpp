// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_CRITTERCISM

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAICrittercismAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	init
	@text	Initialize Crittercism.
	
	@in		string appId			Available in Crittercism dashboard settings.
	@out	nil
*/
int MOAICrittercismAndroid::_init ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( identifier, jidentifier );

	jclass crittercism = env->FindClass ( "com/ziplinegames/moai/MoaiCrittercism" );
    if ( crittercism == NULL ) {

		USLog::Print ( "MOAICrittercismAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiCrittercism" );
    } else {

    	jmethodID init = env->GetStaticMethodID ( crittercism, "init", "(Ljava/lang/String;)V" );
    	if ( init == NULL ) {

			USLog::Print ( "MOAICrittercismAndroid: Unable to find static java method %s", "init" );
    	} else {

			env->CallStaticVoidMethod ( crittercism, init, jidentifier );				
		}
	}
			
	return 0;
}

//----------------------------------------------------------------//
/**	@name	leaveBreadcrumb
	@text	Leave a breadcrumb (log statement) to trace execution.
	
	@in		string breadcrumb		A string describing the code location.
	@out	nil
*/
int MOAICrittercismAndroid::_leaveBreadcrumb ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* breadcrumb = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( breadcrumb, jbreadcrumb );

	jclass crittercism = env->FindClass ( "com/ziplinegames/moai/MoaiCrittercism" );
    if ( crittercism == NULL ) {

		USLog::Print ( "MOAICrittercismAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiCrittercism" );
    } else {

    	jmethodID leaveBreadcrumb = env->GetStaticMethodID ( crittercism, "leaveBreadcrumb", "(Ljava/lang/String;)V" );
    	if ( leaveBreadcrumb == NULL ) {

			USLog::Print ( "MOAICrittercismAndroid: Unable to find static java method %s", "leaveBreadcrumb" );
    	} else {

			env->CallStaticVoidMethod ( crittercism, leaveBreadcrumb, jbreadcrumb );				
		}
	}
			
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setUser
	@text	Sets an identifier for a user
	
	@in		string identifier		A string identifying the user.
	@out	nil
*/
int MOAICrittercismAndroid::_setUser ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* ident = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( ident, jident );

	jclass crittercism = env->FindClass ( "com/ziplinegames/moai/MoaiCrittercism" );
    if ( crittercism == NULL ) {

		USLog::Print ( "MOAICrittercismAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiCrittercism" );
    } else {

    	jmethodID setUser = env->GetStaticMethodID ( crittercism, "setUser", "(Ljava/lang/String;)V" );
    	if ( setUser == NULL ) {

			USLog::Print ( "MOAICrittercismAndroid: Unable to find static java method %s", "setUser" );
    	} else {

			env->CallStaticVoidMethod ( crittercism, setUser, jident );				
		}
	}
			
	return 0;
}

//================================================================//
// MOAICrittercismAndroid
//================================================================//

//----------------------------------------------------------------//
MOAICrittercismAndroid::MOAICrittercismAndroid () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAICrittercismAndroid::~MOAICrittercismAndroid () {

}

//----------------------------------------------------------------//
void MOAICrittercismAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "init",				_init },
		{ "leaveBreadcrumb",	_leaveBreadcrumb },
		{ "setUser",			_setUser },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

#endif
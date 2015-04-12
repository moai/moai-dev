// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include <moai-android-crittercism/MOAICrittercismAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@lua	forceException
	@text	Force and exception to send breadcrumbs to crittercism

	@out	nil
*/
int MOAICrittercismAndroid::_forceException ( lua_State* L ) {

	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );

	jclass crittercism = env->FindClass ( "com/ziplinegames/moai/MoaiCrittercism" );
    if ( crittercism == NULL ) {

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAICrittercismAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiCrittercism" );
    } else {

    	jmethodID forceException = env->GetStaticMethodID ( crittercism, "forceException", "()V" );
    	if ( forceException == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAICrittercismAndroid: Unable to find static java method %s", "forceException" );
    	} else {

			env->CallStaticVoidMethod ( crittercism, forceException );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	init
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

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAICrittercismAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiCrittercism" );
    } else {

    	jmethodID init = env->GetStaticMethodID ( crittercism, "init", "(Ljava/lang/String;)V" );
    	if ( init == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAICrittercismAndroid: Unable to find static java method %s", "init" );
    	} else {

			env->CallStaticVoidMethod ( crittercism, init, jidentifier );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	leaveBreadcrumb
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

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAICrittercismAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiCrittercism" );
    } else {

    	jmethodID leaveBreadcrumb = env->GetStaticMethodID ( crittercism, "leaveBreadcrumb", "(Ljava/lang/String;)V" );
    	if ( leaveBreadcrumb == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAICrittercismAndroid: Unable to find static java method %s", "leaveBreadcrumb" );
    	} else {

			env->CallStaticVoidMethod ( crittercism, leaveBreadcrumb, jbreadcrumb );
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@lua	setUser
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

		ZLLog::LogF ( ZLLog::CONSOLE, "MOAICrittercismAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiCrittercism" );
    } else {

    	jmethodID setUser = env->GetStaticMethodID ( crittercism, "setUser", "(Ljava/lang/String;)V" );
    	if ( setUser == NULL ) {

			ZLLog::LogF ( ZLLog::CONSOLE, "MOAICrittercismAndroid: Unable to find static java method %s", "setUser" );
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
		{ "forceException", 	_forceException },
		{ "init",				_init },
		{ "leaveBreadcrumb",	_leaveBreadcrumb },
		{ "setUser",			_setUser },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}


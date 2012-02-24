// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_CRITTERCISM

#include "pch.h"

#include <jni.h>

#include <android/log.h>

#include <moaiext-android/MOAICrittercism.h>

extern JavaVM* jvm;
extern jobject mMoaiActivity;

jmethodID mInitFunc;

//================================================================//
// Utility macros
//================================================================//

	#define GET_ENV() 	\
		JNIEnv* env; 	\
		jvm->GetEnv (( void** )&env, JNI_VERSION_1_4 );
		
	#define GET_CSTRING(jstr, cstr) \
		const char* cstr = ( jstr != NULL ) ? env->GetStringUTFChars ( jstr, NULL ) : NULL;

	#define RELEASE_CSTRING(jstr, cstr) \
		if ( cstr != NULL ) env->ReleaseStringUTFChars ( jstr, cstr );
		
	#define GET_JSTRING(cstr, jstr) \
		jstring jstr = ( cstr != NULL ) ? env->NewStringUTF (( const char* )cstr ) : NULL;
		
	#define PRINT(str) \
		__android_log_write ( ANDROID_LOG_INFO, "MoaiLog", str );
		
//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAICrittercism::_init ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* identifier = lua_tostring ( state, 1 );

	GET_ENV ();
	GET_JSTRING ( identifier, jidentifier );

	if (mInitFunc == NULL) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mInitFunc = env->GetMethodID ( moaiActivityClass, "initCrittercism", "(Ljava/lang/String;)V" );
	}

	env->CallVoidMethod ( mMoaiActivity, mInitFunc, jidentifier );
		
	return 0;
}

//================================================================//
// MOAICrittercism
//================================================================//

//----------------------------------------------------------------//
MOAICrittercism::MOAICrittercism () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAICrittercism::~MOAICrittercism () {

}

//----------------------------------------------------------------//
void MOAICrittercism::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable[] = {
		{ "init",	_init },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

#endif

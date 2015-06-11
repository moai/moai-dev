// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIJNIUTILS_H
#define MOAIJNIUTILS_H

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>
 #include <moai-android/MOAIJString.h>

//================================================================//
// Utility macros
//================================================================//

#define JNI_GET_ENV(jvm, env) 	\
	JNIEnv* env; 				\
	jvm->GetEnv (( void** )&env, JNI_VERSION_1_4 );
	
#define JNI_GET_CSTRING(jstr, cstr) \
	const char* cstr = ( jstr != NULL ) ? env->GetStringUTFChars ( jstr, NULL ) : NULL;

#define JNI_RELEASE_CSTRING(jstr, cstr) \
	if ( cstr != NULL ) env->ReleaseStringUTFChars ( jstr, cstr );
	
#define JNI_GET_JSTRING(cstr) \
	( cstr != NULL ) ? env->NewStringUTF (( const char* )cstr ) : jstring ( 0 );

#define MOAI_JAVA_LUA_SETUP(type,str)						\
	MOAILuaState state ( L );								\
	if ( !state.CheckParams ( 1, str, true )) return 0;		\
	type* self = &type::Get ();

//================================================================//
// JniUtils
//================================================================//
class JniUtils {
protected:

	jclass		mClass;
	//jobject		mActivity;

public:

	//----------------------------------------------------------------//
	jobject				BundleFromLua				( lua_State* L, int index );
	bool				CallStaticBooleanMethod 	( jmethodID method, ... );
	long				CallStaticLongMethod		( jmethodID method, ... );
	jobject				CallStaticObjectMethod		( jmethodID method, ... );
	void				CallStaticVoidMethod		( jmethodID method, ... );
	void				ClearException				();
	jobject				CreateObjectOfClass			();
	JNIEnv*				Env							();
	jclass				GetClass					( cc8* className );
	jclass				GetClassViaLoader			( cc8* className );
	cc8*				GetCString					( jstring jstr );
	jstring				GetJString					( cc8* cstr );
	jmethodID			GetMethod					( cc8* methodName, cc8* methodSignature );
	jmethodID			GetMethod					( jclass clazz, cc8* methodName, cc8* methodSignature );
	jmethodID			GetStaticMethod				( cc8* methodName, cc8* methodSignature );
	jmethodID			GetStaticMethod				( jclass clazz, cc8* methodName, cc8* methodSignature );
	jobject				HashMapFromLua				( lua_State* L, int index );
						JniUtils					();
						~JniUtils					();
	void				ReleaseCString				( jstring jstr, cc8* cstr );
	bool				SetClass					( cc8* className );
	bool				SetClassViaLoader			( cc8* className );
	jobjectArray		StringArrayFromLua			( lua_State* L, int index );
};

#endif

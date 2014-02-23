// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef MOAIJNIUTILS_H
#define MOAIJNIUTILS_H

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>

#define MOAI_JAVA_LUA_SETUP(type,str)						\
	MOAILuaState state ( L );								\
	if ( !state.CheckParams ( 1, str, true )) return 0;		\
	type* self = &type::Get ();
	
//================================================================//
// JniUtils
//================================================================//
class JniUtils {
protected:

	JNIEnv*		mEnv;
	jclass		mClass;

public:

	//----------------------------------------------------------------//
	jobjectArray		ArrayFromLua				( lua_State* L, int index );
	jobject				BundleFromLua				( lua_State* L, int index );
	bool				CallStaticBooleanMethod 	( jmethodID method, ... );
	long				CallStaticLongMethod		( jmethodID method, ... );
	jobject				CallStaticObjectMethod		( jmethodID method, ... );
	void				CallStaticVoidMethod		( jmethodID method, ... );
	jobject				CreateObjectOfClass			();
	cc8*				GetCString					( jstring jstr );
	jstring				GetJString					( cc8* cstr );
	jmethodID			GetMethod					( cc8* methodName, cc8* methodSignature );
	jmethodID			GetStaticMethod				( cc8* methodName, cc8* methodSignature );
						JniUtils					();
						~JniUtils					();
	void				ReleaseCString				( jstring jstr, cc8* cstr );
	void				SetClass					( cc8* className );
};

#endif

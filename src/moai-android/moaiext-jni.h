// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <android/log.h>

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
		
	#define JNI_GET_JSTRING(cstr, jstr) \
		jstring jstr = ( cstr != NULL ) ? env->NewStringUTF (( const char* )cstr ) : NULL;

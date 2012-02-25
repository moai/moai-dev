// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"

#include <jni.h>

#include <android/log.h>

#include <moaiext-android/MOAINotifications.h>

extern JavaVM* jvm;
extern jobject mMoaiActivity;

jmethodID mRegisterRemoteNotificationsFunc;
jmethodID mUnregisterRemoteNotificationsFunc;

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
int MOAINotifications::_registerForRemoteNotifications ( lua_State* L ) {
	MOAILuaState state ( L );

	cc8* alias = lua_tostring ( state, 1 );

	GET_ENV ();
	GET_JSTRING ( alias, jalias );

	if (mRegisterRemoteNotificationsFunc == NULL) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mRegisterRemoteNotificationsFunc = env->GetMethodID ( moaiActivityClass, "registerForRemoteNotifications", "(Ljava/lang/String;)V" );
	}

	env->CallVoidMethod ( mMoaiActivity, mRegisterRemoteNotificationsFunc, jalias );
		
	return 0;
}

//----------------------------------------------------------------//
int MOAINotifications::_setListener ( lua_State* L ) {
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		MOAINotifications::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAINotifications::_unregisterForRemoteNotifications ( lua_State* L ) {
	MOAILuaState state ( L );

	GET_ENV ();

	if (mUnregisterRemoteNotificationsFunc == NULL) {
		
		jclass moaiActivityClass = env->GetObjectClass ( mMoaiActivity );		
		mUnregisterRemoteNotificationsFunc = env->GetMethodID ( moaiActivityClass, "unregisterForRemoteNotifications", "()V" );
	}

	env->CallVoidMethod ( mMoaiActivity, mUnregisterRemoteNotificationsFunc );
		
	return 0;
}

//================================================================//
// MOAINotifications
//================================================================//

//----------------------------------------------------------------//
MOAINotifications::MOAINotifications () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAINotifications::~MOAINotifications () {

}

//----------------------------------------------------------------//
void MOAINotifications::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "REMOTE_NOTIFICATION_REGISTRATION_COMPLETE", 					( u32 )REMOTE_NOTIFICATION_REGISTRATION_COMPLETE );
	state.SetField ( -1, "REMOTE_NOTIFICATION_MESSAGE_RECEIVED", 						( u32 )REMOTE_NOTIFICATION_MESSAGE_RECEIVED );
        
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_REGISTERED", 						( u32 )REMOTE_NOTIFICATION_RESULT_REGISTERED );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_UNREGISTERED", 					( u32 )REMOTE_NOTIFICATION_RESULT_UNREGISTERED );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_ERROR_SERVICE_NOT_AVAILABLE", 		( u32 )REMOTE_NOTIFICATION_RESULT_ERROR_SERVICE_NOT_AVAILABLE );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_ERROR_ACCOUNT_MISSING", 			( u32 )REMOTE_NOTIFICATION_RESULT_ERROR_ACCOUNT_MISSING );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_ERROR_AUTHENTICATION_FAILED", 		( u32 )REMOTE_NOTIFICATION_RESULT_ERROR_AUTHENTICATION_FAILED );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_ERROR_TOO_MANY_REGISTRATIONS", 	( u32 )REMOTE_NOTIFICATION_RESULT_ERROR_TOO_MANY_REGISTRATIONS );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_ERROR_INVALID_SENDER", 			( u32 )REMOTE_NOTIFICATION_RESULT_ERROR_INVALID_SENDER );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_ERROR_PHONE_REGISTRATION_ERROR",	( u32 )REMOTE_NOTIFICATION_RESULT_ERROR_PHONE_REGISTRATION_ERROR );
	
	luaL_Reg regTable[] = {
		{ "registerForRemoteNotifications",		_registerForRemoteNotifications },
		{ "setListener",						_setListener },
		{ "unregisterForRemoteNotifications",	_unregisterForRemoteNotifications },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAINotifications::NotifyRemoteNotificationReceived ( int entries, cc8** keys, cc8** values ) {
	MOAILuaRef& callback = this->mListeners [ REMOTE_NOTIFICATION_MESSAGE_RECEIVED ];
		
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();

		lua_newtable ( state );

		for ( int i = 0; i < entries; i++ ) {
			state.SetField ( -1, keys [ i ], values [ i ]);
		}
		
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAINotifications::NotifyRemoteRegistrationComplete ( int code, cc8* registration ) {
	MOAILuaRef& callback = this->mListeners [ REMOTE_NOTIFICATION_REGISTRATION_COMPLETE ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushinteger ( state, code );
		lua_pushstring ( state, registration );

		state.DebugCall ( 2, 0 );
	}
}
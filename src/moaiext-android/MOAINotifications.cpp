// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_NOTIFICATIONS

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAINotifications.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAINotifications::_registerForRemoteNotifications ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* alias = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( alias, jalias );

	jclass push = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePush" );
    if ( push == NULL ) {

		USLog::Print ( "MOAINotifications: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePush" );
    } else {

    	jmethodID registerForRemoteNotifications = env->GetStaticMethodID ( push, "registerForRemoteNotifications", "(Ljava/lang/String;)V" );
    	if ( registerForRemoteNotifications == NULL ) {

			USLog::Print ( "MOAINotifications: Unable to find static java method %s", "registerForRemoteNotifications" );
    	} else {

			env->CallStaticVoidMethod ( push, registerForRemoteNotifications, jalias );				
		}
	}
			
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
	
	JNI_GET_ENV ( jvm, env );

	jclass push = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePush" );
    if ( push == NULL ) {

		USLog::Print ( "MOAINotifications: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePush" );
    } else {

    	jmethodID unregisterForRemoteNotifications = env->GetStaticMethodID ( push, "unregisterForRemoteNotifications", "()V" );
    	if ( unregisterForRemoteNotifications == NULL ) {

			USLog::Print ( "MOAINotifications: Unable to find static java method %s", "unregisterForRemoteNotifications" );
    	} else {

			env->CallStaticVoidMethod ( push, unregisterForRemoteNotifications );				
		}
	}

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
	
	luaL_Reg regTable [] = {
		{ "registerForRemoteNotifications",		_registerForRemoteNotifications },
		{ "setListener",						_setListener },
		{ "unregisterForRemoteNotifications",	_unregisterForRemoteNotifications },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
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

//================================================================//
// Notification JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGooglePushReceiver_AKUNotifyGooglePushRemoteNotificationRegistrationComplete ( JNIEnv* env, jclass obj, jint code, jstring jregistration ) {

	JNI_GET_CSTRING ( jregistration, registration );
	
	MOAINotifications::Get ().NotifyRemoteRegistrationComplete ( code , registration );
	
	JNI_RELEASE_CSTRING ( jregistration, registration );	
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGooglePushReceiver_AKUNotifyGooglePushRemoteNotificationReceived ( JNIEnv* env, jclass obj, jobjectArray jkeys, jobjectArray jvalues ) {

	if ( env->GetArrayLength ( jkeys ) != env->GetArrayLength ( jvalues )) return;

	int entries = env->GetArrayLength ( jkeys );
	cc8** keys = ( cc8** )malloc ( entries * sizeof ( cc8* ));
	cc8** values = ( cc8** )malloc ( entries * sizeof ( cc8* ));
		
	for ( int i = 0; i < entries; i++ ) {
		
		jstring jkey = ( jstring ) env->GetObjectArrayElement ( jkeys, i );
		jstring jvalue = ( jstring ) env->GetObjectArrayElement ( jvalues, i );
			
		JNI_GET_CSTRING ( jkey, key );
		JNI_GET_CSTRING ( jvalue, value );
			
		keys [ i ] = key;
		values [ i ] = value;
	}
		
	MOAINotifications::Get ().NotifyRemoteNotificationReceived ( entries, keys, values );

	for ( int i = 0; i < entries; i++ ) {
		
		jstring jkey = ( jstring ) env->GetObjectArrayElement ( jkeys, i );
		jstring jvalue = ( jstring ) env->GetObjectArrayElement ( jvalues, i );
			
		JNI_RELEASE_CSTRING ( jkey, keys [ i ]);
		JNI_RELEASE_CSTRING ( jvalue, values [ i ]);

		keys [ i ] = NULL;
		values [ i ] = NULL;
	}
		
	free ( keys );
	free ( values );
}

#endif
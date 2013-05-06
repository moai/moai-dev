// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_NOTIFICATIONS

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAINotificationsAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
cc8* MOAINotificationsAndroid::_luaParseTable ( lua_State* L, int idx ) {

	switch ( lua_type ( L, idx )) {

		case LUA_TSTRING: {

			cc8* str = lua_tostring ( L, idx );
			return str;
		}
	}

	return NULL;
}

//----------------------------------------------------------------//
/**	@name	getAppIconBadgeNumber
	@text	Get the current icon badge number. Always returns zero.
				
	@out 	integer	count
*/
int MOAINotificationsAndroid::_getAppIconBadgeNumber ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	lua_pushnumber ( state, 0 );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	localNotificationInSeconds
	@text	Schedules a local notification to show a number of seconds after calling.
	
	@in		string message
	@in		integer seconds
				
	@out 	nil
*/
int MOAINotificationsAndroid::_localNotificationInSeconds ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	int seconds = lua_tointeger ( state, 1 );
	cc8* message = lua_tostring ( state, 2 );
	
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( message, jmessage );
	
	jobjectArray jvalues = NULL;
	jobjectArray jkeys = NULL;
	
	if ( state.IsType ( 3, LUA_TTABLE )) {
	
		// table is in the stack at index 'idx'
		lua_pushnil ( state );  // first key
		int numEntries = 0;
		while ( lua_next ( state, 3 ) != 0 ) {
			
			// use the 'key' (at index -2) and 'value' (at index -1)
			cc8* key = _luaParseTable ( state, -2 );
			if ( key ) {
				
				cc8* value = _luaParseTable ( state, -1 );
				if ( value ) {
					++numEntries;
				}
			}			
			
			// removes 'value'; keeps 'key' for next iteration
			lua_pop ( state, 1 );
		}
				
		jvalues = env->NewObjectArray ( numEntries, env->FindClass( "java/lang/String" ), 0 );
		jkeys = env->NewObjectArray ( numEntries, env->FindClass( "java/lang/String" ), 0 );
		
		// table is in the stack at index 'idx'
		lua_pushnil ( state );  // first key
		int curidx = 0;
		while ( lua_next ( state, 3 ) != 0 ) {
			
			// use the 'key' (at index -2) and 'value' (at index -1)
			cc8* key = _luaParseTable ( state, -2 );
			if ( key ) {
				
				cc8* value = _luaParseTable ( state, -1 );
				if ( value ) {

					JNI_GET_JSTRING ( value, jvalue );
					env->SetObjectArrayElement ( jvalues, curidx, jvalue );
					
					JNI_GET_JSTRING ( key, jkey );
					env->SetObjectArrayElement ( jkeys, curidx, jkey );
					
					++curidx;
				}
			}			
			
			// removes 'value'; keeps 'key' for next iteration
			lua_pop ( state, 1 );
		}	
	}
	
	if ( jvalues == NULL ) {
		
		jvalues = env->NewObjectArray ( 0, env->FindClass( "java/lang/String" ), 0 );
	}
	
	if ( jkeys == NULL ) {
		
		jkeys = env->NewObjectArray ( 0, env->FindClass( "java/lang/String" ), 0 );
	}

	jclass moai = env->FindClass ( "com/ziplinegames/moai/Moai" );
    if ( moai == NULL ) {

		USLog::Print ( "MOAINotificationsAndroid: Unable to find java class %s", "com/ziplinegames/moai/Moai" );
    } else {

    	jmethodID localNotificationInSeconds = env->GetStaticMethodID ( moai, "localNotificationInSeconds", "(ILjava/lang/String;[Ljava/lang/String;[Ljava/lang/String;)V" );
    	if ( localNotificationInSeconds == NULL ) {

			USLog::Print ( "MOAINotificationsAndroid: Unable to find static java method %s", "localNotificationInSeconds" );
    	} else {

			env->CallStaticVoidMethod ( moai, localNotificationInSeconds, seconds, jmessage, jkeys, jvalues );				
		}
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	registerForRemoteNotifications
	@text	Register to receive remote notifications.
			
	@in		string	sender			The identity of the entity that will send remote notifications. See Google documentation.
	@out 	nil
*/
int MOAINotificationsAndroid::_registerForRemoteNotifications ( lua_State* L ) {
	
	MOAILuaState state ( L );

	cc8* alias = lua_tostring ( state, 1 );

	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( alias, jalias );

	jclass push = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePush" );
    if ( push == NULL ) {

		USLog::Print ( "MOAINotificationsAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePush" );
    } else {

    	jmethodID registerForRemoteNotifications = env->GetStaticMethodID ( push, "registerForRemoteNotifications", "(Ljava/lang/String;)V" );
    	if ( registerForRemoteNotifications == NULL ) {

			USLog::Print ( "MOAINotificationsAndroid: Unable to find static java method %s", "registerForRemoteNotifications" );
    	} else {

			env->CallStaticVoidMethod ( push, registerForRemoteNotifications, jalias );				
		}
	}
			
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setAppIconBadgeNumber
	@text	Get the current icon badge number. Does nothing.
				
	@out 	nil
*/
int MOAINotificationsAndroid::_setAppIconBadgeNumber ( lua_State* L ) {
	
	return 0;
}

//----------------------------------------------------------------//
int MOAINotificationsAndroid::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		
		MOAINotificationsAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	unregisterForRemoteNotifications
	@text	Dregister for remote notifications.
			
	@out 	nil
*/
int MOAINotificationsAndroid::_unregisterForRemoteNotifications ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );

	jclass push = env->FindClass ( "com/ziplinegames/moai/MoaiGooglePush" );
    if ( push == NULL ) {

		USLog::Print ( "MOAINotificationsAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGooglePush" );
    } else {

    	jmethodID unregisterForRemoteNotifications = env->GetStaticMethodID ( push, "unregisterForRemoteNotifications", "()V" );
    	if ( unregisterForRemoteNotifications == NULL ) {

			USLog::Print ( "MOAINotificationsAndroid: Unable to find static java method %s", "unregisterForRemoteNotifications" );
    	} else {

			env->CallStaticVoidMethod ( push, unregisterForRemoteNotifications );				
		}
	}

	return 0;
}

//================================================================//
// MOAINotificationsAndroid
//================================================================//

//----------------------------------------------------------------//
MOAINotificationsAndroid::MOAINotificationsAndroid () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAINotificationsAndroid::~MOAINotificationsAndroid () {

}

//----------------------------------------------------------------//
void MOAINotificationsAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "REMOTE_NOTIFICATION_NONE",					( u32 )0 );
	state.SetField ( -1, "REMOTE_NOTIFICATION_BADGE",					( u32 )0 );
	state.SetField ( -1, "REMOTE_NOTIFICATION_SOUND",					( u32 )0 );
	state.SetField ( -1, "REMOTE_NOTIFICATION_ALERT",					( u32 )0 );
	
	state.SetField ( -1, "LOCAL_NOTIFICATION_MESSAGE_RECEIVED", 		( u32 )LOCAL_NOTIFICATION_MESSAGE_RECEIVED );
	state.SetField ( -1, "REMOTE_NOTIFICATION_REGISTRATION_COMPLETE",	( u32 )REMOTE_NOTIFICATION_REGISTRATION_COMPLETE );
	state.SetField ( -1, "REMOTE_NOTIFICATION_MESSAGE_RECEIVED", 		( u32 )REMOTE_NOTIFICATION_MESSAGE_RECEIVED );

	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_REGISTERED", 		( u32 )REMOTE_NOTIFICATION_RESULT_REGISTERED );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_UNREGISTERED", 	( u32 )REMOTE_NOTIFICATION_RESULT_UNREGISTERED );
	state.SetField ( -1, "REMOTE_NOTIFICATION_RESULT_ERROR", 			( u32 )REMOTE_NOTIFICATION_RESULT_ERROR );
	
	luaL_Reg regTable [] = {
		{ "getAppIconBadgeNumber",				_getAppIconBadgeNumber },
		{ "localNotificationInSeconds",			_localNotificationInSeconds },
		{ "registerForRemoteNotifications",		_registerForRemoteNotifications },
		{ "setAppIconBadgeNumber",				_setAppIconBadgeNumber },
		{ "setListener",						_setListener },
		{ "unregisterForRemoteNotifications",	_unregisterForRemoteNotifications },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAINotificationsAndroid::NotifyLocalNotificationReceived ( int entries, cc8** keys, cc8** values ) {
	
	MOAILuaRef& callback = this->mListeners [ LOCAL_NOTIFICATION_MESSAGE_RECEIVED ];
		
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
void MOAINotificationsAndroid::NotifyRemoteNotificationReceived ( int entries, cc8** keys, cc8** values ) {
	
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
void MOAINotificationsAndroid::NotifyRemoteRegistrationComplete ( int code, cc8* registration ) {
	
	MOAILuaRef& callback = this->mListeners [ REMOTE_NOTIFICATION_REGISTRATION_COMPLETE ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
	
		if ( code > REMOTE_NOTIFICATION_RESULT_ERROR ) {
			
			code = REMOTE_NOTIFICATION_RESULT_ERROR;
		}
		
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
	
	MOAINotificationsAndroid::Get ().NotifyRemoteRegistrationComplete ( code , registration );
	
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
		
	MOAINotificationsAndroid::Get ().NotifyRemoteNotificationReceived ( entries, keys, values );

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

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiLocalNotificationReceiver_AKUNotifyLocalNotificationReceived ( JNIEnv* env, jclass obj, jobjectArray jkeys, jobjectArray jvalues ) {
	
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
		
	MOAINotificationsAndroid::Get ().NotifyLocalNotificationReceived ( entries, keys, values );

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
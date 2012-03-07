// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_BILLING

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIBilling.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
int MOAIBilling::_checkBillingSupported ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );

	jclass billing = env->FindClass ( "com/ziplinegames/moai/MoaiGoogleBilling" );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBilling: Unable to find java class %s", "com/ziplinegames/moai/MoaiGoogleBilling" );
    } else {

    	jmethodID checkBillingSupported = env->GetStaticMethodID ( billing, "checkBillingSupported", "()Z" );
    	if ( checkBillingSupported == NULL ) {

			USLog::Print ( "MOAIBilling: Unable to find static java method %s", "checkBillingSupported" );
    	} else {

			jboolean jsupported = ( jboolean )env->CallStaticBooleanMethod ( billing, checkBillingSupported );	

			lua_pushboolean ( state, jsupported );

			return 1;
		}
	}

	lua_pushboolean ( state, false );

	return 1;
}

//----------------------------------------------------------------//
int MOAIBilling::_confirmNotification ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* notification = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( notification, jnotification );

	jclass billing = env->FindClass ( "com/ziplinegames/moai/MoaiGoogleBilling" );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBilling: Unable to find java class %s", "com/ziplinegames/moai/MoaiGoogleBilling" );
    } else {

    	jmethodID confirmNotification = env->GetStaticMethodID ( billing, "confirmNotification", "(Ljava/lang/String;)Z" );
    	if ( confirmNotification == NULL ) {

			USLog::Print ( "MOAIBilling: Unable to find static java method %s", "confirmNotification" );
    	} else {

			jboolean jsuccess = ( jboolean )env->CallStaticBooleanMethod ( billing, confirmNotification, jnotification );	

			lua_pushboolean ( state, jsuccess );

			return 1;
		}
	}
	
	lua_pushboolean ( state, false );

	return 1;
}

//----------------------------------------------------------------//
int MOAIBilling::_requestPurchase ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* identifier = lua_tostring ( state, 1 );
	cc8* payload = lua_tostring ( state, 2 );
	
	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( identifier, jidentifier );
	JNI_GET_JSTRING ( payload, jpayload );

	jclass billing = env->FindClass ( "com/ziplinegames/moai/MoaiGoogleBilling" );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBilling: Unable to find java class %s", "com/ziplinegames/moai/MoaiGoogleBilling" );
    } else {

    	jmethodID requestPurchase = env->GetStaticMethodID ( billing, "requestPurchase", "(Ljava/lang/String;Ljava/lang/String;)Z" );
    	if ( requestPurchase == NULL ) {

			USLog::Print ( "MOAIBilling: Unable to find static java method %s", "requestPurchase" );
    	} else {

			jboolean jsuccess = ( jboolean )env->CallStaticBooleanMethod ( billing, requestPurchase, jidentifier, jpayload );	

			lua_pushboolean ( state, jsuccess );

			return 1;
		}
	}

	lua_pushboolean ( state, false );
	
	return 1;
}

//----------------------------------------------------------------//
int MOAIBilling::_restoreTransactions ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );

	jclass billing = env->FindClass ( "com/ziplinegames/moai/MoaiGoogleBilling" );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBilling: Unable to find java class %s", "com/ziplinegames/moai/MoaiGoogleBilling" );
    } else {

    	jmethodID restoreTransactions = env->GetStaticMethodID ( billing, "restoreTransactions", "()Z" );
    	if ( restoreTransactions == NULL ) {

			USLog::Print ( "MOAIBilling: Unable to find static java method %s", "restoreTransactions" );
    	} else {

			jboolean jsuccess = ( jboolean )env->CallStaticBooleanMethod ( billing, restoreTransactions );	

			lua_pushboolean ( state, jsuccess );

			return 1;
		}
	}

	lua_pushboolean ( state, false );
	
	return 1;
}

//----------------------------------------------------------------//
int MOAIBilling::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		
		MOAIBilling::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
int MOAIBilling::_setMarketPublicKey ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* key = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( key, jkey );

	jclass billing = env->FindClass ( "com/ziplinegames/moai/MoaiGoogleBilling" );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBilling: Unable to find java class %s", "com/ziplinegames/moai/MoaiGoogleBilling" );
    } else {

    	jmethodID setMarketPublicKey = env->GetStaticMethodID ( billing, "setMarketPublicKey", "(Ljava/lang/String;)V" );
    	if ( setMarketPublicKey == NULL ) {

			USLog::Print ( "MOAIBilling: Unable to find static java method %s", "setMarketPublicKey" );
    	} else {

			env->CallStaticVoidMethod ( billing, setMarketPublicKey, jkey );
		}
	}
	
	return 0;
}

//================================================================//
// MOAIBilling
//================================================================//

//----------------------------------------------------------------//
MOAIBilling::MOAIBilling () {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAIBilling::~MOAIBilling () {

}

//----------------------------------------------------------------//
void MOAIBilling::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "CHECK_BILLING_SUPPORTED",				( u32 )CHECK_BILLING_SUPPORTED );
	state.SetField ( -1, "PURCHASE_RESPONSE_RECEIVED",			( u32 )PURCHASE_RESPONSE_RECEIVED );
	state.SetField ( -1, "PURCHASE_STATE_CHANGED",				( u32 )PURCHASE_STATE_CHANGED );
	state.SetField ( -1, "RESTORE_RESPONSE_RECEIVED",			( u32 )RESTORE_RESPONSE_RECEIVED );
	
	state.SetField ( -1, "BILLING_RESULT_OK",					( u32 )BILLING_RESULT_OK );
	state.SetField ( -1, "BILLING_RESULT_USER_CANCELED",		( u32 )BILLING_RESULT_USER_CANCELED );
	state.SetField ( -1, "BILLING_RESULT_SERVICE_UNAVAILABLE",	( u32 )BILLING_RESULT_SERVICE_UNAVAILABLE );
	state.SetField ( -1, "BILLING_RESULT_BILLING_UNAVAILABLE",	( u32 )BILLING_RESULT_BILLING_UNAVAILABLE );
	state.SetField ( -1, "BILLING_RESULT_ITEM_UNAVAILABLE",		( u32 )BILLING_RESULT_ITEM_UNAVAILABLE );
	state.SetField ( -1, "BILLING_RESULT_DEVELOPER_ERROR",		( u32 )BILLING_RESULT_DEVELOPER_ERROR );
	state.SetField ( -1, "BILLING_RESULT_ERROR",				( u32 )BILLING_RESULT_ERROR );

	state.SetField ( -1, "BILLING_STATE_ITEM_PURCHASED",		( u32 )BILLING_STATE_ITEM_PURCHASED );
	state.SetField ( -1, "BILLING_STATE_PURCHASE_CANCELED",		( u32 )BILLING_STATE_PURCHASE_CANCELED );
	state.SetField ( -1, "BILLING_STATE_ITEM_REFUNDED",			( u32 )BILLING_STATE_ITEM_REFUNDED );

	luaL_Reg regTable [] = {
		{ "checkBillingSupported",	_checkBillingSupported },
		{ "confirmNotification",	_confirmNotification },
		{ "requestPurchase",		_requestPurchase },
		{ "restoreTransactions",	_restoreTransactions },
		{ "setListener",			_setListener },
		{ "setMarketPublicKey",		_setMarketPublicKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAIBilling::NotifyBillingSupported ( bool supported ) {
	
	MOAILuaRef& callback = this->mListeners [ CHECK_BILLING_SUPPORTED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushboolean ( state, supported );
			
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBilling::NotifyPurchaseResponseReceived ( cc8* identifier, int code ) {
	
	MOAILuaRef& callback = this->mListeners [ PURCHASE_RESPONSE_RECEIVED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushstring ( state, identifier );
		lua_pushinteger ( state, code );	
		
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBilling::NotifyPurchaseStateChanged ( cc8* identifier, int code, cc8* order, cc8* notification, cc8* payload ) {
	
	MOAILuaRef& callback = this->mListeners [ PURCHASE_STATE_CHANGED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushstring ( state, identifier );
		lua_pushinteger ( state, code );
		lua_pushstring ( state, order );
		lua_pushstring ( state, notification );
		lua_pushstring ( state, payload );
		
		state.DebugCall ( 5, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBilling::NotifyRestoreResponseReceived ( int code ) {
	
	MOAILuaRef& callback = this->mListeners [ RESTORE_RESPONSE_RECEIVED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();

		lua_pushinteger ( state, code );	
		
		state.DebugCall ( 1, 0 );
	}
}

//================================================================//
// Billing JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGoogleBilling_AKUNotifyGoogleBillingSupported ( JNIEnv* env, jclass obj, jboolean supported ) {

	MOAIBilling::Get ().NotifyBillingSupported ( supported );
}
	
//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGoogleBilling_AKUNotifyGooglePurchaseResponseReceived ( JNIEnv* env, jclass obj, jstring jidentifier, jint code ) {

	JNI_GET_CSTRING ( jidentifier, identifier );

	MOAIBilling::Get ().NotifyPurchaseResponseReceived ( identifier, code );

	JNI_RELEASE_CSTRING ( jidentifier, identifier );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGoogleBilling_AKUNotifyGooglePurchaseStateChanged ( JNIEnv* env, jclass obj, jstring jidentifier, jint code, jstring jorder, jstring jnotification, jstring jpayload ) {

	JNI_GET_CSTRING ( jidentifier, identifier );
	JNI_GET_CSTRING ( jorder, order );
	JNI_GET_CSTRING ( jnotification, notification );
	JNI_GET_CSTRING ( jpayload, payload );
		
	MOAIBilling::Get ().NotifyPurchaseStateChanged ( identifier, code, order, notification, payload );

	JNI_RELEASE_CSTRING ( jidentifier, identifier );
	JNI_RELEASE_CSTRING ( jorder, order );
	JNI_RELEASE_CSTRING ( jnotification, notification );
	JNI_RELEASE_CSTRING ( jpayload, payload );
}
	
//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGoogleBilling_AKUNotifyGoogleRestoreResponseReceived ( JNIEnv* env, jclass obj, jint code ) {

	MOAIBilling::Get ().NotifyRestoreResponseReceived ( code );
}

#endif
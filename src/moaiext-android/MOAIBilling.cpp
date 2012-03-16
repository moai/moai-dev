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

	jclass billing = env->FindClass ( MOAIBilling::Get ().mBillingProvider );
    if ( billing == NULL ) {
	
		USLog::Print ( "MOAIBilling: Unable to find java class %s", MOAIBilling::Get ().mBillingProvider );
    } else {
	
    	jmethodID checkBillingSupported = env->GetStaticMethodID ( billing, "checkBillingSupported", "()Z" );
    	if ( checkBillingSupported == NULL ) {
	
			USLog::Print ( "MOAIBilling: Unable to find static java method %s", "checkBillingSupported" );
    	} else {
	
			jboolean jsuccess = ( jboolean )env->CallStaticBooleanMethod ( billing, checkBillingSupported );	

			lua_pushboolean ( state, jsuccess );
	
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

	jclass billing = env->FindClass ( MOAIBilling::Get ().mBillingProvider );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBilling: Unable to find java class %s", MOAIBilling::Get ().mBillingProvider );
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
int MOAIBilling::_getUserId ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );

	jclass billing = env->FindClass ( MOAIBilling::Get ().mBillingProvider );
    if ( billing == NULL ) {
	
		USLog::Print ( "MOAIBilling: Unable to find java class %s", MOAIBilling::Get ().mBillingProvider );
    } else {
	
    	jmethodID getUserId = env->GetStaticMethodID ( billing, "getUserId", "()Z" );
    	if ( getUserId == NULL ) {
	
			USLog::Print ( "MOAIBilling: Unable to find static java method %s", "getUserId" );
    	} else {
	
			jboolean jsuccess = ( jboolean )env->CallStaticBooleanMethod ( billing, getUserId );	

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

	jclass billing = env->FindClass ( MOAIBilling::Get ().mBillingProvider );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBilling: Unable to find java class %s", MOAIBilling::Get ().mBillingProvider );
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

	jclass billing = env->FindClass ( MOAIBilling::Get ().mBillingProvider );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBilling: Unable to find java class %s", MOAIBilling::Get ().mBillingProvider );
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
int MOAIBilling::_setBillingProvider ( lua_State* L ) {
	
	MOAILuaState state ( L );

	u32 provider = state.GetValue < u32 >( 1, BILLING_PROVIDER_GOOGLE );
	
	if ( provider == BILLING_PROVIDER_GOOGLE ) {

		MOAIBilling::Get ().mBillingProvider = "com/ziplinegames/moai/MoaiGoogleBilling";
		
		USLog::Print ( "MOAIBilling: Setting in-app billing provider to %s", MOAIBilling::Get ().mBillingProvider );		
	} else if ( provider == BILLING_PROVIDER_AMAZON ) {
		
		MOAIBilling::Get ().mBillingProvider = "com/ziplinegames/moai/MoaiAmazonBilling";

		USLog::Print ( "MOAIBilling: Setting in-app billing provider to %s", MOAIBilling::Get ().mBillingProvider );
	} else {
				
		USLog::Print ( "MOAIBilling: Unknown billing provider, using %s", MOAIBilling::Get ().mBillingProvider );

		lua_pushboolean ( state, false );

		return 1;
	}
	
	lua_pushboolean ( state, true );
	
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
int MOAIBilling::_setPublicKey ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* key = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( key, jkey );

	jclass billing = env->FindClass ( MOAIBilling::Get ().mBillingProvider );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBilling: Unable to find java class %s", MOAIBilling::Get ().mBillingProvider );
    } else {

    	jmethodID setPublicKey = env->GetStaticMethodID ( billing, "setPublicKey", "(Ljava/lang/String;)V" );
    	if ( setPublicKey == NULL ) {

			USLog::Print ( "MOAIBilling: Unable to find static java method %s", "setPublicKey" );
    	} else {

			env->CallStaticVoidMethod ( billing, setPublicKey, jkey );
		}
	}
	
	return 0;
}

//================================================================//
// MOAIBilling
//================================================================//

//----------------------------------------------------------------//
MOAIBilling::MOAIBilling () :
	mBillingProvider ( "com/ziplinegames/moai/MoaiGoogleBilling" ) {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAIBilling::~MOAIBilling () {

}

//----------------------------------------------------------------//
void MOAIBilling::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "CHECK_BILLING_SUPPORTED",						( u32 )CHECK_BILLING_SUPPORTED );
	state.SetField ( -1, "PURCHASE_RESPONSE_RECEIVED",					( u32 )PURCHASE_RESPONSE_RECEIVED );
	state.SetField ( -1, "PURCHASE_STATE_CHANGED",						( u32 )PURCHASE_STATE_CHANGED );
	state.SetField ( -1, "RESTORE_RESPONSE_RECEIVED",					( u32 )RESTORE_RESPONSE_RECEIVED );
	state.SetField ( -1, "USER_ID_DETERMINED",							( u32 )USER_ID_DETERMINED );

	state.SetField ( -1, "BILLING_PROVIDER_GOOGLE",						( u32 )BILLING_PROVIDER_GOOGLE );
	state.SetField ( -1, "BILLING_PROVIDER_AMAZON",						( u32 )BILLING_PROVIDER_AMAZON );
	
	state.SetField ( -1, "BILLING_RESULT_SUCCESS",						( u32 )BILLING_RESULT_SUCCESS );
	state.SetField ( -1, "BILLING_RESULT_USER_CANCELED",				( u32 )BILLING_RESULT_USER_CANCELED );
	state.SetField ( -1, "BILLING_RESULT_BILLING_UNAVAILABLE",			( u32 )BILLING_RESULT_BILLING_UNAVAILABLE );
	state.SetField ( -1, "BILLING_RESULT_ITEM_UNAVAILABLE",				( u32 )BILLING_RESULT_ITEM_UNAVAILABLE );
	state.SetField ( -1, "BILLING_RESULT_ERROR",						( u32 )BILLING_RESULT_ERROR );

	state.SetField ( -1, "BILLING_PURCHASE_STATE_ITEM_PURCHASED",		( u32 )BILLING_PURCHASE_STATE_ITEM_PURCHASED );
	state.SetField ( -1, "BILLING_PURCHASE_STATE_PURCHASE_CANCELED",	( u32 )BILLING_PURCHASE_STATE_PURCHASE_CANCELED );
	state.SetField ( -1, "BILLING_PURCHASE_STATE_ITEM_REFUNDED",		( u32 )BILLING_PURCHASE_STATE_ITEM_REFUNDED );

	luaL_Reg regTable [] = {
		{ "checkBillingSupported",	_checkBillingSupported },
		{ "confirmNotification",	_confirmNotification },
		{ "getUserId",				_getUserId },
		{ "requestPurchase",		_requestPurchase },
		{ "restoreTransactions",	_restoreTransactions },
		{ "setBillingProvider",		_setBillingProvider },
		{ "setListener",			_setListener },
		{ "setPublicKey",			_setPublicKey },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
int MOAIBilling::MapAmazonPurchaseRequestStatus ( int code ) {
	
	switch ( code ) {
        case AMAZON_USER_ID_REQUEST_STATUS_SUCCESS:
        case AMAZON_PURCHASE_REQUEST_STATUS_ALREADY_ENTITLED:
			return BILLING_RESULT_SUCCESS;
        case AMAZON_PURCHASE_REQUEST_STATUS_INVALID_SKU:
			return BILLING_RESULT_ITEM_UNAVAILABLE;
        case AMAZON_USER_ID_REQUEST_STATUS_FAILED:
		default:
			return BILLING_RESULT_ERROR;
	}
}

//----------------------------------------------------------------//
int MOAIBilling::MapAmazonPurchaseStateCode ( int code ) {

	switch ( code ) {
        case AMAZON_PURCHASE_STATE_ITEM_REFUNDED:
			return BILLING_PURCHASE_STATE_ITEM_REFUNDED;
        case AMAZON_PURCHASE_STATE_ITEM_PURCHASED:
		default:
			return BILLING_PURCHASE_STATE_ITEM_PURCHASED;
	}
}

//----------------------------------------------------------------//
int MOAIBilling::MapAmazonUserIdRequestStatus ( int code ) {
	
	switch ( code ) {
        case AMAZON_USER_ID_REQUEST_STATUS_SUCCESS:
			return BILLING_RESULT_SUCCESS;
        case AMAZON_USER_ID_REQUEST_STATUS_FAILED:
		default:
			return BILLING_RESULT_ERROR;
	}
}

//----------------------------------------------------------------//
int MOAIBilling::MapGooglePurchaseStateCode ( int code ) {

	switch ( code ) {
        case GOOGLE_PURCHASE_STATE_PURCHASE_CANCELED:
			return BILLING_PURCHASE_STATE_PURCHASE_CANCELED;
        case GOOGLE_PURCHASE_STATE_ITEM_REFUNDED:
			return BILLING_PURCHASE_STATE_ITEM_REFUNDED;
        case GOOGLE_PURCHASE_STATE_ITEM_PURCHASED:
		default:
			return BILLING_PURCHASE_STATE_ITEM_PURCHASED;
	}
}

//----------------------------------------------------------------//
int MOAIBilling::MapGoogleResponseCode ( int code ) {
	
	switch ( code ) {
        case GOOGLE_RESPONSE_CODE_OK:
			return BILLING_RESULT_SUCCESS;
        case GOOGLE_RESPONSE_CODE_USER_CANCELED:
			return BILLING_RESULT_USER_CANCELED;
        case GOOGLE_RESPONSE_CODE_SERVICE_UNAVAILABLE:
        case GOOGLE_RESPONSE_CODE_BILLING_UNAVAILABLE:
			return BILLING_RESULT_BILLING_UNAVAILABLE;
        case GOOGLE_RESPONSE_CODE_ITEM_UNAVAILABLE:
			return BILLING_RESULT_ITEM_UNAVAILABLE;
        case GOOGLE_RESPONSE_CODE_DEVELOPER_ERROR:
        case GOOGLE_RESPONSE_CODE_ERROR:
		default:
			return BILLING_RESULT_ERROR;
	}
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
void MOAIBilling::NotifyPurchaseStateChanged ( cc8* identifier, int code, cc8* order, cc8* user, cc8* notification, cc8* payload ) {
	
	MOAILuaRef& callback = this->mListeners [ PURCHASE_STATE_CHANGED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushstring ( state, identifier );
		lua_pushinteger ( state, code );
		lua_pushstring ( state, order );
		lua_pushstring ( state, user );
		lua_pushstring ( state, notification );
		lua_pushstring ( state, payload );
		
		state.DebugCall ( 6, 0 );
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

//----------------------------------------------------------------//
void MOAIBilling::NotifyUserIdDetermined ( int code, cc8* user ) {
	
	MOAILuaRef& callback = this->mListeners [ USER_ID_DETERMINED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushinteger ( state, code );
		lua_pushstring ( state, user );
		
		state.DebugCall ( 2, 0 );
	}
}

//================================================================//
// Billing JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiAmazonBilling_AKUNotifyAmazonBillingSupported ( JNIEnv* env, jclass obj, jboolean supported ) {

	MOAIBilling::Get ().NotifyBillingSupported ( supported );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiAmazonBilling_AKUNotifyAmazonPurchaseResponseReceived ( JNIEnv* env, jclass obj, jstring jidentifier, jint code ) {

	JNI_GET_CSTRING ( jidentifier, identifier );

	MOAIBilling::Get ().NotifyPurchaseResponseReceived ( identifier, MOAIBilling::MapAmazonPurchaseRequestStatus ( code ));

	JNI_RELEASE_CSTRING ( jidentifier, identifier );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiAmazonBilling_AKUNotifyAmazonPurchaseStateChanged ( JNIEnv* env, jclass obj, jstring jidentifier, jint code, jstring jorder, jstring juser, jstring jnotification, jstring jpayload ) {

	JNI_GET_CSTRING ( jidentifier, identifier );
	JNI_GET_CSTRING ( jorder, order );
	JNI_GET_CSTRING ( juser, user );
	JNI_GET_CSTRING ( jnotification, notification );
	JNI_GET_CSTRING ( jpayload, payload );
		
	MOAIBilling::Get ().NotifyPurchaseStateChanged ( identifier, MOAIBilling::MapAmazonPurchaseStateCode ( code ), order, user, notification, payload );

	JNI_RELEASE_CSTRING ( jidentifier, identifier );
	JNI_RELEASE_CSTRING ( jorder, order );
	JNI_RELEASE_CSTRING ( juser, user );
	JNI_RELEASE_CSTRING ( jnotification, notification );
	JNI_RELEASE_CSTRING ( jpayload, payload );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiAmazonBilling_AKUNotifyAmazonUserIdDetermined ( JNIEnv* env, jclass obj, jint code, jstring juser ) {

	JNI_GET_CSTRING ( juser, user );

	MOAIBilling::Get ().NotifyUserIdDetermined ( MOAIBilling::MapAmazonUserIdRequestStatus ( code ), user );

	JNI_RELEASE_CSTRING ( juser, user );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGoogleBilling_AKUNotifyGoogleBillingSupported ( JNIEnv* env, jclass obj, jboolean supported ) {

	MOAIBilling::Get ().NotifyBillingSupported ( supported );
}
	
//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGoogleBilling_AKUNotifyGooglePurchaseResponseReceived ( JNIEnv* env, jclass obj, jstring jidentifier, jint code ) {

	JNI_GET_CSTRING ( jidentifier, identifier );

	MOAIBilling::Get ().NotifyPurchaseResponseReceived ( identifier, MOAIBilling::MapGoogleResponseCode ( code ));

	JNI_RELEASE_CSTRING ( jidentifier, identifier );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGoogleBilling_AKUNotifyGooglePurchaseStateChanged ( JNIEnv* env, jclass obj, jstring jidentifier, jint code, jstring jorder, jstring juser, jstring jnotification, jstring jpayload ) {

	JNI_GET_CSTRING ( jidentifier, identifier );
	JNI_GET_CSTRING ( jorder, order );
	JNI_GET_CSTRING ( juser, user );
	JNI_GET_CSTRING ( jnotification, notification );
	JNI_GET_CSTRING ( jpayload, payload );
		
	MOAIBilling::Get ().NotifyPurchaseStateChanged ( identifier, MOAIBilling::MapGooglePurchaseStateCode ( code ), order, user, notification, payload );

	JNI_RELEASE_CSTRING ( jidentifier, identifier );
	JNI_RELEASE_CSTRING ( jorder, order );
	JNI_RELEASE_CSTRING ( juser, user );
	JNI_RELEASE_CSTRING ( jnotification, notification );
	JNI_RELEASE_CSTRING ( jpayload, payload );
}
	
//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGoogleBilling_AKUNotifyGoogleRestoreResponseReceived ( JNIEnv* env, jclass obj, jint code ) {

	MOAIBilling::Get ().NotifyRestoreResponseReceived ( MOAIBilling::MapGoogleResponseCode ( code ) );
}

#endif
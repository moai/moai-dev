// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef DISABLE_BILLING

#include "pch.h"

#include <jni.h>

#include <moaiext-android/moaiext-jni.h>
#include <moaiext-android/MOAIBillingAndroid.h>

extern JavaVM* jvm;

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
cc8* MOAIBillingAndroid::_luaParseTable ( lua_State* L, int idx ) {

	switch ( lua_type ( L, idx )) {

		case LUA_TSTRING: {

			cc8* str = lua_tostring ( L, idx );
			return str;
		}
	}

	return NULL;
}

//----------------------------------------------------------------//
/**	@name	checkBillingSupported
	@text	Check to see if the currently selected billing provider is available.
				
	@out 	boolean	success			True, if the request was successfully initiated.
*/
int MOAIBillingAndroid::_checkBillingSupported ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );

	jclass billing = env->FindClass ( MOAIBillingAndroid::Get ().mBillingProvider );
    if ( billing == NULL ) {
	
		USLog::Print ( "MOAIBillingAndroid: Unable to find java class %s", MOAIBillingAndroid::Get ().mBillingProvider );
    } else {
	
    	jmethodID checkBillingSupported = env->GetStaticMethodID ( billing, "checkBillingSupported", "()Z" );
    	if ( checkBillingSupported == NULL ) {
	
			USLog::Print ( "MOAIBillingAndroid: Unable to find static java method %s", "checkBillingSupported" );
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
/**	@name	confirmNotification
	@text	Confirm a previously received notification. Only applies
			to the Google Play billing provider.
	
	@in		string	notification	The notification ID to confirm.
	@out 	boolean	success			True, if the request was successfully initiated.
*/
int MOAIBillingAndroid::_confirmNotification ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* notification = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( notification, jnotification );

	jclass billing = env->FindClass ( MOAIBillingAndroid::Get ().mBillingProvider );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBillingAndroid: Unable to find java class %s", MOAIBillingAndroid::Get ().mBillingProvider );
    } else {

    	jmethodID confirmNotification = env->GetStaticMethodID ( billing, "confirmNotification", "(Ljava/lang/String;)Z" );
    	if ( confirmNotification == NULL ) {

			USLog::Print ( "MOAIBillingAndroid: Unable to find static java method %s", "confirmNotification" );
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
/**	@name	getUserId
	@text	Get the ID of the current user for the currently selected 
			billing provider. Only applies to the Amazon billing 
			provider.
	
	@out 	boolean	success			True, if the request was successfully initiated.
*/
int MOAIBillingAndroid::_getUserId ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );

	jclass billing = env->FindClass ( MOAIBillingAndroid::Get ().mBillingProvider );
    if ( billing == NULL ) {
	
		USLog::Print ( "MOAIBillingAndroid: Unable to find java class %s", MOAIBillingAndroid::Get ().mBillingProvider );
    } else {
	
    	jmethodID getUserId = env->GetStaticMethodID ( billing, "getUserId", "()Z" );
    	if ( getUserId == NULL ) {
	
			USLog::Print ( "MOAIBillingAndroid: Unable to find static java method %s", "getUserId" );
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
/**	@name	requestPurchase
	@text	Request the purchase of an item.
	
	@in		string	sku				The SKU to purchase.
	@opt	string	payload			The request payload to be returned upon request completion. Default is nil.
	@out 	boolean	success			True, if the request was successfully initiated.
*/
int MOAIBillingAndroid::_requestPurchase ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* identifier = lua_tostring ( state, 1 );
	cc8* payload = lua_tostring ( state, 2 );
	
	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( identifier, jidentifier );
	JNI_GET_JSTRING ( payload, jpayload );

	jclass billing = env->FindClass ( MOAIBillingAndroid::Get ().mBillingProvider );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBillingAndroid: Unable to find java class %s", MOAIBillingAndroid::Get ().mBillingProvider );
    } else {

    	jmethodID requestPurchase = env->GetStaticMethodID ( billing, "requestPurchase", "(Ljava/lang/String;Ljava/lang/String;)Z" );
    	if ( requestPurchase == NULL ) {

			USLog::Print ( "MOAIBillingAndroid: Unable to find static java method %s", "requestPurchase" );
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
/**	@name	restoreTransactions
	@text	Request the restoration of any previously purchased items.
	
	@opt	string	offset			The offset in the paginated results to start from. Only applies to the Amazon billing provider. Default is nil.
	@out 	boolean	success			True, if the request was successfully initiated.
*/
int MOAIBillingAndroid::_restoreTransactions ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* offset = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );
	
	JNI_GET_JSTRING ( offset, joffset );

	jclass billing = env->FindClass ( MOAIBillingAndroid::Get ().mBillingProvider );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBillingAndroid: Unable to find java class %s", MOAIBillingAndroid::Get ().mBillingProvider );
    } else {

    	jmethodID restoreTransactions = env->GetStaticMethodID ( billing, "restoreTransactions", "(Ljava/lang/String;)Z" );
    	if ( restoreTransactions == NULL ) {

			USLog::Print ( "MOAIBillingAndroid: Unable to find static java method %s", "restoreTransactions" );
    	} else {

			jboolean jsuccess = ( jboolean )env->CallStaticBooleanMethod ( billing, restoreTransactions, joffset );	

			lua_pushboolean ( state, jsuccess );

			return 1;
		}
	}

	lua_pushboolean ( state, false );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	setBillingProvider
	@text	Set the billing provider to use for in-app purchases.
	
	@in		int		provider		The billing provider.
	@out 	boolean	success			True, if the provider was successfully set.
*/
int MOAIBillingAndroid::_setBillingProvider ( lua_State* L ) {
	
	MOAILuaState state ( L );

	u32 provider = state.GetValue < u32 >( 1, BILLING_PROVIDER_GOOGLE );
	
	if ( provider == BILLING_PROVIDER_GOOGLE ) {

		MOAIBillingAndroid::Get ().mBillingProvider = "com/ziplinegames/moai/MoaiGoogleBilling";
		
		USLog::Print ( "MOAIBillingAndroid: Setting in-app billing provider to %s", MOAIBillingAndroid::Get ().mBillingProvider );		
	} else if ( provider == BILLING_PROVIDER_AMAZON ) {
		
		MOAIBillingAndroid::Get ().mBillingProvider = "com/ziplinegames/moai/MoaiAmazonBilling";

		USLog::Print ( "MOAIBillingAndroid: Setting in-app billing provider to %s", MOAIBillingAndroid::Get ().mBillingProvider );
	} else if ( provider == BILLING_PROVIDER_TSTORE ) {
		
		MOAIBillingAndroid::Get ().mBillingProvider = "com/ziplinegames/moai/MoaiTstoreBilling";

		USLog::Print ( "MOAIBillingAndroid: Setting in-app billing provider to %s", MOAIBillingAndroid::Get ().mBillingProvider );
	} else {
				
		USLog::Print ( "MOAIBillingAndroid: Unknown billing provider, using %s", MOAIBillingAndroid::Get ().mBillingProvider );

		lua_pushboolean ( state, false );

		return 1;
	}
	
	lua_pushboolean ( state, true );
	
	return 1;
}

//----------------------------------------------------------------//
int MOAIBillingAndroid::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );

	if ( idx < TOTAL ) {
		
		MOAIBillingAndroid::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	setPublicKey
	@text	Set the public key to be used for receipt verification.
			Only applies to the Google Play billing provider. 
	
	@in		string	key				The public key.		
	@out 	nil
*/
int MOAIBillingAndroid::_setPublicKey ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* key = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( key, jkey );

	jclass billing = env->FindClass ( MOAIBillingAndroid::Get ().mBillingProvider );
    if ( billing == NULL ) {

		USLog::Print ( "MOAIBillingAndroid: Unable to find java class %s", MOAIBillingAndroid::Get ().mBillingProvider );
    } else {

    	jmethodID setPublicKey = env->GetStaticMethodID ( billing, "setPublicKey", "(Ljava/lang/String;)V" );
    	if ( setPublicKey == NULL ) {

			USLog::Print ( "MOAIBillingAndroid: Unable to find static java method %s", "setPublicKey" );
    	} else {

			env->CallStaticVoidMethod ( billing, setPublicKey, jkey );
		}
	}
	
	return 0;
}

// Google Play In App Biling v3: TODO - make this meld with existing interface better...

//----------------------------------------------------------------//
/**	@name	checkInAppSupported
	@text	Check to see if the device can get in app billing
				
	@out 	boolean	success	
*/
int MOAIBillingAndroid::_checkInAppSupported ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );

	jclass billing = env->FindClass ( "com/ziplinegames/moai/MoaiGoogleBilling" );
    if ( billing == NULL ) {
	
		USLog::Print ( "MOAIBillingAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGoogleBilling" );
    } else {
	
    	jmethodID checkInAppSupported = env->GetStaticMethodID ( billing, "checkInAppSupported", "()Z" );
    	if ( checkInAppSupported == NULL ) {
	
			USLog::Print ( "MOAIBillingAndroid: Unable to find static java method %s", "checkInAppSupported" );
    	} else {
	
			jboolean jsuccess = ( jboolean )env->CallStaticBooleanMethod ( billing, checkInAppSupported );	

			lua_pushboolean ( state, jsuccess );	
			return 1;
		}
	}

	lua_pushboolean ( state, false );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	checkSubscriptionSupported
	@text	Check to see if the device can get subscription billing
				
	@out 	boolean	success	
*/
int MOAIBillingAndroid::_checkSubscriptionSupported ( lua_State* L ) {
	
	MOAILuaState state ( L );
		
	JNI_GET_ENV ( jvm, env );

	jclass billing = env->FindClass ( "com/ziplinegames/moai/MoaiGoogleBilling" );
    if ( billing == NULL ) {
	
		USLog::Print ( "MOAIBillingAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGoogleBilling" );
    } else {
	
    	jmethodID checkSubscriptionSupported = env->GetStaticMethodID ( billing, "checkSubscriptionSupported", "()Z" );
    	if ( checkSubscriptionSupported == NULL ) {
	
			USLog::Print ( "MOAIBillingAndroid: Unable to find static java method %s", "checkSubscriptionSupported" );
    	} else {
	
			jboolean jsuccess = ( jboolean )env->CallStaticBooleanMethod ( billing, checkSubscriptionSupported );	

			lua_pushboolean ( state, jsuccess );	
			return 1;
		}
	}

	lua_pushboolean ( state, false );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	consumePurchaseSync
	@text	Consumes a purchase
	
	@in		string token
	@out 	nil
*/
int MOAIBillingAndroid::_consumePurchaseSync ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* token = lua_tostring ( state, 1 );
	
	JNI_GET_ENV ( jvm, env );
	JNI_GET_JSTRING ( token, jtoken );

	jclass billing = env->FindClass ( "com/ziplinegames/moai/MoaiGoogleBilling" );
    if ( billing == NULL ) {
	
		USLog::Print ( "MOAIBillingAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGoogleBilling" );
    } else {
	
    	jmethodID consumePurchaseSync = env->GetStaticMethodID ( billing, "consumePurchaseSync", "(Ljava/lang/String;)I" );
    	if ( consumePurchaseSync == NULL ) {
	
			USLog::Print ( "MOAIBillingAndroid: Unable to find static java method %s", "consumePurchaseSync" );
    	} else {
	
			jint result = ( jint )env->CallStaticIntMethod ( billing, consumePurchaseSync, jtoken );
			lua_pushinteger ( state, result );
			return 1;
		}
	}

	lua_pushnumber ( state, BILLINGV3_RESPONSE_RESULT_ERROR );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	getPurchasedProducts
	@text	Gets the user's purchased products 
				
	@in		int 	type
	@opt	string 	continuation
	@out 	string	json string of products
*/
int MOAIBillingAndroid::_getPurchasedProducts ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );
	
	// get type
	int type = lua_tointeger ( state, 1 );
	cc8* continuation = lua_tostring ( state, 2 );
	JNI_GET_JSTRING ( continuation, jcontinuation );

	jclass billing = env->FindClass ( "com/ziplinegames/moai/MoaiGoogleBilling" );
    if ( billing == NULL ) {
	
		USLog::Print ( "MOAIBillingAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGoogleBilling" );
    } else {
	
    	jmethodID getPurchasedProducts = env->GetStaticMethodID ( billing, "getPurchasedProducts", "(ILjava/lang/String;)Ljava/lang/String;" );
    	if ( getPurchasedProducts == NULL ) {
	
			USLog::Print ( "MOAIBillingAndroid: Unable to find static java method %s", "getPurchasedProducts" );
    	} else {
	
			jstring jresult = ( jstring )env->CallStaticObjectMethod ( billing, getPurchasedProducts, type, jcontinuation );	

			JNI_GET_CSTRING ( jresult, result );
			lua_pushstring ( state, result );
			JNI_RELEASE_CSTRING ( jresult, result );
			return 1;
		}
	}

	return 0;
}

//----------------------------------------------------------------//
/**	@name	purchaseProduct
	@text	Starts a purchase intent for the desired product
	
	@in		string sku
	@in		int type
	@opt	string devPayload
	@out 	nil
*/
int MOAIBillingAndroid::_purchaseProduct ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* sku = lua_tostring ( state, 1 );
	int type = lua_tointeger ( state, 2 );
	cc8* devPayload = lua_tostring ( state, 3 );
	
	JNI_GET_ENV ( jvm, env );
	JNI_GET_JSTRING ( sku, jsku );
	JNI_GET_JSTRING ( devPayload, jdevPayload );

	jclass billing = env->FindClass ( "com/ziplinegames/moai/MoaiGoogleBilling" );
    if ( billing == NULL ) {
	
		USLog::Print ( "MOAIBillingAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGoogleBilling" );
    } else {
	
    	jmethodID purchaseProduct = env->GetStaticMethodID ( billing, "purchaseProduct", "(Ljava/lang/String;ILjava/lang/String;)I" );
    	if ( purchaseProduct == NULL ) {
	
			USLog::Print ( "MOAIBillingAndroid: Unable to find static java method %s", "purchaseProduct" );
    	} else {
	
			jint result = ( jint )env->CallStaticIntMethod ( billing, purchaseProduct, jsku, type, jdevPayload );
			lua_pushinteger ( state, result );
			return 1;
		}
	}

	lua_pushnumber ( state, BILLINGV3_RESPONSE_RESULT_ERROR );
	return 1;
}

//----------------------------------------------------------------//
/**	@name	requestProductsSync
	@text	Gets the products from Google Play for the current app
				
    @in		table   skus
	@in	    int		type
	@out 	string	json string of products
*/
int MOAIBillingAndroid::_requestProductsSync ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	JNI_GET_ENV ( jvm, env );
	
	//read skus from lua table 
	jobjectArray jskus = NULL;
	
	if ( state.IsType ( 1, LUA_TTABLE )) {
	
		int numEntries = 0;
		for ( int key = 1; ; ++key ) {
	
			state.GetField ( 1, key );
			cc8* value = _luaParseTable ( state, -1 );
			lua_pop ( state, 1 );
	
			if ( !value ) {
				
				numEntries = key - 1;
				break;
			}
		}
	
		jskus = env->NewObjectArray ( numEntries, env->FindClass( "java/lang/String" ), 0 );
		for ( int key = 1; ; ++key ) {
	
			state.GetField ( 1, key );
			cc8* value = _luaParseTable ( state, -1 );
			lua_pop ( state, 1 );
	
			if ( value ) {
				
				JNI_GET_JSTRING ( value, jvalue );
				env->SetObjectArrayElement ( jskus, key - 1, jvalue );
			}
			else {
				
				break;
			}	
		}
	}
	
	if ( jskus == NULL ) {
		
		jskus = env->NewObjectArray ( 0, env->FindClass( "java/lang/String" ), 0 );
	}	
	
	// get type
	int type = lua_tointeger ( state, 2 );

	jclass billing = env->FindClass ( "com/ziplinegames/moai/MoaiGoogleBilling" );
    if ( billing == NULL ) {
	
		USLog::Print ( "MOAIBillingAndroid: Unable to find java class %s", "com/ziplinegames/moai/MoaiGoogleBilling" );
    } else {
	
    	jmethodID requestProductsSync = env->GetStaticMethodID ( billing, "requestProductsSync", "([Ljava/lang/String;I)Ljava/lang/String;" );
    	if ( requestProductsSync == NULL ) {
	
			USLog::Print ( "MOAIBillingAndroid: Unable to find static java method %s", "requestProductsSync" );
    	} else {
	
			jstring jresult = ( jstring )env->CallStaticObjectMethod ( billing, requestProductsSync, jskus, type );	

			JNI_GET_CSTRING ( jresult, result );
			lua_pushstring ( state, result );
			JNI_RELEASE_CSTRING ( jresult, result );
			return 1;
		}
	}

	return 0;
}


//================================================================//
// MOAIBillingAndroid
//================================================================//

//----------------------------------------------------------------//
MOAIBillingAndroid::MOAIBillingAndroid () :
	mBillingProvider ( "com/ziplinegames/moai/MoaiGoogleBilling" ) {

	RTTI_SINGLE ( MOAILuaObject )	
}

//----------------------------------------------------------------//
MOAIBillingAndroid::~MOAIBillingAndroid () {

}

//----------------------------------------------------------------//
void MOAIBillingAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	state.SetField ( -1, "CHECK_BILLING_SUPPORTED",						( u32 )CHECK_BILLING_SUPPORTED );
	state.SetField ( -1, "PURCHASE_RESPONSE_RECEIVED",					( u32 )PURCHASE_RESPONSE_RECEIVED );
	state.SetField ( -1, "PURCHASE_STATE_CHANGED",						( u32 )PURCHASE_STATE_CHANGED );
	state.SetField ( -1, "RESTORE_RESPONSE_RECEIVED",					( u32 )RESTORE_RESPONSE_RECEIVED );
	state.SetField ( -1, "USER_ID_DETERMINED",							( u32 )USER_ID_DETERMINED );

	state.SetField ( -1, "BILLING_PROVIDER_GOOGLE",						( u32 )BILLING_PROVIDER_GOOGLE );
	state.SetField ( -1, "BILLING_PROVIDER_AMAZON",						( u32 )BILLING_PROVIDER_AMAZON );
	state.SetField ( -1, "BILLING_PROVIDER_TSTORE",						( u32 )BILLING_PROVIDER_TSTORE );
	
	state.SetField ( -1, "BILLING_RESULT_SUCCESS",						( u32 )BILLING_RESULT_SUCCESS );
	state.SetField ( -1, "BILLING_RESULT_USER_CANCELED",				( u32 )BILLING_RESULT_USER_CANCELED );
	state.SetField ( -1, "BILLING_RESULT_BILLING_UNAVAILABLE",			( u32 )BILLING_RESULT_BILLING_UNAVAILABLE );
	state.SetField ( -1, "BILLING_RESULT_ITEM_UNAVAILABLE",				( u32 )BILLING_RESULT_ITEM_UNAVAILABLE );
	state.SetField ( -1, "BILLING_RESULT_ERROR",						( u32 )BILLING_RESULT_ERROR );

	state.SetField ( -1, "BILLING_PURCHASE_STATE_ITEM_PURCHASED",		( u32 )BILLING_PURCHASE_STATE_ITEM_PURCHASED );
	state.SetField ( -1, "BILLING_PURCHASE_STATE_PURCHASE_CANCELED",	( u32 )BILLING_PURCHASE_STATE_PURCHASE_CANCELED );
	state.SetField ( -1, "BILLING_PURCHASE_STATE_ITEM_REFUNDED",		( u32 )BILLING_PURCHASE_STATE_ITEM_REFUNDED );
	
	// Google Play In App Biling v3: TODO - make this meld with existing interface better...
	state.SetField ( -1, "BILLINGV3_PRODUCT_INAPP",						( u32 )BILLINGV3_PRODUCT_INAPP );
	state.SetField ( -1, "BILLINGV3_PRODUCT_SUBSCRIPTION",				( u32 )BILLINGV3_PRODUCT_SUBSCRIPTION );
		
	state.SetField ( -1, "BILLINGV3_RESPONSE_RESULT_OK",					( u32 )BILLINGV3_RESPONSE_RESULT_OK );
	state.SetField ( -1, "BILLINGV3_RESPONSE_RESULT_USER_CANCELED",			( u32 )BILLINGV3_RESPONSE_RESULT_USER_CANCELED );
	state.SetField ( -1, "BILLINGV3_RESPONSE_RESULT_BILLING_UNAVAILABLE",	( u32 )BILLINGV3_RESPONSE_RESULT_BILLING_UNAVAILABLE );
	state.SetField ( -1, "BILLINGV3_RESPONSE_RESULT_ITEM_UNAVAILABLE",		( u32 )BILLINGV3_RESPONSE_RESULT_ITEM_UNAVAILABLE );
	state.SetField ( -1, "BILLINGV3_RESPONSE_RESULT_DEVELOPER_ERROR",		( u32 )BILLINGV3_RESPONSE_RESULT_DEVELOPER_ERROR );
	state.SetField ( -1, "BILLINGV3_RESPONSE_RESULT_ERROR",					( u32 )BILLINGV3_RESPONSE_RESULT_ERROR );
	state.SetField ( -1, "BILLINGV3_RESPONSE_RESULT_ITEM_ALREADY_OWNED",	( u32 )BILLINGV3_RESPONSE_RESULT_ITEM_ALREADY_OWNED );
	state.SetField ( -1, "BILLINGV3_RESPONSE_RESULT_ITEM_NOT_OWNED",		( u32 )BILLINGV3_RESPONSE_RESULT_ITEM_NOT_OWNED );

	luaL_Reg regTable [] = {
		{ "checkBillingSupported",	_checkBillingSupported },
		{ "confirmNotification",	_confirmNotification },
		{ "getUserId",				_getUserId },
		{ "requestPurchase",		_requestPurchase },
		{ "restoreTransactions",	_restoreTransactions },
		{ "setBillingProvider",		_setBillingProvider },
		{ "setListener",			_setListener },
		{ "setPublicKey",			_setPublicKey },
		
		// Google Play In App Biling v3: TODO - make this meld with existing interface better...
		{ "checkInAppSupported", 			_checkInAppSupported },
		{ "checkSubscriptionSupported",		_checkSubscriptionSupported },
		{ "consumePurchaseSync",			_consumePurchaseSync },
		{ "getPurchasedProducts",	 		_getPurchasedProducts },
		{ "purchaseProduct",	 			_purchaseProduct },
		{ "requestProductsSync", 			_requestProductsSync },
		
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
int MOAIBillingAndroid::MapAmazonPurchaseRequestStatus ( int code ) {
	
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
int MOAIBillingAndroid::MapAmazonPurchaseStateCode ( int code ) {

	switch ( code ) {
        case AMAZON_PURCHASE_STATE_ITEM_REFUNDED:
			return BILLING_PURCHASE_STATE_ITEM_REFUNDED;
        case AMAZON_PURCHASE_STATE_ITEM_PURCHASED:
		default:
			return BILLING_PURCHASE_STATE_ITEM_PURCHASED;
	}
}

//----------------------------------------------------------------//
int MOAIBillingAndroid::MapAmazonRestoreRequestStatus ( int code ) {
	
	switch ( code ) {
        case AMAZON_USER_ID_RESTORE_STATUS_SUCCESS:
			return BILLING_RESULT_SUCCESS;
        case AMAZON_USER_ID_RESTORE_STATUS_FAILED:
		default:
			return BILLING_RESULT_ERROR;
	}
}

//----------------------------------------------------------------//
int MOAIBillingAndroid::MapAmazonUserIdRequestStatus ( int code ) {
	
	switch ( code ) {
        case AMAZON_USER_ID_REQUEST_STATUS_SUCCESS:
			return BILLING_RESULT_SUCCESS;
        case AMAZON_USER_ID_REQUEST_STATUS_FAILED:
		default:
			return BILLING_RESULT_ERROR;
	}
}

//----------------------------------------------------------------//
int MOAIBillingAndroid::MapGooglePurchaseStateCode ( int code ) {

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
int MOAIBillingAndroid::MapGoogleResponseCode ( int code ) {
	
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
void MOAIBillingAndroid::NotifyBillingSupported ( bool supported ) {
	
	MOAILuaRef& callback = this->mListeners [ CHECK_BILLING_SUPPORTED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushboolean ( state, supported );
			
		state.DebugCall ( 1, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBillingAndroid::NotifyPurchaseResponseReceived ( int code, cc8* identifier ) {
	
	MOAILuaRef& callback = this->mListeners [ PURCHASE_RESPONSE_RECEIVED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushinteger ( state, code );	
		lua_pushstring ( state, identifier );
		
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBillingAndroid::NotifyPurchaseStateChanged ( int code, cc8* identifier, cc8* order, cc8* user, cc8* notification, cc8* payload ) {
	
	MOAILuaRef& callback = this->mListeners [ PURCHASE_STATE_CHANGED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushinteger ( state, code );
		lua_pushstring ( state, identifier );
		lua_pushstring ( state, order );
		lua_pushstring ( state, user );
		lua_pushstring ( state, notification );
		lua_pushstring ( state, payload );
		
		state.DebugCall ( 6, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBillingAndroid::NotifyRestoreResponseReceived ( int code, bool more, cc8* offset ) {
	
	MOAILuaRef& callback = this->mListeners [ RESTORE_RESPONSE_RECEIVED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();

		lua_pushinteger ( state, code );
		lua_pushboolean ( state, more );
		lua_pushstring ( state, offset );
		
		state.DebugCall ( 3, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBillingAndroid::NotifyUserIdDetermined ( int code, cc8* user ) {
	
	MOAILuaRef& callback = this->mListeners [ USER_ID_DETERMINED ];
	
	if ( callback ) {
		
		MOAILuaStateHandle state = callback.GetSelf ();
		
		lua_pushinteger ( state, code );
		lua_pushstring ( state, user );
		
		state.DebugCall ( 2, 0 );
	}
}

//================================================================//
// Amazon Billing JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiAmazonBilling_AKUNotifyAmazonBillingSupported ( JNIEnv* env, jclass obj, jboolean supported ) {

	MOAIBillingAndroid::Get ().NotifyBillingSupported ( supported );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiAmazonBilling_AKUNotifyAmazonPurchaseResponseReceived ( JNIEnv* env, jclass obj, jint code, jstring jidentifier ) {

	JNI_GET_CSTRING ( jidentifier, identifier );

	MOAIBillingAndroid::Get ().NotifyPurchaseResponseReceived ( MOAIBillingAndroid::MapAmazonPurchaseRequestStatus ( code ), identifier );

	JNI_RELEASE_CSTRING ( jidentifier, identifier );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiAmazonBilling_AKUNotifyAmazonPurchaseStateChanged ( JNIEnv* env, jclass obj, jint code, jstring jidentifier, jstring jorder, jstring juser, jstring jpayload ) {

	JNI_GET_CSTRING ( jidentifier, identifier );
	JNI_GET_CSTRING ( jorder, order );
	JNI_GET_CSTRING ( juser, user );
	JNI_GET_CSTRING ( jpayload, payload );
		
	MOAIBillingAndroid::Get ().NotifyPurchaseStateChanged ( MOAIBillingAndroid::MapAmazonPurchaseStateCode ( code ), identifier, order, user, NULL, payload );

	JNI_RELEASE_CSTRING ( jidentifier, identifier );
	JNI_RELEASE_CSTRING ( jorder, order );
	JNI_RELEASE_CSTRING ( juser, user );
	JNI_RELEASE_CSTRING ( jpayload, payload );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiAmazonBilling_AKUNotifyAmazonRestoreResponseReceived ( JNIEnv* env, jclass obj, jint code, jboolean more, jstring joffset ) {

	JNI_GET_CSTRING ( joffset, offset );

	MOAIBillingAndroid::Get ().NotifyRestoreResponseReceived ( MOAIBillingAndroid::MapAmazonRestoreRequestStatus ( code ), more, offset );

	JNI_RELEASE_CSTRING ( joffset, offset );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiAmazonBilling_AKUNotifyAmazonUserIdDetermined ( JNIEnv* env, jclass obj, jint code, jstring juser ) {

	JNI_GET_CSTRING ( juser, user );

	MOAIBillingAndroid::Get ().NotifyUserIdDetermined ( MOAIBillingAndroid::MapAmazonUserIdRequestStatus ( code ), user );

	JNI_RELEASE_CSTRING ( juser, user );
}

//================================================================//
// Google Billing JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGoogleBilling_AKUNotifyGoogleBillingSupported ( JNIEnv* env, jclass obj, jboolean supported ) {

	MOAIBillingAndroid::Get ().NotifyBillingSupported ( supported );
}
	
//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGoogleBilling_AKUNotifyGooglePurchaseResponseReceived ( JNIEnv* env, jclass obj, jint code, jstring jidentifier ) {

	JNI_GET_CSTRING ( jidentifier, identifier );

	MOAIBillingAndroid::Get ().NotifyPurchaseResponseReceived ( MOAIBillingAndroid::MapGoogleResponseCode ( code ), identifier);

	JNI_RELEASE_CSTRING ( jidentifier, identifier );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGoogleBilling_AKUNotifyGooglePurchaseStateChanged ( JNIEnv* env, jclass obj, jint code, jstring jidentifier, jstring jorder, jstring jnotification, jstring jpayload ) {

	JNI_GET_CSTRING ( jidentifier, identifier );
	JNI_GET_CSTRING ( jorder, order );
	JNI_GET_CSTRING ( jnotification, notification );
	JNI_GET_CSTRING ( jpayload, payload );
		
	MOAIBillingAndroid::Get ().NotifyPurchaseStateChanged ( MOAIBillingAndroid::MapGooglePurchaseStateCode ( code ), identifier, order, NULL, notification, payload );

	JNI_RELEASE_CSTRING ( jidentifier, identifier );
	JNI_RELEASE_CSTRING ( jorder, order );
	JNI_RELEASE_CSTRING ( jnotification, notification );
	JNI_RELEASE_CSTRING ( jpayload, payload );
}
	
//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiGoogleBilling_AKUNotifyGoogleRestoreResponseReceived ( JNIEnv* env, jclass obj, jint code ) {

	MOAIBillingAndroid::Get ().NotifyRestoreResponseReceived ( MOAIBillingAndroid::MapGoogleResponseCode ( code ), false, NULL );
}

//================================================================//
// Tstore Billing JNI methods
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiTstoreBilling_AKUNotifyTstorePurchaseResponseReceived ( JNIEnv* env, jclass obj, jint code, jstring jidentifier ) {

	JNI_GET_CSTRING ( jidentifier, identifier );

	MOAIBillingAndroid::Get ().NotifyPurchaseResponseReceived ( MOAIBillingAndroid::MapAmazonPurchaseRequestStatus ( code ), identifier );

	JNI_RELEASE_CSTRING ( jidentifier, identifier );
}

//----------------------------------------------------------------//
extern "C" void Java_com_ziplinegames_moai_MoaiTstoreBilling_AKUNotifyTstorePurchaseStateChanged ( JNIEnv* env, jclass obj, jint code, jstring jidentifier, jstring jorder, jstring juser, jstring jpayload ) {

	JNI_GET_CSTRING ( jidentifier, identifier );
	JNI_GET_CSTRING ( jorder, order );
	JNI_GET_CSTRING ( juser, user );
	JNI_GET_CSTRING ( jpayload, payload );
		
	MOAIBillingAndroid::Get ().NotifyPurchaseStateChanged ( MOAIBillingAndroid::MapAmazonPurchaseStateCode ( code ), identifier, order, user, NULL, payload );

	JNI_RELEASE_CSTRING ( jidentifier, identifier );
	JNI_RELEASE_CSTRING ( jorder, order );
	JNI_RELEASE_CSTRING ( juser, user );
	JNI_RELEASE_CSTRING ( jpayload, payload );
}


#endif
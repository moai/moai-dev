// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBILLINGANDROID_H
#define	MOAIBILLINGANDROID_H

#ifndef DISABLE_BILLING

#include <moaicore/moaicore.h>

//================================================================//
// MOAIBillingAndroid
//================================================================//
/**	@name	MOAIBillingAndroid
	@text	Wrapper for in-app purchase integration on Android 
			devices using either Google Play or Amazon. Exposed 
			to lua via MOAIBilling on all mobile platforms, but 
			API differs on iOS and Android.

	@const	CHECK_BILLING_SUPPORTED						Event code for billing support request completion.
	@const	PURCHASE_RESPONSE_RECEIVED					Event code for item purchase request receipt.
	@const	PURCHASE_STATE_CHANGED						Event code for item purchase state change (purchased, refunded, etc.).
	@const	RESTORE_RESPONSE_RECEIVED					Event code for restore purchases request receipt.
	@const	USER_ID_DETERMINED							Event code for user ID request completion.

	@const	BILLING_PROVIDER_GOOGLE						Provider code for Google Play.
	@const	BILLING_PROVIDER_AMAZON						Provider code for Amazon.

	@const	BILLING_RESULT_SUCCESS						Error code for a successful billing request.
	@const	BILLING_RESULT_USER_CANCELED				Error code for a billing request canceled by the user, if detected.
	@const	BILLING_RESULT_BILLING_UNAVAILABLE			Error code for a billing request attempted with no billing provider present.
	@const	BILLING_RESULT_ITEM_UNAVAILABLE				Error code for a billing request for an unavailable item.
	@const	BILLING_RESULT_ERROR						Error code for a billing request error.

	@const	BILLING_PURCHASE_STATE_ITEM_PURCHASED		Purchase state code for a successfully purchased item.
	@const	BILLING_PURCHASE_STATE_PURCHASE_CANCELED	Purchase state code for a canceled purchase.
	@const	BILLING_PURCHASE_STATE_ITEM_REFUNDED		Purchase state code for a refunded/revoked purchase.
*/
class MOAIBillingAndroid :
	public MOAIGlobalClass < MOAIBillingAndroid, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_checkBillingSupported	( lua_State* L );
	static int	_confirmNotification	( lua_State* L );
	static int	_getUserId				( lua_State* L );
	static int	_requestPurchase		( lua_State* L );
	static int	_restoreTransactions	( lua_State* L );
	static int	_setBillingProvider		( lua_State* L );
	static int	_setListener			( lua_State* L );
	static int	_setPublicKey			( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIBillingAndroid );
		
	enum {
		CHECK_BILLING_SUPPORTED,
		PURCHASE_RESPONSE_RECEIVED,
		PURCHASE_STATE_CHANGED,
		RESTORE_RESPONSE_RECEIVED,
		USER_ID_DETERMINED,
		TOTAL,
	};

	enum {
        BILLING_PROVIDER_GOOGLE,
        BILLING_PROVIDER_AMAZON,
		BILLING_PROVIDER_TSTORE
	};

	enum {
        BILLING_RESULT_SUCCESS,
        BILLING_RESULT_USER_CANCELED,
        BILLING_RESULT_BILLING_UNAVAILABLE,
        BILLING_RESULT_ITEM_UNAVAILABLE,
        BILLING_RESULT_ERROR,
	};

	enum {
        BILLING_PURCHASE_STATE_ITEM_PURCHASED,
        BILLING_PURCHASE_STATE_PURCHASE_CANCELED,
        BILLING_PURCHASE_STATE_ITEM_REFUNDED,
	};
	
	enum {
        GOOGLE_RESPONSE_CODE_OK,
        GOOGLE_RESPONSE_CODE_USER_CANCELED,
        GOOGLE_RESPONSE_CODE_SERVICE_UNAVAILABLE,
        GOOGLE_RESPONSE_CODE_BILLING_UNAVAILABLE,
        GOOGLE_RESPONSE_CODE_ITEM_UNAVAILABLE,
        GOOGLE_RESPONSE_CODE_DEVELOPER_ERROR,
        GOOGLE_RESPONSE_CODE_ERROR,
	};
	
	enum {
        GOOGLE_PURCHASE_STATE_ITEM_PURCHASED,
        GOOGLE_PURCHASE_STATE_PURCHASE_CANCELED,
        GOOGLE_PURCHASE_STATE_ITEM_REFUNDED,
	};
	
	enum {
		AMAZON_USER_ID_REQUEST_STATUS_SUCCESS,
		AMAZON_USER_ID_REQUEST_STATUS_FAILED,
	};

	enum {
		AMAZON_PURCHASE_REQUEST_STATUS_SUCCESS,
		AMAZON_PURCHASE_REQUEST_STATUS_FAILED,
		AMAZON_PURCHASE_REQUEST_STATUS_INVALID_SKU,
		AMAZON_PURCHASE_REQUEST_STATUS_ALREADY_ENTITLED,
	};

	enum {
		AMAZON_PURCHASE_STATE_ITEM_PURCHASED,
		AMAZON_PURCHASE_STATE_ITEM_REFUNDED,
	};

	enum {
		AMAZON_USER_ID_RESTORE_STATUS_SUCCESS,
		AMAZON_USER_ID_RESTORE_STATUS_FAILED,
	};

	cc8*			mBillingProvider;
	MOAILuaRef		mListeners [ TOTAL ];
	
					MOAIBillingAndroid				();
					~MOAIBillingAndroid				();
	static int		MapAmazonPurchaseRequestStatus	( int code );
	static int		MapAmazonPurchaseStateCode		( int code );
	static int		MapAmazonRestoreRequestStatus	( int code );
	static int		MapAmazonUserIdRequestStatus	( int code );
	static int		MapGooglePurchaseStateCode		( int code );
	static int		MapGoogleResponseCode			( int code );
	void			NotifyBillingSupported			( bool supported );
	void			NotifyPurchaseResponseReceived	( int code, cc8* identifier );
	void			NotifyPurchaseStateChanged		( int code, cc8* identifier, cc8* order, cc8* user, cc8* notification, cc8* payload );
	void			NotifyRestoreResponseReceived	( int code, bool more, cc8* offset );
	void			NotifyUserIdDetermined			( int code, cc8* user );
	void			RegisterLuaClass				( MOAILuaState& state );
};

#endif  //DISABLE_BILLING

#endif  //MOAIBILLING_H
// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBILLING_H
#define	MOAIBILLING_H

#ifndef DISABLE_BILLING

#include <moaicore/moaicore.h>

//================================================================//
// MOAIBilling
//================================================================//
class MOAIBilling :
	public MOAIGlobalClass < MOAIBilling, MOAILuaObject > {
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

	DECL_LUA_SINGLETON ( MOAIBilling );
		
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
	
					MOAIBilling						();
					~MOAIBilling					();
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
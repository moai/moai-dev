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
	static int	_requestPurchase		( lua_State* L );
	static int	_restoreTransactions	( lua_State* L );
	static int	_setListener			( lua_State* L );
	static int	_setMarketPublicKey		( lua_State* L );
	
public:

	DECL_LUA_SINGLETON ( MOAIBilling );
		
	enum {
		CHECK_BILLING_SUPPORTED,
		PURCHASE_RESPONSE_RECEIVED,
		PURCHASE_STATE_CHANGED,
		RESTORE_RESPONSE_RECEIVED,
		TOTAL,
	};

	enum {
        BILLING_RESULT_OK,
        BILLING_RESULT_USER_CANCELED,
        BILLING_RESULT_SERVICE_UNAVAILABLE,
        BILLING_RESULT_BILLING_UNAVAILABLE,
        BILLING_RESULT_ITEM_UNAVAILABLE,
        BILLING_RESULT_DEVELOPER_ERROR,
        BILLING_RESULT_ERROR,
	};

	enum {
        BILLING_STATE_ITEM_PURCHASED,
        BILLING_STATE_PURCHASE_CANCELED,
        BILLING_STATE_ITEM_REFUNDED,
	};
		
	MOAILuaRef		mListeners [ TOTAL ];
	
					MOAIBilling						();
					~MOAIBilling					();
	void			NotifyBillingSupported			( bool supported );
	void			NotifyPurchaseResponseReceived	( cc8* identifier, int code );
	void			NotifyPurchaseStateChanged		( cc8* identifier, int code, cc8* order, cc8* notification, cc8* payload );
	void			NotifyRestoreResponseReceived	( int code );
	void			RegisterLuaClass				( MOAILuaState& state );
};

#endif  //DISABLE_BILLING

#endif  //MOAIBILLING_H

// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPP_H
#define	MOAIAPP_H

#include <moaicore/moaicore.h>

//================================================================//
// MOAIApp
//================================================================//
class MOAIApp :
	public MOAIGlobalClass < MOAIApp, MOAILuaObject > {
private:

	enum {
		SESSION_START,
		SESSION_END,
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
	
	//----------------------------------------------------------------//
	static int		_setListener				( lua_State* L );
	static int		_checkBillingSupported		( lua_State* L );
	static int		_requestPurchase			( lua_State* L );
	static int		_confirmNotification		( lua_State* L );
	static int		_restoreTransactions		( lua_State* L );
	static int		_setMarketPublicKey			( lua_State* L );

	bool ( *checkBillingSupportedFunc )	( void );
	bool ( *requestPurchaseFunc ) 		( cc8* );
	bool ( *confirmNotificationFunc ) 	( cc8* );
	bool ( *restoreTransactionsFunc )	( void );
	void ( *setMarketPublicKeyFunc )	( cc8* );

public:
	
	DECL_LUA_SINGLETON ( MOAIApp )
	
	//----------------------------------------------------------------//
	void		DidStartSession					();
				MOAIApp							();
				~MOAIApp						();
	void		OnInit							();
	void		RegisterLuaClass				( MOAILuaState& state );
	void		Reset							();
	void		WillEndSession					();
	void		SetCheckBillingSupportedFunc	( bool ( *billingSupportedFunc ) ());
	void		SetRequestPurchaseFunc			( bool ( *purchaseFunc ) ( cc8* ));
	void		SetConfirmNotificationFunc		( bool ( *confirmFunc ) ( cc8* ));
	void		SetRestoreTransactionsFunc		( bool ( *restoreFunc ) () );
	void		SetMarketPublicKeyFunc			( void ( *setKeyFunc ) ( cc8* ));
	void		NotifyBillingSupported			( bool supported );
	void		NotifyPurchaseResponseReceived	( cc8* identifier, int code );
	void		NotifyPurchaseStateChanged		( cc8* identifier, int code, cc8* order, cc8* notification, cc8* payload );
	void		NotifyRestoreResponseReceived	( int code );
};

#endif

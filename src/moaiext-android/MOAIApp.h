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
		BACK_BUTTON_PRESSED,
		DIALOG_DISMISSED,
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
	
	enum {
        DIALOG_RESULT_POSITIVE,
        DIALOG_RESULT_NEUTRAL,
        DIALOG_RESULT_NEGATIVE,
        DIALOG_RESULT_CANCEL,
	};
	
	MOAILuaRef		mListeners [ TOTAL ];
	
	//----------------------------------------------------------------//
	static int		_checkBillingSupported		( lua_State* L );
	static int		_confirmNotification		( lua_State* L );
	static int		_openURL					( lua_State* L );
	static int		_requestPurchase			( lua_State* L );
	static int		_restoreTransactions		( lua_State* L );
	static int		_setListener				( lua_State* L );
	static int		_setMarketPublicKey			( lua_State* L );
	static int		_showDialog					( lua_State* L );

	bool ( *checkBillingSupportedFunc )			( void );
	bool ( *confirmNotificationFunc ) 			( cc8* );
	void ( *openURLFunc ) 						( cc8* );
	bool ( *requestPurchaseFunc ) 				( cc8* );
	bool ( *restoreTransactionsFunc )			( void );
	void ( *setMarketPublicKeyFunc )			( cc8* );
	void ( *showDialogFunc )					( cc8*, cc8*, cc8*, cc8*, cc8*, bool );

public:
	
	DECL_LUA_SINGLETON ( MOAIApp )
	
	//----------------------------------------------------------------//
	void		DidStartSession					();
				MOAIApp							();
				~MOAIApp						();
	bool		NotifyBackButtonPressed			();
	void		NotifyBillingSupported			( bool supported );
	void		NotifyDialogDismissed			( int code );
	void		NotifyPurchaseResponseReceived	( cc8* identifier, int code );
	void		NotifyPurchaseStateChanged		( cc8* identifier, int code, cc8* order, cc8* notification, cc8* payload );
	void		NotifyRestoreResponseReceived	( int code );
	void		OnInit							();
	void		RegisterLuaClass				( MOAILuaState& state );
	void		Reset							();
	void		SetCheckBillingSupportedFunc	( bool ( *func ) ());
	void		SetConfirmNotificationFunc		( bool ( *func ) ( cc8* ));
	void		SetOpenURLFunc					( void ( *func ) ( cc8* ));
	void		SetRequestPurchaseFunc			( bool ( *func ) ( cc8* ));
	void		SetRestoreTransactionsFunc		( bool ( *func ) () );
	void		SetMarketPublicKeyFunc			( void ( *func ) ( cc8* ));
	void		SetShowDialogFunc				( void ( *func ) ( cc8*, cc8*, cc8*, cc8*, cc8*, bool ));
	void		WillEndSession					();
};

#endif

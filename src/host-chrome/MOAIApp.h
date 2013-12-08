// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIAPP_H
#define	MOAIAPP_H

#include "moai-core/headers.h"

class MOAIApp :
	public MOAIGlobalClass < MOAIApp, MOAILuaObject > {
private:

	enum {
		ERROR,
		DID_REGISTER,
		LOCAL_NOTIFICATION,
		PAYMENT_QUEUE_TRANSACTION,
		PAYMENT_QUEUE_ERROR,
		PRODUCT_REQUEST_RESPONSE,
		REMOTE_NOTIFICATION,
		ASYNC_NAME_RESOLVE,
		TOTAL,
	};
	
	enum {
		TRANSACTION_STATE_PURCHASING,
		TRANSACTION_STATE_PURCHASED,
		TRANSACTION_STATE_FAILED,
		TRANSACTION_STATE_RESTORED,
		TRANSACTION_STATE_CANCELLED,
	};
		
	enum {
		DOMAIN_DOCUMENTS   = 0,
		DOMAIN_APP_SUPPORT = 1,
	};
	
	MOAILuaRef				mListeners [ TOTAL ];
	//MOAIStoreKitListener*	mStoreKitListener;
	//int						mNumRequests;
	//ReachabilityListener*	mReachabilityListener;

	//----------------------------------------------------------------//
	static int		_alert									( lua_State* L );

	static int		_canMakePayments						( lua_State* L );
	static int		_restoreCompletedTransactions			( lua_State* L );
	static int		_requestPaymentForProduct				( lua_State* L );
	static int		_requestProductIdentifiers				( lua_State* L );

	static int		_setListener							( lua_State* L );
	static int		_getDirectoryInDomain					( lua_State* L );
	static int		_openURL								( lua_State* L );
	static int		_openURLPOP								( lua_State* L );

	//----------------------------------------------------------------//
	//void			PushPaymentTransaction					( lua_State* L, SKPaymentTransaction* transaction );
	void			PushPaymentTransaction					( int transactionResult, lua_State *L );

public:
	
	DECL_LUA_SINGLETON ( MOAIApp )
	
	static void HttpLoaded ( GetURLHandler *handler, const char *buffer, int32_t size );
	static void VerifyPaymentMainThread ( void * userData, int32_t result );

	static void RedeemProduct ( );
	static void DummyHttpLoaded ( GetURLHandler *handler, const char *buffer, int32_t size );


	//SET ( UIApplication*, Application, mApplication )
	
	//----------------------------------------------------------------//
				MOAIApp														();
				~MOAIApp													();
	void		OnInit														();
	void		Reset														();

	void		DidReceivePaymentQueueError									( cc8 *extraInfo );
	void		PaymentQueueUpdatedTransactions								( int transactionResult, const char *extraInfo );
	void		ProductsRequestDidReceiveResponse							();
	void		RegisterLuaClass											( MOAILuaState& state );

	static void HandleStoreMessage											( std::string & message );

	/*void		DidReceivePaymentQueueError									( NSError *error, cc8 *extraInfo );
	void		PaymentQueueUpdatedTransactions								( SKPaymentQueue* queue, NSArray* transactions );
	void		ProductsRequestDidReceiveResponse							( SKProductsRequest* request, SKProductsResponse* response );
	void		RegisterLuaClass											( USLuaState& state );*/
};

#endif

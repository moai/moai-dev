// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#ifndef	MOAIBILLINGIOS_H
#define	MOAIBILLINGIOS_H

#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import <UIKit/UIKit.h>
#import <moaicore/moaicore.h>

@class MOAIStoreKitListener;

//================================================================//
// MOAIBillingIOS
//================================================================//
/**	@name	MOAIBillingIOS
	@text	Wrapper for StoreKit.
	
	@const	PAYMENT_QUEUE_TRANSACTION
	@const	PAYMENT_QUEUE_ERROR
	@const	PRODUCT_REQUEST_RESPONSE
	
	@const	TRANSACTION_STATE_PURCHASING
	@const	TRANSACTION_STATE_PURCHASED
	@const	TRANSACTION_STATE_FAILED
	@const	TRANSACTION_STATE_RESTORED
	@const	TRANSACTION_STATE_CANCELLED
*/
class MOAIBillingIOS :
	public MOAIGlobalClass < MOAIBillingIOS, MOAILuaObject > {
private:

	enum {
		PAYMENT_QUEUE_TRANSACTION,
		PAYMENT_QUEUE_ERROR,
		PRODUCT_REQUEST_RESPONSE,
		TOTAL,
	};
	
	enum {
		TRANSACTION_STATE_PURCHASING,
		TRANSACTION_STATE_PURCHASED,
		TRANSACTION_STATE_FAILED,
		TRANSACTION_STATE_RESTORED,
		TRANSACTION_STATE_CANCELLED,
	};
	
	MOAILuaRef				mListeners [ TOTAL ];
	MOAIStoreKitListener*	mStoreKitListener;

	//----------------------------------------------------------------//
	static int		_canMakePayments						( lua_State* L );
	static int		_restoreCompletedTransactions			( lua_State* L );
	static int		_requestPaymentForProduct				( lua_State* L );
	static int		_requestProductIdentifiers				( lua_State* L );
	static int		_setListener							( lua_State* L );

	//----------------------------------------------------------------//
	void			PushPaymentTransaction					( lua_State* L, SKPaymentTransaction* transaction );

public:
	
	DECL_LUA_SINGLETON ( MOAIBillingIOS )
	
	//----------------------------------------------------------------//
	void		DidReceivePaymentQueueError									( NSError *error, cc8 *extraInfo );
	void		InitStoreKit												();
				MOAIBillingIOS												();
				~MOAIBillingIOS												();
	void		OnInit														();
	void		PaymentQueueUpdatedTransactions								( SKPaymentQueue* queue, NSArray* transactions );
	void		ProductsRequestDidReceiveResponse							( SKProductsRequest* request, SKProductsResponse* response );
	void		RegisterLuaClass											( MOAILuaState& state );
};

#endif

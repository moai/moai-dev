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
	@text	Wrapper for in-app purchase integration on iOS devices 
			using Apple StoreKit. Exposed to lua via MOAIBilling on 
			all mobile platforms, but API differs on iOS and Android.
	
	@const	PAYMENT_QUEUE_TRANSACTION		Event invoked when a transaction changes state.
	@const	PAYMENT_QUEUE_ERROR				Event invoked when a transaction fails.
	@const	PRODUCT_REQUEST_RESPONSE		Event invoked when a product information request completes.
	@const	PRODUCT_RESTORE_FINISHED		Event invoked when a transactions restore is finished.
	
	@const	TRANSACTION_STATE_PURCHASING	Error code indicating a transaction in progress.
	@const	TRANSACTION_STATE_PURCHASED		Error code indicating a completed transaction.
	@const	TRANSACTION_STATE_FAILED		Error code indicating a failed transaction.
	@const	TRANSACTION_STATE_RESTORED		Error code indicating a restored transaction.
	@const	TRANSACTION_STATE_CANCELLED		Error code indicating a canceled transaction.
*/
class MOAIBillingIOS :
	public MOAIGlobalClass < MOAIBillingIOS, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int	_canMakePayments				( lua_State* L );
	static int	_restoreCompletedTransactions	( lua_State* L );
	static int	_requestPaymentForProduct		( lua_State* L );
	static int	_requestProductIdentifiers		( lua_State* L );
	static int	_setListener					( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAIBillingIOS )
	
	enum {
		PAYMENT_QUEUE_TRANSACTION,
		PAYMENT_QUEUE_ERROR,
		PRODUCT_REQUEST_RESPONSE,
		PAYMENT_RESTORE_FINISHED,
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

			MOAIBillingIOS						();
			~MOAIBillingIOS						();
	void	DidReceivePaymentQueueError			( NSError *error, cc8 *extraInfo );
	void	DidReceiveRestoreFinished			( SKPaymentQueue* queue );
	void	InitStoreKit						();
	void	OnInit								();
	void	PaymentQueueUpdatedTransactions		( SKPaymentQueue* queue, NSArray* transactions );
	void	ProductsRequestDidReceiveResponse	( SKProductsRequest* request, SKProductsResponse* response );
	void	PushPaymentTransaction				( lua_State* L, SKPaymentTransaction* transaction );
	void	RegisterLuaClass					( MOAILuaState& state );
};

#endif
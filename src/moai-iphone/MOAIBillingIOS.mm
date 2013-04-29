// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#import <StoreKit/StoreKit.h>
#import <moaiext-iphone/MOAIBillingIOS.h>
#import <moaiext-iphone/MOAIStoreKitListener.h>
#import <moaiext-iphone/NSData+MOAILib.h>
#import <moaiext-iphone/NSDate+MOAILib.h>
#import <moaiext-iphone/NSDictionary+MOAILib.h>
#import <moaiext-iphone/NSError+MOAILib.h>
#import <moaiext-iphone/NSString+MOAILib.h>

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	canMakePayments
	@text	Determines whether or not the app has permission to 
			request payments for in-app purchases.
	
	@out	bool enabled
*/
int MOAIBillingIOS::_canMakePayments ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	BOOL result = [ SKPaymentQueue canMakePayments ];
	lua_pushboolean ( state, result );
	
	return 1;
}

//----------------------------------------------------------------//
/**	@name	requestPaymentForProduct
	@text	Request payment for an in-app purchase.
	
	@in		string 	sku				The SKU to purchase.
	@opt	number 	quantity		Default value is 1.
	@out	nil
*/
int MOAIBillingIOS::_requestPaymentForProduct ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	cc8* identifier = state.GetValue < cc8* >( 1, "" );
	int quantity = state.GetValue < int >( 2, 1 );
	
	if ( quantity ) {
		
		SKMutablePayment* payment = [ SKMutablePayment paymentWithProductIdentifier:[ NSString stringWithUTF8String:identifier ]];
		payment.quantity = quantity;
		[[ SKPaymentQueue defaultQueue ] addPayment:payment ];
	}
	
	return 0;
}

//----------------------------------------------------------------//
/**	@name	requestProductIdentifiers
	@text	Verify the validity of a set of products and retrieve
			relevant information thereabout.
	
	@in		table skus				A list of SKUs.
	@out	nil
*/
int MOAIBillingIOS::_requestProductIdentifiers ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	NSMutableSet* productSet = [[[ NSMutableSet alloc ] init ] autorelease ];
	
	int top = state.GetTop ();
	for ( int i = 1; i <= top; ++i ) {
	
		if ( state.IsType ( i, LUA_TSTRING )) {
			
			cc8* identifier = state.GetValue < cc8* >( i, "" );
			[ productSet addObject :[ NSString stringWithUTF8String:identifier ]];
		}
	
		if ( state.IsType ( i, LUA_TTABLE )) {
			
			int itr = state.PushTableItr ( i );
			while ( state.TableItrNext ( itr )) {
				
				cc8* identifier = state.GetValue < cc8* >( -1, "" );
				[ productSet addObject :[ NSString stringWithUTF8String:identifier ]];
			}
		}
	}
	
	SKProductsRequest* request = [[ SKProductsRequest alloc ] initWithProductIdentifiers:productSet ];
	request.delegate = MOAIBillingIOS::Get ().mStoreKitListener;
	[ request start ];
	
	return 0;
}

//----------------------------------------------------------------//
/** @name	restoreCompletedTransactions
	@text	Request the restoration of any previously purchased items.

	@out 	nil
*/
int MOAIBillingIOS::_restoreCompletedTransactions( lua_State* L ) {
	
	UNUSED ( L );
	
	[[SKPaymentQueue defaultQueue] restoreCompletedTransactions];

	return 0;
}

//----------------------------------------------------------------//
int MOAIBillingIOS::_setListener ( lua_State* L ) {
	
	MOAILuaState state ( L );
	
	u32 idx = state.GetValue < u32 >( 1, TOTAL );
	
	if ( idx < TOTAL ) {
		MOAIBillingIOS::Get ().mListeners [ idx ].SetStrongRef ( state, 2 );
	}
	
	return 0;
}

//================================================================//
// MOAIBillingIOS
//================================================================//

//----------------------------------------------------------------//
MOAIBillingIOS::MOAIBillingIOS () {

	RTTI_SINGLE ( MOAILuaObject )
	
	this->mStoreKitListener = [[ MOAIStoreKitListener alloc ] init ];
	[[ SKPaymentQueue defaultQueue ] addTransactionObserver:this->mStoreKitListener ];
}

//----------------------------------------------------------------//
MOAIBillingIOS::~MOAIBillingIOS () {

	[[ SKPaymentQueue defaultQueue ] removeTransactionObserver:this->mStoreKitListener];
	[ this->mStoreKitListener release ];
}

//----------------------------------------------------------------//
void MOAIBillingIOS::DidReceivePaymentQueueError ( NSError* error, cc8* extraInfo ) {
	
	MOAILuaRef& callback = this->mListeners [ PAYMENT_QUEUE_ERROR ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();
		
		[ error toLua:state ];
		lua_pushstring(state, extraInfo);
		
		state.DebugCall ( 2, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBillingIOS::DidReceiveRestoreFinished ( SKPaymentQueue* queue ) {
	
	UNUSED ( queue );
	
	MOAILuaRef& callback = this->mListeners [ PAYMENT_RESTORE_FINISHED ];
	
	if ( callback ) {
		MOAILuaStateHandle state = callback.GetSelf ();		
		state.DebugCall ( 0, 0 );
	}
}

//----------------------------------------------------------------//
void MOAIBillingIOS::PaymentQueueUpdatedTransactions ( SKPaymentQueue* queue, NSArray* transactions ) {
	
	UNUSED ( queue );

	MOAILuaRef& callback = this->mListeners [ PAYMENT_QUEUE_TRANSACTION ];

	for ( SKPaymentTransaction* transaction in transactions ) {
	
		if ( callback ) {
		
			MOAILuaStateHandle state = callback.GetSelf ();
			this->PushPaymentTransaction ( state, transaction );
			state.DebugCall ( 1, 0 );
		}
		
		if ( transaction.transactionState != SKPaymentTransactionStatePurchasing ) {
			
			[[ SKPaymentQueue defaultQueue ] finishTransaction:transaction ];
		}
	}
}

//----------------------------------------------------------------//
void MOAIBillingIOS::ProductsRequestDidReceiveResponse ( SKProductsRequest* request, SKProductsResponse* response ) {

	MOAILuaRef& callback = this->mListeners [ PRODUCT_REQUEST_RESPONSE ];
	if ( callback ) {
		
		NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
		[formatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
		[formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
		
		MOAILuaStateHandle state = callback.GetSelf ();
		lua_newtable ( state );
		
		int count = 1;
		for ( SKProduct* product in response.products ) {
		
			lua_pushnumber ( state, count++ );
			lua_newtable ( state );
		
			[formatter setLocale:product.priceLocale];
			NSString * formattedString = [ formatter stringFromNumber:product.price];
			
			state.SetField ( -1, "localizedTitle", [ product.localizedTitle UTF8String ]);
			state.SetField ( -1, "localizedDescription", [ product.localizedDescription UTF8String ]);
			state.SetField ( -1, "price", [ product.price floatValue ]);
			state.SetField ( -1, "localizedPrice", [ formattedString UTF8String]);
			state.SetField ( -1, "priceLocale", [ product.priceLocale.localeIdentifier UTF8String ]);
			state.SetField ( -1, "productIdentifier", [ product.productIdentifier UTF8String ]);
			
			lua_settable ( state, -3 );
		}
		
		// Note: If you're testing in-app purchases, chances are your product Id
		// will take time to propagate into Apple's sandbox/test environment and
		// thus the id's will be invalid for hours(?) (at the time of writing this).
		for ( NSString *invalidProductId in response.invalidProductIdentifiers ) {
			
			NSLog(@"StoreKit: Invalid product id: %@" , invalidProductId);
		}
		
		state.DebugCall ( 1, 0 );
		[formatter release];
		
	}
	
	[ request autorelease ];
}

//----------------------------------------------------------------//
void MOAIBillingIOS::PushPaymentTransaction ( lua_State* L, SKPaymentTransaction* transaction ) {

	lua_newtable ( L );
	
	lua_pushstring ( L, "transactionState" );
	
	switch ( transaction.transactionState ) {
		
		case SKPaymentTransactionStatePurchasing: {
			
			lua_pushnumber ( L, TRANSACTION_STATE_PURCHASING );
			break;
		}
		case SKPaymentTransactionStatePurchased: {
			
			lua_pushnumber ( L, TRANSACTION_STATE_PURCHASED );
			break;
		}
		case SKPaymentTransactionStateFailed: {
			
			if ( transaction.error.code == SKErrorPaymentCancelled ) {
				
				lua_pushnumber ( L, TRANSACTION_STATE_CANCELLED );
			}
			else {
				
				lua_pushnumber ( L, TRANSACTION_STATE_FAILED );
			}
			break;
		}
		case SKPaymentTransactionStateRestored: {
			
			lua_pushnumber ( L, TRANSACTION_STATE_RESTORED );
			break;
		}
		default: {
			
			lua_pushnil ( L );
			break;
		}
	}
	
	lua_settable ( L, -3 );
	
	if ( transaction.payment ) {
		
		lua_pushstring ( L, "payment" );
		lua_newtable ( L );
		
		lua_pushstring ( L, "productIdentifier" );
		[ transaction.payment.productIdentifier toLua:L ];
		lua_settable ( L, -3 );
		
		lua_pushstring ( L, "quantity" );
		lua_pushnumber ( L, transaction.payment.quantity );
		lua_settable ( L, -3 );
		
		lua_settable ( L, -3 );
	}
	
	if ( transaction.transactionState == SKPaymentTransactionStateFailed ) {
		
		lua_pushstring ( L, "error" );
		[ transaction.error toLua:L ];
		lua_settable ( L, -3 );
	}
	
	if ( transaction.transactionState == SKPaymentTransactionStateRestored ) {
		
		lua_pushstring ( L, "originalTransaction" );
		this->PushPaymentTransaction ( L, transaction.originalTransaction );
		lua_settable ( L, -3 );
	}
	
	if ( transaction.transactionState == SKPaymentTransactionStatePurchased ) {
		
		lua_pushstring ( L, "transactionReceipt" );
		if( transaction.transactionReceipt != nil ) {
			
			[ transaction.transactionReceipt toLua:L ];
		}
		else {
			
			lua_pushnil ( L );
		}

		lua_settable ( L, -3 );
	}
	
	if (( transaction.transactionState == SKPaymentTransactionStatePurchased ) || ( transaction.transactionState == SKPaymentTransactionStateRestored )) {
		
		lua_pushstring ( L, "transactionDate" );
		[ transaction.transactionDate toLua:L ];
		lua_settable ( L, -3 );

		lua_pushstring ( L, "transactionIdentifier" );
		[ transaction.transactionIdentifier toLua:L ];
		lua_settable ( L, -3 );
	}
}

//----------------------------------------------------------------//
void MOAIBillingIOS::RegisterLuaClass ( MOAILuaState& state ) {
	
	state.SetField ( -1, "PAYMENT_QUEUE_TRANSACTION",	( u32 )PAYMENT_QUEUE_TRANSACTION );
	state.SetField ( -1, "PRODUCT_REQUEST_RESPONSE",	( u32 )PRODUCT_REQUEST_RESPONSE );
	state.SetField ( -1, "PAYMENT_QUEUE_ERROR",			( u32 )PAYMENT_QUEUE_ERROR );
	state.SetField ( -1, "PAYMENT_RESTORE_FINISHED",	( u32 )PAYMENT_RESTORE_FINISHED );

	state.SetField ( -1, "TRANSACTION_STATE_PURCHASING",( u32 )TRANSACTION_STATE_PURCHASING );
	state.SetField ( -1, "TRANSACTION_STATE_PURCHASED", ( u32 )TRANSACTION_STATE_PURCHASED );
	state.SetField ( -1, "TRANSACTION_STATE_FAILED",    ( u32 )TRANSACTION_STATE_FAILED );
	state.SetField ( -1, "TRANSACTION_STATE_RESTORED",  ( u32 )TRANSACTION_STATE_RESTORED );
	state.SetField ( -1, "TRANSACTION_STATE_CANCELLED", ( u32 )TRANSACTION_STATE_CANCELLED );
	
	luaL_Reg regTable [] = {
		{ "canMakePayments",				_canMakePayments },
		{ "restoreCompletedTransactions",	_restoreCompletedTransactions },
		{ "requestPaymentForProduct",		_requestPaymentForProduct },
		{ "requestProductIdentifiers",		_requestProductIdentifiers },
		{ "setListener",					_setListener },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}
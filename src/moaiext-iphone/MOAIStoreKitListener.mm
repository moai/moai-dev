//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <aku/AKU.h>
#import <aku/AKU-iphone.h>
#import <moaiext-iphone/MOAIApp.h>
#import <moaiext-iphone/MOAIStoreKitListener.h>

//================================================================//
// MOAIStoreKitListener
//================================================================//
@implementation MOAIStoreKitListener

	//================================================================//
	#pragma mark -
	#pragma mark Protocol SKPaymentTransactionObserver
	//================================================================//

	//----------------------------------------------------------------//
	-( void ) paymentQueue:( SKPaymentQueue* )queue removedTransactions:( NSArray* )transactions {
		UNUSED ( queue );
		UNUSED ( transactions );
	}
	
	//----------------------------------------------------------------//
	-( void ) paymentQueue:( SKPaymentQueue* )queue restoreCompletedTransactionsFailedWithError:( NSError* )error {
		UNUSED ( queue );
		UNUSED ( error );
		NSLog ( @"StoreKit Restore Completed Transactions Error: %@", error.localizedDescription );
		MOAIApp::Get().DidReceivePaymentQueueError(error, "restoreCompletedTransactions");
	}
	
	//----------------------------------------------------------------//
	-( void ) paymentQueue:( SKPaymentQueue* )queue updatedTransactions:( NSArray* )transactions {
	
		MOAIApp::Get ().PaymentQueueUpdatedTransactions ( queue, transactions );
	}

	//----------------------------------------------------------------//
	-( void ) paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue* )queue {
		UNUSED ( queue );
	}

	//================================================================//
	#pragma mark -
	#pragma mark Protocol SKProductsDelegate
	//================================================================//

	//----------------------------------------------------------------//
	-( void ) request:( SKRequest* )request didFailWithError:( NSError* )error {
		UNUSED ( request );
	
		NSLog ( @"StoreKit Request Error: %@", error.localizedDescription );
		MOAIApp::Get().DidReceivePaymentQueueError(error, "request");
	}

	//================================================================//
	#pragma mark -
	#pragma mark Protocol SKProductsRequestDelegate
	//================================================================//

	//----------------------------------------------------------------//
	-( void ) productsRequest:( SKProductsRequest* )request didReceiveResponse:( SKProductsResponse* )response {
		
		MOAIApp::Get ().ProductsRequestDidReceiveResponse ( request, response );
	}

@end

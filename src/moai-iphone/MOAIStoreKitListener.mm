//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

#import <aku/aku.h>
#import <aku/aku-iphone.h>
#import <moai-iphone/MOAIApp.h>
#import <moai-iphone/MOAIStoreKitListener.h>

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
	}
	
	//----------------------------------------------------------------//
	-( void ) paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue* )queue {
		UNUSED ( queue );
	}
	
	//----------------------------------------------------------------//
	-( void ) paymentQueue:( SKPaymentQueue* )queue updatedTransactions:( NSArray* )transactions {
	
		MOAIApp::Get ().PaymentQueueUpdatedTransactions ( queue, transactions );
	}

	//================================================================//
	#pragma mark -
	#pragma mark Protocol SKProductsDelegate
	//================================================================//

	//----------------------------------------------------------------//
	-( void ) request:( SKRequest* )request didFailWithError:( NSError* )error {
		
		NSLog ( @"%@", error.localizedDescription );
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

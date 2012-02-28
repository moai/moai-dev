//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import @PACKAGE@.AndroidMarketBillingConstants.PurchaseState;
import @PACKAGE@.AndroidMarketBillingConstants.ResponseCode;
import @PACKAGE@.AndroidMarketBillingService.RequestPurchase;
import @PACKAGE@.AndroidMarketBillingService.RestoreTransactions;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

//================================================================//
// AndroidMarketBillingResponseHandler
//================================================================//
public class AndroidMarketBillingResponseHandler {
	
    private static AndroidMarketBillingPurchaseObserver sPurchaseObserver;

    public static synchronized void register ( AndroidMarketBillingPurchaseObserver observer ) {
	
        sPurchaseObserver = observer;
    }

    public static synchronized void unregister ( AndroidMarketBillingPurchaseObserver observer ) {
	
        sPurchaseObserver = null;
    }

    public static void checkBillingSupportedResponse ( boolean supported ) {
	
        if ( sPurchaseObserver != null ) {
	
            sPurchaseObserver.onBillingSupported ( supported );
        }
    }

 	public static void buyPageIntentResponse ( PendingIntent pendingIntent, Intent intent ) {
	
        if ( sPurchaseObserver != null ) {
	
        	sPurchaseObserver.startBuyPageActivity ( pendingIntent, intent );
		}
    }

    public static void purchaseResponse ( final PurchaseState purchaseState, final String productId, final String orderId, final String notificationId, final String developerPayload ) {

    	if ( sPurchaseObserver != null ) {
	
        	sPurchaseObserver.onPurchaseStateChange ( purchaseState, productId, orderId, notificationId, developerPayload );
        }
    }
    
    public static void responseCodeReceived ( RequestPurchase request, ResponseCode responseCode ) {
	
        if ( sPurchaseObserver != null ) {
	
            sPurchaseObserver.onRequestPurchaseResponse ( request, responseCode );
        }
    }
    
    public static void responseCodeReceived ( RestoreTransactions request, ResponseCode responseCode ) {
	
        if ( sPurchaseObserver != null ) {
	
            sPurchaseObserver.onRestoreTransactionsResponse ( request, responseCode );
        }
    }
}

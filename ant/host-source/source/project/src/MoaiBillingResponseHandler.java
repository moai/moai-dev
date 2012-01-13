//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import @PACKAGE@.MoaiBillingConstants.PurchaseState;
import @PACKAGE@.MoaiBillingConstants.ResponseCode;
import @PACKAGE@.MoaiBillingService.RequestPurchase;
import @PACKAGE@.MoaiBillingService.RestoreTransactions;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class MoaiBillingResponseHandler {
    private static final String TAG = "MoaiBillingResponseHandler";

    private static MoaiBillingPurchaseObserver sPurchaseObserver;
    public static synchronized void register ( MoaiBillingPurchaseObserver observer ) {
        sPurchaseObserver = observer;
    }

    public static synchronized void unregister ( MoaiBillingPurchaseObserver observer ) {
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

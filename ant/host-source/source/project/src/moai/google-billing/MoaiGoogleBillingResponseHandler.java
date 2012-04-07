//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.PendingIntent;
import android.content.Intent;

//================================================================//
// MoaiGoogleBillingResponseHandler
//================================================================//
public class MoaiGoogleBillingResponseHandler {
	
    private static MoaiGoogleBillingPurchaseObserver sPurchaseObserver = null;

	//----------------------------------------------------------------//
    public static synchronized void register ( MoaiGoogleBillingPurchaseObserver observer ) {
	
        sPurchaseObserver = observer;
    }

	//----------------------------------------------------------------//
    public static synchronized void unregister () {
	
        sPurchaseObserver = null;
    }

	//----------------------------------------------------------------//
    public static void checkBillingSupportedResponse ( boolean supported ) {
	
        if ( sPurchaseObserver != null ) {
	
            sPurchaseObserver.onBillingSupported ( supported );
        }
    }

	//----------------------------------------------------------------//
 	public static void buyPageIntentResponse ( PendingIntent pendingIntent, Intent intent ) {
	
        if ( sPurchaseObserver != null ) {
	
        	sPurchaseObserver.showBuyPage ( pendingIntent, intent );
		}
    }

	//----------------------------------------------------------------//
    public static void purchaseResponse ( MoaiGoogleBillingConstants.PurchaseState purchaseState, String productId, String orderId, String notificationId, String developerPayload ) {

    	if ( sPurchaseObserver != null ) {
	
        	sPurchaseObserver.onPurchaseStateChange ( purchaseState, productId, orderId, notificationId, developerPayload );
        }
    }
    
	//----------------------------------------------------------------//
    public static void responseCodeReceived ( MoaiGoogleBillingService.RequestPurchase request, MoaiGoogleBillingConstants.ResponseCode responseCode ) {
	
        if ( sPurchaseObserver != null ) {
	
            sPurchaseObserver.onRequestPurchaseResponse ( request, responseCode );
        }
    }
    
	//----------------------------------------------------------------//
    public static void responseCodeReceived ( MoaiGoogleBillingService.RestoreTransactions request, MoaiGoogleBillingConstants.ResponseCode responseCode ) {
	
        if ( sPurchaseObserver != null ) {
	
            sPurchaseObserver.onRestoreTransactionsResponse ( request, responseCode );
        }
    }
}

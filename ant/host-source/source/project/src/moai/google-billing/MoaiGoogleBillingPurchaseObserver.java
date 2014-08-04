//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.app.PendingIntent;
import android.app.PendingIntent.CanceledException;
import android.content.Intent;
import android.content.IntentSender;

import java.lang.reflect.Method;

//================================================================//
// MoaiGoogleBillingPurchaseObserver
//================================================================//
public abstract class MoaiGoogleBillingPurchaseObserver {
	
    private Activity	mActivity = null;
    private Method 		mStartIntentSender = null;
    private Object [] 	mStartIntentSenderArgs = new Object [ 5 ];

    private static final Class < ? > [] START_INTENT_SENDER_SIG = new Class [] {
	
        IntentSender.class, Intent.class, int.class, int.class, int.class
    };

	//----------------------------------------------------------------//
    public MoaiGoogleBillingPurchaseObserver ( Activity activity ) {
	
        mActivity = activity;

         try {

           	mStartIntentSender = mActivity.getClass ().getMethod ( "startIntentSender", START_INTENT_SENDER_SIG );
        } catch ( SecurityException e ) {

            mStartIntentSender = null;
        } catch ( NoSuchMethodException e ) {

            mStartIntentSender = null;
        }
    }

	//----------------------------------------------------------------//
    public abstract void onBillingSupported ( boolean supported );

	//----------------------------------------------------------------//
    public abstract void onPurchaseStateChange ( MoaiGoogleBillingConstants.PurchaseState purchaseState, String itemId, String orderId, String notificationId, String developerPayload );

	//----------------------------------------------------------------//
    public abstract void onRequestPurchaseResponse ( MoaiGoogleBillingService.RequestPurchase request, MoaiGoogleBillingConstants.ResponseCode responseCode );

	//----------------------------------------------------------------//
    public abstract void onRestoreTransactionsResponse ( MoaiGoogleBillingService.RestoreTransactions request, MoaiGoogleBillingConstants.ResponseCode responseCode );

	//----------------------------------------------------------------//
    void showBuyPage ( PendingIntent pendingIntent, Intent intent ) {
	
        if ( mStartIntentSender != null ) {
	
			// Android 2.0+
            try {
	
                mStartIntentSenderArgs [ 0 ] = pendingIntent.getIntentSender ();
                mStartIntentSenderArgs [ 1 ] = intent;
                mStartIntentSenderArgs [ 2 ] = Integer.valueOf ( 0 );
                mStartIntentSenderArgs [ 3 ] = Integer.valueOf ( 0 );
                mStartIntentSenderArgs [ 4 ] = Integer.valueOf ( 0 );
                mStartIntentSender.invoke ( mActivity, mStartIntentSenderArgs );
            } catch ( Exception e ) {
	
                MoaiLog.e ( "MoaiGoogleBillingPurchaseObserver startBuyPageActivity: error invoking intent sender", e );
            }
        } else {
	
			// Android 1.6
            try {
	
                pendingIntent.send ( mActivity, 0, intent );
            } catch ( CanceledException e ) {
	
                MoaiLog.e ( "MoaiGoogleBillingPurchaseObserver startBuyPageActivity: error sending intent to activity", e );
            }
        }
    }
}

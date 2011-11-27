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

import android.app.Activity;
import android.app.PendingIntent;
import android.app.PendingIntent.CanceledException;
import android.content.Intent;
import android.content.IntentSender;
import android.os.Handler;
import android.util.Log;

import java.lang.reflect.Method;

public abstract class MoaiBillingPurchaseObserver {
    private static final String TAG = "MoaiBillingPurchaseObserver";
    private final Activity mActivity;
    private final Handler mHandler;
    private Method mStartIntentSender;
    private Object[] mStartIntentSenderArgs = new Object[5];
    private static final Class[] START_INTENT_SENDER_SIG = new Class[] {
        IntentSender.class, Intent.class, int.class, int.class, int.class
    };
    

    public MoaiBillingPurchaseObserver ( Activity activity, Handler handler ) {
        mActivity = activity;
        mHandler = handler;
        initCompatibilityLayer();
    }

    public abstract void onBillingSupported ( boolean supported );

    public abstract void onPurchaseStateChange ( PurchaseState purchaseState, String itemId, String orderId, String notificationId, String developerPayload );

    public abstract void onRequestPurchaseResponse ( RequestPurchase request, ResponseCode responseCode );

    public abstract void onRestoreTransactionsResponse ( RestoreTransactions request, ResponseCode responseCode );

    private void initCompatibilityLayer () {
        try {
            mStartIntentSender = mActivity.getClass ().getMethod ( "startIntentSender", START_INTENT_SENDER_SIG );
        } catch ( SecurityException e ) {
            mStartIntentSender = null;
        } catch ( NoSuchMethodException e ) {
            mStartIntentSender = null;
        }
    }

    void startBuyPageActivity ( PendingIntent pendingIntent, Intent intent ) {
        if ( mStartIntentSender != null ) {
			// Android 2.0+
            try {
                mStartIntentSenderArgs[0] = pendingIntent.getIntentSender ();
                mStartIntentSenderArgs[1] = intent;
                mStartIntentSenderArgs[2] = Integer.valueOf(0);
                mStartIntentSenderArgs[3] = Integer.valueOf(0);
                mStartIntentSenderArgs[4] = Integer.valueOf(0);
                mStartIntentSender.invoke ( mActivity, mStartIntentSenderArgs );
            } catch ( Exception e ) {
                Log.e ( TAG, "error starting activity", e );
            }
        } else {
			// Android 1.6
            try {
                pendingIntent.send ( mActivity, 0, intent );
            } catch ( CanceledException e ) {
                Log.e ( TAG, "error starting activity", e );
            }
        }
    }
}

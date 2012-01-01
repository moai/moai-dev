//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import @PACKAGE@.MoaiBillingConstants.ResponseCode;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

//================================================================//
// MoaiBillingReceiver
//================================================================//
public class MoaiBillingReceiver extends BroadcastReceiver {
    private static final String TAG = "MoaiBillingReceiver";

    @Override
    public void onReceive ( Context context, Intent intent ) {
        String action = intent.getAction();
        if ( MoaiBillingConstants.ACTION_PURCHASE_STATE_CHANGED.equals ( action )) {
            String signedData = intent.getStringExtra ( MoaiBillingConstants.INAPP_SIGNED_DATA );
            String signature = intent.getStringExtra ( MoaiBillingConstants.INAPP_SIGNATURE );
            purchaseStateChanged ( context, signedData, signature );
        } else if ( MoaiBillingConstants.ACTION_NOTIFY.equals ( action )) {
            String notifyId = intent.getStringExtra ( MoaiBillingConstants.NOTIFICATION_ID );
            notify ( context, notifyId );
        } else if ( MoaiBillingConstants.ACTION_RESPONSE_CODE.equals ( action )) {
            long requestId = intent.getLongExtra ( MoaiBillingConstants.INAPP_REQUEST_ID, -1 );
            int responseCodeIndex = intent.getIntExtra ( MoaiBillingConstants.INAPP_RESPONSE_CODE, ResponseCode.RESULT_ERROR.ordinal ());
            checkResponseCode ( context, requestId, responseCodeIndex );
        } else {
            Log.w ( TAG, "unexpected action: " + action );
        }
    }

    private void purchaseStateChanged ( Context context, String signedData, String signature ) {
        Intent intent = new Intent ( MoaiBillingConstants.ACTION_PURCHASE_STATE_CHANGED );
        intent.setClass ( context, MoaiBillingService.class );
        intent.putExtra ( MoaiBillingConstants.INAPP_SIGNED_DATA, signedData );
        intent.putExtra ( MoaiBillingConstants.INAPP_SIGNATURE, signature );
        context.startService ( intent );
    }

    private void notify ( Context context, String notifyId ) {
        Intent intent = new Intent ( MoaiBillingConstants.ACTION_GET_PURCHASE_INFORMATION );
        intent.setClass ( context, MoaiBillingService.class );
        intent.putExtra ( MoaiBillingConstants.NOTIFICATION_ID, notifyId );
        context.startService ( intent);
    }

    private void checkResponseCode ( Context context, long requestId, int responseCodeIndex ) {
        Intent intent = new Intent ( MoaiBillingConstants.ACTION_RESPONSE_CODE );
        intent.setClass ( context, MoaiBillingService.class );
        intent.putExtra ( MoaiBillingConstants.INAPP_REQUEST_ID, requestId );
        intent.putExtra ( MoaiBillingConstants.INAPP_RESPONSE_CODE, responseCodeIndex );
        context.startService ( intent );
    }
}

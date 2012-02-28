//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import @PACKAGE@.AndroidMarketBillingConstants.ResponseCode;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

// Moai
import com.ziplinegames.moai.*;

//================================================================//
// AndroidMarketBillingReceiver
//================================================================//
public class AndroidMarketBillingReceiver extends BroadcastReceiver {

    @Override
    public void onReceive ( Context context, Intent intent ) {
	
        String action = intent.getAction ();
        if ( AndroidMarketBillingConstants.ACTION_PURCHASE_STATE_CHANGED.equals ( action )) {
	
            String signedData = intent.getStringExtra ( AndroidMarketBillingConstants.INAPP_SIGNED_DATA );
            String signature = intent.getStringExtra ( AndroidMarketBillingConstants.INAPP_SIGNATURE );
            purchaseStateChanged ( context, signedData, signature );
        } else if ( AndroidMarketBillingConstants.ACTION_NOTIFY.equals ( action )) {
	
            String notifyId = intent.getStringExtra ( AndroidMarketBillingConstants.NOTIFICATION_ID );
            notify ( context, notifyId );
        } else if ( AndroidMarketBillingConstants.ACTION_RESPONSE_CODE.equals ( action )) {
	
            long requestId = intent.getLongExtra ( AndroidMarketBillingConstants.INAPP_REQUEST_ID, -1 );
            int responseCodeIndex = intent.getIntExtra ( AndroidMarketBillingConstants.INAPP_RESPONSE_CODE, ResponseCode.RESULT_ERROR.ordinal ());
            checkResponseCode ( context, requestId, responseCodeIndex );
        } else {
	
            MoaiLog.w ( "AndroidMarketBillingReceiver onReceive: unexpected action ( " + action + " )" );
        }
    }

    private void purchaseStateChanged ( Context context, String signedData, String signature ) {
	
        Intent intent = new Intent ( AndroidMarketBillingConstants.ACTION_PURCHASE_STATE_CHANGED );
        intent.setClass ( context, AndroidMarketBillingService.class );
        intent.putExtra ( AndroidMarketBillingConstants.INAPP_SIGNED_DATA, signedData );
        intent.putExtra ( AndroidMarketBillingConstants.INAPP_SIGNATURE, signature );
        context.startService ( intent );
    }

    private void notify ( Context context, String notifyId ) {
	
        Intent intent = new Intent ( AndroidMarketBillingConstants.ACTION_GET_PURCHASE_INFORMATION );
        intent.setClass ( context, AndroidMarketBillingService.class );
        intent.putExtra ( AndroidMarketBillingConstants.NOTIFICATION_ID, notifyId );
        context.startService ( intent );
    }

    private void checkResponseCode ( Context context, long requestId, int responseCodeIndex ) {
	
        Intent intent = new Intent ( AndroidMarketBillingConstants.ACTION_RESPONSE_CODE );
        intent.setClass ( context, AndroidMarketBillingService.class );
        intent.putExtra ( AndroidMarketBillingConstants.INAPP_REQUEST_ID, requestId );
        intent.putExtra ( AndroidMarketBillingConstants.INAPP_RESPONSE_CODE, responseCodeIndex );
        context.startService ( intent );
    }
}

//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

//================================================================//
// MoaiGoogleBillingReceiver
//================================================================//
public class MoaiGoogleBillingReceiver extends BroadcastReceiver {

	//----------------------------------------------------------------//
    @Override
    public void onReceive ( Context context, Intent intent ) {
	
		if ( Moai.getApplicationState () == Moai.ApplicationState.APPLICATION_UNINITIALIZED ) {
			
			// If the application was not started as the result of a LAUNCHER action,
			// then AKU has not been initialized, libmoai.so has not been loaded and
			// no Lua scripts have been run, so we ignore the event. Important events
			// will be resent by Android Market if not confirmed.
			return;
		}
		
        String action = intent.getAction ();
        if ( MoaiGoogleBillingConstants.ACTION_PURCHASE_STATE_CHANGED.equals ( action )) {
	
            String signedData = intent.getStringExtra ( MoaiGoogleBillingConstants.INAPP_SIGNED_DATA );
            String signature = intent.getStringExtra ( MoaiGoogleBillingConstants.INAPP_SIGNATURE );
            purchaseStateChanged ( context, signedData, signature );
        } else if ( MoaiGoogleBillingConstants.ACTION_NOTIFY.equals ( action )) {
	
            String notifyId = intent.getStringExtra ( MoaiGoogleBillingConstants.NOTIFICATION_ID );
            notify ( context, notifyId );
        } else if ( MoaiGoogleBillingConstants.ACTION_RESPONSE_CODE.equals ( action )) {
	
            long requestId = intent.getLongExtra ( MoaiGoogleBillingConstants.INAPP_REQUEST_ID, -1 );
            int responseCodeIndex = intent.getIntExtra ( MoaiGoogleBillingConstants.INAPP_RESPONSE_CODE, MoaiGoogleBillingConstants.ResponseCode.RESULT_ERROR.ordinal ());
            checkResponseCode ( context, requestId, responseCodeIndex );
        } else {
	
            MoaiLog.w ( "MoaiGoogleBillingReceiver onReceive: unexpected action ( " + action + " )" );
        }
    }

	//----------------------------------------------------------------//
    private void purchaseStateChanged ( Context context, String signedData, String signature ) {
	
        Intent intent = new Intent ( MoaiGoogleBillingConstants.ACTION_PURCHASE_STATE_CHANGED );
        intent.setClass ( context, MoaiGoogleBillingService.class );
        intent.putExtra ( MoaiGoogleBillingConstants.INAPP_SIGNED_DATA, signedData );
        intent.putExtra ( MoaiGoogleBillingConstants.INAPP_SIGNATURE, signature );
        context.startService ( intent );
    }

	//----------------------------------------------------------------//
    private void notify ( Context context, String notifyId ) {
	
        Intent intent = new Intent ( MoaiGoogleBillingConstants.ACTION_NOTIFY );
        intent.setClass ( context, MoaiGoogleBillingService.class );
        intent.putExtra ( MoaiGoogleBillingConstants.NOTIFICATION_ID, notifyId );
        context.startService ( intent );
    }

	//----------------------------------------------------------------//
    private void checkResponseCode ( Context context, long requestId, int responseCodeIndex ) {
	
        Intent intent = new Intent ( MoaiGoogleBillingConstants.ACTION_RESPONSE_CODE );
        intent.setClass ( context, MoaiGoogleBillingService.class );
        intent.putExtra ( MoaiGoogleBillingConstants.INAPP_REQUEST_ID, requestId );
        intent.putExtra ( MoaiGoogleBillingConstants.INAPP_RESPONSE_CODE, responseCodeIndex );
        context.startService ( intent );
    }
}
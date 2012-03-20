//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import com.android.vending.billing.IMarketBillingService;

import android.app.PendingIntent;
import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;

//================================================================//
// MoaiGoogleBillingService
//================================================================//
public class MoaiGoogleBillingService extends Service implements ServiceConnection {
	
    private static IMarketBillingService 			mService = null;
    private static LinkedList < BillingRequest > 	mPendingRequests = new LinkedList < BillingRequest > ();
    private static HashMap < Long, BillingRequest >	mSentRequests = new HashMap < Long, BillingRequest > ();

	//================================================================//
	// BillingRequest
	//================================================================//
    abstract class BillingRequest {
	
        private final int 	mStartId;
        protected long 		mRequestId = -1;

		//----------------------------------------------------------------//
        public BillingRequest ( int startId ) {
	
            mStartId = startId;
        }

		//----------------------------------------------------------------//
        public int getStartId () {
	
            return mStartId;
        }

		//----------------------------------------------------------------//
        public boolean runRequest () {
	
            if ( runIfConnected ()) {
	
                return true;
            }

            mPendingRequests.add ( this );
            if ( bindToMarketBillingService ()) {
	
                return true;
            }
			
            mPendingRequests.remove ( this );
            return false;
        }

		//----------------------------------------------------------------//
        public boolean runIfConnected () {
	
            if ( mService != null ) {
	
                try {
	
                    mRequestId = run ();
                    if ( mRequestId >= 0 ) {
	
                        mSentRequests.put ( mRequestId, this );
                    }

                    return true;
                } catch ( RemoteException e ) {
	
                    onRemoteException ( e );
            	} catch ( NullPointerException e ) {
	
                	onNullPointerException ( e );
            	}
            }

            return false;
        }

		//----------------------------------------------------------------//
        protected void onRemoteException ( RemoteException e ) {

            MoaiLog.w ( "MoaiGoogleBillingService onRemoteException: remote billing service crashed" );
            mService = null;

			e.printStackTrace ();
        }

		//----------------------------------------------------------------//
        protected void onNullPointerException ( NullPointerException e ) {
	
            MoaiLog.w ( "MoaiGoogleBillingService onNullPointerException: remote billing service uninitialized" );
            mService = null;

			e.printStackTrace ();
        }

		//----------------------------------------------------------------//
        abstract protected long run () throws RemoteException, NullPointerException;

		//----------------------------------------------------------------//
        protected void responseCodeReceived ( MoaiGoogleBillingConstants.ResponseCode responseCode ) {
	
        }

		//----------------------------------------------------------------//
        protected Bundle makeRequestBundle ( String method ) {
	
            Bundle request = new Bundle ();
            request.putString ( MoaiGoogleBillingConstants.BILLING_REQUEST_METHOD, method );
            request.putInt ( MoaiGoogleBillingConstants.BILLING_REQUEST_API_VERSION, 1 );
            request.putString ( MoaiGoogleBillingConstants.BILLING_REQUEST_PACKAGE_NAME, getPackageName ());
            return request;
        }
    }

	//================================================================//
	// CheckBillingSupported
	//================================================================//
    class CheckBillingSupported extends BillingRequest {
	
		//----------------------------------------------------------------//
        public CheckBillingSupported () {
	
            super (( int )MoaiGoogleBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }

		//----------------------------------------------------------------//
        @Override
        protected long run () throws RemoteException, NullPointerException {
	
            Bundle request = makeRequestBundle ( "CHECK_BILLING_SUPPORTED" );
            Bundle response = mService.sendBillingRequest ( request );

		 	int responseCode = MoaiGoogleBillingConstants.ResponseCode.RESULT_ERROR.ordinal ();
		    if ( response.containsKey ( MoaiGoogleBillingConstants.BILLING_RESPONSE_RESPONSE_CODE )) {
		    	responseCode = response.getInt ( MoaiGoogleBillingConstants.BILLING_RESPONSE_RESPONSE_CODE );
		    }

            boolean billingSupported = ( responseCode == MoaiGoogleBillingConstants.ResponseCode.RESULT_OK.ordinal ());
            MoaiGoogleBillingResponseHandler.checkBillingSupportedResponse ( billingSupported );
            return MoaiGoogleBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID;
        }
    }

	//================================================================//
	// RequestPurchase
	//================================================================//
    class RequestPurchase extends BillingRequest {
	
        public final String mProductId;
        public final String mDeveloperPayload;
    
		//----------------------------------------------------------------//
        public RequestPurchase ( String itemId ) {
	
            this ( itemId, null );
        }
    
		//----------------------------------------------------------------//
        public RequestPurchase ( String itemId, String developerPayload ) {
	
            super (( int )MoaiGoogleBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );

            mProductId = itemId;
            mDeveloperPayload = developerPayload;
        }
    
		//----------------------------------------------------------------//
        @Override
        protected long run () throws RemoteException, NullPointerException {
	
            Bundle request = makeRequestBundle ( "REQUEST_PURCHASE" );
            request.putString ( MoaiGoogleBillingConstants.BILLING_REQUEST_ITEM_ID, mProductId );
            if ( mDeveloperPayload != null ) {
	
                request.putString ( MoaiGoogleBillingConstants.BILLING_REQUEST_DEVELOPER_PAYLOAD, mDeveloperPayload );
            }

            Bundle response = mService.sendBillingRequest ( request );

            PendingIntent pendingIntent = response.getParcelable ( MoaiGoogleBillingConstants.BILLING_RESPONSE_PURCHASE_INTENT );
            if ( pendingIntent == null ) {
	
                return MoaiGoogleBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID;
            }
    
            Intent intent = new Intent ();
            MoaiGoogleBillingResponseHandler.buyPageIntentResponse ( pendingIntent, intent );
            return response.getLong ( MoaiGoogleBillingConstants.BILLING_RESPONSE_REQUEST_ID, MoaiGoogleBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    
 		//----------------------------------------------------------------//
       @Override
        protected void responseCodeReceived ( MoaiGoogleBillingConstants.ResponseCode responseCode ) {
	
            MoaiGoogleBillingResponseHandler.responseCodeReceived ( this, responseCode );
        }
    }

	//================================================================//
	// ConfirmNotifications
	//================================================================//
    class ConfirmNotifications extends BillingRequest {
	
        final String [] mNotifyIds;

		//----------------------------------------------------------------//
        public ConfirmNotifications ( String [] notifyIds ) {
	
            super (( int )MoaiGoogleBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );

            mNotifyIds = notifyIds;
        }
    
		//----------------------------------------------------------------//
        @Override
        protected long run () throws RemoteException, NullPointerException {
	
            Bundle request = makeRequestBundle ( "CONFIRM_NOTIFICATIONS" );
            request.putStringArray ( MoaiGoogleBillingConstants.BILLING_REQUEST_NOTIFY_IDS, mNotifyIds );

            Bundle response = mService.sendBillingRequest ( request );
            return response.getLong ( MoaiGoogleBillingConstants.BILLING_RESPONSE_REQUEST_ID, MoaiGoogleBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    }

	//================================================================//
	// GetPurchaseInformation
	//================================================================//
    class GetPurchaseInformation extends BillingRequest {
	
        long 			mNonce = -1;
        final String []	mNotifyIds;
    
		//----------------------------------------------------------------//
        public GetPurchaseInformation ( int startId, String [] notifyIds ) {
	
            super ( startId );

            mNotifyIds = notifyIds;
        }
    
		//----------------------------------------------------------------//
        @Override
        protected long run () throws RemoteException, NullPointerException {
	
            mNonce = MoaiGoogleBillingSecurity.generateNonce ();
    
            Bundle request = makeRequestBundle ( "GET_PURCHASE_INFORMATION" );
            request.putLong ( MoaiGoogleBillingConstants.BILLING_REQUEST_NONCE, mNonce );
            request.putStringArray ( MoaiGoogleBillingConstants.BILLING_REQUEST_NOTIFY_IDS, mNotifyIds );

            Bundle response = mService.sendBillingRequest ( request );
            return response.getLong ( MoaiGoogleBillingConstants.BILLING_RESPONSE_REQUEST_ID, MoaiGoogleBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    
		//----------------------------------------------------------------//
        @Override
        protected void onRemoteException ( RemoteException e ) {
	
            super.onRemoteException ( e );

            MoaiGoogleBillingSecurity.removeNonce ( mNonce );
        }

		//----------------------------------------------------------------//
        @Override
        protected void onNullPointerException ( NullPointerException e ) {
	
            super.onNullPointerException ( e );

            MoaiGoogleBillingSecurity.removeNonce ( mNonce );
        }
    }

	//================================================================//
	// RestoreTransactions
	//================================================================//
    class RestoreTransactions extends BillingRequest {
	
        long mNonce = -1;
    
		//----------------------------------------------------------------//
        public RestoreTransactions () {
	
            super (( int )MoaiGoogleBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    
		//----------------------------------------------------------------//
        @Override
        protected long run () throws RemoteException, NullPointerException {
	
            mNonce = MoaiGoogleBillingSecurity.generateNonce ();
    
            Bundle request = makeRequestBundle ( "RESTORE_TRANSACTIONS" );
            request.putLong ( MoaiGoogleBillingConstants.BILLING_REQUEST_NONCE, mNonce );

            Bundle response = mService.sendBillingRequest ( request );
            return response.getLong ( MoaiGoogleBillingConstants.BILLING_RESPONSE_REQUEST_ID, MoaiGoogleBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    
		//----------------------------------------------------------------//
        @Override
        protected void onRemoteException ( RemoteException e ) {
	
            super.onRemoteException ( e );

            MoaiGoogleBillingSecurity.removeNonce ( mNonce );
        }

		//----------------------------------------------------------------//
        @Override
        protected void onNullPointerException ( NullPointerException e ) {
	
            super.onNullPointerException ( e );

            MoaiGoogleBillingSecurity.removeNonce ( mNonce );
        }
    
		//----------------------------------------------------------------//
        @Override
        protected void responseCodeReceived ( MoaiGoogleBillingConstants.ResponseCode responseCode ) {
	
            MoaiGoogleBillingResponseHandler.responseCodeReceived ( this, responseCode );
        }
    }

	//----------------------------------------------------------------//
    public MoaiGoogleBillingService () {
	
        super ();
    }

	//----------------------------------------------------------------//
    public void setContext ( Context context ) {
	
        attachBaseContext ( context );
    }

	//----------------------------------------------------------------//
    @Override
    public IBinder onBind ( Intent intent ) {
	
        return null;
    }

	//----------------------------------------------------------------//
    @Override
   	public int onStartCommand ( Intent intent, int flags, int startId ) {
	
		if ( intent != null ) {
			
			handleCommand ( intent, startId );
		}
		
		return START_STICKY;
	}

	//----------------------------------------------------------------//
    public void handleCommand ( Intent intent, int startId ) {
	
        String action = intent.getAction ();
        if ( MoaiGoogleBillingConstants.ACTION_NOTIFY.equals ( action )) {
	
            String notifyId = intent.getStringExtra ( MoaiGoogleBillingConstants.NOTIFICATION_ID );
            getPurchaseInformation ( startId, new String [] { notifyId } );
        } else if ( MoaiGoogleBillingConstants.ACTION_PURCHASE_STATE_CHANGED.equals ( action )) {
	
            String signedData = intent.getStringExtra ( MoaiGoogleBillingConstants.INAPP_SIGNED_DATA );
            String signature = intent.getStringExtra ( MoaiGoogleBillingConstants.INAPP_SIGNATURE );
            purchaseStateChanged ( startId, signedData, signature );
        } else if ( MoaiGoogleBillingConstants.ACTION_RESPONSE_CODE.equals ( action )) {
	
            long requestId = intent.getLongExtra ( MoaiGoogleBillingConstants.INAPP_REQUEST_ID, -1 );
            int responseCodeIndex = intent.getIntExtra ( MoaiGoogleBillingConstants.INAPP_RESPONSE_CODE, MoaiGoogleBillingConstants.ResponseCode.RESULT_ERROR.ordinal ());
            MoaiGoogleBillingConstants.ResponseCode responseCode = MoaiGoogleBillingConstants.ResponseCode.valueOf ( responseCodeIndex );
            checkResponseCode ( requestId, responseCode );
        }
    }

	//----------------------------------------------------------------//
    private boolean bindToMarketBillingService () {
	
        try {
	
            MoaiLog.i ( "MarketBillingService bindToMarketBillingService: binding to Market billing service" );

            boolean bindResult = bindService ( new Intent ( MoaiGoogleBillingConstants.MARKET_BILLING_SERVICE_ACTION ), this, Context.BIND_AUTO_CREATE );
            if ( bindResult ) {
	
                return true;
            } else {
	
                MoaiLog.e ( "MarketBillingService bindToMarketBillingService: Could not bind to service" );
            }
        } catch ( SecurityException e ) {
	
            MoaiLog.e ( "MarketBillingService bindToMarketBillingService: Security exception", e );
        }

        return false;
    }

	//----------------------------------------------------------------//
    public boolean checkBillingSupported () {
	
        return new CheckBillingSupported ().runRequest ();
    }

	//----------------------------------------------------------------//
    public boolean requestPurchase ( String productId, String developerPayload ) {
	
        return new RequestPurchase ( productId, developerPayload ).runRequest ();
    }

	//----------------------------------------------------------------//
    public boolean restoreTransactions () {
	
        return new RestoreTransactions().runRequest();
    }

	//----------------------------------------------------------------//
    public boolean confirmNotifications ( String [] notifyIds ) {
	
        return new ConfirmNotifications ( notifyIds ).runRequest ();
    }

	//----------------------------------------------------------------//
    private boolean getPurchaseInformation ( int startId, String [] notifyIds ) {
	
    	return new GetPurchaseInformation ( startId, notifyIds ).runRequest ();
    }

	//----------------------------------------------------------------//
    private void purchaseStateChanged ( int startId, String signedData, String signature ) {
	
        ArrayList < MoaiGoogleBillingSecurity.VerifiedPurchase > purchases = MoaiGoogleBillingSecurity.verifyPurchase ( signedData, signature );
        if ( purchases == null ) {
	
            return;
        }
    
        for ( MoaiGoogleBillingSecurity.VerifiedPurchase vp : purchases ) {
	
            MoaiGoogleBillingResponseHandler.purchaseResponse ( vp.purchaseState, vp.productId, vp.orderId, vp.notificationId, vp.developerPayload );
        }
    }

	//----------------------------------------------------------------//
    private void checkResponseCode ( long requestId, MoaiGoogleBillingConstants.ResponseCode responseCode ) {
	
        BillingRequest request = mSentRequests.get ( requestId );
        if ( request != null ) {
	
            request.responseCodeReceived ( responseCode );
        }

        mSentRequests.remove ( requestId );
    }

	//----------------------------------------------------------------//
    private void runPendingRequests () {
	
        int maxStartId = -1;
        BillingRequest request = null;
        while (( request = mPendingRequests.peek ()) != null) {
	
            if ( request.runIfConnected ()) {
	
                mPendingRequests.remove ();

                if ( maxStartId < request.getStartId () ) {
	
                    maxStartId = request.getStartId ();
                }
            } else {
	
                bindToMarketBillingService ();
                return;
            }
        }

        if ( maxStartId >= 0 ) {
	
            stopSelf ( maxStartId );
        }
    }

	//----------------------------------------------------------------//
    public void onServiceConnected ( ComponentName name, IBinder service ) {
	
        MoaiLog.d ( "MoaiGoogleBillingService onServiceConnected: Billing service connected" );

        mService = IMarketBillingService.Stub.asInterface ( service );

        runPendingRequests ();
    }

	//----------------------------------------------------------------//
    public void onServiceDisconnected ( ComponentName name ) {
	
        MoaiLog.w ( "MoaiGoogleBillingService onServiceDisconnected: Billing service disconnected" );

        mService = null;
    }

	//----------------------------------------------------------------//
    public void unbind () {
	
        try {
	
            unbindService ( this );
        } catch ( IllegalArgumentException e ) {
	
            // This might happen if the service was disconnected.
        }
    }
}

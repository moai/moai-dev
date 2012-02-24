//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import com.android.vending.billing.IMarketBillingService;

import @PACKAGE@.AndroidMarketBillingConstants.PurchaseState;
import @PACKAGE@.AndroidMarketBillingConstants.ResponseCode;
import @PACKAGE@.AndroidMarketBillingSecurity.VerifiedPurchase;

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
// AndroidMarketBillingService
//================================================================//
public class AndroidMarketBillingService extends Service implements ServiceConnection {
	
    private static IMarketBillingService 				mService;
    private static LinkedList < BillingRequest > 		mPendingRequests 	= new LinkedList < BillingRequest > ();
    private static HashMap < Long, BillingRequest > 	mSentRequests 		= new HashMap < Long, BillingRequest > ();

    abstract class BillingRequest {
	
        private final int 	mStartId;
        protected long 		mRequestId;

        public BillingRequest ( int startId ) {
	
            mStartId = startId;
        }

        public int getStartId () {
	
            return mStartId;
        }

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

        protected void onRemoteException ( RemoteException e ) {
	
            MoaiLog.w ( "AndroidMarketBillingService onRemoteException: remote billing service crashed" );
            mService = null;
        }

        protected void onNullPointerException ( NullPointerException e ) {
	
            MoaiLog.w ( "AndroidMarketBillingService onNullPointerException: remote billing service uninitialized" );
            mService = null;
        }

        abstract protected long run () throws RemoteException, NullPointerException;

        protected void responseCodeReceived ( ResponseCode responseCode ) {
	
        }

        protected Bundle makeRequestBundle ( String method ) {
	
            Bundle request = new Bundle ();
            request.putString ( AndroidMarketBillingConstants.BILLING_REQUEST_METHOD, method );
            request.putInt ( AndroidMarketBillingConstants.BILLING_REQUEST_API_VERSION, 1 );
            request.putString ( AndroidMarketBillingConstants.BILLING_REQUEST_PACKAGE_NAME, getPackageName ());
            return request;
        }
    }

    class CheckBillingSupported extends BillingRequest {
	
        public CheckBillingSupported () {
	
            super (( int )AndroidMarketBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }

        @Override
        protected long run () throws RemoteException, NullPointerException {
	
            Bundle request = makeRequestBundle ( "CHECK_BILLING_SUPPORTED" );
            Bundle response = mService.sendBillingRequest ( request );

		 	int responseCode = ResponseCode.RESULT_ERROR.ordinal ();
		    if ( response.containsKey ( AndroidMarketBillingConstants.BILLING_RESPONSE_RESPONSE_CODE )) {
		    	responseCode = response.getInt ( AndroidMarketBillingConstants.BILLING_RESPONSE_RESPONSE_CODE );
		    }

            boolean billingSupported = ( responseCode == ResponseCode.RESULT_OK.ordinal ());
            AndroidMarketBillingResponseHandler.checkBillingSupportedResponse ( billingSupported );
            return AndroidMarketBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID;
        }
    }

    class RequestPurchase extends BillingRequest {
	
        public final String mProductId;
        public final String mDeveloperPayload;
    
        public RequestPurchase ( String itemId ) {
	
            this ( itemId, null );
        }
    
        public RequestPurchase ( String itemId, String developerPayload ) {
	
            super (( int )AndroidMarketBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
            mProductId = itemId;
            mDeveloperPayload = developerPayload;
        }
    
        @Override
        protected long run () throws RemoteException, NullPointerException {
	
            Bundle request = makeRequestBundle ( "REQUEST_PURCHASE" );
            request.putString ( AndroidMarketBillingConstants.BILLING_REQUEST_ITEM_ID, mProductId );
            if ( mDeveloperPayload != null ) {
	
                request.putString ( AndroidMarketBillingConstants.BILLING_REQUEST_DEVELOPER_PAYLOAD, mDeveloperPayload );
            }

            Bundle response = mService.sendBillingRequest ( request );

            PendingIntent pendingIntent = response.getParcelable ( AndroidMarketBillingConstants.BILLING_RESPONSE_PURCHASE_INTENT );
            if ( pendingIntent == null ) {
	
                return AndroidMarketBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID;
            }
    
            Intent intent = new Intent ();
            AndroidMarketBillingResponseHandler.buyPageIntentResponse ( pendingIntent, intent );
            return response.getLong ( AndroidMarketBillingConstants.BILLING_RESPONSE_REQUEST_ID, AndroidMarketBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    
        @Override
        protected void responseCodeReceived ( ResponseCode responseCode ) {
	
            AndroidMarketBillingResponseHandler.responseCodeReceived ( this, responseCode );
        }
    }

    class ConfirmNotifications extends BillingRequest {
	
        final String [] mNotifyIds;

        public ConfirmNotifications ( String[] notifyIds ) {
            super (( int )AndroidMarketBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
            mNotifyIds = notifyIds;
        }
    
        @Override
        protected long run () throws RemoteException, NullPointerException {
	
            Bundle request = makeRequestBundle ( "CONFIRM_NOTIFICATIONS" );
            request.putStringArray ( AndroidMarketBillingConstants.BILLING_REQUEST_NOTIFY_IDS, mNotifyIds );

            Bundle response = mService.sendBillingRequest ( request );
            return response.getLong ( AndroidMarketBillingConstants.BILLING_RESPONSE_REQUEST_ID, AndroidMarketBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    }

    class GetPurchaseInformation extends BillingRequest {
	
        long 			mNonce;
        final String[] 	mNotifyIds;
    
        public GetPurchaseInformation ( int startId, String[] notifyIds ) {
	
            super ( startId );
            mNotifyIds = notifyIds;
        }
    
        @Override
        protected long run () throws RemoteException, NullPointerException {
	
            mNonce = AndroidMarketBillingSecurity.generateNonce ();
    
            Bundle request = makeRequestBundle ( "GET_PURCHASE_INFORMATION" );
            request.putLong ( AndroidMarketBillingConstants.BILLING_REQUEST_NONCE, mNonce );
            request.putStringArray ( AndroidMarketBillingConstants.BILLING_REQUEST_NOTIFY_IDS, mNotifyIds );

            Bundle response = mService.sendBillingRequest ( request );
            return response.getLong ( AndroidMarketBillingConstants.BILLING_RESPONSE_REQUEST_ID, AndroidMarketBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    
        @Override
        protected void onRemoteException ( RemoteException e ) {
	
            super.onRemoteException ( e );
            AndroidMarketBillingSecurity.removeNonce ( mNonce );
        }

        @Override
        protected void onNullPointerException ( NullPointerException e ) {
	
            super.onNullPointerException ( e );
            AndroidMarketBillingSecurity.removeNonce ( mNonce );
        }
    }

    class RestoreTransactions extends BillingRequest {
	
        long mNonce;
    
        public RestoreTransactions () {
	
            super (( int )AndroidMarketBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    
        @Override
        protected long run () throws RemoteException, NullPointerException {
	
            mNonce = AndroidMarketBillingSecurity.generateNonce ();
    
            Bundle request = makeRequestBundle ( "RESTORE_TRANSACTIONS" );
            request.putLong ( AndroidMarketBillingConstants.BILLING_REQUEST_NONCE, mNonce );

            Bundle response = mService.sendBillingRequest ( request );
            return response.getLong ( AndroidMarketBillingConstants.BILLING_RESPONSE_REQUEST_ID, AndroidMarketBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    
        @Override
        protected void onRemoteException ( RemoteException e ) {
	
            super.onRemoteException ( e );
            AndroidMarketBillingSecurity.removeNonce ( mNonce );
        }

        @Override
        protected void onNullPointerException ( NullPointerException e ) {
	
            super.onNullPointerException ( e );
            AndroidMarketBillingSecurity.removeNonce ( mNonce );
        }
    
        @Override
        protected void responseCodeReceived ( ResponseCode responseCode ) {
	
            AndroidMarketBillingResponseHandler.responseCodeReceived ( this, responseCode );
        }
    }

    public AndroidMarketBillingService () {
	
        super ();
    }

    public void setContext ( Context context ) {
	
        attachBaseContext ( context );
    }

    @Override
    public IBinder onBind ( Intent intent ) {
	
        return null;
    }

    @Override
   	public int onStartCommand ( Intent intent, int flags, int startId ) {
	
		if ( intent != null ) handleCommand ( intent, startId );
		return START_STICKY;
	}

    public void handleCommand ( Intent intent, int startId ) {
	
        String action = intent.getAction ();
        if ( AndroidMarketBillingConstants.ACTION_GET_PURCHASE_INFORMATION.equals ( action )) {
	
            String notifyId = intent.getStringExtra ( AndroidMarketBillingConstants.NOTIFICATION_ID );
            getPurchaseInformation ( startId, new String [] { notifyId } );
        } else if ( AndroidMarketBillingConstants.ACTION_PURCHASE_STATE_CHANGED.equals ( action )) {
	
            String signedData = intent.getStringExtra ( AndroidMarketBillingConstants.INAPP_SIGNED_DATA );
            String signature = intent.getStringExtra ( AndroidMarketBillingConstants.INAPP_SIGNATURE );
            purchaseStateChanged ( startId, signedData, signature );
        } else if ( AndroidMarketBillingConstants.ACTION_RESPONSE_CODE.equals ( action )) {
	
            long requestId = intent.getLongExtra ( AndroidMarketBillingConstants.INAPP_REQUEST_ID, -1 );
            int responseCodeIndex = intent.getIntExtra ( AndroidMarketBillingConstants.INAPP_RESPONSE_CODE, ResponseCode.RESULT_ERROR.ordinal ());
            ResponseCode responseCode = ResponseCode.valueOf ( responseCodeIndex );
            checkResponseCode ( requestId, responseCode );
        }
    }

    private boolean bindToMarketBillingService() {
	
        try {
	
            MoaiLog.i ( "MarketBillingService bindToMarketBillingService: binding to Market billing service" );

            boolean bindResult = bindService ( new Intent ( AndroidMarketBillingConstants.MARKET_BILLING_SERVICE_ACTION ), this, Context.BIND_AUTO_CREATE );
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

    public boolean checkBillingSupported () {
	
        return new CheckBillingSupported ().runRequest ();
    }

    public boolean requestPurchase ( String productId, String developerPayload ) {
	
        return new RequestPurchase ( productId, developerPayload ).runRequest ();
    }

    public boolean restoreTransactions() {
	
        return new RestoreTransactions().runRequest();
    }

    public boolean confirmNotifications ( String [] notifyIds ) {
	
        return new ConfirmNotifications ( notifyIds ).runRequest ();
    }

    private boolean getPurchaseInformation ( int startId, String [] notifyIds ) {
	
    	return new GetPurchaseInformation ( startId, notifyIds ).runRequest ();
    }

    private void purchaseStateChanged ( int startId, String signedData, String signature ) {
	
        ArrayList < AndroidMarketBillingSecurity.VerifiedPurchase > purchases = AndroidMarketBillingSecurity.verifyPurchase ( signedData, signature );
        if ( purchases == null ) {
	
            return;
        }
    
        for ( VerifiedPurchase vp : purchases ) {
	
            AndroidMarketBillingResponseHandler.purchaseResponse ( vp.purchaseState, vp.productId, vp.orderId, vp.notificationId, vp.developerPayload );
        }
    }

    private void checkResponseCode ( long requestId, ResponseCode responseCode ) {
	
        BillingRequest request = mSentRequests.get ( requestId );
        if ( request != null ) {
	
            request.responseCodeReceived ( responseCode );
        }

        mSentRequests.remove ( requestId );
    }

    private void runPendingRequests () {
	
        int maxStartId = -1;
        BillingRequest request;
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

    public void onServiceConnected ( ComponentName name, IBinder service ) {
	
        MoaiLog.d ( "AndroidMarketBillingService onServiceConnected: Billing service connected" );

        mService = IMarketBillingService.Stub.asInterface ( service );
        runPendingRequests ();
    }

    public void onServiceDisconnected ( ComponentName name ) {
	
        MoaiLog.w ( "AndroidMarketBillingService onServiceDisconnected: Billing service disconnected" );
        mService = null;
    }

    public void unbind () {
	
        try {
	
            unbindService ( this );
        } catch ( IllegalArgumentException e ) {
	
            // This might happen if the service was disconnected
        }
    }
}

//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import com.android.vending.billing.IMarketBillingService;

import @PACKAGE@.MoaiBillingConstants.PurchaseState;
import @PACKAGE@.MoaiBillingConstants.ResponseCode;
import @PACKAGE@.MoaiBillingSecurity.VerifiedPurchase;

import android.app.PendingIntent;
import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;

//================================================================//
// MoaiBillingService
//================================================================//
public class MoaiBillingService extends Service implements ServiceConnection {
    private static final String TAG = "MoaiBillingService";

    private static IMarketBillingService mService;
    private static LinkedList < BillingRequest > mPendingRequests = new LinkedList < BillingRequest >();
    private static HashMap < Long, BillingRequest > mSentRequests = new HashMap < Long, BillingRequest >();

    abstract class BillingRequest {
        private final int mStartId;
        protected long mRequestId;

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

            mPendingRequests.add(this);
            if ( bindToMarketBillingService ()) {
                return true;
            }
			
            mPendingRequests.remove(this);
            return false;
        }

        public boolean runIfConnected () {
            if ( MoaiBillingConstants.DEBUG ) {
                Log.d ( TAG, getClass ().getSimpleName ());
            }

            if ( mService != null ) {
                try {
                    mRequestId = run();
                    if ( MoaiBillingConstants.DEBUG ) {
                        Log.d ( TAG, "request id: " + mRequestId );
                    }
                    if ( mRequestId >= 0 ) {
                        mSentRequests.put ( mRequestId, this );
                    }
                    return true;
                } catch ( RemoteException e ) {
                    onRemoteException ( e );
                }
            }
            return false;
        }

        protected void onRemoteException ( RemoteException e ) {
            Log.w ( TAG, "remote billing service crashed" );
            mService = null;
        }

        abstract protected long run () throws RemoteException;

        protected void responseCodeReceived ( ResponseCode responseCode ) {
	
        }

        protected Bundle makeRequestBundle ( String method ) {
            Bundle request = new Bundle ();
            request.putString ( MoaiBillingConstants.BILLING_REQUEST_METHOD, method );
            request.putInt ( MoaiBillingConstants.BILLING_REQUEST_API_VERSION, 1 );
            request.putString ( MoaiBillingConstants.BILLING_REQUEST_PACKAGE_NAME, getPackageName ());
            return request;
        }

        protected void logResponseCode ( String method, Bundle response ) {
            ResponseCode responseCode = ResponseCode.valueOf ( response.getInt ( MoaiBillingConstants.BILLING_RESPONSE_RESPONSE_CODE ));
            if ( MoaiBillingConstants.DEBUG ) {
                Log.e ( TAG, method + " received " + responseCode.toString ());
            }
        }
    }

    class CheckBillingSupported extends BillingRequest {
        public CheckBillingSupported () {
            super (( int ) MoaiBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }

        @Override
        protected long run () throws RemoteException {
            Bundle request = makeRequestBundle ( "CHECK_BILLING_SUPPORTED" );
            Bundle response = mService.sendBillingRequest ( request );

		 	int responseCode = ResponseCode.RESULT_ERROR.ordinal();
		    if (response.containsKey(MoaiBillingConstants.BILLING_RESPONSE_RESPONSE_CODE)) {
		    	responseCode = response.getInt(MoaiBillingConstants.BILLING_RESPONSE_RESPONSE_CODE);
		    }

            if ( MoaiBillingConstants.DEBUG ) {
                Log.i ( TAG, "CheckBillingSupported response code: " + ResponseCode.valueOf ( responseCode ));
            }

            boolean billingSupported = (responseCode == ResponseCode.RESULT_OK.ordinal ());
            MoaiBillingResponseHandler.checkBillingSupportedResponse ( billingSupported );
            return MoaiBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID;
        }
    }

    class RequestPurchase extends BillingRequest {
        public final String mProductId;
        public final String mDeveloperPayload;
    
        public RequestPurchase ( String itemId ) {
            this ( itemId, null );
        }
    
        public RequestPurchase ( String itemId, String developerPayload ) {
            super (( int ) MoaiBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
            mProductId = itemId;
            mDeveloperPayload = developerPayload;
        }
    
        @Override
        protected long run () throws RemoteException {
            Bundle request = makeRequestBundle ( "REQUEST_PURCHASE" );
            request.putString ( MoaiBillingConstants.BILLING_REQUEST_ITEM_ID, mProductId );
            if ( mDeveloperPayload != null ) {
                request.putString ( MoaiBillingConstants.BILLING_REQUEST_DEVELOPER_PAYLOAD, mDeveloperPayload );
            }

            Bundle response = mService.sendBillingRequest ( request );

            PendingIntent pendingIntent = response.getParcelable ( MoaiBillingConstants.BILLING_RESPONSE_PURCHASE_INTENT );
            if ( pendingIntent == null ) {
                Log.e ( TAG, "Error with requestPurchase" );
                return MoaiBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID;
            }
    
            Intent intent = new Intent();
            MoaiBillingResponseHandler.buyPageIntentResponse ( pendingIntent, intent );
            return response.getLong ( MoaiBillingConstants.BILLING_RESPONSE_REQUEST_ID, MoaiBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    
        @Override
        protected void responseCodeReceived ( ResponseCode responseCode ) {
            MoaiBillingResponseHandler.responseCodeReceived ( this, responseCode );
        }
    }

    class ConfirmNotifications extends BillingRequest {
        final String[] mNotifyIds;

        public ConfirmNotifications ( String[] notifyIds ) {
            super (( int ) MoaiBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
            mNotifyIds = notifyIds;
        }
    
        @Override
        protected long run () throws RemoteException {
            Bundle request = makeRequestBundle ( "CONFIRM_NOTIFICATIONS" );
            request.putStringArray ( MoaiBillingConstants.BILLING_REQUEST_NOTIFY_IDS, mNotifyIds );

            Bundle response = mService.sendBillingRequest ( request );
            return response.getLong ( MoaiBillingConstants.BILLING_RESPONSE_REQUEST_ID, MoaiBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    }

    class GetPurchaseInformation extends BillingRequest {
        long mNonce;
        final String[] mNotifyIds;
    
        public GetPurchaseInformation ( int startId, String[] notifyIds ) {
            super ( startId );
            mNotifyIds = notifyIds;
        }
    
        @Override
        protected long run () throws RemoteException {
            mNonce = MoaiBillingSecurity.generateNonce ();
    
            Bundle request = makeRequestBundle ( "GET_PURCHASE_INFORMATION" );
            request.putLong ( MoaiBillingConstants.BILLING_REQUEST_NONCE, mNonce );
            request.putStringArray ( MoaiBillingConstants.BILLING_REQUEST_NOTIFY_IDS, mNotifyIds );

            Bundle response = mService.sendBillingRequest ( request );
            return response.getLong ( MoaiBillingConstants.BILLING_RESPONSE_REQUEST_ID, MoaiBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    
        @Override
        protected void onRemoteException ( RemoteException e ) {
            super.onRemoteException ( e );
            MoaiBillingSecurity.removeNonce ( mNonce );
        }
    }

    class RestoreTransactions extends BillingRequest {
        long mNonce;
    
        public RestoreTransactions () {
            super (( int ) MoaiBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    
        @Override
        protected long run () throws RemoteException {
            mNonce = MoaiBillingSecurity.generateNonce ();
    
            Bundle request = makeRequestBundle ( "RESTORE_TRANSACTIONS" );
            request.putLong ( MoaiBillingConstants.BILLING_REQUEST_NONCE, mNonce );

            Bundle response = mService.sendBillingRequest ( request );
            return response.getLong ( MoaiBillingConstants.BILLING_RESPONSE_REQUEST_ID, MoaiBillingConstants.BILLING_RESPONSE_INVALID_REQUEST_ID );
        }
    
        @Override
        protected void onRemoteException ( RemoteException e ) {
            super.onRemoteException ( e );
            MoaiBillingSecurity.removeNonce ( mNonce );
        }
    
        @Override
        protected void responseCodeReceived ( ResponseCode responseCode ) {
            MoaiBillingResponseHandler.responseCodeReceived ( this, responseCode );
        }
    }

    public MoaiBillingService () {
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
        if ( MoaiBillingConstants.DEBUG ) {
            Log.i ( TAG, "handleCommand() action: " + action ); 
        }

        if ( MoaiBillingConstants.ACTION_GET_PURCHASE_INFORMATION.equals ( action )) {
            String notifyId = intent.getStringExtra ( MoaiBillingConstants.NOTIFICATION_ID );
            getPurchaseInformation ( startId, new String[] { notifyId } );
        } else if ( MoaiBillingConstants.ACTION_PURCHASE_STATE_CHANGED.equals ( action )) {
            String signedData = intent.getStringExtra ( MoaiBillingConstants.INAPP_SIGNED_DATA );
            String signature = intent.getStringExtra ( MoaiBillingConstants.INAPP_SIGNATURE );
            purchaseStateChanged ( startId, signedData, signature );
        } else if ( MoaiBillingConstants.ACTION_RESPONSE_CODE.equals ( action )) {
            long requestId = intent.getLongExtra ( MoaiBillingConstants.INAPP_REQUEST_ID, -1 );
            int responseCodeIndex = intent.getIntExtra ( MoaiBillingConstants.INAPP_RESPONSE_CODE, ResponseCode.RESULT_ERROR.ordinal ());
            ResponseCode responseCode = ResponseCode.valueOf ( responseCodeIndex );
            checkResponseCode ( requestId, responseCode );
        }
    }

    private boolean bindToMarketBillingService() {
        try {
            if ( MoaiBillingConstants.DEBUG ) {
                Log.i ( TAG, "binding to Market billing service" );
            }

            boolean bindResult = bindService ( new Intent ( MoaiBillingConstants.MARKET_BILLING_SERVICE_ACTION ), this, Context.BIND_AUTO_CREATE );
            if ( bindResult ) {
                return true;
            } else {
                Log.e ( TAG, "Could not bind to service." );
            }
        } catch ( SecurityException e ) {
            Log.e ( TAG, "Security exception: " + e );
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

    public boolean confirmNotifications ( String[] notifyIds ) {
        return new ConfirmNotifications ( notifyIds ).runRequest ();
    }

    private boolean getPurchaseInformation ( int startId, String[] notifyIds ) {
    	return new GetPurchaseInformation ( startId, notifyIds ).runRequest ();
    }

    private void purchaseStateChanged ( int startId, String signedData, String signature ) {
        ArrayList<MoaiBillingSecurity.VerifiedPurchase> purchases = MoaiBillingSecurity.verifyPurchase ( signedData, signature );
        if ( purchases == null ) {
            return;
        }
    
        for ( VerifiedPurchase vp : purchases ) {
            MoaiBillingResponseHandler.purchaseResponse ( vp.purchaseState, vp.productId, vp.orderId, vp.notificationId, vp.developerPayload );
        }
    }

    private void checkResponseCode ( long requestId, ResponseCode responseCode ) {
        BillingRequest request = mSentRequests.get ( requestId );
        if ( request != null ) {
            if ( MoaiBillingConstants.DEBUG ) {
                Log.d ( TAG, request.getClass ().getSimpleName () + ": " + responseCode );
            }

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
            if ( MoaiBillingConstants.DEBUG ) {
                Log.i ( TAG, "stopping service, startId: " + maxStartId );
            }

            stopSelf(maxStartId);
        }
    }

    public void onServiceConnected ( ComponentName name, IBinder service ) {
        if ( MoaiBillingConstants.DEBUG ) {
            Log.d ( TAG, "Billing service connected" );
        }

        mService = IMarketBillingService.Stub.asInterface(service);
        runPendingRequests();
    }

    public void onServiceDisconnected ( ComponentName name ) {
        Log.w ( TAG, "Billing service disconnected" );
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

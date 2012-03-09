//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.os.Bundle;

import java.util.ArrayList;

//================================================================//
// MoaiGoogleBilling
//================================================================//
public class MoaiGoogleBilling extends MoaiGoogleBillingPurchaseObserver {

	private static Activity 				sActivity = null;
	private static MoaiGoogleBillingService	sBillingService = null;

	protected static native void AKUNotifyGoogleBillingSupported			( boolean supported );
	protected static native void AKUNotifyGooglePurchaseResponseReceived	( String productId, int responseCode );
	protected static native void AKUNotifyGooglePurchaseStateChanged		( String productId, int purchaseState, String orderId, String notificationId, String developerPayload );
	protected static native void AKUNotifyGoogleRestoreResponseReceived		( int responseCode );

	//----------------------------------------------------------------//
    public MoaiGoogleBilling ( Activity activity ) {
	
		super ( activity );
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity, Bundle extras ) {
		
		MoaiLog.i ( "onCreate: Initializing Google Billing" );
		
		sActivity = activity;

		sBillingService = new MoaiGoogleBillingService ();
		sBillingService.setContext ( sActivity );
	}

	//----------------------------------------------------------------//
	public static void onDestroy () {
	
		sBillingService.unbind ();
	}

	//----------------------------------------------------------------//
	public static void onStart () {
	
	    MoaiGoogleBillingResponseHandler.register ( new MoaiGoogleBilling ( sActivity ));
	}

	//----------------------------------------------------------------//
	public static void onStop () {
	
	    MoaiGoogleBillingResponseHandler.unregister ();
	}

	//================================================================//
	// Google Billing (Android Market) JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static boolean checkBillingSupported () {
		
		return sBillingService.checkBillingSupported ();				
	}

	//----------------------------------------------------------------//
	public static boolean confirmNotification ( String notificationId ) {

		ArrayList < String > notifyList = new ArrayList < String > ();
		notifyList.add ( notificationId );
		
		return sBillingService.confirmNotifications ( notifyList.toArray ( new String [ notifyList.size ()] ) );
	}
		
	//----------------------------------------------------------------//
	public static boolean requestPurchase ( String productId, String developerPayload ) {

		return sBillingService.requestPurchase ( productId, developerPayload );
	}
	
	//----------------------------------------------------------------//
	public static boolean restoreTransactions () {

		return sBillingService.restoreTransactions ();
	}
	
	//----------------------------------------------------------------//
	public static void setMarketPublicKey ( String key ) {
	
		MoaiGoogleBillingSecurity.setPublicKey ( key );
	}
	
	//================================================================//
	// MoaiGoogleBillingPurchaseObserver methods
	//================================================================//

	//----------------------------------------------------------------//
	@Override
    public void onBillingSupported ( boolean supported ) {
	
		AKUNotifyGoogleBillingSupported ( supported );
    }
	
	//----------------------------------------------------------------//
    @Override
    public void onPurchaseStateChange ( MoaiGoogleBillingConstants.PurchaseState purchaseState, String itemId, String orderId, String notificationId, String developerPayload ) {
	        
		AKUNotifyGooglePurchaseStateChanged ( itemId, purchaseState.ordinal(), orderId, notificationId, developerPayload );
    }
	
	//----------------------------------------------------------------//
	@Override
    public void onRequestPurchaseResponse ( MoaiGoogleBillingService.RequestPurchase request, MoaiGoogleBillingConstants.ResponseCode responseCode ) {
	
		AKUNotifyGooglePurchaseResponseReceived ( request.mProductId, responseCode.ordinal() );
    }
	
	//----------------------------------------------------------------//
    @Override
    public void onRestoreTransactionsResponse ( MoaiGoogleBillingService.RestoreTransactions request, MoaiGoogleBillingConstants.ResponseCode responseCode ) {
	
		AKUNotifyGoogleRestoreResponseReceived ( responseCode.ordinal() );
	}
}
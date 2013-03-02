//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;

import java.util.ArrayList;

//================================================================//
// MoaiGoogleBilling
//================================================================//
public class MoaiGoogleBilling extends MoaiGoogleBillingPurchaseObserver {

	private static Activity 				sActivity = null;
	private static MoaiGoogleBillingService	sBillingService = null;

	protected static native void AKUNotifyGoogleBillingSupported			( boolean supported );
	protected static native void AKUNotifyGooglePurchaseResponseReceived	( int responseCode, String productId );
	protected static native void AKUNotifyGooglePurchaseStateChanged		( int purchaseState, String productId, String orderId, String notificationId, String developerPayload );
	protected static native void AKUNotifyGoogleRestoreResponseReceived		( int responseCode );

	//----------------------------------------------------------------//
    public MoaiGoogleBilling ( Activity activity ) {
	
		super ( activity );
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		
		MoaiLog.i ( "MoaiGoogleBilling onCreate: Initializing Google Billing" );
		
		sActivity = activity;

		sBillingService = new MoaiGoogleBillingService ();
		sBillingService.setContext ( sActivity );
	}

	//----------------------------------------------------------------//
	public static void onDestroy () {
	
		MoaiLog.i ( "MoaiGoogleBilling onDestroy: Unbinding billing service" );

		sBillingService.unbind ();
	}

	//----------------------------------------------------------------//
	public static void onStart () {
	
		MoaiLog.i ( "MoaiGoogleBilling onStart: Registering billing handler" );

	    MoaiGoogleBillingResponseHandler.register ( new MoaiGoogleBilling ( sActivity ));
	}

	//----------------------------------------------------------------//
	public static void onStop () {
	
		MoaiLog.i ( "MoaiGoogleBilling onStop: Unregistering billing handler" );

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
	public static boolean getUserId () {

		return false;
	}
		
	//----------------------------------------------------------------//
	public static boolean requestPurchase ( String productId, String developerPayload ) {

		return sBillingService.requestPurchase ( productId, developerPayload );
	}
	
	//----------------------------------------------------------------//
	public static boolean restoreTransactions ( String offset ) {

		return sBillingService.restoreTransactions ();
	}
	
	//----------------------------------------------------------------//
	public static void setPublicKey ( String key ) {
	
		MoaiGoogleBillingSecurity.setPublicKey ( key );
	}
	
	//================================================================//
	// MoaiGoogleBillingPurchaseObserver methods
	//================================================================//

	//----------------------------------------------------------------//
	@Override
    public void onBillingSupported ( boolean supported ) {
			
		synchronized ( Moai.sAkuLock ) {
			AKUNotifyGoogleBillingSupported ( supported );
		}
    }
	
	//----------------------------------------------------------------//
    @Override
    public void onPurchaseStateChange ( MoaiGoogleBillingConstants.PurchaseState purchaseState, String itemId, String orderId, String notificationId, String developerPayload ) {
	        
		synchronized ( Moai.sAkuLock ) {
			AKUNotifyGooglePurchaseStateChanged ( purchaseState.ordinal(), itemId, orderId, notificationId, developerPayload );
		}
    }
	
	//----------------------------------------------------------------//
	@Override
    public void onRequestPurchaseResponse ( MoaiGoogleBillingService.RequestPurchase request, MoaiGoogleBillingConstants.ResponseCode responseCode ) {
		
		synchronized ( Moai.sAkuLock ) {
			AKUNotifyGooglePurchaseResponseReceived ( responseCode.ordinal (), request.mProductId );
		}
    }
	
	//----------------------------------------------------------------//
    @Override
    public void onRestoreTransactionsResponse ( MoaiGoogleBillingService.RestoreTransactions request, MoaiGoogleBillingConstants.ResponseCode responseCode ) {
		
		synchronized ( Moai.sAkuLock ) {
			AKUNotifyGoogleRestoreResponseReceived ( responseCode.ordinal ());
		}
	}
}
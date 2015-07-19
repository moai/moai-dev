//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.os.Bundle;

import java.util.HashSet;
import java.util.HashMap;

import com.amazon.inapp.purchasing.BasePurchasingObserver;
import com.amazon.inapp.purchasing.GetUserIdResponse;
import com.amazon.inapp.purchasing.Item;
import com.amazon.inapp.purchasing.ItemDataResponse;
import com.amazon.inapp.purchasing.Offset;
import com.amazon.inapp.purchasing.PurchaseResponse;
import com.amazon.inapp.purchasing.PurchasingManager;
import com.amazon.inapp.purchasing.PurchaseUpdatesResponse;
import com.amazon.inapp.purchasing.Receipt;

//================================================================//
// MoaiAmazonBilling
//================================================================//
public class MoaiAmazonBilling extends BasePurchasingObserver {

	private static Activity											sActivity = null;
	private static boolean											sBillingAvailable = false;
	private static HashMap < String, HashMap < String, String >> 	sPendingPurchases = new HashMap < String, HashMap < String, String >> ();
	
	protected static native void AKUNotifyAmazonBillingSupported			( boolean supported );
	protected static native void AKUNotifyAmazonPurchaseResponseReceived	( int responseCode, String productId );
	protected static native void AKUNotifyAmazonPurchaseStateChanged		( int purchaseState, String productId, String orderId, String userId, String developerPayload );
	protected static native void AKUNotifyAmazonRestoreResponseReceived		( int responseCode, boolean more, String offset );
	protected static native void AKUNotifyAmazonUserIdDetermined			( int responseCode, String userId );
	
	//----------------------------------------------------------------//
    public MoaiAmazonBilling ( Activity activity ) {
	
		super ( activity );
	}
	
	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		
		MoaiLog.i ( "MoaiAmazonBilling onCreate: Initializing Amazon Billing" );
		
		sActivity = activity;
	}

	//----------------------------------------------------------------//
	public static void onStart () {
	
		MoaiLog.i ( "MoaiAmazonBilling onStart: Registering billing observer" );

	    PurchasingManager.registerObserver ( new MoaiAmazonBilling ( sActivity ));
	}

	//----------------------------------------------------------------//
	public static void onStop () {
	
		MoaiLog.i ( "MoaiAmazonBilling onStop: Unregistering billing observer" );

	    PurchasingManager.registerObserver ( null );
	}
	
	//================================================================//
	// Amazon Billing (Amazon App Store) JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static boolean checkBillingSupported () {
		
		MoaiLog.i ( "MoaiAmazonBilling checkBillingSupported" );

		// sBillingAvailable is set to true in onSDKAvailable() which is called after this
		// function is called 
		AKUNotifyAmazonBillingSupported ( true );//sBillingAvailable );
		
		return true;
	}
	
	//----------------------------------------------------------------//
	public static boolean confirmNotification ( String notificationId ) {

		return false;
	}
	
	//----------------------------------------------------------------//
	public static boolean getUserId () {
		
		return ( PurchasingManager.initiateGetUserIdRequest () != null );
	}

	//----------------------------------------------------------------//
	public static boolean requestPurchase ( String productId, String developerPayload ) {

		String requestId = PurchasingManager.initiatePurchaseRequest ( productId );

		if ( requestId != null ) {
			
			MoaiLog.i ( "MoaiAmazonBilling requestPurchase requestID true" );
			HashMap < String, String > state = new HashMap < String, String > ();
			state.put ( MoaiAmazonBillingConstants.PENDING_PURCHASE_STATE_PRODUCT_ID , productId );
			state.put ( MoaiAmazonBillingConstants.PENDING_PURCHASE_STATE_DEVELOPER_PAYLOAD , developerPayload );
			sPendingPurchases.put ( requestId, state );

		} else {

			MoaiLog.i ( "MoaiAmazonBilling requestPurchase requestID false" );
		}

		return ( requestId != null );
	}

	//----------------------------------------------------------------//
	public static boolean restoreTransactions ( String offset ) {

		return ( PurchasingManager.initiatePurchaseUpdatesRequest (( offset != null ) ? Offset.fromString ( offset ) : Offset.BEGINNING ) != null );
	}

	//----------------------------------------------------------------//
	public static void setPublicKey ( String key ) {
	
	}

	//================================================================//
	// BasePurchasingObserver methods
	//================================================================//

	//----------------------------------------------------------------//
	@Override
	public void onGetUserIdResponse ( GetUserIdResponse response ) {
	
		AKUNotifyAmazonUserIdDetermined ( response.getUserIdRequestStatus ().ordinal (), response.getUserId ());
	}
	
	//----------------------------------------------------------------//
	@Override
	public void onItemDataResponse ( ItemDataResponse response ) {
		
		// MoaiLog.i ( "ALREADY_ENTITLED: " + PurchaseResponse.PurchaseRequestStatus.valueOf ("ALREADY_ENTITLED").ordinal());
		// MoaiLog.i ( "FAILED: " + PurchaseResponse.PurchaseRequestStatus.valueOf ("FAILED").ordinal());
		// MoaiLog.i ( "INVALID_SKU: " + PurchaseResponse.PurchaseRequestStatus.valueOf ("INVALID_SKU").ordinal());
		// MoaiLog.i ( "SUCCESSFUL: " + PurchaseResponse.PurchaseRequestStatus.valueOf ("SUCCESSFUL").ordinal());
		
		// MoaiLog.i ( "MoaiAmazonBilling onItemDataResponse: " + response.getItemDataRequestStatus ());
		// 
		// for ( String sku : response.getItemData ().keySet ()) {
		// 	
		// 	Item item = response.getItemData ().get ( sku );
		// 	
		// 	MoaiLog.i ( "SKU = " + sku );
		// 	MoaiLog.i ( "TYPE = " + item.getItemType ());
		// 	MoaiLog.i ( "PRICE = " + item.getPrice ());
		// 	MoaiLog.i ( "TITLE = " + item.getTitle ());
		// 	MoaiLog.i ( "DESCRIPTION = " + item.getDescription ());
		// }
	}
	
	//----------------------------------------------------------------//
	@Override
	public void onPurchaseResponse ( PurchaseResponse response ) {
				
		MoaiLog.i ( "MoaiAmazonBilling onPurchaseResponse" );
		HashMap < String, String > state = sPendingPurchases.get ( response.getRequestId ());
		if ( state != null ) {
			
			MoaiLog.i ( "MoaiAmazonBilling state not null" );
			String productId = state.get ( MoaiAmazonBillingConstants.PENDING_PURCHASE_STATE_PRODUCT_ID );
			String developerPayload = state.get ( MoaiAmazonBillingConstants.PENDING_PURCHASE_STATE_DEVELOPER_PAYLOAD );
			
			AKUNotifyAmazonPurchaseResponseReceived ( response.getPurchaseRequestStatus ().ordinal (), productId );
			
			if ( response.getPurchaseRequestStatus () == PurchaseResponse.PurchaseRequestStatus.SUCCESSFUL ) {

				AKUNotifyAmazonPurchaseStateChanged ( MoaiAmazonBillingConstants.PurchaseState.PURCHASE_COMPLETED.ordinal (), productId, response.getRequestId (), response.getUserId (), developerPayload );
			}
			
			sPendingPurchases.remove ( response.getRequestId ());
		} else {

			MoaiLog.i ( "MoaiAmazonBilling state is null" );
		}
	}
	
	//----------------------------------------------------------------//
	@Override
	public void onPurchaseUpdatesResponse ( PurchaseUpdatesResponse response ) {
		
		AKUNotifyAmazonRestoreResponseReceived ( response.getPurchaseUpdatesRequestStatus ().ordinal (), response.isMore (), ( response.isMore ()) ? response.getOffset ().toString () : null );

		for ( Receipt receipt : response.getReceipts ())
		{
			AKUNotifyAmazonPurchaseStateChanged ( MoaiAmazonBillingConstants.PurchaseState.PURCHASE_COMPLETED.ordinal (), receipt.getSku (), response.getRequestId (), response.getUserId (), null );
		}

		for ( String productId : response.getRevokedSkus ())
		{
			AKUNotifyAmazonPurchaseStateChanged ( MoaiAmazonBillingConstants.PurchaseState.PURCHASE_REFUNDED.ordinal (), productId, response.getRequestId (), response.getUserId (), null );
		}
	}
	
	//----------------------------------------------------------------//
	@Override
	public void onSdkAvailable ( boolean isSandboxMode ) {
		
		MoaiLog.i ( "MoaiAmazonBilling onSdkAvailable: " + (( isSandboxMode ) ? "SANDBOX" : "PRODUCTION" ));
		
		sBillingAvailable = true;		
	}
}
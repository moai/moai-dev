//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;

//import java.util.ArrayList;

import android.util.Base64;
import tv.ouya.console.api.*;
import java.util.*;
import org.json.JSONObject;
import java.security.SecureRandom;
import android.os.Bundle;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.Cipher;

//================================================================//
//================================================================//
public class MoaiOuyaBilling {
	private boolean isOuya = false;
	private CancelIgnoringOuyaResponseListener<String> receiptListListener;
	private OuyaResponseListener<String> purchaseListener;
	private java.security.PublicKey	publicKey = null;

	private static MoaiOuyaBilling sBilling = null;

	private Map<String, String> mOutstandingPurchaseRequests = new HashMap<String, String>();	

	protected static native void AKUNotifyBillingSupported			 ( boolean supported );
	protected static native void AKUNotifyBillingPurchaseStateChanged( int purchaseState, String productId, String orderId, String notificationId, String developerPayload );
	protected static native void AKUNotifyBillingPurchaseResponseReceived( int responseCode, String productId );
	protected static native void AKUNotifyBillingRestoreResponseReceived( int responseCode );

    public enum ResponseCode {
	
        RESULT_OK,
        RESULT_USER_CANCELED,
        RESULT_SERVICE_UNAVAILABLE,
        RESULT_BILLING_UNAVAILABLE,
        RESULT_ITEM_UNAVAILABLE,
        RESULT_DEVELOPER_ERROR,
        RESULT_ERROR;

        public static ResponseCode valueOf ( int index ) {
	
            ResponseCode [] values = ResponseCode.values ();
            if (( index < 0 ) || ( index >= values.length )) {
	
                return RESULT_ERROR;
            }

            return values [ index ];
        }
    }

    public enum PurchaseState {
	
        PURCHASE_COMPLETED,
        PURCHASE_CANCELED,
        PURCHASE_REFUNDED;

        public static PurchaseState valueOf ( int index ) {
	
            PurchaseState [] values = PurchaseState.values ();
            if (( index < 0 ) || ( index >= values.length )) {

                return PURCHASE_COMPLETED;
            }

            return values [ index ];
        }
    }

    public static final String MARKET_BILLING_SERVICE_ACTION = 			"com.android.vending.billing.MarketBillingService.BIND";

    public static final String ACTION_NOTIFY = 							"com.android.vending.billing.IN_APP_NOTIFY";
    public static final String ACTION_RESPONSE_CODE = 					"com.android.vending.billing.RESPONSE_CODE";
    public static final String ACTION_PURCHASE_STATE_CHANGED = 			"com.android.vending.billing.PURCHASE_STATE_CHANGED";

    public static final String NOTIFICATION_ID = 						"notification_id";
    public static final String INAPP_SIGNED_DATA = 						"inapp_signed_data";
    public static final String INAPP_SIGNATURE = 						"inapp_signature";
    public static final String INAPP_REQUEST_ID = 						"request_id";
    public static final String INAPP_RESPONSE_CODE = 					"response_code";

    public static final String BILLING_REQUEST_METHOD = 				"BILLING_REQUEST";
    public static final String BILLING_REQUEST_API_VERSION = 			"API_VERSION";
    public static final String BILLING_REQUEST_PACKAGE_NAME = 			"PACKAGE_NAME";
    public static final String BILLING_REQUEST_ITEM_ID = 				"ITEM_ID";
    public static final String BILLING_REQUEST_DEVELOPER_PAYLOAD = 		"DEVELOPER_PAYLOAD";
    public static final String BILLING_REQUEST_NOTIFY_IDS = 			"NOTIFY_IDS";
    public static final String BILLING_REQUEST_NONCE = 					"NONCE";

    public static final String BILLING_RESPONSE_RESPONSE_CODE = 		"RESPONSE_CODE";
    public static final String BILLING_RESPONSE_PURCHASE_INTENT = 		"PURCHASE_INTENT";
    public static final String BILLING_RESPONSE_REQUEST_ID = 			"REQUEST_ID";

    public static long BILLING_RESPONSE_INVALID_REQUEST_ID = 			-1;

	//----------------------------------------------------------------//
	public MoaiOuyaBilling () {
		MoaiLog.i ( "MoaiOuyaBilling: Initializing OUYA Billing" );
		isOuya = OuyaFacade.getInstance().isRunningOnOUYAHardware();
		if (isOuya) {
			setupRecieptListener();
			setupPurchaseListener();
		}
	}

	private void setupRecieptListener() {
		if (publicKey != null) {
			receiptListListener = new CancelIgnoringOuyaResponseListener<String>() {
				@Override
				public void onSuccess(String receiptResponse) {
					OuyaEncryptionHelper helper = new OuyaEncryptionHelper();
					List<Receipt> receipts = null;
					try {
						JSONObject response = new JSONObject(receiptResponse);
						receipts = helper.decryptReceiptResponse(response, publicKey);
					} catch (Exception e) {
						throw new RuntimeException(e);
					}
					AKUNotifyBillingRestoreResponseReceived(ResponseCode.RESULT_OK.ordinal());
					for (Receipt r : receipts) {
						AKUNotifyBillingPurchaseStateChanged(
							PurchaseState.PURCHASE_COMPLETED.ordinal(),
							r.getIdentifier(),
							null,
							null,
							null
						);
					}
				}

				@Override
				public void onFailure(int errorCode, String errorMessage, android.os.Bundle errorBundle) {
					AKUNotifyBillingRestoreResponseReceived(ResponseCode.RESULT_ERROR.ordinal());
					MoaiLog.e(errorMessage);
				}
			};
		}
	}

	private void setupPurchaseListener() {
		if (publicKey != null) {
			purchaseListener = new OuyaResponseListener<String>() {
				@Override
				public void onSuccess(String response) {
					try {
						OuyaEncryptionHelper helper = new OuyaEncryptionHelper();
						org.json.JSONObject jsonresponse = new org.json.JSONObject(response);
						String uuid = helper.decryptPurchaseResponse(jsonresponse, publicKey);
						String productId = null;
						synchronized (mOutstandingPurchaseRequests) {
							productId = mOutstandingPurchaseRequests.remove(uuid);
						}

						if(productId == null) {
							MoaiLog.e("OH GOD. NO MATCHING PRODUCT. WTF.");
							return;
						}

						AKUNotifyBillingPurchaseResponseReceived(
							ResponseCode.RESULT_OK.ordinal(),
							productId
						);

						AKUNotifyBillingPurchaseStateChanged(
							PurchaseState.PURCHASE_COMPLETED.ordinal(),
							productId,
							null,
							null,
							null
						);

					} catch (Exception e) {
						MoaiLog.e("Your purchase failed.", e);
					}
				}

				@Override
				public void onFailure(int errorCode, String errorMessage, Bundle errorBundle) {
					AKUNotifyBillingPurchaseResponseReceived(
							ResponseCode.RESULT_ERROR.ordinal(),
							"unknown"
						);
					MoaiLog.d(errorMessage);
				}

				@Override
				public void onCancel() {
					AKUNotifyBillingPurchaseResponseReceived(
						ResponseCode.RESULT_USER_CANCELED.ordinal(),
						"unknown"
					);
					MoaiLog.d("Purchase Cancelled");
				}				
			};
		}
	}

	public static MoaiOuyaBilling getInstance() {
		if (sBilling == null) {
			sBilling = new MoaiOuyaBilling();
		}
		return sBilling;
	}

	public static void setPublicKey(java.security.PublicKey publicKey) {
		getInstance().publicKey = publicKey;
		getInstance().setupRecieptListener();
		getInstance().setupPurchaseListener();
	}

	//================================================================//
	// Ouya Billing JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static boolean checkBillingSupported () {
		boolean isOuya = getInstance().isOuya;
		MoaiLog.i ( "MoaiOuyaBilling: Is billing supported? " + isOuya );
		synchronized ( Moai.sAkuLock ) {
			AKUNotifyBillingSupported ( isOuya );
		}
		return true;
	}

	public static boolean requestPurchase ( String productId, String developerPayload ) {
		try {
			SecureRandom sr = SecureRandom.getInstance("SHA1PRNG");
			JSONObject purchaseRequest = new JSONObject();
			String uniqueId = Long.toHexString(sr.nextLong());
			
			purchaseRequest.put("uuid", uniqueId);
			purchaseRequest.put("identifier", productId);
			String purchaseRequestJson = purchaseRequest.toString();

			byte[] keyBytes = new byte[16];
			sr.nextBytes(keyBytes);
			SecretKey key = new SecretKeySpec(keyBytes, "AES");

			byte[] ivBytes = new byte[16];
			sr.nextBytes(ivBytes);
			IvParameterSpec iv = new IvParameterSpec(ivBytes);

			Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding", "BC");
			cipher.init(Cipher.ENCRYPT_MODE, key, iv);
			byte[] payload = cipher.doFinal(purchaseRequestJson.getBytes("UTF-8"));

			cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding", "BC");
			cipher.init(Cipher.ENCRYPT_MODE, getInstance().publicKey);
			byte[] encryptedKey = cipher.doFinal(keyBytes);

			Purchasable purchasable =
			        new Purchasable(
			                productId,
			                Base64.encodeToString(encryptedKey, Base64.NO_WRAP),
			                Base64.encodeToString(ivBytes, Base64.NO_WRAP),
			                Base64.encodeToString(payload, Base64.NO_WRAP) );

			synchronized (getInstance().mOutstandingPurchaseRequests) {
				getInstance().mOutstandingPurchaseRequests.put(uniqueId, productId);
			}

			OuyaFacade.getInstance().requestPurchase(purchasable, getInstance().purchaseListener);
			return true;
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}

	public static boolean restoreTransactions ( String offset ) {
		OuyaFacade.getInstance().requestReceipts(getInstance().receiptListListener);
		return true;
	}

/*	//----------------------------------------------------------------//
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

	
	//----------------------------------------------------------------//
	
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
	}*/
}
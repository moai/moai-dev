//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

//================================================================//
// MoaiGoogleBillingConstants
//================================================================//
public class MoaiGoogleBillingConstants {
	
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
}

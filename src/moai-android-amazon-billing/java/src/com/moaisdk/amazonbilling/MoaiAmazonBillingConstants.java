//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.moaisdk.amazonbilling;

@SuppressWarnings("unused")

//================================================================//
// MoaiAmazonBillingConstants
//================================================================//
public class MoaiAmazonBillingConstants {

   public enum PurchaseState {

       PURCHASE_COMPLETED,
       PURCHASE_REFUNDED;

        public static PurchaseState valueOf ( int index ) {

            PurchaseState [] values = PurchaseState.values ();
            if (( index < 0 ) || ( index >= values.length )) {

                return PURCHASE_COMPLETED;
            }

            return values [ index ];
        }
    }
	
    public static final String PENDING_PURCHASE_STATE_PRODUCT_ID = 			"product_id";
    public static final String PENDING_PURCHASE_STATE_DEVELOPER_PAYLOAD = 	"developer_payload";
}

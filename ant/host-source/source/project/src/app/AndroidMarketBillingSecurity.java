//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import @PACKAGE@.AndroidMarketBillingConstants.PurchaseState;
import @PACKAGE@.Base64;
import @PACKAGE@.Base64DecoderException;

import android.text.TextUtils;

import java.security.InvalidKeyException;
import java.security.KeyFactory;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.Signature;
import java.security.SignatureException;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.X509EncodedKeySpec;
import java.util.ArrayList;
import java.util.HashSet;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

// Moai
import com.ziplinegames.moai.*;

//================================================================//
// AndroidMarketBillingSecurity
//================================================================//
public class AndroidMarketBillingSecurity {
	
    private static final String 		KEY_FACTORY_ALGORITHM = 	"RSA";
    private static final String 		SIGNATURE_ALGORITHM = 		"SHA1withRSA";
    private static final SecureRandom 	RANDOM = 					new SecureRandom ();

	private static String 				sBase64EncodedPublicKey = 	null;
    private static HashSet < Long >		sKnownNonces = 				new HashSet<Long> ();

    public static class VerifiedPurchase {
	
        public PurchaseState 	purchaseState;
        public String 			notificationId;
        public String 			productId;
        public String 			orderId;
        public long 			purchaseTime;
        public String 			developerPayload;

        public VerifiedPurchase ( PurchaseState purchaseState, String notificationId, String productId, String orderId, long purchaseTime, String developerPayload ) {
	
            this.purchaseState = purchaseState;
            this.notificationId = notificationId;
            this.productId = productId;
            this.orderId = orderId;
            this.purchaseTime = purchaseTime;
            this.developerPayload = developerPayload;
        }
    }

    public static long generateNonce () {
	
        long nonce = RANDOM.nextLong ();
        sKnownNonces.add ( nonce );
        return nonce;
    }

	public static void setPublicKey ( String key ) {
		
		sBase64EncodedPublicKey = key;
	}

    public static void removeNonce ( long nonce ) {
	
        sKnownNonces.remove ( nonce );
    }

    public static boolean isNonceKnown ( long nonce ) {
	
        return sKnownNonces.contains ( nonce );
    }

    public static ArrayList < VerifiedPurchase > verifyPurchase ( String signedData, String signature ) {
	
        if ( sBase64EncodedPublicKey == null ) {
	
            MoaiLog.e ( "AndroidMarketBillingSecurity verifyPurchase: please specify your Android Market public key using MOAIApp.setMarketPublicKey ()" );
            return null;
        }

        if ( signedData == null ) {
	
            MoaiLog.e ( "AndroidMarketBillingSecurity verifyPurchase: data is null" );
            return null;
        }

        boolean verified = false;
        if ( !TextUtils.isEmpty ( signature )) {
	
            PublicKey key = AndroidMarketBillingSecurity.generatePublicKey ( sBase64EncodedPublicKey );
            verified = AndroidMarketBillingSecurity.verify ( key, signedData, signature );
            if ( !verified ) {
	
                MoaiLog.w ( "AndroidMarketBillingSecurity verifyPurchase: signature does not match data" );
                return null;
            }
        }

        JSONObject jObject;
        JSONArray jTransactionsArray = null;
        int numTransactions = 0;
        long nonce = 0L;
        try {
	
            jObject = new JSONObject ( signedData );

            nonce = jObject.optLong ( "nonce" );
            jTransactionsArray = jObject.optJSONArray ( "orders" );
            if ( jTransactionsArray != null ) {
	
                numTransactions = jTransactionsArray.length ();
            }
        } catch ( JSONException e ) {
	
            MoaiLog.e ( "AndroidMarketBillingSecurity verifyPurchase: json exception", e );
            return null;
        }

        if ( !AndroidMarketBillingSecurity.isNonceKnown ( nonce )) {
	
            MoaiLog.w ( "AndroidMarketBillingSecurity verifyPurchase: nonce not found ( " + nonce + " )" );
            return null;
        }

        ArrayList < VerifiedPurchase > purchases = new ArrayList < VerifiedPurchase > ();
        try {
	
            for ( int i = 0; i < numTransactions; i++ ) {
	
                JSONObject jElement = jTransactionsArray.getJSONObject ( i );
                int response = jElement.getInt ( "purchaseState" );
                PurchaseState purchaseState = PurchaseState.valueOf ( response );
                String productId = jElement.getString ( "productId" );
                String packageName = jElement.getString ( "packageName" );
                long purchaseTime = jElement.getLong ( "purchaseTime" );
                String orderId = jElement.optString ( "orderId", "" );
                String notifyId = null;

                if ( jElement.has ( "notificationId" )) {
	
                    notifyId = jElement.getString ( "notificationId" );
                }

                String developerPayload = jElement.optString ( "developerPayload", null );

                if (( purchaseState == PurchaseState.PURCHASED ) && !verified ) {
	
                    continue;
                }

                purchases.add ( new VerifiedPurchase ( purchaseState, notifyId, productId, orderId, purchaseTime, developerPayload ));
            }
        } catch ( JSONException e ) {
	
            MoaiLog.e ( "AndroidMarketBillingSecurity verifyPurchase: json exception", e );
            return null;
        }

        removeNonce ( nonce );
        return purchases;
    }

    public static PublicKey generatePublicKey ( String encodedPublicKey ) {
	
        try {
	
            byte [] decodedKey = Base64.decode ( encodedPublicKey );
            KeyFactory keyFactory = KeyFactory.getInstance ( KEY_FACTORY_ALGORITHM );
            return keyFactory.generatePublic ( new X509EncodedKeySpec ( decodedKey ));
        } catch ( NoSuchAlgorithmException e ) {
	
            MoaiLog.e ( "AndroidMarketBillingSecurity generatePublicKey: no such algorithm" );
            throw new RuntimeException ( e );
        } catch ( InvalidKeySpecException e ) {
	
            MoaiLog.e ( "AndroidMarketBillingSecurity generatePublicKey: invalid key" );
            throw new IllegalArgumentException ( e );
        } catch ( Base64DecoderException e ) {
	
            MoaiLog.e ( "AndroidMarketBillingSecurity generatePublicKey: base64 decoding failed" );
            throw new IllegalArgumentException ( e );
        }
    }

    public static boolean verify ( PublicKey publicKey, String signedData, String signature ) {
	
        Signature sig;
        try {
	
            sig = Signature.getInstance ( SIGNATURE_ALGORITHM );
            sig.initVerify ( publicKey );
            sig.update ( signedData.getBytes ());
            if ( !sig.verify ( Base64.decode ( signature ))) {
	
                MoaiLog.e ( "AndroidMarketBillingSecurity verify: signature verification failed" );
                return false;
            }

            return true;
        } catch ( NoSuchAlgorithmException e ) {
	
            MoaiLog.e ( "AndroidMarketBillingSecurity verify: no such algorithm" );
        } catch ( InvalidKeyException e ) {
	
            MoaiLog.e ( "AndroidMarketBillingSecurity verify: invalid key" );
        } catch ( SignatureException e ) {
	
            MoaiLog.e ( "AndroidMarketBillingSecurity verify: invalid signature" );
        } catch ( Base64DecoderException e ) {
	
            MoaiLog.e ( "AndroidMarketBillingSecurity verify: base64 decoding failed" );
        }

        return false;
    }
}

//----------------------------------------------------------------//
// Portions Copyright (c) 2010-2011 Zipline Games, Inc. 
// Adapted from In-App Billing sample code from Google, Inc.
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package @PACKAGE@;

import @PACKAGE@.MoaiBillingConstants.PurchaseState;
import @PACKAGE@.Base64;
import @PACKAGE@.Base64DecoderException;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.text.TextUtils;
import android.util.Log;

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

//================================================================//
// MoaiBillingSecurity
//================================================================//
public class MoaiBillingSecurity {
    private static final String TAG = "MoaiBillingSecurity";

    private static final String KEY_FACTORY_ALGORITHM = "RSA";
    private static final String SIGNATURE_ALGORITHM = "SHA1withRSA";
    private static final SecureRandom RANDOM = new SecureRandom ();

	private static String sBase64EncodedPublicKey = null;
    private static HashSet<Long> sKnownNonces = new HashSet<Long> ();

    public static class VerifiedPurchase {
        public PurchaseState purchaseState;
        public String notificationId;
        public String productId;
        public String orderId;
        public long purchaseTime;
        public String developerPayload;

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

    public static ArrayList<VerifiedPurchase> verifyPurchase ( String signedData, String signature ) {
        if ( sBase64EncodedPublicKey == null ) {
            Log.e ( "MoaiLog", "Please specify your Android Market public key using MOAIApp.setMarketPublicKey()" );
            return null;
        }

        if ( signedData == null ) {
            Log.e ( TAG, "data is null" );
            return null;
        }

        if ( MoaiBillingConstants.DEBUG ) {
            Log.i ( TAG, "signedData: " + signedData );
        }

        boolean verified = false;
        if ( !TextUtils.isEmpty ( signature )) {
            PublicKey key = MoaiBillingSecurity.generatePublicKey ( sBase64EncodedPublicKey );
            verified = MoaiBillingSecurity.verify ( key, signedData, signature );
            if ( !verified ) {
                Log.w ( TAG, "signature does not match data." );
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
            if (jTransactionsArray != null) {
                numTransactions = jTransactionsArray.length ();
            }
        } catch ( JSONException e ) {
            return null;
        }

        if ( !MoaiBillingSecurity.isNonceKnown ( nonce )) {
            Log.w ( TAG, "Nonce not found: " + nonce );
            return null;
        }

        ArrayList<VerifiedPurchase> purchases = new ArrayList<VerifiedPurchase> ();
        try {
            for (int i = 0; i < numTransactions; i++) {
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

                purchases.add(new VerifiedPurchase ( purchaseState, notifyId, productId, orderId, purchaseTime, developerPayload ));
            }
        } catch ( JSONException e ) {
            Log.e ( TAG, "JSON exception: ", e );
            return null;
        }

        removeNonce ( nonce );
        return purchases;
    }

    public static PublicKey generatePublicKey ( String encodedPublicKey ) {
        try {
            byte[] decodedKey = Base64.decode ( encodedPublicKey );
            KeyFactory keyFactory = KeyFactory.getInstance ( KEY_FACTORY_ALGORITHM );
            return keyFactory.generatePublic ( new X509EncodedKeySpec ( decodedKey ));
        } catch ( NoSuchAlgorithmException e ) {
            throw new RuntimeException ( e );
        } catch ( InvalidKeySpecException e ) {
            Log.e ( TAG, "Invalid key specification." );
            throw new IllegalArgumentException ( e );
        } catch ( Base64DecoderException e ) {
            Log.e ( TAG, "Base64 decoding failed." );
            throw new IllegalArgumentException ( e );
        }
    }

    public static boolean verify ( PublicKey publicKey, String signedData, String signature ) {
        if ( MoaiBillingConstants.DEBUG ) {
            Log.i ( TAG, "signature: " + signature );
        }

        Signature sig;
        try {
            sig = Signature.getInstance ( SIGNATURE_ALGORITHM );
            sig.initVerify ( publicKey );
            sig.update ( signedData.getBytes ());
            if ( !sig.verify ( Base64.decode ( signature ))) {
                Log.e ( TAG, "Signature verification failed." );
                return false;
            }
            return true;
        } catch ( NoSuchAlgorithmException e ) {
            Log.e ( TAG, "NoSuchAlgorithmException." );
        } catch ( InvalidKeyException e ) {
            Log.e ( TAG, "Invalid key specification." );
        } catch ( SignatureException e ) {
            Log.e ( TAG, "Signature exception." );
        } catch ( Base64DecoderException e ) {
            Log.e ( TAG, "Base64 decoding failed." );
        }
        return false;
    }
}

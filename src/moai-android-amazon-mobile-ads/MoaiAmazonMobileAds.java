//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import com.amazon.device.ads.*;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

//================================================================//
// MoaiAmazonMobileAds
//================================================================//
public class MoaiAmazonMobileAds extends Activity {

	private static Activity sActivity = null;
    private static InterstitialAd interstitial = null;
    private static MoaiAmazonMobileAdListener adListener = new MoaiAmazonMobileAdListener ();

	protected static native void AKUInvokeListener ( int eventID );

	//----------------------------------------------------------------//

    /**
     * When the activity starts, set up the click events for the buttons so that they load or show ads when clicked.
     */
	public static void onCreate ( Activity activity ) {
	
		MoaiLog.i ( "MoaiAmazonMobileAds: onCreate" );
		sActivity = activity;
		interstitial = new InterstitialAd ( sActivity );
        interstitial.setListener ( adListener );
	}

	//----------------------------------------------------------------//
	public static void cacheInterstitial ( String location ) {
		
		if ( !adListener.adLoaded && !interstitial.isLoading ()) {
			
			boolean isLoading = interstitial.loadAd ();
			MoaiLog.i ( "MoaiAmazonMobileAds: cacheInterstitial is loading: " + isLoading );

		} else {
			MoaiLog.i ( "MoaiAmazonMobileAds: cacheInterstitial interstitial still loading" );
		}
	}

	//----------------------------------------------------------------//
	public static boolean hasCachedInterstitial ( String location ) {
		
		MoaiLog.i ( "MoaiAmazonMobileAds: hasCachedInterstitial AdLoaded :" + adListener.adLoaded + " interstitial.isLoading: " + interstitial.isLoading ());
		return adListener.adLoaded;
	}

	//----------------------------------------------------------------//
	public static void init ( String appId ) {
		
		MoaiLog.i ( "MoaiAmazonMobileAds: init" );

        AdRegistration.setAppKey ( appId );
	}

	//----------------------------------------------------------------//
	public static void showInterstitial ( String location ) {
				
		
		if ( adListener.adLoaded && !interstitial.isShowing ()) {
			
			boolean showedAd = interstitial.showAd ();
			adListener.adLoaded = false;
			MoaiLog.i ( "MoaiAmazonMobileAds: showInterstitial ad shown: " + showedAd );

		} else {
			
			MoaiLog.i ( "MoaiAmazonMobileAds: showInterstitial adListener.AdLoaded: " + adListener.adLoaded + " interstitial.isShowing: " + interstitial.isShowing ());

		}
	}
}

/**
 * This class is for an event listener that tracks ad lifecycle events.
 * It extends DefaultAdListener, so you can override only the methods that you need.
 * In this case, there is no need to override methods specific to expandable ads.
 */
class MoaiAmazonMobileAdListener extends DefaultAdListener {
    /**
     * This event is called once an ad loads successfully.
     */

    public static boolean adLoaded = false;

    public void onAdLoaded ( final Ad ad, final AdProperties adProperties ) {

		MoaiLog.i ( "MoaiAmazonMobileAds: onAdLoaded" );
		adLoaded = true;
    }

    /**
     * This event is called if an ad fails to load.
     */
    public void onAdFailedToLoad ( final Ad view, final AdError error ) {

		MoaiLog.i ( "MoaiAmazonMobileAds: onAdFailedToLoad:" );
		adLoaded = false;
    }
    
    /**
     * This event is called when an interstitial ad has been dismissed by the user.
     */
    public void onAdDismissed ( final Ad ad ) {

		MoaiLog.i ( "MoaiAmazonMobileAds: onAdDismissed" );
		adLoaded = false;
    }
}
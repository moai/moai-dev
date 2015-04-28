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
public class MoaiAmazonMobileAds extends DefaultAdListener {

	public enum ListenerEvent {
		INTERSTITIAL_LOAD_FAILED,
		INTERSTITIAL_DISMISSED,
    }

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
	public static void onBackPressed ( Activity activity ) {
		
        MoaiLog.i ( "MoaiAmazonMobileAds: onBackPressed" );
    }
	
	//----------------------------------------------------------------//
	public static void onDestroy ( Activity activity ) {
 
		MoaiLog.i ( "MoaiAmazonMobileAds: onDestroy" );
	}

	//----------------------------------------------------------------//
	public static void onPause () {
 
		MoaiLog.i ( "MoaiAmazonMobileAds: onPause" );
	}

	//----------------------------------------------------------------//
	public static void onResume () {
 
		MoaiLog.i ( "MoaiAmazonMobileAds: onResume" );
	}
		
	//----------------------------------------------------------------//
	public static void onStart () {
		
		MoaiLog.i ( "MoaiAmazonMobileAds: onStart" );
	}
	
	//----------------------------------------------------------------//
	public static void onStop () {

		MoaiLog.i ( "MoaiAmazonMobileAds: onStop" );
	}

	//================================================================//
	// ChartBoost JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static void cacheInterstitial ( String location ) {
		
		MoaiLog.i ( "MoaiAmazonMobileAds: cacheInterstitial" );

		if ( !adListener.AdLoaded () && !interstitial.isLoading ()) {
			interstitial.loadAd ();
		} else {
			MoaiLog.i ( "MoaiAmazonMobileAds: interstitial still loading" );
		}
	}

	//----------------------------------------------------------------//
	public static boolean hasCachedInterstitial ( String location ) {
		
		MoaiLog.i ( "MoaiAmazonMobileAds: hasCachedInterstitial (loadingAd,interstitial.isLoading ()) " + adListener.AdLoaded () + interstitial.isLoading ());
		return adListener.AdLoaded ();
	}

	//----------------------------------------------------------------//
	public static void init ( String appId ) {
		
		MoaiLog.i ( "MoaiAmazonMobileAds: init" );

		AdRegistration.enableLogging ( true );
        AdRegistration.enableTesting ( true );
        AdRegistration.setAppKey ( appId );
	}

	//----------------------------------------------------------------//
	public static void showInterstitial ( String location ) {
				
		MoaiLog.i ( "MoaiAmazonMobileAds: showInterstitial" );
		
		if ( adListener.AdLoaded () && !interstitial.isShowing ()) {
			
			interstitial.showAd ();
			adListener.AdLoaded ( false );

		} else {
			MoaiLog.i ( "MoaiAmazonMobileAds: no cached ads" );
			MoaiLog.i ( "MoaiAmazonMobileAds: loadingAd " + adListener.AdLoaded () );
			MoaiLog.i ( "MoaiAmazonMobileAds: interstitial.isLoading () " + interstitial.isLoading () );
			MoaiLog.i ( "MoaiAmazonMobileAds: interstitial.isShowing () " + interstitial.isShowing () );
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

    private static boolean adLoaded = false;

    public static boolean AdLoaded () {
    	return adLoaded;
    }

	public static void AdLoaded ( boolean val ) {
    	adLoaded = val;
    }

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
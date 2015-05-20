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

	public enum ListenerEvent {
		INTERSTITIAL_LOAD_FAILED,
		INTERSTITIAL_DISMISSED,
    }

    public static InterstitialAd adToLoad = null;
    public static InterstitialAd adToShow = null;
    public static InterstitialAd ad_1 = null;
    public static InterstitialAd ad_2 = null;
    public static boolean loadedAd_1 = false;
    public static boolean loadedAd_2 = false;
    private static MoaiAmazonMobileAdListener adListener = new MoaiAmazonMobileAdListener ();
    private static AdTargetingOptions targetOptions;

	protected static native void AKUInvokeListener ( int eventID );

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
	
		MoaiLog.i ( "MoaiAmazonMobileAds: onCreate" );
		ad_1 = new InterstitialAd ( activity );
        ad_1.setListener ( adListener );
        ad_1.setTimeout ( 5000 );

		ad_2 = new InterstitialAd ( activity );
        ad_2.setListener ( adListener );
        ad_2.setTimeout ( 5000 );

        adToLoad = ad_1;

        targetOptions = new AdTargetingOptions ();
        targetOptions.enableGeoLocation ( true );
	}

	//----------------------------------------------------------------//
	public static void cacheInterstitial ( String location ) {
		
		if ( ( !loadedAd_1 || !loadedAd_2 ) && !adToLoad.isLoading ()) {
			
			boolean isLoading = adToLoad.loadAd ( targetOptions );
			MoaiLog.i ( "MoaiAmazonMobileAds: cacheInterstitial is loading: " + isLoading );
			if ( adToLoad == ad_1 ) { 
				MoaiLog.i ( "MoaiAmazonMobileAds: cacheInterstitial ad_1" );
			} else {

				MoaiLog.i ( "MoaiAmazonMobileAds: cacheInterstitial ad_2" );
			}

		} else {
			MoaiLog.i ( "MoaiAmazonMobileAds: cacheInterstitial interstitial still loading" );
		}
	}

	//----------------------------------------------------------------//
	public static boolean hasCachedInterstitial ( String location ) {
		
		MoaiLog.i ( "MoaiAmazonMobileAds: hasCachedInterstitial loadedAd_1: " + loadedAd_1 + " loadedAd_2: " + loadedAd_2 + " adToLoad.isLoading: " + adToLoad.isLoading ());
		if ( loadedAd_1 ) { 
				MoaiLog.i ( "MoaiAmazonMobileAds: hasCachedInterstitial ad_1" );
			} 
		if ( loadedAd_2 ) { 

				MoaiLog.i ( "MoaiAmazonMobileAds: hasCachedInterstitial ad_2" );
			}
		return loadedAd_1 || loadedAd_2;
	}

	//----------------------------------------------------------------//
	public static void init ( String appId ) {
		
		MoaiLog.i ( "MoaiAmazonMobileAds: init" );

        AdRegistration.setAppKey ( appId );
        AdRegistration.enableLogging ( true );
        AdRegistration.enableTesting ( false );
	}

	//----------------------------------------------------------------//
	public static boolean showInterstitial ( String location ) {
				
		if ( adToShow == null && ( loadedAd_1 || loadedAd_2 )) {

			MoaiLog.i ( "MoaiAmazonMobileAds: showInterstitial pushLoadedAdToShow adToShow == null loadedAd_1: " + loadedAd_1 + " loadedAd_2: " + loadedAd_2  );
			pushLoadedAdToShow ();
		}

		if ( adToShow != null && !adToShow.isShowing ()) {

			if ( adToShow == ad_1 ) {

				MoaiLog.i ( "MoaiAmazonMobileAds: showInterstitial ad_1" );
				loadedAd_1 = false;

			} else {
				
				MoaiLog.i ( "MoaiAmazonMobileAds: showInterstitial ad_2" );
				loadedAd_2 = false;
			}

			boolean showedAd = adToShow.showAd ();
			adToShow = null;

			if ( loadedAd_1 || loadedAd_2 ) {

				MoaiLog.i ( "MoaiAmazonMobileAds: showInterstitial pushLoadedAdToShow next loaded loadedAd_1: " + loadedAd_1 + " loadedAd_2: " + loadedAd_2  );
				pushLoadedAdToShow ();
			}

			MoaiLog.i ( "MoaiAmazonMobileAds: showInterstitial ad shown: " + showedAd + "  loadedAd_1: " + loadedAd_1 + " loadedAd_2: " + loadedAd_2  );
			return true;

		} else {
			
			MoaiLog.i ( "MoaiAmazonMobileAds: showInterstitial loadedAd_1: " + loadedAd_1 + " loadedAd_2: " + loadedAd_2 + " adToShow.isShowing: " + adToShow.isShowing ());
			return false;
		}
	}

	public static void pushLoadedAdToShow () {

		if ( adToShow == null || ( !adToShow.isShowing () && adToShow != adToLoad && !( loadedAd_1 || loadedAd_2 ))) {
			
			MoaiLog.i ( "MoaiAmazonMobileAds: pushLoadedAdToShow ad swap loadedAd_1: " + loadedAd_1 + " loadedAd_2: " + loadedAd_2  );
			adToShow = adToLoad;

			if ( adToLoad == ad_1 ) {
				
				adToLoad = ad_2;

			} else {
				
				adToLoad = ad_1;
			}

			cacheInterstitial ( "" );
		} else {

			MoaiLog.i ( "MoaiAmazonMobileAds: pushLoadedAdToShow failed loadedAd_1: " + loadedAd_1 + " loadedAd_2: " + loadedAd_2 );
		}

	}
}

class MoaiAmazonMobileAdListener extends DefaultAdListener {

    public void onAdLoaded ( Ad ad, AdProperties adProperties ) {

		if ( ad == MoaiAmazonMobileAds.ad_1 ) {
			MoaiAmazonMobileAds.loadedAd_1 = true;
		} else {
			MoaiAmazonMobileAds.loadedAd_2 = true;
		}

		MoaiLog.i ( "MoaiAmazonMobileAds: onAdLoaded loadedAd_1: " + MoaiAmazonMobileAds.loadedAd_1 + " loadedAd_2: " + MoaiAmazonMobileAds.loadedAd_2 );

		MoaiAmazonMobileAds.pushLoadedAdToShow ();
    }

    public void onAdFailedToLoad ( Ad view, AdError error ) {

		if ( view == MoaiAmazonMobileAds.ad_1 ) {
			MoaiAmazonMobileAds.loadedAd_1 = false;
		} else {
			MoaiAmazonMobileAds.loadedAd_2 = false;
		}

		MoaiLog.i ( "MoaiAmazonMobileAds: onAdFailedToLoad:" + error.getMessage () + "  loadedAd_1: " + MoaiAmazonMobileAds.loadedAd_1 + " loadedAd_2: " + MoaiAmazonMobileAds.loadedAd_2 );
			
		if ( error.getCode () == AdError.ErrorCode.NETWORK_ERROR ) {
			MoaiLog.i ( "MoaiAmazonMobileAds: onAdFailedToLoad NETWORK_ERROR" );
			MoaiAmazonMobileAds.cacheInterstitial ( "" );
		}

		if ( error.getCode () == AdError.ErrorCode.NETWORK_TIMEOUT ) {
			MoaiLog.i ( "MoaiAmazonMobileAds: onAdFailedToLoad NETWORK_TIMEOUT" );
			MoaiAmazonMobileAds.cacheInterstitial ( "" );
		}

		if ( error.getCode () == AdError.ErrorCode.NO_FILL ) {
			MoaiLog.i ( "MoaiAmazonMobileAds: onAdFailedToLoad NO_FILL" );
		}

		if ( error.getCode () == AdError.ErrorCode.INTERNAL_ERROR ) {
			MoaiLog.i ( "MoaiAmazonMobileAds: onAdFailedToLoad INTERNAL_ERROR" );
			MoaiAmazonMobileAds.cacheInterstitial ( "" );
		}

		if ( error.getCode () == AdError.ErrorCode.REQUEST_ERROR ) {
			MoaiLog.i ( "MoaiAmazonMobileAds: onAdFailedToLoad REQUEST_ERROR" );
		}
    }
}
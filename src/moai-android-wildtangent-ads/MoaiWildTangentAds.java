//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;
import com.ziplinegames.moai.*;

import com.google.android.gms.ads.doubleclick.PublisherInterstitialAd;
import com.google.android.gms.ads.doubleclick.PublisherAdRequest;

import android.app.Activity;
import android.view.View;

import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

//================================================================//
// MoaiWildTangentAds
//================================================================//
public class MoaiWildTangentAds {

	public static boolean mloadAdRequested = false;

	private static final String TEST_UNIT_ID    = "/6499/example/interstitial";
	private static final String RELEASE_UNIT_ID = "/4337/Mobile/Interstitial";
	private static final boolean RELEASE_MODE   = true;

	private static PublisherInterstitialAd mInterstitialAd = null;
	private static Activity mActivity					   = null;

	private static MoaiWildTangentAdsListener adListener = new MoaiWildTangentAdsListener ();

	// AKU callbacks
	protected static native void AKUInvokeListener ( int eventID );

	//----------------------------------------------------------------//
	
	public static void onCreate ( Activity activity ) {

		MoaiLog.i ( "MoaiWildTangentAds onCreate" );
		mActivity = activity;
	}

	//----------------------------------------------------------------//
	public static void onResume () {
 
		MoaiLog.i ( "MoaiWildTangentAds: onResume" );
	}
	
	//----------------------------------------------------------------//
	public static void onStart () {
		
		MoaiLog.i ( "MoaiWildTangentAds: onStart" );
	}
	
	//----------------------------------------------------------------//
	public static void onStop () {

		MoaiLog.i ( "MoaiWildTangentAds: onStop" );
	}

	//================================================================//
	// MoaiWildTangentAds JNI callback methods
	//================================================================//
	//----------------------------------------------------------------//
	public static void cacheInterstitial ( String location ) {
		
		MoaiLog.i ( "MoaiWildTangentAds: cacheInterstitial" );

		if ( !mloadAdRequested ) {

			MoaiLog.i ( "MoaiWildTangentAds: cacheInterstitial: caching" );
			PublisherAdRequest adRequest = new PublisherAdRequest.Builder ().build ();

        	mInterstitialAd.loadAd ( adRequest );
        	mloadAdRequested = true;
        } else {
			MoaiLog.i ( "MoaiWildTangentAds: cacheInterstitial: already caching" );
        }
	}

	//----------------------------------------------------------------//
	public static boolean hasCachedInterstitial ( String location ) {
		
		MoaiLog.i ( "MoaiWildTangentAds: hasCachedInterstitial: mloadAdRequested " + mloadAdRequested +"  mInterstitialAd.isLoaded () " + mInterstitialAd.isLoaded ());

		return mloadAdRequested && mInterstitialAd.isLoaded ();
	}

	//----------------------------------------------------------------//
	public static void init () {
		
		MoaiLog.i ( "MoaiWildTangentAds: init" );
		
		mInterstitialAd = new PublisherInterstitialAd ( mActivity );

		if ( RELEASE_MODE ) {

			MoaiLog.i ( "MoaiWildTangentAds: init: setAdUnitId " + RELEASE_UNIT_ID);
			mInterstitialAd.setAdUnitId ( RELEASE_UNIT_ID );
		} else {
			MoaiLog.i ( "MoaiWildTangentAds: init: setAdUnitId " + TEST_UNIT_ID);
			mInterstitialAd.setAdUnitId ( TEST_UNIT_ID );
		}
		mInterstitialAd.setAdListener ( adListener );
	}

	//----------------------------------------------------------------//
	public static void showInterstitial ( String location ) {
				
		MoaiLog.i ( "MoaiWildTangentAds: showInterstitial" );
		
		if ( mloadAdRequested && mInterstitialAd.isLoaded ()) {
			mInterstitialAd.show ();
			mloadAdRequested = false;
		}
	}
}
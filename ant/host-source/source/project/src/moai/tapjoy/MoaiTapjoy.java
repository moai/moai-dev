//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;

import com.tapjoy.TapjoyConnect;
import com.tapjoy.TapjoyVideoNotifier;

public class MoaiTapjoy implements TapjoyVideoNotifier {

	private static Activity mActivity = null;

	protected static native void AKUNotifyTapjoyVideoAdReady ();
	protected static native void AKUNotifyTapjoyVideoAdError ( int statusCode );
	protected static native void AKUNotifyTapjoyVideoAdClose ();

	public static void initialize ( Activity activity ) {
		
		MoaiLog.i ( "Initializing Tapjoy" );
		
		mActivity = activity;
	}

	//================================================================//
	// Tapjoy JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static String getUserId () {

		return TapjoyConnect.getTapjoyConnectInstance ().getUserID ();
	}
	 
	//----------------------------------------------------------------//
	public static void initVideoAds () {

		TapjoyConnect.getTapjoyConnectInstance ().initVideoAd ( new MoaiTapjoy ());
	}

	//----------------------------------------------------------------//
	public static void requestTapjoyConnect ( String appId, String appSecret ) {

		TapjoyConnect.requestTapjoyConnect ( mActivity, appId, appSecret );
	}
		
	//----------------------------------------------------------------//
	public static void setVideoAdCacheCount ( int count ) {

		TapjoyConnect.getTapjoyConnectInstance ().setVideoCacheCount ( count );
	}
	
	//----------------------------------------------------------------//
	public static void showOffers () {

		TapjoyConnect.getTapjoyConnectInstance ().showOffers ();
	}

	//================================================================//
	// TapjoyVideoNotifier methods
	//================================================================//	

	//----------------------------------------------------------------//
	public void videoComplete () {

		AKUNotifyTapjoyVideoAdClose ();
	}

	//----------------------------------------------------------------//
	public void videoError ( int statusCode ) {

		AKUNotifyTapjoyVideoAdError ( statusCode );
	}

	//----------------------------------------------------------------//
	public void videoReady () {

		AKUNotifyTapjoyVideoAdReady ();
	}
}
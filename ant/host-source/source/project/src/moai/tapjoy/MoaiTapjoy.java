//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.content.Context;

import com.tapjoy.TapjoyConnect;
import com.tapjoy.TapjoyVideoNotifier;

//================================================================//
// MoaiTapjoy
//================================================================//
public class MoaiTapjoy implements TapjoyVideoNotifier {

	private static Context mContext = null;

	protected static native void AKUNotifyTapjoyVideoAdReady	();
	protected static native void AKUNotifyTapjoyVideoAdError	( int statusCode );
	protected static native void AKUNotifyTapjoyVideoAdClose	();

	//----------------------------------------------------------------//
	public static void initialize ( Context context ) {
		
		MoaiLog.i ( "Initializing Tapjoy" );
		
		mContext = context;
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

		TapjoyConnect.requestTapjoyConnect ( mContext, appId, appSecret );
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
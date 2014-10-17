//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;

import com.tapjoy.TapjoyConnect;
import com.tapjoy.TapjoyOffersNotifier;
import com.tapjoy.TapjoyVideoNotifier;

//================================================================//
// MoaiTapjoy
//================================================================//
public class MoaiTapjoy implements
	TapjoyOffersNotifier, TapjoyVideoNotifier {

	public enum ListenerEvent {
		TAPJOY_VIDEO_AD_START,
		TAPJOY_VIDEO_AD_COMPLETE,
		TAPJOY_VIDEO_AD_ERROR,
		TAPJOY_OFFERS_RESPONSE,
		TAPJOY_OFFERS_RESPONSE_FAILED,
    }

	private static Activity sActivity = null;

	protected static native void AKUInvokeListener				( int eventID );
	protected static native void AKUInvokeListenerWithCode		( int eventID, int statusCode );

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		MoaiLog.i ( "MoaiTapjoy: onCreate" );
		sActivity = activity;
	}

	//----------------------------------------------------------------//
	public static void onPause () {
		MoaiLog.i ( "MoaiTapjoy: onPause" );
		TapjoyConnect.getTapjoyConnectInstance ().appPause ();
	}
	
	//----------------------------------------------------------------//
	public static void onResume () {
		MoaiLog.i ( "MoaiTapjoy: onResume" );
		TapjoyConnect.getTapjoyConnectInstance ().appResume ();
	}

	//================================================================//
	// Tapjoy JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static void init ( String appId, String appSecret, int videoCacheCount ) {

		TapjoyConnect.requestTapjoyConnect ( sActivity, appId, appSecret );
		TapjoyConnect.getTapjoyConnectInstance ().setVideoNotifier ( new MoaiTapjoy ());
		TapjoyConnect.getTapjoyConnectInstance ().setVideoCacheCount ( videoCacheCount );
	}

	//----------------------------------------------------------------//
	public static void setUserId ( String userId ) {

		TapjoyConnect.getTapjoyConnectInstance ().setUserID ( userId );
	}

	//----------------------------------------------------------------//
	public static void showOffers () {

		TapjoyConnect.getTapjoyConnectInstance ().showOffers ();
	}

	//================================================================//
	// TapjoyOffersNotifier methods
	//================================================================//

	//----------------------------------------------------------------//
	public void getOffersResponse () {
		
		synchronized ( Moai.sAkuLock ) {
			AKUInvokeListener ( ListenerEvent.TAPJOY_OFFERS_RESPONSE.ordinal ());
		}
	}

	//----------------------------------------------------------------//
	public void getOffersResponseFailed ( String error ) {
		
		synchronized ( Moai.sAkuLock ) {
			AKUInvokeListener ( ListenerEvent.TAPJOY_OFFERS_RESPONSE_FAILED.ordinal ());
		}
	}

	//================================================================//
	// TapjoyVideoNotifier methods
	//================================================================//

	//----------------------------------------------------------------//
	public void videoComplete () {

		synchronized ( Moai.sAkuLock ) {
			AKUInvokeListener ( ListenerEvent.TAPJOY_VIDEO_AD_COMPLETE.ordinal ());
		}
	}

	//----------------------------------------------------------------//
	public void videoError ( int statusCode ) {

		synchronized ( Moai.sAkuLock ) {
			AKUInvokeListenerWithCode ( ListenerEvent.TAPJOY_VIDEO_AD_ERROR.ordinal (), statusCode );
		}
	}

	//----------------------------------------------------------------//
	public void videoStart () {

		synchronized ( Moai.sAkuLock ) {
			AKUInvokeListener ( ListenerEvent.TAPJOY_VIDEO_AD_START.ordinal ());
		}
	}
}

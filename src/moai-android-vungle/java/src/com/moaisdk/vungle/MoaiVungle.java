//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.moaisdk.vungle;

import com.moaisdk.core.MoaiLog;

import android.app.Activity;

import com.vungle.sdk.VunglePub;

@SuppressWarnings("unused")

//================================================================//
// MoaiVungle
//================================================================//

public class MoaiVungle implements VunglePub.EventListener {

	public enum ListenerEvent {
		AD_START,
		AD_END,
		AD_VIEWED,
    }

	private static Activity sActivity = null;

	protected static native void AKUInvokeListener ( int eventID );
	protected static native void AKUOnView ( double watched, double length );

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {

		MoaiLog.i ( "MoaiVungle: onCreate" );

		sActivity = activity;
	}

	//----------------------------------------------------------------//
	public static void onPause ( ) {

		MoaiLog.i ( "MoaiVungle: onPause" );

		VunglePub.onPause ();
	}

	//----------------------------------------------------------------//
	public static void onResume ( ) {

		MoaiLog.i ( "MoaiVungle: onResume" );

		VunglePub.onResume ();
	}

	//================================================================//
	// MoaiVungle JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static boolean displayAdvert ( boolean incentivized, boolean showCloseButton ) {
		
		if ( incentivized ) {
			return VunglePub.displayIncentivizedAdvert ( showCloseButton );
		}
		return VunglePub.displayAdvert ();
	}

	//----------------------------------------------------------------//
	public static void init ( String appId ) {
		
		VunglePub.init ( sActivity, appId );
		VunglePub.setEventListener ( new MoaiVungle ());
	}

	//----------------------------------------------------------------//
	public static boolean isVideoAvailable ( boolean debug ) {
		
		//MoaiLog.i ( "MoaiVungle: isVideoAvailable" );
		return VunglePub.isVideoAvailable ( debug );
	}

	//================================================================//
	// VunglePub.EventListener
	//================================================================//

	//----------------------------------------------------------------//
	public void onVungleAdEnd () {
		
		MoaiLog.i ( "MoaiVungle: onVungleAdEnd" );
		AKUInvokeListener ( ListenerEvent.AD_END.ordinal ());
	}

	//----------------------------------------------------------------//
	public void onVungleAdStart () {
		
		MoaiLog.i ( "MoaiVungle: onVungleAdStart" );
		AKUInvokeListener ( ListenerEvent.AD_START.ordinal ());
	}

	//----------------------------------------------------------------//
	public void onVungleView ( double watched, double length ) {
		
		MoaiLog.i ( String.format ( "MoaiVungle: onVungleView (%f, %f)", watched, length ));
		AKUOnView ( watched, length );
	}
}
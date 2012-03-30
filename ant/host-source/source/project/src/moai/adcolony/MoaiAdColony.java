//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.os.Bundle;

import java.util.Arrays;
import java.util.ArrayList;

import com.jirbo.adcolony.AdColony;
import com.jirbo.adcolony.AdColonyVideoAd;
import com.jirbo.adcolony.AdColonyVideoListener;

//================================================================//
// MoaiAdColony
//================================================================//
public class MoaiAdColony implements AdColonyVideoListener {

	private static Activity sActivity = null;
	
	protected static native void AKUNotifyAdColonyVideoComplete ();

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		
		MoaiLog.i ( "MoaiAdColony onCreate: Initializing AdColony" );
		
		sActivity = activity;
	}
	
	//----------------------------------------------------------------//
	public static void onPause ( ) {
	
		MoaiLog.i ( "MoaiAdColony onPause: Notifying AdColony" );

		AdColony.setAppStatus ( true );
	}
	
	//----------------------------------------------------------------//
	public static void onResume ( ) {
	
		MoaiLog.i ( "MoaiAdColony onResume: Notifying AdColony" );

		AdColony.setAppStatus ( false );
	}

	//================================================================//
	// AdColony JNI callback methods
	//================================================================//
	
	//----------------------------------------------------------------//
	public static String getDeviceID () {

		return AdColony.getDeviceID ();
	}
	
	//----------------------------------------------------------------//
	public static void init ( String appId, String [] zoneIds ) {

		String appVersion;
		try {
			
			appVersion = sActivity.getApplicationContext ().getPackageManager ().getPackageInfo ( appId, 0 ).versionName;
		}
		catch ( Exception e ) {
			
			appVersion = "UNKNOWN";
		}

		ArrayList < String > parameters = new ArrayList < String > ( Arrays.asList ( zoneIds ));
		parameters.add ( 0, appId );
		
		AdColony.configure ( sActivity, appVersion, parameters.toArray ( new String [ parameters.size ()]));
	}

	//----------------------------------------------------------------//
	public static boolean isVideoReady ( String zoneId ) {

		return new AdColonyVideoAd ( zoneId ).isReady ();
	}
	
	//----------------------------------------------------------------//
	public static void playVideo ( String zoneId, boolean showPrompt, boolean showConfirmation ) {
		
		AdColonyVideoAd ad = new AdColonyVideoAd ( zoneId );
		if ( showPrompt ) {
			
			ad.offerV4VC ( new MoaiAdColony (), showConfirmation );
		} else {
			
			ad.showV4VC ( new MoaiAdColony (), showConfirmation );
		}				
	}
	
	//================================================================//
	// AdColonyVideoListener methods
	//================================================================//	

	//----------------------------------------------------------------//
	public void onAdColonyVideoFinished () {

		AKUNotifyAdColonyVideoComplete ();
	}

	//----------------------------------------------------------------//
	public void onAdColonyVideoStarted () {

		// TODO: Notify
	}
}
//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;

import java.util.Arrays;
import java.util.ArrayList;

import com.jirbo.adcolony.AdColony;
import com.jirbo.adcolony.AdColonyVideoAd;

//================================================================//
// MoaiAdColony
//================================================================//
public class MoaiAdColony {

	private static Activity sActivity = null;

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		
		MoaiLog.i ( "onCreate: Initializing AdColony" );
		
		sActivity = activity;
	}

	//================================================================//
	// AdColony JNI callback methods
	//================================================================//

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
			
			ad.offerV4VC ( null, showConfirmation );
		} else {
			
			ad.showV4VC ( null, showConfirmation );
		}
		
		// TODO: Add listener to allow callbacks into Lua
	}
}
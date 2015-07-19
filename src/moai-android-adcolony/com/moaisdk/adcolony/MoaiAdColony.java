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

import com.jirbo.adcolony.*;

//================================================================//
// MoaiAdColony
//================================================================//

public class MoaiAdColony implements AdColonyAdListener {

	public enum ListenerEvent {
		VIDEO_STARTED,
		VIDEO_SHOWN,
		VIDEO_FAILED,
    }

	private static Activity sActivity = null;

	protected static native void AKUInvokeListener ( int eventID );

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {

		MoaiLog.i ( "MoaiAdColony: onCreate" );

		sActivity = activity;
	}

	//----------------------------------------------------------------//
	public static void onPause ( ) {

		MoaiLog.i ( "MoaiAdColony: onPause" );

		AdColony.pause ();
	}

	//----------------------------------------------------------------//
	public static void onResume ( ) {

		MoaiLog.i ( "MoaiAdColony: onResume" );

		AdColony.resume ( sActivity );
	}

	//================================================================//
	// AdColony JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static void init ( String appId, String clientOptions, String [] zoneIds ) {

		MoaiLog.i ( "MoaiAdColony: init" );
		MoaiLog.i ( appId );
		MoaiLog.i ( clientOptions );

		MoaiLog.i ( "zones:" );

		for ( String zone: zoneIds ) {
			MoaiLog.i ( zone );
		}

		AdColony.configure ( sActivity, clientOptions, appId, zoneIds );
	}

	//----------------------------------------------------------------//
	public static boolean isVideoReady ( String zoneId ) {

		String zoneStatus = AdColony.statusForZone ( zoneId );
		boolean result = new AdColonyVideoAd ( zoneId ).isReady ();
		return result;
	}

	//----------------------------------------------------------------//
	public static void playVideo ( String zoneId, boolean showPrompt, boolean showConfirmation ) {

		MoaiLog.i ( "MoaiAdColony: playVideo" );

		AdColonyVideoAd ad = new AdColonyVideoAd ( zoneId ).withListener ( new MoaiAdColony ());
		ad.show ();
	}

	//================================================================//
	// AdColonyAdListener methods
	//================================================================//

	//----------------------------------------------------------------//
	public void onAdColonyAdAttemptFinished ( AdColonyAd ad ) {

		MoaiLog.i ( "MoaiAdColony: onAdColonyAdAttemptFinished" );

		ListenerEvent eventID = null;

		synchronized ( Moai.sAkuLock ) {
			if ( ad.shown ()) {
				MoaiAdColony.AKUInvokeListener ( ListenerEvent.VIDEO_SHOWN.ordinal ());
			}
			else {
				MoaiAdColony.AKUInvokeListener ( ListenerEvent.VIDEO_FAILED.ordinal ());
			}
		}
	}

	//----------------------------------------------------------------//
	public void onAdColonyAdStarted ( AdColonyAd ad ) {

		MoaiLog.i ( "MoaiAdColony: onAdColonyAdStarted" );

		synchronized ( Moai.sAkuLock ) {
			MoaiAdColony.AKUInvokeListener ( ListenerEvent.VIDEO_STARTED.ordinal ());
		}
	}
}
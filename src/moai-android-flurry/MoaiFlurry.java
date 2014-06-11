//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import java.util.Map;
import android.app.Activity;
import android.util.Log;

import com.flurry.android.FlurryAgent;

//================================================================//
// MoaiFlurry
//================================================================//
public class MoaiFlurry {

	private static Activity sActivity = null;

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		MoaiLog.i ( "MoaiFlurry onCreate: Initializing Flurry" );
		sActivity = activity;
	}

	//----------------------------------------------------------------//
	public static void onStop () {
		MoaiLog.i ( "MoaiFlurry: onStop" );
		FlurryAgent.onEndSession ( sActivity );
	}

	//================================================================//
	// MoaiFlurry JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static void enableLogging ( boolean enable ) {
	
		MoaiLog.i ( String.format ( "MoaiFlurry: enableLogging %s", enable ? "true" : "false" ));
	
		FlurryAgent.setLogEnabled ( enable );
		FlurryAgent.setLogEvents ( enable );
		FlurryAgent.setLogLevel ( Log.VERBOSE );
	}

	//----------------------------------------------------------------//
	public static void endEvent ( String eventId ) {
		
		MoaiLog.i ( String.format ( "MoaiFlurry: endEvent %s", eventId ));
		
		FlurryAgent.endTimedEvent ( eventId );
	}

	//----------------------------------------------------------------//
	public static void init ( String apiKey ) {
		
		MoaiLog.i ( String.format ( "MoaiFlurry: init %s", apiKey ));
		
		FlurryAgent.onStartSession ( sActivity, apiKey );
	}
	
	//----------------------------------------------------------------//
	public static void logEvent ( String eventId, Map < String, String > parameters, boolean timed ) {
		
		MoaiLog.i ( String.format ( "MoaiFlurry: logEvent %s%s", eventId, timed ? " (timed)" : "" ));
		
		if ( parameters != null ) {
			MoaiLog.i ( String.format ( "MoaiFlurry: logEvent with parameters" ));
			
			//for ( Map.Entry < String, String > entry : parameters.entrySet ()) {
			//    MoaiLog.i ( String.format ( "%s: %s", entry.getKey (), entry.getValue ()));
			//}
			
			FlurryAgent.logEvent ( eventId, parameters, timed );
		}
		else {
			FlurryAgent.logEvent ( eventId, timed );
		}
	}
}
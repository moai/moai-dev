//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;
import android.os.Bundle;
import com.google.android.gcm.GCMRegistrar;

//================================================================//
// MoaiGooglePush
//================================================================//
public class MoaiGooglePush {

	private static Activity sActivity = null;
	private static Bundle sPendingNotification = null;

	//----------------------------------------------------------------//
	public static void onApplicationStateChanged ( Moai.ApplicationState state ) {
		
		MoaiLog.i ( "MoaiGooglePush onApplicationStateChanged: " + state );

		if (( state == Moai.ApplicationState.APPLICATION_RUNNING ) && ( sPendingNotification != null )) {
			
			Intent intent = new Intent ().putExtras ( sPendingNotification );
			MoaiGooglePushReceiver.handleMessage ( sActivity, intent );
			
			sPendingNotification = null;
		}
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		
		MoaiLog.i ( "MoaiGooglePush onCreate: Initializing Google Push" );
		
		sActivity = activity;
		
		GCMRegistrar.checkDevice ( activity );
		GCMRegistrar.checkManifest ( activity );
	}

	//----------------------------------------------------------------//
	public static void onResume ( ) {

		MoaiLog.i ( "MoaiGooglePush onResume: Checking for pending notification" );
		
		Bundle notification = sActivity.getIntent ().getExtras ().getBundle ( MoaiGooglePushConstants.ACTION_RECEIVE );
    	if ( notification != null ) {

			MoaiLog.i ( "MoaiGooglePush onResume: Got a remote notification in app resume" );

			if ( Moai.getApplicationState () == Moai.ApplicationState.APPLICATION_RUNNING ) {
				
				Intent intent = new Intent ().putExtras ( notification );
				MoaiGooglePushReceiver.handleMessage ( sActivity, intent );

				sPendingNotification = null;
			} else {
				
				sPendingNotification = new Bundle ( notification );
			}

			sActivity.setIntent ( sActivity.getIntent ().cloneFilter ());
		}
	}

	//================================================================//
	// Google Push (C2DM) JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static void registerForRemoteNotifications ( String alias ) {
	
		Intent intent = new Intent ( MoaiGooglePushConstants.ACTION_REGISTER );
		intent.putExtra ( MoaiGooglePushConstants.APP_ID, PendingIntent.getBroadcast ( sActivity, 0, new Intent (), 0 ));
		intent.putExtra ( MoaiGooglePushConstants.SENDER_ALIAS, alias );
		sActivity.startService ( intent );  
	}
	
	//----------------------------------------------------------------//
	public static void unregisterForRemoteNotifications ( ) {
	
		Intent intent = new Intent ( MoaiGooglePushConstants.ACTION_UNREGISTER );
		intent.putExtra ( MoaiGooglePushConstants.APP_ID, PendingIntent.getBroadcast ( sActivity, 0, new Intent (), 0 ));
		sActivity.startService ( intent );
	}
}
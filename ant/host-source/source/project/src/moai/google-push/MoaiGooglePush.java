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

//================================================================//
// MoaiGooglePush
//================================================================//
public class MoaiGooglePush {

	private static Activity sActivity = null;
	private static Bundle sPendingNotification = null;

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity, Bundle extras ) {
		
		MoaiLog.i ( "MoaiGooglePush onCreate: Initializing Google Push" );
		
		sActivity = activity;
		
		Bundle notification = extras.getBundle ( MoaiGooglePushConstants.ACTION_RECEIVE );
    	if ( notification != null ) {

			MoaiLog.i ( "MoaiGooglePush onCreate: Got a remote notification in app launch" );
			
			for ( String key : notification.keySet ()) {

				if ( notification.getString ( key ) != null ) {
					
					MoaiLog.i ( "Key = " + key );
					MoaiLog.i ( "Value = " + notification.getString ( key ));
				}
			}

			// TODO: Need to handle case where app was backgrounded - onCreate isn't called.

			if ( Moai.getApplicationState () == Moai.ApplicationState.APPLICATION_RUNNING ) {
				
				Intent intent = new Intent ().putExtras ( notification );
				MoaiGooglePushReceiver.handleMessage ( activity, intent );

				sPendingNotification = null;
			} else {
				
				sPendingNotification = notification;
			}
		}
	}

	//----------------------------------------------------------------//
	public static void onApplicationStateChanged ( Moai.ApplicationState state ) {
		
		MoaiLog.i ( "MoaiGooglePush onApplicationStateChanged: " + state );

		if (( state == Moai.ApplicationState.APPLICATION_RUNNING ) && ( sPendingNotification != null )) {
			
			Intent intent = new Intent ().putExtras ( sPendingNotification );
			MoaiGooglePushReceiver.handleMessage ( sActivity, intent );
			
			sPendingNotification = null;
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
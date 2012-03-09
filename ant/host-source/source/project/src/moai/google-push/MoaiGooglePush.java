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

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity, Bundle extras ) {
		
		MoaiLog.i ( "onCreate: Initializing Google Push" );
		
		sActivity = activity;
		
    	if ( extras.getBoolean ( MoaiGooglePushConstants.ACTION_RECEIVE )) {

			MoaiLog.i ( "Got a remote notification in app launch" );
			
			for ( String key : extras.keySet ()) {

				if ( extras.getString ( key ) != null ) {
					
					MoaiLog.i ( "Key = " + key );
					MoaiLog.i ( "Value = " + extras.getString ( key ));
				}
			}
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
//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;

//================================================================//
// MoaiGooglePush
//================================================================//
public class MoaiGooglePush {

	private static Activity sActivity = null;

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		
		MoaiLog.i ( "onCreate: Initializing Google Push" );
		
		sActivity = activity;
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
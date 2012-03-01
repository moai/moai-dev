//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

//================================================================//
// MoaiGooglePush
//================================================================//
public class MoaiGooglePush {

	private static Context mContext = null;

	//----------------------------------------------------------------//
	public static void initialize ( Context context ) {
		
		MoaiLog.i ( "Initializing Google Push" );
		
		mContext = context;
	}

	//================================================================//
	// Google Push (C2DM) JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static void registerForRemoteNotifications ( String alias ) {
	
		Intent intent = new Intent ( "com.google.android.c2dm.intent.REGISTER" );
		intent.putExtra ( "app", PendingIntent.getBroadcast ( mContext, 0, new Intent (), 0 ));
		intent.putExtra ( "sender", alias );
		mContext.startService ( intent );
	}
	
	//----------------------------------------------------------------//
	public static void unregisterForRemoteNotifications ( ) {
	
		Intent intent = new Intent ( "com.google.android.c2dm.intent.UNREGISTER" );
		intent.putExtra ( "app", PendingIntent.getBroadcast ( mContext, 0, new Intent (), 0 ));
		mContext.startService ( intent );
	}
}
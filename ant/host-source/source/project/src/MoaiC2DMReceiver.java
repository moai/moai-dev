//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences.Editor;
import android.util.Log;

public class MoaiC2DMReceiver extends BroadcastReceiver {

	@Override
	public void onReceive ( Context context, Intent intent ) {
		
		if ( intent.getAction ().equals ( "com.google.android.c2dm.intent.REGISTRATION" )) {

	        handleRegistration ( context, intent );
	    } else if ( intent.getAction ().equals ( "com.google.android.c2dm.intent.RECEIVE" )) {

	        handleMessage ( context, intent );
	    }
	 }

	private void handleRegistration ( Context context, Intent intent ) {
		
	    if ( intent.getStringExtra ( "error" ) != null ) {

		    String error = intent.getStringExtra ( "error" );
		    Log.d ( "C2DM", "Registration failed: " + error );		
	    } else if ( intent.getStringExtra ( "unregistered" ) != null ) {

		    String unregistered = intent.getStringExtra ( "unregistered" );
	    	Log.d ( "C2DM", "Unregistered successfully: " + unregistered );
	    } else if ( intent.getStringExtra ( "registration_id" ) != null ) {

		    String registration = intent.getStringExtra ( "registration_id" );
	    	Log.d ( "C2DM", "Registered successfully: " + registration );
	    }
	}

	private void handleMessage ( Context context, Intent intent )
	{

	}
}
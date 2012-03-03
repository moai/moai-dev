//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

import java.util.ArrayList;

//================================================================//
// MoaiGooglePushReceiver
//================================================================//
public class MoaiGooglePushReceiver extends BroadcastReceiver {

	protected static native void AKUNotifyGooglePushRemoteNotificationRegistrationComplete	( int code , String registration );
	protected static native void AKUNotifyGooglePushRemoteNotificationReceived 				( String [] keys, String [] values );

	//----------------------------------------------------------------//
	@Override
	public void onReceive ( Context context, Intent intent ) {
		
		if ( intent.getAction ().equals ( MoaiGooglePushConstants.ACTION_REGISTRATION )) {

	        handleRegistration ( context, intent );
	    } else if ( intent.getAction ().equals ( MoaiGooglePushConstants.ACTION_RECEIVE )) {

	        handleMessage ( context, intent );
	    }
	}

	//----------------------------------------------------------------//
	private void handleRegistration ( Context context, Intent intent ) {
		
	    if ( intent.getStringExtra ( MoaiGooglePushConstants.ERROR ) != null ) {

		    String errorMessage = intent.getStringExtra ( MoaiGooglePushConstants.ERROR );
		    MoaiLog.e ( "MoaiGooglePushReceiver handleRegistration: registration failed ( " + errorMessage + " )" );
		
			AKUNotifyGooglePushRemoteNotificationRegistrationComplete ( MoaiGooglePushConstants.RegistrationCode.valueOf ( "RESULT_ERROR_" + errorMessage ).ordinal (), null );
	    } else if ( intent.getStringExtra ( MoaiGooglePushConstants.UNREGISTERED ) != null ) {

		    String packageName = intent.getStringExtra ( MoaiGooglePushConstants.UNREGISTERED );
	    	MoaiLog.i ( "MoaiGooglePushReceiver handleRegistration: unregistered successfully ( " + packageName + " )" );

			AKUNotifyGooglePushRemoteNotificationRegistrationComplete ( MoaiGooglePushConstants.RegistrationCode.valueOf ( "RESULT_UNREGISTERED" ).ordinal (), null );
	    } else if ( intent.getStringExtra ( MoaiGooglePushConstants.REGISTRATION_ID ) != null ) {

		    String registrationId = intent.getStringExtra ( MoaiGooglePushConstants.REGISTRATION_ID );
	    	MoaiLog.i ( "MoaiGooglePushReceiver handleRegistration: registered successfully ( " + registrationId + " )" );

			AKUNotifyGooglePushRemoteNotificationRegistrationComplete ( MoaiGooglePushConstants.RegistrationCode.valueOf ( "RESULT_REGISTERED" ).ordinal (), registrationId );
	    }
	}

	//----------------------------------------------------------------//
	private void handleMessage ( Context context, Intent intent ) {
		
		ArrayList < String > keys = new ArrayList < String > ();
		ArrayList < String > values = new ArrayList < String > ();
		
		for ( String key : intent.getExtras ().keySet ()) {

			if ( intent.getExtras ().getString ( key ) != null ) {
				
				keys.add ( key );
				values.add ( intent.getExtras ().getString ( key ));
			}
		}
		
		AKUNotifyGooglePushRemoteNotificationReceived ( keys.toArray ( new String [ keys.size ()]), values.toArray ( new String [ values.size ()]));
	}
}
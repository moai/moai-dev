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
	
	public enum RegistrationCode {
		
		RESULT_REGISTERED,
		RESULT_UNREGISTERED,
        RESULT_ERROR_SERVICE_NOT_AVAILABLE,
        RESULT_ERROR_ACCOUNT_MISSING,
        RESULT_ERROR_AUTHENTICATION_FAILED,
        RESULT_ERROR_TOO_MANY_REGISTRATIONS,
        RESULT_ERROR_INVALID_SENDER,
        RESULT_ERROR_PHONE_REGISTRATION_ERROR;
						
        public static RegistrationCode valueOf ( int index ) {
	
            RegistrationCode [] values = RegistrationCode.values ();
            if (( index < 0 ) || ( index >= values.length )) {
	
                return RESULT_ERROR_PHONE_REGISTRATION_ERROR;
            }

            return values [ index ];
        }
    }

	protected static native void AKUNotifyGooglePushRemoteNotificationRegistrationComplete	( int code , String registration );
	protected static native void AKUNotifyGooglePushRemoteNotificationReceived 				( String [] keys, String [] values );

	//----------------------------------------------------------------//
	@Override
	public void onReceive ( Context context, Intent intent ) {
		
		if ( intent.getAction ().equals ( "com.google.android.c2dm.intent.REGISTRATION" )) {

	        handleRegistration ( context, intent );
	    } else if ( intent.getAction ().equals ( "com.google.android.c2dm.intent.RECEIVE" )) {

	        handleMessage ( context, intent );
	    }
	}

	//----------------------------------------------------------------//
	private void handleRegistration ( Context context, Intent intent ) {
		
	    if ( intent.getStringExtra ( "error" ) != null ) {

		    String errorMessage = intent.getStringExtra ( "error" );
		    MoaiLog.e ( "MoaiGooglePushReceiver handleRegistration: registration failed ( " + errorMessage + " )" );
		
			AKUNotifyGooglePushRemoteNotificationRegistrationComplete ( RegistrationCode.valueOf ( "RESULT_ERROR_" + errorMessage ).ordinal (), null );
	    } else if ( intent.getStringExtra ( "unregistered" ) != null ) {

		    String packageName = intent.getStringExtra ( "unregistered" );
	    	MoaiLog.i ( "MoaiGooglePushReceiver handleRegistration: unregistered successfully ( " + packageName + " )" );

			AKUNotifyGooglePushRemoteNotificationRegistrationComplete ( RegistrationCode.valueOf ( "RESULT_UNREGISTERED" ).ordinal (), null );
	    } else if ( intent.getStringExtra ( "registration_id" ) != null ) {

		    String registrationId = intent.getStringExtra ( "registration_id" );
	    	MoaiLog.i ( "MoaiGooglePushReceiver handleRegistration: registered successfully ( " + registrationId + " )" );

			AKUNotifyGooglePushRemoteNotificationRegistrationComplete ( RegistrationCode.valueOf ( "RESULT_REGISTERED" ).ordinal (), registrationId );
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
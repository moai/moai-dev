//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;

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
	private static void handleRegistration ( Context context, Intent intent ) {
		
		if ( Moai.getApplicationState () == Moai.ApplicationState.APPLICATION_UNINITIALIZED ) {
			
			// If the application was not started as the result of a LAUNCHER action,
			// then AKU has not been initialized, libmoai.so has not been loaded and
			// no Lua scripts have been run, so we ignore the event. 
			return;
		}
		
	    if ( intent.getStringExtra ( MoaiGooglePushConstants.ERROR ) != null ) {

		    String errorMessage = intent.getStringExtra ( MoaiGooglePushConstants.ERROR );
		    MoaiLog.e ( "MoaiGooglePushReceiver handleRegistration: registration failed ( " + errorMessage + " )" );
		
			
			synchronized ( Moai.sAkuLock ) {
				AKUNotifyGooglePushRemoteNotificationRegistrationComplete ( MoaiGooglePushConstants.RegistrationCode.valueOf ( "RESULT_ERROR_" + errorMessage ).ordinal (), null );
			}
	    } else if ( intent.getStringExtra ( MoaiGooglePushConstants.UNREGISTERED ) != null ) {

		    String packageName = intent.getStringExtra ( MoaiGooglePushConstants.UNREGISTERED );
	    	MoaiLog.i ( "MoaiGooglePushReceiver handleRegistration: unregistered successfully ( " + packageName + " )" );
			
			synchronized ( Moai.sAkuLock ) {
				AKUNotifyGooglePushRemoteNotificationRegistrationComplete ( MoaiGooglePushConstants.RegistrationCode.valueOf ( "RESULT_UNREGISTERED" ).ordinal (), null );
			}
	    } else if ( intent.getStringExtra ( MoaiGooglePushConstants.REGISTRATION_ID ) != null ) {

		    String registrationId = intent.getStringExtra ( MoaiGooglePushConstants.REGISTRATION_ID );
	    	MoaiLog.i ( "MoaiGooglePushReceiver handleRegistration: registered successfully ( " + registrationId + " )" );
			
			synchronized ( Moai.sAkuLock ) {
				AKUNotifyGooglePushRemoteNotificationRegistrationComplete ( MoaiGooglePushConstants.RegistrationCode.valueOf ( "RESULT_REGISTERED" ).ordinal (), registrationId );
			}
	    }
	}

	//----------------------------------------------------------------//
	/* package private */ static void handleMessage ( Context context, Intent intent ) {
		
		if ( Moai.getApplicationState () != Moai.ApplicationState.APPLICATION_RUNNING ) {

			MoaiLog.i ( "MoaiGooglePushReceiver handleMessage: Adding notification to tray" );
				
			// If the application is not actively running, then we want to send the
			// notification to the notification tray.
			
			// By default, look for a key called "title" in the extras bundle. If we
			// can't find one, then default to the application name.
			String title = intent.getStringExtra ( "title" );
			if ( title == null ) {

				try {

			    	title = context.getPackageManager ().getApplicationLabel ( context.getPackageManager ().getApplicationInfo ( context.getPackageName (), 0 )).toString ();
				} catch ( Exception e ) {

					title = "UNKNOWN";
				}
			}

			// Look for a key called "message" in the extras bundle. If we can't find
			// one, then construct a default one.
			String message = intent.getStringExtra ( "message" );
			if ( message == null ) {
				
				message = "A new message is waiting for you. Click here to view!";
			}

			int icon = context.getResources ().getIdentifier( "icon", "drawable", context.getPackageName ());

			// Add the notification data from the originating intent to the new intent
			// that will launch the application from the notification tray and "mark"
			// the intent with a known key (we'll use the Google-defined one for C2DM
			// message receipt) so that we recognize it when the application is launched.
		 	Intent notifyIntent = context.getPackageManager ().getLaunchIntentForPackage ( context.getPackageName ());
			notifyIntent.putExtra ( MoaiGooglePushConstants.ACTION_RECEIVE, new Bundle ( intent.getExtras ()));

		    PendingIntent contentIntent = PendingIntent.getActivity ( context, 0, notifyIntent, PendingIntent.FLAG_UPDATE_CURRENT );

		    NotificationManager notificationManager = ( NotificationManager ) context.getSystemService ( Context.NOTIFICATION_SERVICE );
		    Notification notification = new Notification ( icon, message, System.currentTimeMillis ()); 
		    notification.setLatestEventInfo ( context, title, message, contentIntent );
		    notification.flags |= Notification.FLAG_AUTO_CANCEL;

			String tag = intent.getStringExtra ( "collapse_key" );
			int id = ( intent.getStringExtra ( "id" ) != null ) ? Integer.parseInt ( intent.getStringExtra ( "id" )) : 1;

		    notificationManager.notify ( tag, id, notification );		
		} else {
		
			MoaiLog.i ( "MoaiGooglePushReceiver handleMessage: delivering notification" );
				
			ArrayList < String > keys = new ArrayList < String > ();
			ArrayList < String > values = new ArrayList < String > ();
		
			for ( String key : intent.getExtras ().keySet ()) {

				if ( intent.getExtras ().getString ( key ) != null ) {
				
					keys.add ( key );
					values.add ( intent.getExtras ().getString ( key ));
				}
			}

			
			synchronized ( Moai.sAkuLock ) {
				AKUNotifyGooglePushRemoteNotificationReceived ( keys.toArray ( new String [ keys.size ()]), values.toArray ( new String [ values.size ()]));
			}
		}		
	}
}
//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;
import com.ziplinegames.moai.*;

import android.app.Activity;
import android.app.Dialog;
import android.accounts.AccountManager;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;
import android.support.v4.app.DialogFragment;
import android.os.Bundle;

import com.google.android.gms.auth.GoogleAuthUtil;
import com.google.android.gms.auth.GoogleAuthException;
import com.google.android.gms.auth.GooglePlayServicesAvailabilityException;
import com.google.android.gms.auth.UserRecoverableAuthException;
import com.google.android.gms.common.AccountPicker;
import com.google.android.gms.common.Scopes;
import com.google.android.gms.common.GoogleApiAvailability;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.ErrorDialogFragment;
import com.google.android.gms.common.api.GoogleApiClient.ConnectionCallbacks;
import com.google.android.gms.common.api.GoogleApiClient.OnConnectionFailedListener;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.*;

import java.io.IOException;
import android.support.v4.app.FragmentActivity;

//================================================================//
// MoaiGooglePlayServices
//================================================================//
public class MoaiGooglePlayServices {

    public static final int REQUEST_RESOLVE_ERROR 	    = 1001;
	public static final int ALERT_DIALOG_REQUEST_CODE   = 7007;
	public static final int ACCOUNT_PICKER_REQUEST_CODE = 7117;
	public static final int AUTH_REQUEST_CODE 			= 7227;
	public static final int LEADERBOARD_REQUEST_CODE 	= 7337;
	public static final int	CONNECTION_RESOLUTION_CODE 	= 7447;
	public static final int	ACHIEVEMENT_REQUEST_CODE 	= 7557;

	private static final String STATE_RESOLVING_ERROR = "resolving_error";
	private static final int RESULT_OK			      = -1;

	private static Activity mActivity     							   = null;
    private static boolean mResolvingError 							   = false;
	private static String mToken 									   = null;
	private static String mAccountName 								   = null;
    private static int mServiceStatus								   = 0;
	private static GoogleApiClient mGoogleApiClient                    = null;
    private static MoaiGooglePlayServicesCallbacks mConnectionCallback = null;
    private static MoaiGooglePlayServicesCallbacks mFailedCallback     = null;


	// AKU callbacks
	public static native void AKUNotifyConnectionComplete ();
	
	//----------------------------------------------------------------//
	public static Activity getActivity () {
		return mActivity;
	}

	//----------------------------------------------------------------//
	public static boolean getResolvingError () {
		return mResolvingError;
	}

	//----------------------------------------------------------------//
	public static void setResolvingError ( boolean val ) {
		mResolvingError = val;
	}

	//----------------------------------------------------------------//
	public void onActivityResult ( int requestCode, int resultCode, Intent data ) {

		MoaiLog.i ( "MoaiGooglePlayServices onActivityResult: " + requestCode + " " + resultCode );
		if ( requestCode == ACCOUNT_PICKER_REQUEST_CODE && resultCode == Activity.RESULT_OK ) {

			MoaiLog.i ( "MoaiGooglePlayServices ACCOUNT_PICKER_REQUEST_CODE" );
			mAccountName = data.getStringExtra ( AccountManager.KEY_ACCOUNT_NAME );
		} else if ( requestCode == LEADERBOARD_REQUEST_CODE && resultCode == GamesActivityResultCodes.RESULT_RECONNECT_REQUIRED ) {

			MoaiLog.i ( "MoaiGooglePlayServices LEADERBOARD_REQUEST_CODE" );
			mGoogleApiClient.reconnect ();
		} else if ( requestCode == CONNECTION_RESOLUTION_CODE && resultCode == Activity.RESULT_OK ) {

			MoaiLog.i ( "MoaiGooglePlayServices CONNECTION_RESOLUTION_CODE" );
			mGoogleApiClient.connect ();
		}
	}

	//----------------------------------------------------------------//
	
	public static void onCreate ( Activity activity ) {

		MoaiLog.i ( "MoaiGooglePlayServices onCreate" );
		mActivity = activity;

        mConnectionCallback = new MoaiGooglePlayServicesCallbacks ();
        mFailedCallback = new MoaiGooglePlayServicesCallbacks ();

        mGoogleApiClient = new GoogleApiClient.Builder ( mActivity )
                            .addApiIfAvailable ( Games.API )
                            .addConnectionCallbacks ( mConnectionCallback )
                            .addOnConnectionFailedListener ( mFailedCallback )
                            .build ();

	    //mResolvingError = savedInstanceState != null && savedInstanceState.getBoolean ( STATE_RESOLVING_ERROR, false );
	}

	//----------------------------------------------------------------//
	public static void onResume () {
 
		MoaiLog.i ( "MoaiGooglePlayServices: onResume" );
    	mServiceStatus = GoogleApiAvailability.getInstance ().isGooglePlayServicesAvailable ( mActivity );
	}
	
	//----------------------------------------------------------------//
	public static void onStart () {
		
		MoaiLog.i ( "MoaiGooglePlayServices: onStart" );
		
		//if ( isGooglePlayServicesAvailable () && !mResolvingError ) {  
            //mGoogleApiClient.connect ();
        //}
	}
	
	//----------------------------------------------------------------//
	public static void onStop () {

		MoaiLog.i ( "MoaiGooglePlayServices: onStop" );
        mGoogleApiClient.disconnect ();
	}
	
	//================================================================//
	// MoaiGooglePlayServices private functions
	//================================================================//

	//----------------------------------------------------------------//
	private static void getAndUseAuthTokenBlocking () {

		try {

	    	// Retrieve a token for the given account and scope. It will always return either
	        // a non-empty String or throw an exception.
	        mToken = GoogleAuthUtil.getToken ( mActivity, mAccountName, Scopes.PLUS_LOGIN );


		 } catch ( GooglePlayServicesAvailabilityException playEx ) {

		     Dialog alert = GooglePlayServicesUtil.getErrorDialog ( playEx.getConnectionStatusCode (), mActivity, AUTH_REQUEST_CODE );

		 } catch ( UserRecoverableAuthException userAuthEx ) {

	          // Start the user recoverable action using the intent returned by getIntent ()
	          mActivity.startActivityForResult ( userAuthEx.getIntent (), AUTH_REQUEST_CODE );
	          return;

		 } catch ( IOException transientEx ) {

		      // network or server error, the call is expected to succeed if you try again later.
	          // Don't attempt to call again immediately - the request is likely to
	          // fail, you'll hit quotas or back-off.
	          return;

		 } catch ( GoogleAuthException authEx ) {

		      // Failure. The call is not expected to ever succeed so it should not be
	          // retried.

	          return;
	     }
	}

	//----------------------------------------------------------------//
	private static boolean isServicesAvailable ( boolean showDialog ) {

		if ( mServiceStatus == ConnectionResult.SUCCESS ) {

			// everything is good
			return true;

		} else if ( mServiceStatus == ConnectionResult.SERVICE_MISSING ||
					mServiceStatus == ConnectionResult.SERVICE_VERSION_UPDATE_REQUIRED ||
					mServiceStatus == ConnectionResult.SERVICE_DISABLED ) {

			if ( showDialog ) {
				// prompt user to update or turn on services
				Dialog errorDialog = GooglePlayServicesUtil.getErrorDialog ( mServiceStatus, mActivity, ALERT_DIALOG_REQUEST_CODE );
				errorDialog.show ();
				return false;
			}

		} else {

			// log the error
			MoaiLog.i ( "MoaiGooglePlayServices isServicesAvailable: " + GooglePlayServicesUtil.getErrorString ( mServiceStatus ));
			return false;
		}

		return false;
	}

	//----------------------------------------------------------------//
	private static void getAccountInfo () {

		Intent intent = AccountPicker.newChooseAccountIntent ( null, null, new String [] { "com.google" }, false, null, null, null, null );
		mActivity.startActivityForResult ( intent, ACCOUNT_PICKER_REQUEST_CODE );
	}

	//================================================================//
	// Google Services JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static boolean connect () {

		MoaiLog.i ( "MoaiGooglePlayServices connect" );
		if ( isServicesAvailable ( true )) {

			mGoogleApiClient.connect ();
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------//
	public static boolean isConnected () {

		return mGoogleApiClient.isConnected ();
	}
	
	//----------------------------------------------------------------//
	public static void showAchievements ( ) {

		if ( isServicesAvailable ( true )) {

			//Intent achIntent = mGoogleApiClient.getAchievementsIntent ( );
			//mActivity.startActivityForResult ( achIntent, ACHIEVEMENT_REQUEST_CODE );
		}
	}

	//----------------------------------------------------------------//
	public static void showLeaderboard ( String leaderboardID ) {

		if ( isServicesAvailable ( true )) {

			//Intent lbIntent = mGoogleApiClient.getLeaderboardIntent ( leaderboardID );
			//mActivity.startActivityForResult ( lbIntent, LEADERBOARD_REQUEST_CODE );
		}
	}

	//----------------------------------------------------------------//
	public static void submitScore ( String leaderboardID, long score ) {

		if ( isServicesAvailable ( false )) {

			//mGoogleApiClient.submitScore ( leaderboardID, score );
		}
	}

	//----------------------------------------------------------------//
	public static void unlockAchievement ( String achID ) {

		if ( isServicesAvailable ( false )) {

			//mGoogleApiClient.unlockAchievement ( achID );
		}
	}
}
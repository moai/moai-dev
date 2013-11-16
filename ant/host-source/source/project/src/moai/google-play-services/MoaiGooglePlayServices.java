//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;
import com.ziplinegames.moai.*;

import android.accounts.AccountManager;
import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;
import android.os.AsyncTask;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;

import com.google.android.gms.auth.*;
import com.google.android.gms.common.*;
import com.google.android.gms.common.GooglePlayServicesClient.*;
import com.google.android.gms.games.*;
import com.google.android.gms.games.GamesClient;

import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;

//================================================================//
// MoaiGooglePlayServices
//================================================================//
public class MoaiGooglePlayServices {

	private static Activity 					sActivity = null;

	private static GamesClient					sGameClient = null;
	private static MoaiPlayServicesCallbacks 	sConnectionCallbacks = null;
	private static MoaiPlayServicesCallbacks 	sFailedCallback = null;
	private static String						sAccountName = null;
	private static int							sServicesStatus;
	private static String						sToken = null;

	public static final int 					ALERT_DIALOG_REQUEST_CODE = 7007;
	public static final int 					ACCOUNT_PICKER_REQUEST_CODE = 7117;
	public static final int 					AUTH_REQUEST_CODE = 7227;
	public static final int 					LEADERBOARD_REQUEST_CODE = 7337;
	public static final int						CONNECTION_RESOLUTION_CODE = 7447;
	public static final int						ACHIEVEMENT_REQUEST_CODE = 7557;

	//----------------------------------------------------------------//
	public static Activity getActivity () {

		return sActivity;
	}

	//----------------------------------------------------------------//
	public static void onActivityResult ( int requestCode, int resultCode, Intent data ) {

		MoaiLog.i ( "MoaiGooglePlayServices onActivityResult: " + requestCode + " " + resultCode );
		if ( requestCode == ACCOUNT_PICKER_REQUEST_CODE && resultCode == Activity.RESULT_OK ) {

			MoaiLog.i ( "MoaiGooglePlayServices ACCOUNT_PICKER_REQUEST_CODE" );
			sAccountName = data.getStringExtra ( AccountManager.KEY_ACCOUNT_NAME );
		} else if ( requestCode == LEADERBOARD_REQUEST_CODE && resultCode == GamesActivityResultCodes.RESULT_RECONNECT_REQUIRED ) {

			MoaiLog.i ( "MoaiGooglePlayServices LEADERBOARD_REQUEST_CODE" );
			sGameClient.reconnect ();
		} else if ( requestCode == CONNECTION_RESOLUTION_CODE && resultCode == Activity.RESULT_OK ) {

			MoaiLog.i ( "MoaiGooglePlayServices CONNECTION_RESOLUTION_CODE" );
			sGameClient.connect ();
		}
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {

		MoaiLog.i ( "MoaiGooglePlayServices onCreate" );
		sActivity = activity;

		sConnectionCallbacks = new MoaiPlayServicesCallbacks ();
		sFailedCallback = new MoaiPlayServicesCallbacks ();
		sGameClient = new GamesClient.Builder ( sActivity, sConnectionCallbacks, sFailedCallback ).create ();
	}

	//----------------------------------------------------------------//
	public static void onResume () {

		MoaiLog.i ( "MoaiGooglePlayServices onResume: Verify play services is available" );

		sServicesStatus = GooglePlayServicesUtil.isGooglePlayServicesAvailable ( sActivity );
	}

	//----------------------------------------------------------------//
	public static void onStart () {

		MoaiLog.i ( "MoaiGooglePlayServices onStart" );
		//sGameClient.connect ();
	}

	//----------------------------------------------------------------//
	public static void onStop () {

		MoaiLog.i ( "MoaiGooglePlayServices onStop" );
		sGameClient.disconnect ();
	}

	//================================================================//
	// MoaiGooglePlayServices private functions
	//================================================================//

	//----------------------------------------------------------------//
	private static void getAndUseAuthTokenBlocking () {

		try {

	    	// Retrieve a token for the given account and scope. It will always return either
	        // a non-empty String or throw an exception.
	        sToken = GoogleAuthUtil.getToken ( sActivity, sAccountName, Scopes.PLUS_LOGIN );


		 } catch ( GooglePlayServicesAvailabilityException playEx ) {

		     Dialog alert = GooglePlayServicesUtil.getErrorDialog ( playEx.getConnectionStatusCode (), sActivity, AUTH_REQUEST_CODE );

		 } catch ( UserRecoverableAuthException userAuthEx ) {

	          // Start the user recoverable action using the intent returned by getIntent ()
	          sActivity.startActivityForResult ( userAuthEx.getIntent (), AUTH_REQUEST_CODE );
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

		if ( sServicesStatus == ConnectionResult.SUCCESS ) {

			// everything is good
			return true;

		} else if ( sServicesStatus == ConnectionResult.SERVICE_MISSING ||
					sServicesStatus == ConnectionResult.SERVICE_VERSION_UPDATE_REQUIRED ||
					sServicesStatus == ConnectionResult.SERVICE_DISABLED ) {

			if ( showDialog ) {
				// prompt user to update or turn on services
				Dialog errorDialog = GooglePlayServicesUtil.getErrorDialog ( sServicesStatus, sActivity, ALERT_DIALOG_REQUEST_CODE );
				errorDialog.show ();
				return false;
			}

		} else {

			// log the error
			MoaiLog.i ( "MoaiGooglePlayServices isServicesAvailable: " + GooglePlayServicesUtil.getErrorString ( sServicesStatus ));
			return false;
		}

		return false;
	}

	//----------------------------------------------------------------//
	private static void getAccountInfo () {

		Intent intent = AccountPicker.newChooseAccountIntent ( null, null, new String [] { "com.google" }, false, null, null, null, null );
		sActivity.startActivityForResult ( intent, ACCOUNT_PICKER_REQUEST_CODE );
	}

	//================================================================//
	// Google Services JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static boolean connect () {

		MoaiLog.i ( "MoaiGooglePlayServices connect" );
		if ( isServicesAvailable ( true )) {

			sGameClient.connect ();
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------//
	public static boolean isConnected () {

		return sGameClient.isConnected ();
	}

	//----------------------------------------------------------------//
	public static void showAchievements ( ) {

		if ( isServicesAvailable ( true )) {

			Intent achIntent = sGameClient.getAchievementsIntent ( );
			sActivity.startActivityForResult ( achIntent, ACHIEVEMENT_REQUEST_CODE );
		}
	}

	//----------------------------------------------------------------//
	public static void showLeaderboard ( String leaderboardID ) {

		if ( isServicesAvailable ( true )) {

			Intent lbIntent = sGameClient.getLeaderboardIntent ( leaderboardID );
			sActivity.startActivityForResult ( lbIntent, LEADERBOARD_REQUEST_CODE );
		}
	}

	//----------------------------------------------------------------//
	public static void submitScore ( String leaderboardID, long score ) {

		if ( isServicesAvailable ( false )) {

			sGameClient.submitScore ( leaderboardID, score );
		}
	}

	//----------------------------------------------------------------//
	public static void unlockAchievement ( String achID ) {

		if ( isServicesAvailable ( false )) {

			sGameClient.unlockAchievement ( achID );
		}
	}
}
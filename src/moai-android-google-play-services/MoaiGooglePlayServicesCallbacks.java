//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;
import com.ziplinegames.moai.*;

import android.support.v4.app.DialogFragment;
import android.app.Dialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;

import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.ErrorDialogFragment;
import com.google.android.gms.common.api.GoogleApiClient.ConnectionCallbacks;
import com.google.android.gms.common.api.GoogleApiClient.OnConnectionFailedListener;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.*;

import android.support.v4.app.FragmentActivity;

public class MoaiGooglePlayServicesCallbacks
    implements ConnectionCallbacks, OnConnectionFailedListener {

    // Unique tag for the error dialog fragment
    public static final String DIALOG_ERROR = "dialog_error";

    @Override
    public void onConnected ( Bundle connectionHint ) {

		MoaiLog.i ( "MoaiGooglePlayServicesCallbacks onConnected");
        // Connected to Google Play services!
        // The good stuff goes here.
    }

	@Override
    public void onConnectionSuspended ( int cause ) {

		MoaiLog.i ( "MoaiGooglePlayServicesCallbacks onConnectionSuspended: " + cause );
        // The connection has been interrupted.
        // Disable any UI components that depend on Google APIs
        // until onConnected() is called.
    }

    // This callback is important for handling errors that
    // may occur while attempting to connect with Google.
    @Override
    public void onConnectionFailed ( ConnectionResult result ) {
        
		MoaiLog.i ( "MoaiGooglePlayServicesCallbacks onConnectionFailed" );
        if ( MoaiGooglePlayServices.getResolvingError ()) {
            
			MoaiLog.i ( "MoaiGooglePlayServicesCallbacks onConnectionFailed: Already attempting to resolve an error." );
            return;
        } else if ( result.hasResolution ()) {
            
            MoaiLog.i ( "MoaiGooglePlayServicesCallbacks onConnectionFailed: result.hasResolution ()" );
            try {

                MoaiGooglePlayServices.setResolvingError ( true );
                result.startResolutionForResult ( MoaiGooglePlayServices.getActivity (), MoaiGooglePlayServices.REQUEST_RESOLVE_ERROR) ;
            } catch ( SendIntentException e ) {
                
				//MoaiLog.i ( "MoaiGooglePlayActivity onConnectionFailed: There was an error with the resolution intent. Try again." );
                //mGoogleApiClient.connect ();
            }
        } else {

			MoaiLog.i ( "MoaiGooglePlayServicesCallbacks onConnectionFailed: Show dialog using GooglePlayServicesUtil.getErrorDialog()" );
            showErrorDialog ( result.getErrorCode ());
            MoaiGooglePlayServices.setResolvingError ( true );
        }
    }

    /* Called from ErrorDialogFragment when the dialog is dismissed. */
    public static void onDialogDismissed () {

        MoaiLog.i ( "MoaiGooglePlayServicesCallbacks onDialogDismissed" );
        MoaiGooglePlayServices.setResolvingError ( false );
    }

    private void showErrorDialog ( int errorCode ) {
        
        MoaiLog.i ( "MoaiGooglePlayServicesCallbacks showErrorDialog" );

        // Create a fragment for the error dialog
        ErrorDialogFragment dialogFragment = new ErrorDialogFragment ();
        
        // Pass the error that should be displayed
        Bundle args = new Bundle ();
        args.putInt ( DIALOG_ERROR, errorCode );
        dialogFragment.setArguments ( args );
        //dialogFragment.show ( MoaiGooglePlayActivity.getSupportFragmentManager (), "errordialog" );
    }

    public static class ErrorDialogFragment extends DialogFragment {

        public ErrorDialogFragment () { }

        public Dialog onCreateDialog ( Bundle savedInstanceState ) {

            // Get the error code and retrieve the appropriate dialog
            int errorCode = this.getArguments ().getInt ( DIALOG_ERROR );

            return GooglePlayServicesUtil.getErrorDialog ( errorCode, MoaiGooglePlayServices.getActivity (), MoaiGooglePlayServices.REQUEST_RESOLVE_ERROR );
        }

        public void onDismiss ( DialogInterface dialog ) {
            MoaiGooglePlayServicesCallbacks.onDialogDismissed ();
        }
    }
}
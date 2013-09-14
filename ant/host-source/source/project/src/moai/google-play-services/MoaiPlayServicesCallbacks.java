//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;
import android.os.Bundle;

import com.google.android.gms.common.*;
import com.google.android.gms.common.GooglePlayServicesClient.*;

//================================================================//
// MoaiPlayServicesCallbacks
//================================================================//
public class MoaiPlayServicesCallbacks implements ConnectionCallbacks, OnConnectionFailedListener {

	//----------------------------------------------------------------//
	@Override
	public void onConnected ( Bundle connectionHint ) {

		MoaiLog.i ( "MoaiPlayServicesCallbacks onConnected" );
	}

	//----------------------------------------------------------------//
	@Override
	public void onDisconnected () {

		MoaiLog.i ( "MoaiPlayServicesCallbacks onDisconnected" );
	}

	//----------------------------------------------------------------//
	@Override
	public void onConnectionFailed ( ConnectionResult result ) {

		MoaiLog.i ( "MoaiPlayServicesCallbacks onConnectionFailed: " + result.toString ());

		if ( result.hasResolution ()) {

			try {

				MoaiLog.i ( "MoaiPlayServicesCallbacks attempting resolution" );
				result.startResolutionForResult ( MoaiGooglePlayServices.getActivity (), MoaiGooglePlayServices.CONNECTION_RESOLUTION_CODE );
			} catch ( SendIntentException e ) {

				MoaiLog.i ( "MoaiPlayServicesCallbacks EXCEPTION" );
				e.printStackTrace ();
			}
		}
	}
}
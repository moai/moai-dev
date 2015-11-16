//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.moaisdk.fortumo;

import com.moaisdk.core.MoaiLog;

import android.app.Activity;

@SuppressWarnings("unused")

//================================================================//
// MoaiFortumo
// TODO: Does nothing, all payment code has to be on the main activity, fix later
//================================================================//

public class MoaiFortumo {

	private static Activity sActivity = null;


	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {

		MoaiLog.i ( "MoaiFortumo onCreate: Initializing Fortumo" );

		sActivity = activity;

	}

	//----------------------------------------------------------------//
	public static void onPause ( ) {

		MoaiLog.i ( "MoaiFortumo onPause: Notifying Fortumo" );
	}

	//----------------------------------------------------------------//
	public static void onResume ( ) {

		MoaiLog.i ( "MoaiFortumo onResume: Notifying Fortumo" );
	}


}
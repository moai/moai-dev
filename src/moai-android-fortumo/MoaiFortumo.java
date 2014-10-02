//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.os.Bundle;

import java.util.Arrays;
import java.util.ArrayList;

import com.fortumo.android.Fortumo;
import com.fortumo.android.PaymentActivity;
import com.fortumo.android.PaymentRequestBuilder;
import com.fortumo.android.PaymentRequest;
import com.fortumo.android.PaymentResponse;

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
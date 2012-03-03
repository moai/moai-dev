//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;

//================================================================//
// MoaiFacebook
//================================================================//
public class MoaiFacebook {

	private static Activity sActivity = null;

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		
		MoaiLog.i ( "onCreate: Initializing Facebook" );
		
		sActivity = activity;
	}

	//================================================================//
	// Facebook JNI callback methods
	//================================================================//

}
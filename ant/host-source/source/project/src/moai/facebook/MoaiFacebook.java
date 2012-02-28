//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;

public class MoaiFacebook {

	private static Activity mActivity = null;

	public static void initialize ( Activity activity ) {
		
		MoaiLog.i ( "Initializing Facebook" );
		
		mActivity = activity;
	}

	//================================================================//
	// Facebook JNI callback methods
	//================================================================//

}
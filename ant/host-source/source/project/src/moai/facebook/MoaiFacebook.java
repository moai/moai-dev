//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.content.Context;

//================================================================//
// MoaiFacebook
//================================================================//
public class MoaiFacebook {

	private static Context mContext = null;

	//----------------------------------------------------------------//
	public static void initialize ( Context context ) {
		
		MoaiLog.i ( "Initializing Facebook" );
		
		mContext = context;
	}

	//================================================================//
	// Facebook JNI callback methods
	//================================================================//

}
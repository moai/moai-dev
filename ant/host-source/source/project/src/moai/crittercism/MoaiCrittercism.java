//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;

import com.crittercism.app.Crittercism;

//================================================================//
// MoaiCrittercism
//================================================================//
public class MoaiCrittercism {

	private static Activity sActivity = null;

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {
		
		MoaiLog.i ( "MoaiCrittercism onCreate: Initializing Crittercism" );
		
		sActivity = activity;
	}

	//================================================================//
	// Crittercism JNI callback methods
	//================================================================//

	//----------------------------------------------------------------//
	public static void init ( String appId ) {

		Crittercism.init ( sActivity, appId );
	}

	//----------------------------------------------------------------//
	public static void leaveBreadcrumb ( String breadcrumb ) {

		Crittercism.leaveBreadcrumb ( breadcrumb );
	}
		
	//----------------------------------------------------------------//
	public static void setUser ( String ident ) {

		Crittercism.setUsername ( ident );
	}
}
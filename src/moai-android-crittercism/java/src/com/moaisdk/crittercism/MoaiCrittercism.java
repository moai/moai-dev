//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc.
// All Rights Reserved.
// http://getmoai.com
//----------------------------------------------------------------//

package com.moaisdk.crittercism;

import com.moaisdk.core.MoaiLog;

import android.app.Activity;

import com.crittercism.app.Crittercism;
import com.crittercism.app.CrittercismConfig;

@SuppressWarnings("unused")

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
	public static void forceException () throws Exception {

		// this will force a crash
		//sActivity = null;
		//int a = sActivity.getResources ().getDisplayMetrics ().densityDpi;

		throw new Exception ( "Moai Force Exception" );
	}

	//----------------------------------------------------------------//
	public static void init ( String appId ) {

		MoaiLog.i ( String.format ( "MoaiCrittercism: init %s", appId ));
		
		CrittercismConfig config = new CrittercismConfig();
		config.setLogcatReportingEnabled( true );
		
		Crittercism.initialize ( sActivity.getApplicationContext (), appId, config );
	}

	//----------------------------------------------------------------//
	public static void leaveBreadcrumb ( String breadcrumb ) {

		Crittercism.leaveBreadcrumb ( breadcrumb );
	}

	//----------------------------------------------------------------//
	public static void setUser ( String ident ) {

		MoaiLog.i ( String.format ( "MoaiCrittercism: setUser %s", ident ));

		Crittercism.setUsername ( ident );
	}
}
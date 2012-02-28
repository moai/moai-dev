//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;

import java.lang.reflect.Method;

public class Moai {

	public static void initialize ( Activity activity ) {
		
		// Tapjoy may not have been added to this host when configured, so we 
		// use reflection and only initialize it if it is present.
		try {
			
			Class tapjoy = Class.forName ( "com.ziplinegames.moai.MoaiTapjoy" );
			Method initialize = tapjoy.getMethod ( "initialize", Activity.class );

			initialize.invoke ( null, activity );
		} catch ( Throwable e ) {

		}

		// Facebook may not have been added to this host when configured, so we 
		// use reflection and only initialize it if it is present.
		try {
			
			Class facebook = Class.forName ( "com.ziplinegames.moai.MoaiFacebook" );
			Method initialize = facebook.getMethod ( "initialize", Activity.class );

			initialize.invoke ( null, activity );
		} catch ( Throwable e ) {

		}
	}
}
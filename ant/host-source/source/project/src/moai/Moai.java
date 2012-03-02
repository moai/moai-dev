//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.content.Context;

import java.lang.reflect.Method;

public class Moai {

	public static void initialize ( Context context ) {
		
		// Tapjoy may not have been added to this host when configured, so we 
		// use reflection and only initialize it if it is present.
		try {
			
			Class tapjoy = Class.forName ( "com.ziplinegames.moai.MoaiTapjoy" );
			Method initialize = tapjoy.getMethod ( "initialize", Context.class );

			initialize.invoke ( null, context );
		} catch ( Throwable e ) {

		}

		// Facebook may not have been added to this host when configured, so we 
		// use reflection and only initialize it if it is present.
		try {
			
			Class facebook = Class.forName ( "com.ziplinegames.moai.MoaiFacebook" );
			Method initialize = facebook.getMethod ( "initialize", Context.class );

			initialize.invoke ( null, context );
		} catch ( Throwable e ) {

		}

		// Google Push may not have been added to this host when configured, so we 
		// use reflection and only initialize it if it is present.
		try {
			
			Class push = Class.forName ( "com.ziplinegames.moai.MoaiGooglePush" );
			Method initialize = push.getMethod ( "initialize", Context.class );

			initialize.invoke ( null, context );
		} catch ( Throwable e ) {

		}
	}
}
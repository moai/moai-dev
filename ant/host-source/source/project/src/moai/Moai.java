//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import java.lang.reflect.Method;
import java.util.ArrayList;

//================================================================//
// Moai
//================================================================//
public class Moai {

	public enum ApplicationState {
	
        APPLICATION_UNINITIALIZED,
        APPLICATION_RUNNING,
        APPLICATION_PAUSED;

        public static ApplicationState valueOf ( int index ) {
	
            ApplicationState [] values = ApplicationState.values ();
            if (( index < 0 ) || ( index >= values.length )) {
	
                return APPLICATION_UNINITIALIZED;
            }

            return values [ index ];
        }
    }

	private static String [] sExternalClasses = {
		"com.ziplinegames.moai.MoaiAdColony",
		"com.ziplinegames.moai.MoaiCrittercism",
		"com.ziplinegames.moai.MoaiFacebook",
		"com.ziplinegames.moai.MoaiGoogleBilling",
		"com.ziplinegames.moai.MoaiGooglePush",
		"com.ziplinegames.moai.MoaiTapjoy",
	};
	
	private static ApplicationState sApplicationState = ApplicationState.APPLICATION_UNINITIALIZED;
	private static ArrayList < Class > sAvailableClasses = new ArrayList < Class > ();

	//----------------------------------------------------------------//
	static {
		
		for ( String className : sExternalClasses )
		{
			Class theClass = findClass ( className );
			if ( theClass != null ) {
				
				sAvailableClasses.add ( theClass );
			}
		}
	}

	//----------------------------------------------------------------//
	public static ApplicationState getApplicationState () {

		return sApplicationState;
	}
	
	//----------------------------------------------------------------//
	public static void onActivityResult ( int requestCode, int resultCode, Intent data ) {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onActivityResult", new Class [] { java.lang.Integer.TYPE, java.lang.Integer.TYPE, Intent.class }, new Object [] { new Integer ( requestCode ), new Integer ( resultCode ), data });
		}	
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity, Bundle extras ) {

		for ( Class theClass : sAvailableClasses ) {
			
			executeMethod ( theClass, null, "onCreate", new Class [] { Activity.class, Bundle.class }, new Object [] { activity, extras });
		}
	}
	
	//----------------------------------------------------------------//
	public static void onDestroy () {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onDestroy", new Class [] { }, new Object [] { });
		}		
	}

	//----------------------------------------------------------------//
	public static void onPause () {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onPause", new Class [] { }, new Object [] { });
		}		
	}

	//----------------------------------------------------------------//
	public static void onResume ( Bundle extras ) {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onResume", new Class [] { Bundle.class }, new Object [] { extras });
		}		
	}

	//----------------------------------------------------------------//
	public static void onStart () {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onStart", new Class [] { }, new Object [] { });
		}		
	}

	//----------------------------------------------------------------//
	public static void onStop () {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onStop", new Class [] { }, new Object [] { });
		}		
	}

	//----------------------------------------------------------------//
	public static void setApplicationState ( ApplicationState state ) {

		if ( state != sApplicationState ) {
			
			sApplicationState = state;
		
			for ( Class theClass : sAvailableClasses ) {
			
				executeMethod ( theClass, null, "onApplicationStateChanged", new Class [] { ApplicationState.class }, new Object [] { sApplicationState });
			}
		}
	}
	
	//================================================================//
	// Private methods
	//================================================================//
	
	//----------------------------------------------------------------//
	private static Class findClass ( String className ) {
		
		Class theClass = null;
		try {

			theClass = Class.forName ( className );
		} catch ( Throwable e ) {

		}
		
		return theClass;
	}
	
	//----------------------------------------------------------------//
	private static Object executeMethod ( Class theClass, Object theInstance, String methodName, Class [] parameterTypes, Object [] parameterValues ) {
		
		Object result = null;
		if ( theClass != null ) {
			
			try {

				Method theMethod = theClass.getMethod ( methodName, parameterTypes );

				result = theMethod.invoke ( theInstance, parameterValues );
			} catch ( Throwable e ) {
				
			}			
		}
		
		return result;
	}
}
//----------------------------------------------------------------//
// Copyright (c) 2010-2011 Zipline Games, Inc. 
// All Rights Reserved. 
// http://getmoai.com
//----------------------------------------------------------------//

package com.ziplinegames.moai;

import android.app.Activity;
import android.content.Intent;

import java.lang.reflect.Method;
import java.util.ArrayList;

//================================================================//
// Moai
//================================================================//
public class Moai {
	
	private static String [] sExternalClasses = {
		"com.ziplinegames.moai.MoaiTapjoy",
		"com.ziplinegames.moai.MoaiFacebook",
		"com.ziplinegames.moai.MoaiGooglePush",
		"com.ziplinegames.moai.MoaiGoogleBilling",
	};
	
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
	public static void onActivityResult ( int requestCode, int resultCode, Intent data ) {
	
	}

	//----------------------------------------------------------------//
	public static void onCreate ( Activity activity ) {

		for ( Class theClass : sAvailableClasses ) {
			
			executeMethod ( theClass, null, "onCreate", new Class [] { Activity.class }, new Object [] { activity });
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
	public static void onResume () {
	
		for ( Class theClass : sAvailableClasses ) {

			executeMethod ( theClass, null, "onResume", new Class [] { }, new Object [] { });
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